/*
 * PrintMessage.cpp - Print MIDI Messages
 *
 * This program is part of mirmidivi
 *
 * Copyright (C) 2016-2017, 2019 Shun Terabayashi <shunonymous@gmail.com>
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
#include <sstream>
#include <iomanip>

#include <fluidsynth.h>

#include "mirmidivi/mirmidivi.hpp"
#include "mirmidivi/sleep.hpp"
#include "mirmidivi/fluidsynth.hpp"
#include "mirmidivi/message.hpp"

namespace mirmidivi
{
    namespace PrintText
    {
	void PrintMessage(Option Options, fluidsynth::Synth& Synth, bool& QuitFlag)
	{
	    const int line_length = 60;
	    const int type_length = 20;

	    int type;
	    int ch;

	    while(!QuitFlag)
	    {
		type = Synth.getType();
		ch = Synth.getChannel();

		std::cout << std::setw(line_length);

		switch(type)
		{
		case NOTE_ON:
		    std::cout << std::left << std::setw(type_length) << "NOTE ON:"
			      << "CH:" << std::setw(3) << ch
			      << "NOTE:" << std::setw(4) << Synth.getKey()
			      << "VELOCITY:" << Synth.getVelocity()
			      << "\r";
		    break;
		case NOTE_OFF:
		    std::cout << std::left << std::setw(type_length) << "NOTE OFF:"
			      << "CH:" << std::setw(3) << ch
			      << "NOTE:" << std::setw(4) << Synth.getKey()
			      << "\r";
		    break;
		case CONTROL_CHANGE:
		    std::cout << std::left << std::setw(type_length) << "CONTROLL CHANGE:"
			      << "CH:" << std::setw(3) << ch
			      << "CONTROL:"  << std::setw(4)<< Synth.getControl()
			      << "VAL:" << std::setw(4) << Synth.getValue()
			      << "\r";
		    break;
		case PROGRAM_CHANGE:
		    std::cout << std::left << std::setw(type_length) << "CONTROLL CHANGE:"
			      << "CH:" << std::setw(3) << ch
			      << "PROGRAM:" << std::setw(4) << Synth.getProgram()
			      << "\r";
		    break;
		case CHANNEL_PRESSURE:
		    std::cout << std::left << std::setw(type_length) << "CHANNEL PRESSURE:"
			      << "CH:" << std::setw(3) << ch
			      << "PROGRAM:" << std::setw(4) << Synth.getProgram()
			      << "\r";
		    break;
		case KEY_PRESSURE:
		    std::cout << std::left << std::setw(type_length) << "KEY PRESSURE:"
			      << "CH:" << std::setw(3) << ch
			      << "KEY:" << std::setw(4) << Synth.getKey()
			      << "VAL:" << std::setw(4) << Synth.getValue()
			      << "\r";
		    break;
		case PITCH_BEND:
		    std::cout << std::left << std::setw(type_length) << "PITCH BEND:"
			      << "CH:" << std::setw(3) << ch
			      << "PITCH:" << std::setw(4) << Synth.getPitchbend()
			      << "\r";
		    break;
		case MIDI_SYSTEM_RESET:
		    std::cout << std::left << std::setw(type_length) << "MIDI SYSTEM RESET:"
			      << "\r";
		    break;
		case MIDI_SYSEX:
		    std::cout << std::left << std::setw(type_length) << "SYSEX:"
			      << "\r";
		    break;
		case MIDI_TEXT:
		    std::cout << std::left << std::setw(type_length) << "MIDI TEXT:" << "\r";
		    break;
		case MIDI_LYRIC:
		    std::cout << std::left << std::setw(type_length) << "MIDI LYRIC:" << "\r";
		    break;
		case MIDI_SET_TEMPO:
		    std::cout << std::left << std::setw(type_length) << "MIDI SET TEMPO:" << "\r";
		    break;
		default:
		    break;
		}
		std::cout << std::flush;
		sleep(10us);
	    }
	    std::cout << std::endl;
	} // void PrintMessage
    } // namespace PrintText
} // namespace mirmidivi

// Call from external source    
extern "C" void Rendering(mirmidivi::Option Options, mirmidivi::fluidsynth::Synth& Synth, bool& QuitFlag)
{
    std::cout << "Rendering called" << std::endl;
    mirmidivi::PrintText::PrintMessage(Options, Synth, QuitFlag);
}
