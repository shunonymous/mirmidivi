/*
 * curses.cpp - Rendering MIDI to console
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
#include <thread>
#include <vector>
#include <algorithm>

#include "mirmidivi/mirmidivi.hpp"
#include "mirmidivi/fluidsynth.hpp"
#include "mirmidivi/message.hpp"
#include "mirmidivi/sleep.hpp"
#include "mirmidivi/rendering/PianoRoll.hpp"

enum ExtColors
{
    COLOR_LIGHTGREEN = 8,
    COLOR_DEEPPINK = 9,
};

namespace curses{
#include <curses.h>
}

namespace mirmidivi
{
    namespace curses
    {
	struct Point
	{ int x = 0, y = 0; };

	struct Screen
	{
	    Point Offset = {-2, 0};
	    sysclk::duration ViewingTime = 0s;
	    Point Size;
	    int Scale = 1;
	};

	void InitCurses()
	{
	    using namespace ::curses;
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

	void DrawNote(const Note& Note, Screen& Screen, const sysclk::duration& BaseDuration)
	{
	    using namespace ::curses;
	    using namespace std::chrono;

	    // Start drawing from beginning point.
	    // Center of display is center, and C4(60) notes appear here as default.
	    int BeginningPoint =
		Screen.Size.x +	Screen.Offset.x -
		(duration_cast<milliseconds>(BaseDuration - Note.BeginTime).count() * Screen.Scale * 0.1);
	    int EndPoint = Screen.Size.x + Screen.Offset.x -
		(duration_cast<milliseconds>(BaseDuration - Note.EndTime).count() * Screen.Scale * 0.1);
	    int VertexPosition = (Screen.Size.y / 2) - Note.Scale + 60 + Screen.Offset.y;

	    if(BeginningPoint < 0)
		BeginningPoint = 0;
	    if(EndPoint < 0)
		EndPoint = 0;
	    if(BeginningPoint > Screen.Size.x)
		BeginningPoint = Screen.Size.x;
	    if(EndPoint > Screen.Size.x)
		EndPoint = Screen.Size.x;

	    std::string prt;
	    prt.append(EndPoint - BeginningPoint, '|');

	    if(!prt.empty() and
	       (0 <= VertexPosition) and (VertexPosition < Screen.Size.y))
	    {
		move(VertexPosition,
		     BeginningPoint);
		attrset(COLOR_PAIR(Note.Ch + 1));
		printw(prt.c_str());
	    }
	}

	void RenderToCurses(const Option& Options, const std::shared_ptr<fluidsynth::Synth>& Synth, const bool& QuitFlag)
	{
	    Screen Screen;

	    PianoRoll Drawer(Synth);
	    InitCurses();
	    ::curses::clear();

	    int FrameCount = 0;
	    auto RefreshdFpsPoint = sysclk::now();
	    float fps;

	    auto OneFrameDuration = 1s / Options.getFramePerSecond();

	    while(!QuitFlag)
	    {
		using namespace ::curses;

		auto FrameBegin = sysclk::now();
		sysclk::duration BaseDuration = (FrameBegin + Screen.ViewingTime) - Synth->getBeginTimePoint();
		getmaxyx(stdscr, Screen.Size.y, Screen.Size.x);

		erase();

		for(const auto& Note : Drawer.getNotes(BaseDuration - (5s * Screen.Scale), BaseDuration))
		    DrawNote(Note, Screen, BaseDuration);

		// Display infos
		++FrameCount;
		move(Screen.Size.y - 1, Screen.Size.x - 11);
		attrset(COLOR_WHITE);
		printw("%5.2ffps", fps);
		move(Screen.Size.y - 2, Screen.Size.x - 15);
		printw("size:%dx%d", Screen.Size.x, Screen.Size.y);
		float TimeDiff = std::chrono::duration_cast<std::chrono::seconds>(sysclk::now() - RefreshdFpsPoint).count();
		if (TimeDiff >= 5)
		{
		    fps = FrameCount / TimeDiff;
		    FrameCount = 0;
		    RefreshdFpsPoint = sysclk::now();
		}

		// Note counter
		move(Screen.Size.y - 1, 2);
		attrset(COLOR_WHITE);
		printw("NOTES:%d", Drawer.getNotes().size());

		while((sysclk::now() - FrameBegin) < OneFrameDuration)
		    sleep(0.1ms);

		refresh();
	    }
	    ::curses::endwin();
	} // void PrintMessage
    } // namespace curses
} // namespace mirmidivi

// Call from external source    
extern "C" void SynthRendering(const mirmidivi::Option& Options, std::shared_ptr<mirmidivi::fluidsynth::Synth>& Synth, bool& QuitFlag)
{
    std::cout << "Rendering called" << std::endl;
    mirmidivi::curses::RenderToCurses(Options, Synth, QuitFlag);
}

extern "C" void PlayerRendering(const mirmidivi::Option& Options, std::shared_ptr<mirmidivi::fluidsynth::Player>& Player, bool& QuitFlag)
{
    std::cout << "Rendering called" << std::endl;
    mirmidivi::curses::RenderToCurses(Options, Player->getSynth(), QuitFlag);
}
