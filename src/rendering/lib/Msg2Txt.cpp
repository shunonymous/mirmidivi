/*
 * Msg2Txt.cpp
 *
 * Copyright (C) 2019 Shun Terabayashi <shunonymous@gmail.com>
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

#include <string>
#include <sstream>
#include <iomanip>

#include "mirmidivi/fluidsynth.hpp"
#include "mirmidivi/message.hpp"
#include "mirmidivi/rendering/Msg2Txt.hpp"

namespace mirmidivi
{
    std::string Msg2Txt(const std::shared_ptr<mirmidivi::fluidsynth::Synth> Synth)
    {
	const int type_length = 20;

	int type;
	int ch;
	std::stringstream Text;

	if(Synth->getEvent() != nullptr)
	{
	    type = Synth->getType();
	    ch = Synth->getChannel();
	} else {
	    type = 0;
	    ch = 0;
	}
	switch(type)
	{
	case NOTE_ON:
	    Text << std::left << std::setw(type_length) << "NOTE ON:"
		 << "CH:" << std::setw(3) << ch
		 << "NOTE:" << std::setw(4) << Synth->getKey()
		 << "VELOCITY:" << Synth->getVelocity();
	    break;
	case NOTE_OFF:
	    Text << std::left << std::setw(type_length) << "NOTE OFF:"
		 << "CH:" << std::setw(3) << ch
		 << "NOTE:" << std::setw(4) << Synth->getKey();
	    break;
	case CONTROL_CHANGE:
	    Text << std::left << std::setw(type_length) << "CONTROLL CHANGE:"
		 << "CH:" << std::setw(3) << ch
		 << "CONTROL:"  << std::setw(4)<< Synth->getControl()
		 << "VAL:" << std::setw(4) << Synth->getValue();
	    break;
	case PROGRAM_CHANGE:
	    Text << std::left << std::setw(type_length) << "CONTROLL CHANGE:"
		 << "CH:" << std::setw(3) << ch
		 << "PROGRAM:" << std::setw(4) << Synth->getProgram();
	    break;
	case CHANNEL_PRESSURE:
	    Text << std::left << std::setw(type_length) << "CHANNEL PRESSURE:"
		 << "CH:" << std::setw(3) << ch
		 << "PROGRAM:" << std::setw(4) << Synth->getProgram();
	    break;
	case KEY_PRESSURE:
	    Text << std::left << std::setw(type_length) << "KEY PRESSURE:"
		 << "CH:" << std::setw(3) << ch
		 << "KEY:" << std::setw(4) << Synth->getKey()
		 << "VAL:" << std::setw(4) << Synth->getValue();
	    break;
	case PITCH_BEND:
	    Text << std::left << std::setw(type_length) << "PITCH BEND:"
		 << "CH:" << std::setw(3) << ch
		 << "PITCH:" << std::setw(4) << Synth->getPitchbend();
	    break;
	case MIDI_SYSTEM_RESET:
	    Text << std::left << std::setw(type_length) << "MIDI SYSTEM RESET:";
	    break;
	case MIDI_SYSEX:
	    Text << std::left << std::setw(type_length) << "SYSEX:";
	    break;
	case MIDI_TEXT:
	    Text << std::left << std::setw(type_length) << "MIDI TEXT:";
	    break;
	case MIDI_LYRIC:
	    Text << std::left << std::setw(type_length) << "MIDI LYRIC:";
	    break;
	case MIDI_SET_TEMPO:
	    Text << std::left << std::setw(type_length) << "MIDI SET TEMPO:";
	    break;
	default:
	    break;
	}
	return Text.str();
    }
} // namespace mirmidivi

