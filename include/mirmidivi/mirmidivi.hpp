/*
 * mirmidivi.hpp
 *
 * Copyright (C) 2015-2017 Shun Terabayashi <shunonymous@gmail.com>
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

#ifndef MIRMIDIVI_HPP
#define MIRMIDIVI_HPP

#include <string>
#include <chrono>

#include <jdksmidi/world.h>
#include <jdksmidi/midi.h>
#include <jdksmidi/msg.h>
#include <jdksmidi/sysex.h>
#include <jdksmidi/parser.h>

namespace mirmidivi
{
    // Store options from command-line args (or other)
    class Option
    {
    private:
	// bool RealTime;
	// bool RtMidi;
	float FramePerSecond;
	int VertexResolution;
	int HorizonalResolution;
	// bool FullScreen;
	std::string MidiInApi;
	std::string MidiInPlatform;
	std::string RenderApi;

    public:
	void setFramePerSecond(float fps) { FramePerSecond = fps; }
	float getFramePerSecond(){ return FramePerSecond; };

	std::string getMidiInApi(){ return MidiInApi; }
	std::string getMidiInPlatform(){ return MidiInPlatform; }
	std::string getRenderingApi(){ return RenderApi; }
	Option(int argc, char** argv);
    }; // class Option

    class Midi
    {
    private:
	jdksmidi::MIDIMessage InputMidiData;
	std::vector<jdksmidi::MIDIMessage> StoredMidi;
    public:
	
    };	

    // aliases
    using sysclk = std::chrono::system_clock;

} // namespace mirmidivi

#endif
