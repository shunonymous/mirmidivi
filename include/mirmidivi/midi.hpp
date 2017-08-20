/*
 * midi.hpp - Declaretion midi class members for mirmidivi
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
#include <memory>
#include <mutex>

#include <RtMidi.h>

#include <jdksmidi/world.h>
#include <jdksmidi/midi.h>
#include <jdksmidi/msg.h>
#include <jdksmidi/sysex.h>
#include <jdksmidi/parser.h>

namespace mirmidivi
{
    class MidiReceiver
    {
    private:
	std::vector<::RtMidi::Api> CompiledAPI;
    public:
	std::unique_ptr<RtMidiIn> MidiIn;
	std::vector<unsigned char> MidiRawMessage;
	std::vector<std::pair<::jdksmidi::MIDIClockTime, std::vector<unsigned char>>> MidiCachedMessages;
	double stamp;
	std::mutex Mutex;
	auto getCompiledApi() { return CompiledAPI; }
	MidiReceiver();
    };

    class MidiUtils
    {
    private:
	::jdksmidi::MIDIClockTime tick = 0;
	int MidiTimeBase = 480;
	int Num_Tracks = 2;
	::jdksmidi::MIDITimedBigMessage m;
	int BeatsPerMinute;
	int Tempo = 120;
	void jdksmidiInit();
    public:
	::jdksmidi::MIDITimedBigMessage MidiParsedMessage;
	std::unique_ptr<::jdksmidi::MIDIMultiTrack> MidiTracks;
	void setMidiTick(::jdksmidi::MIDIClockTime Tick) { tick = Tick; }
	auto getMidiTick() { return tick; }
	void setMidiTimeBase(int TimeBase);
	auto getMidiTimeBase() { return MidiTimeBase; }
	void setTextEvent(std::string Text);
	void setBPM(int bpm);
	auto getBPM() { return BeatsPerMinute; }
	auto getNumTracks() { return Num_Tracks; }
	
	MidiUtils();
	MidiUtils(int num_trks, int TimeBase);
    };
}
