/*
 * PrintMessage.cpp
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

#include "mirmidivi/mirmidivi.hpp"
#include "mirmidivi/sleep.hpp"
#include "mirmidivi/midi.hpp"

namespace mirmidivi
{
    namespace PrintText
    {
	void PrintMessage(Option Options, mirmidivi::MidiReceiver& MidiReceivedData, mirmidivi::MidiUtils& MidiInData, bool& QuitFlag)
	{

	    char Text[129];
	    jdksmidi::MIDIParser Parser(32 * 1024);

	    while(!QuitFlag)
	    {
		auto MidiCurrentMessage = MidiReceivedData.MidiRawMessage;
		for(int i=0; i<MidiCurrentMessage.size(); ++i)
		    Parser.Parse(MidiCurrentMessage[i], &MidiInData.MidiParsedMessage);

		MidiInData.MidiParsedMessage.MsgToText(Text);
		
		std::cout << Text << "\r" << std::flush;
		sleep(10us);
	    }
	    std::cout << std::endl;
	} // void PrintMessage
    } // namespace PrintText
} // namespace mirmidivi

// Call from external source    
extern "C" void Rendering(mirmidivi::Option Options, mirmidivi::MidiReceiver& MidiReceivedData, mirmidivi::MidiUtils& MidiInData, bool& QuitFlag)
{
    std::cout << "Rendering called" << std::endl;
    mirmidivi::PrintText::PrintMessage(Options, MidiReceivedData, MidiInData, QuitFlag);
}
