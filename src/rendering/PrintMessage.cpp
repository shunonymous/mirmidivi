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

#include "mirmidivi/mirmidivi.hpp"
#include "mirmidivi/sleep.hpp"
#include "mirmidivi/fluidsynth.hpp"
#include "mirmidivi/message.hpp"

#include "mirmidivi/rendering/Msg2Txt.hpp"

namespace mirmidivi
{
    namespace PrintText
    {
	void PrintMessage(const Option& Options, const std::shared_ptr<mirmidivi::fluidsynth::Synth>& Synth, bool& QuitFlag)
	{
	    while(!QuitFlag)
	    {
		std::cout << Msg2Txt(Synth) << "\r" << std::flush;
		sleep(10us);
	    }
	    std::cout << std::endl;
	} // void PrintMessage
    } // namespace PrintText
} // namespace mirmidivi

// Call from external source    
extern "C" void SynthRendering(const mirmidivi::Option& Options, std::shared_ptr<mirmidivi::fluidsynth::Synth>& Synth, bool& QuitFlag)
{
    std::cout << "Rendering called" << std::endl;
    mirmidivi::PrintText::PrintMessage(Options, Synth, QuitFlag);
}

// Call from external source    
extern "C" void PlayerRendering(const mirmidivi::Option& Options, std::shared_ptr<mirmidivi::fluidsynth::Player>& Player, bool& QuitFlag)
{
    std::cout << "Rendering called" << std::endl;
    mirmidivi::PrintText::PrintMessage(Options, Player->getSynth(), QuitFlag);
}

