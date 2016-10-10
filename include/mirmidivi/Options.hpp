/*
 * midmidivi.hpp - Declare class and functions
 *
 * Copyright (C) 2015-2016 Shun Terabayashi <shunonymous@gmail.com>
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

namespace mirmidivi
{
    class MidiEnvironment
    {
    public:
	bool RealTime;
	// bool RtMidi;
	unsigned int BufferMilliSecond;
    }; // class MidiEnvironment

    class RenderEnvironment
    {
    public:
	float FramePerSecond = 30.0;
	int VertexResolution;
	int HorizonalResolution;
	bool FullScreen;
    }; // class RenderEnvironment

    class Environment : public MidiEnvironment,public RenderEnvironment
    {
    public:
	std::string CurrentMidiModule;
	std::string CurrentRenderModule;
    }; // class Environment : public MidiEnvironment,public RenderEnvironment

    Environment parseOptions(int argc,char** argv);

} // namespace mirmidivi

#endif
