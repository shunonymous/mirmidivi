/*
 * RtMidi.cpp - Input MIDI using RtMidi for mirmidivi
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
#include <cstdlib>
#include <signal.h>
#include <stk/RtMidi.h>

#include "mirmidivi/MidiDataStructure.hpp"
#include "mirmidivi/Options.hpp"
#include "mirmidivi/sleep.hpp"

namespace mirmidivi
{
    namespace RtMidi
    {
	void MidiIn(Midi& MidiInData, bool& QuitFlag)
	{
	    RtMidiIn *midiin = new RtMidiIn();
	    std::vector<unsigned char> message;
	    int nBytes,i;
	    double stamp;

	    std::cout << "Start" << std::endl;
	    std::vector<::RtMidi::Api> CompiledAPI;
	    midiin->getCompiledApi(CompiledAPI);
	    std::cout << "Enable ";
	    for(auto elem:CompiledAPI)
		std::cout << elem;
	    std::cout << std::endl;

	    // Check available ports.
	    unsigned int nPorts = midiin->getPortCount();
	    std::cout << "Check available ports." << std::endl;
	    if (nPorts == 0){
		std::cout << "No ports available!" << std::endl;
		goto cleanup;
	    }

	    // Open a Midi input port
	    midiin->openPort(0,"mirmidivi");
	    // Don't ignore sysex, timing, or active sending messages.
	    midiin->ignoreTypes(false,false,false);
	    // Periodically check input queue.
//	    std::cout <<"Reading MIDI from port with " << midiin->getCurrentApi() <<  "... quit with Ctrl+C." << std::endl;
	    while(!QuitFlag){
		stamp = midiin->getMessage(&message);
		nBytes = message.size();
		for(i=0;i<nBytes;i++)
		    MidiInData.syncData(static_cast<int>(message[i]));
		sleep(10us);
	    }

	cleanup:
	    delete midiin;
	} // void Initialize
    } // namespace RtMidi
} // namespace mirmidivi

// Call from external source    
extern "C" void MidiIn(mirmidivi::Environment CoreEnv, mirmidivi::Midi& MidiInData, bool& QuitFlag)
{
    std::cout << "MidiIn called" << std::endl;
    mirmidivi::RtMidi::MidiIn(MidiInData, QuitFlag);
}
