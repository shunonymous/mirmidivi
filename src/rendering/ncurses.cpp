/*
 * ncurses.cpp - Rendering MIDI to console
 *
 * This program is part of mirmidivi
 *
 * Copyright (C) 2017 Shun Terabayashi <shunonymous@gmail.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

#include "mirmidivi/mirmidivi.hpp"
#include "mirmidivi/sleep.hpp"
#include "mirmidivi/midi.hpp"

#include "DynamicLoader/DynamicLoader.hpp"

enum ExtColors
{
    COLOR_LIGHTGREEN = 8,
    COLOR_DEEPPINK = 9,
};

namespace ncurses{
#include <ncurses.h>
}

namespace mirmidivi
{
    namespace ncurses
    {
	using Piano = std::vector<std::pair<bool, short>>;
	using PianoRoll = std::vector<Piano>;

	void InitNcurses()
	{
	    using namespace ::ncurses;
	    initscr();
	    keypad(stdscr, TRUE);
	    nonl();
	    cbreak();
	    echo();

	    if(has_colors())
	    {
		start_color();
		init_color(COLOR_DEEPPINK, 1000, 78, 574);
		init_pair(1, COLOR_RED,     COLOR_BLACK);
		init_pair(2, COLOR_GREEN,   COLOR_BLACK);
		init_pair(3, COLOR_YELLOW,  COLOR_BLACK);
		init_pair(4, COLOR_BLUE,    COLOR_BLACK);
		init_pair(5, COLOR_CYAN,    COLOR_BLACK);
		init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
		init_pair(7, COLOR_WHITE,   COLOR_BLACK);
		init_pair(8, COLOR_DEEPPINK, COLOR_BLACK);
		init_pair(9, COLOR_RED,     COLOR_BLACK);
		init_pair(10, COLOR_GREEN,   COLOR_BLACK);
		init_pair(11, COLOR_YELLOW,  COLOR_BLACK);
		init_pair(12, COLOR_BLUE,    COLOR_BLACK);
		init_pair(13, COLOR_CYAN,    COLOR_BLACK);
		init_pair(14, COLOR_MAGENTA, COLOR_BLACK);
		init_pair(15, COLOR_WHITE,   COLOR_BLACK);
		init_pair(16, COLOR_DEEPPINK, COLOR_BLACK);
	    }

	    erase();

	    refresh();
	    curs_set(0);	    
	}
	
	void RenderToNcurses(Option Options, mirmidivi::MidiReceiver& MidiReceivedData, mirmidivi::MidiUtils& MidiInData, bool& QuitFlag)
	{
//	    auto FrameTime = 1s / Options.FramePerSecond;
	    auto FrameTime = 1s / 60.0;
	    float fps = 0.00;
	    int FrameCount = 0;

	    int Channel, NoteNumber;
	    PianoRoll PianoRollMap;
	    int WorkingEventNumber = 0, CurrentEventNumber = 0;

	    struct { int Width, Height; } TermSize;
	    struct { int x, y; } StartingPiont;

	    // Pre-reserve memory
	    PianoRollMap.reserve(200);
	    for(auto& PianoRoll : PianoRollMap)
		PianoRoll.reserve(128);
	    
	    InitNcurses();
	    ::jdksmidi::MIDIClockTime CurrentTick = 0;
	    Piano WritingBuffer(128,std::make_pair(false, 0));

	    auto TimePoint = sysclk::now();
	    auto FramePoint = sysclk::now();

	    struct
	    {
		int Begin = 0, End = 0;
	    } ScanningRange;

	    while(!QuitFlag)
	    {
		using namespace ::ncurses;
		auto Begin = sysclk::now();

		// Set starting point
		getmaxyx(stdscr, TermSize.Height, TermSize.Width);
		StartingPiont.x = TermSize.Width;
		StartingPiont.y = TermSize.Height / 2;

		CurrentTick = MidiInData.TimeToTick(Begin - TimePoint);
		
		MidiInData.MidiTracks->GetTrack(1)->FindEventNumber(CurrentTick, &CurrentEventNumber);

 		for(;WorkingEventNumber < CurrentEventNumber; ++WorkingEventNumber)
		{
		    if(MidiInData.MidiTracks->GetTrack(1)->GetEvent(1)->IsNoteOn())
		    {
			NoteNumber = static_cast<int>(MidiInData.MidiTracks->GetTrack(1)->GetEvent(WorkingEventNumber)->GetNote());
			Channel = MidiInData.MidiTracks->GetTrack(1)->GetEvent(WorkingEventNumber)->GetChannel();

			WritingBuffer[NoteNumber] = std::make_pair(true, Channel);
		    } else if(MidiInData.MidiTracks->GetTrack(1)->GetEvent(WorkingEventNumber)->IsNoteOff())
		    {
			NoteNumber = static_cast<int>(MidiInData.MidiTracks->GetTrack(1)->GetEvent(WorkingEventNumber)->GetNote());
			Channel = MidiInData.MidiTracks->GetTrack(1)->GetEvent(WorkingEventNumber)->GetChannel();

			WritingBuffer[NoteNumber] = std::make_pair(false, Channel);
		    } else if(MidiInData.MidiTracks->GetTrack(1)->GetEvent(WorkingEventNumber)->IsAllNotesOff())
		    {
			for(auto&& Buf : WritingBuffer)
			    Buf = std::make_pair(false, Channel);
		    }
		}
		PianoRollMap.push_back(WritingBuffer);
		
		erase();
			
		ScanningRange.End = PianoRollMap.size() - 3;
		if(PianoRollMap.size() < TermSize.Width)
		    ScanningRange.Begin = 0;
		else
		    ScanningRange.Begin = ScanningRange.End - TermSize.Width + 3;

		// Scan stored PianoRollMap between displaying area
		for(int i = ScanningRange.Begin; i < ScanningRange.End; ++i)
		{
		    for(int NoteNum = 0; NoteNum <= 127; ++NoteNum)
		    {
			if(PianoRollMap[i][NoteNum].first)
			{
			    // C4(60) is center
			    move(StartingPiont.y - NoteNum + 60, StartingPiont.x - (PianoRollMap.size() - i - 1));
			    attrset(COLOR_PAIR(PianoRollMap[i][NoteNum].second + 1));
			    printw("|");
			}
		    }
		}

		if(PianoRollMap.size() > 8192)
		{
		    PianoRollMap.erase(PianoRollMap.begin(), PianoRollMap.begin() + 512);
		    ScanningRange.Begin = ScanningRange.Begin - 512;
		    ScanningRange.End = ScanningRange.End - 512;
		}
		
		while((sysclk::now() - Begin) <= FrameTime)
		{
		    sleep(10us);
		}


		FrameCount++;
		// Display infos
		move(TermSize.Height - 1, TermSize.Width - 8);
		attrset(COLOR_WHITE);
		printw("%.2ffps", fps);
		move(TermSize.Height - 2, TermSize.Width - 15);
		printw("size:%dx%d", TermSize.Width, TermSize.Height);
		if (sysclk::now() - FramePoint >= 5s)
		{
		    fps = FrameCount / 5.00;
		    FrameCount = 0;
		    FramePoint = sysclk::now();
		}
		refresh();

	    } // while(!QuitFlag)
	    ::ncurses::endwin();
	} // void PrintMessage
    } // namespace ncurses
} // namespace mirmidivi

// Call from external source    
extern "C" void Rendering(mirmidivi::Option Options, mirmidivi::MidiReceiver& MidiReceivedData, mirmidivi::MidiUtils& MidiInData, bool& QuitFlag)
{
    std::cout << "Rendering called" << std::endl;
    mirmidivi::ncurses::RenderToNcurses(Options, MidiReceivedData, MidiInData, QuitFlag);
}
