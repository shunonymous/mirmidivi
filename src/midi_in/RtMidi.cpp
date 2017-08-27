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
#include <thread>
#include <mutex>

#include "mirmidivi/mirmidivi.hpp"
#include "mirmidivi/sleep.hpp"
#include "mirmidivi/midi.hpp"
namespace mirmidivi
{
    namespace RtMidi
    {
	std::mutex LockMidiCachedMessage;

	void MidiParse(std::vector<std::pair<::jdksmidi::MIDIClockTime, std::vector<unsigned char>>>& MidiCachedTimedMessages, MidiUtils& MidiInData, bool& QuitFlag)
	{
	    jdksmidi::MIDIParser Parser(32 * 1024); // "32 * 1024" is max sysex size (32KiB)

	    while(!QuitFlag)
	    {
		// If MIDI cash(es) exist, parse it
		if(!MidiCachedTimedMessages.empty())
		{
		    // Copy Midi cash. Lock-time necessary that as short as a possible.
		    LockMidiCachedMessage.lock();
		    auto CurrentMidiTimedMessage = MidiCachedTimedMessages;
		    MidiCachedTimedMessages.clear();
		    LockMidiCachedMessage.unlock();
		    for(auto&& TimedMessage : CurrentMidiTimedMessage)
		    {
			MidiInData.MidiParsedMessage.SetTime(TimedMessage.first);
			for(auto&& MidiByte : TimedMessage.second)
			    Parser.Parse(MidiByte, &MidiInData.MidiParsedMessage);

			// Put to track.
			MidiInData.MidiTracks->GetTrack(1)->PutEvent(MidiInData.MidiParsedMessage);
		    }
		    Parser.Clear();
		}
		
		// For avoid SEGV
		sleep(10us);
	    }
	}

	void MidiIn(MidiReceiver& MidiReceivedData, MidiUtils& MidiInData, bool& QuitFlag)
	{
	    double stamp;
	    jdksmidi::MIDIClockTime t;
	    std::vector<std::pair<::jdksmidi::MIDIClockTime, std::vector<unsigned char>>> MidiCachedTimedMessages;

	    // Parse and store MIDI to track using jdksmidi
	    std::thread MidiParseThread(MidiParse, std::ref(MidiCachedTimedMessages), std::ref(MidiInData), std::ref(QuitFlag));

	    std::vector<unsigned char> PrevMessage;
	    auto MidiTimePoint = std::chrono::system_clock::now();

	    while(!QuitFlag){
		// Get MIDI message
		stamp = MidiReceivedData.MidiIn->getMessage(&MidiReceivedData.MidiRawMessage);

		// Store message if message was changed
		if(PrevMessage != MidiReceivedData.MidiRawMessage)
		{
		    // Past time in MIDI tick
		    t = MidiInData.TimeToTick(std::chrono::system_clock::now() - MidiTimePoint);
		    
		    // Cash message.
		    LockMidiCachedMessage.lock();
		    MidiCachedTimedMessages.push_back(std::make_pair(t, MidiReceivedData.MidiRawMessage));
		    LockMidiCachedMessage.unlock();

		    PrevMessage = MidiReceivedData.MidiRawMessage;
		}
		sleep(10ns);
	    } // while(!QuitFlag)
	    MidiParseThread.join();
	} // void MidiIn
    } // namespace RtMidi
} // namespace mirmidivi

// Call from external source    
extern "C" void MidiIn(mirmidivi::Option Options, mirmidivi::MidiReceiver& MidiReceivedData, mirmidivi::MidiUtils& MidiInData, bool& QuitFlag)
{
    std::cout << "MidiIn called" << std::endl;
    mirmidivi::RtMidi::MidiIn(MidiReceivedData, MidiInData, QuitFlag);
}
