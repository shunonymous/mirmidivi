/*
 * text.cpp
 *
 * Copyright (C) 2016 Shun Terabayashi <shunonymous@gmail.com>
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
#include <iomanip>

#include "mirmidivi/MidiDataStructure.hpp"
#include "mirmidivi/Options.hpp"
#include "mirmidivi/sleep.hpp"

namespace mirmidivi
{
    namespace PrintText
    {
	void PrintMessage(Environment CoreEnv, Midi& MidiInData, bool& QuitFlag)
	{
	    auto duration = 1s / CoreEnv.FramePerSecond;
	    while(!QuitFlag)
	    {
		std::cout << std::hex << MidiInData.getRawData() << "\r" << std::flush;
		sleep(duration);
	    }
	    std::cout << std::endl;
	} // void PrintMessage
    } // namespace PrintText
} // namespace mirmidivi

// Call from external source    
extern "C" void Rendering(mirmidivi::Environment CoreEnv, mirmidivi::Midi& MidiInData, bool& QuitFlag)
{
    std::cout << "Rendering called" << std::endl;
    mirmidivi::PrintText::PrintMessage(CoreEnv, MidiInData, QuitFlag);
}
