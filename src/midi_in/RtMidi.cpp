/*
 * RtMidi.cpp - Input MIDI using RtMidi for mirmidivi
 *
 * Copyright (C) 2016-2017 Shun Terabayashi <shunonymous@gmail.com>
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
#include <cstdlib>
#include <signal.h>

#include "mirmidivi/mirmidivi.hpp"
#include "mirmidivi/sleep.hpp"
#include "mirmidivi/midi.hpp"
namespace mirmidivi
{
    namespace RtMidi
    {
	void MidiIn(MidiReceiver& MidiReceivedData, MidiUtils& MidiInData, bool& QuitFlag)
	{
	    double stamp;
	    jdksmidi::MIDIClockTime t;

	    std::vector<unsigned char> PrevMessage;
	    auto MidiTimePoint = std::chrono::system_clock::now();

	    while(!QuitFlag){
		// Get MIDI message
		stamp = MidiReceivedData.MidiIn->getMessage(&MidiReceivedData.MidiRawMessage);

		// Store message if message was changed
		if(PrevMessage != MidiReceivedData.MidiRawMessage)
		{
		    // Past time in MIDI tick
		    t = (std::chrono::duration_cast<std::chrono::nanoseconds>
			 (std::chrono::system_clock::now() - MidiTimePoint).count())
			/ (60.00 * 1e9 / MidiInData.getBPM() / MidiInData.getMidiTimeBase()
			    );
		    
		    PrevMessage = MidiReceivedData.MidiRawMessage;

		    // Store message 
		    MidiReceivedData.Mutex.lock();
		    MidiReceivedData.MidiCachedMessages.push_back(std::make_pair(t, MidiReceivedData.MidiRawMessage));
		    MidiReceivedData.Mutex.unlock();
		}
		sleep(10us);
	    } // while(!QuitFlag)
	} // void MidiIn
    } // namespace RtMidi
} // namespace mirmidivi

// Call from external source    
extern "C" void MidiIn(mirmidivi::Option Options, mirmidivi::MidiReceiver& MidiReceivedData, mirmidivi::MidiUtils& MidiInData, bool& QuitFlag)
{
    std::cout << "MidiIn called" << std::endl;
    mirmidivi::RtMidi::MidiIn(MidiReceivedData, MidiInData, QuitFlag);
}
