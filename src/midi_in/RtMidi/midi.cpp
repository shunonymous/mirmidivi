/*
 * midi.cpp - Definition midi class members for mirmidivi
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

#include "midi.hpp"

namespace mirmidivi
{
/*    std::vector<std::string> MidiReceiver::getCompiledApi()
    {
	std::vector<std::string> ApiList, CompiledApiList;
	
	ApiList = {"none", "coremidi", "alsa", "jack", "winmm", "dummy"};
	
	return CompiledApiList;
    }
*/  
    MidiReceiver::MidiReceiver()
    {
	// Open port with name
	MidiIn.reset(new RtMidiIn(RtMidi::UNSPECIFIED, "mirmidivi Input Client (by RtMidi)", 100));
	std::cout << "Start" << std::endl;

	// Check available ports.
	unsigned int nPorts = MidiIn->getPortCount();
	std::cout << "Check available ports." << std::endl;
	if (nPorts == 0){
	    std::cout << "No ports available!" << std::endl;
	}

	// Open a Midi input port
	MidiIn->openPort(0,"mirmidivi");
	// Don't ignore sysex, timing, or active sending messages.
	MidiIn->ignoreTypes(false,false,false);
	// Periodically check input queue.
	std::cout <<"Reading MIDI from port with " << MidiIn->getCurrentApi() <<  "... quit with Ctrl+C." << std::endl;
    }

    void MidiUtils::jdksmidiInit()
    {
	MidiTracks.reset(new ::jdksmidi::MIDIMultiTrack(Num_Tracks));

	// Write MIDI Informations(ex.time base) to track
	MidiTracks->SetClksPerBeat(MidiTimeBase);
	tick = 0;
	int track = 0;

	m.SetTime(tick);
	m.SetTimeSig(4, 2); // 4/2^2
	MidiTracks->GetTrack(track)->PutEvent(m);
    }

    void MidiUtils::setMidiTimeBase(int TimeBase)
    {
	MidiTimeBase = TimeBase;
	MidiTracks->SetClksPerBeat(MidiTimeBase);
    }

    void MidiUtils::setTextEvent(std::string Text)
    {
	MidiTracks->GetTrack(0)->PutTextEvent(tick, ::jdksmidi::META_TRACK_NAME, Text.c_str());
    }

    void MidiUtils::setBPM(int bpm)
    {
	BeatsPerMinute = bpm;
	Tempo = ((1e6 * 60) / bpm);
	m.SetTime(tick);
	m.SetTempo(Tempo);
	MidiTracks->GetTrack(0)->PutEvent(m);
    }

    jdksmidi::MIDIClockTime MidiUtils::TimeToTick(std::chrono::system_clock::duration Time)
    {
	return (std::chrono::duration_cast<std::chrono::microseconds>(Time).count()
		/ Tempo / getMidiTimeBase());
    }

    MidiUtils::MidiUtils()
    {
	jdksmidiInit();
    }

    MidiUtils::MidiUtils(int num_trks, int TimeBase)
    {
	Num_Tracks = num_trks;
	MidiTimeBase = TimeBase;
	jdksmidiInit();
    }
}
