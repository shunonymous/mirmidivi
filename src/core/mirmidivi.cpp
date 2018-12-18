/*
 * mirmidivi is rendering midi as visual
 *
 * Copyright (C) 2016-2018 Shun Terabayashi <shunonymous@gmail.com>
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
#include <thread>
#include <csignal>

#include "mirmidivi/mirmidivi.hpp"
#include "mirmidivi/midi.hpp"

#include "dlldr.hpp"


//////////////////////////////////////////////////////////////
// Handle SIGINT signal (Ctrl+C), for quit all loop thread. //
//////////////////////////////////////////////////////////////
bool QuitFlag;

void Quit(int Signal)
{
    QuitFlag = true;
    std::cout << "Interrupt." << std::endl;
}



int main(int argc, char** argv)
{
    using namespace mirmidivi;
    using namespace dlldr;
    Option Options(argc, argv);

    // Signal handling
    QuitFlag = false;
    std::signal(SIGINT, Quit);

    // Midi Data
    MidiReceiver MidiReceivedData;
    MidiUtils MidiInData;

    // Using MIDI API
    std::cout << "MIDI API:" << Options.getMidiInApi()  << std::endl;

    // Dynamic loading Libraries
    auto dlldr_mode =
	shared_library::add_decorations +
	shared_library::search_system_directories;
	
    shared_library MidiInLibrary("mirmidivi_" + Options.MidiInLibname.at(Options.getMidiInApi()), dlldr_mode);
    shared_library RenderingLibrary("mirmidivi_" + Options.RenderLibName.at(Options.getRenderingApi()), dlldr_mode);

    auto MidiIn = MidiInLibrary.get_if<void(Option, MidiReceiver&, MidiUtils&, bool&)>("MidiIn");
    auto Rendering = RenderingLibrary.get_if<void(Option, MidiReceiver&, MidiUtils&, bool&)>("Rendering");
    
    // Launch threads
    std::thread MidiInThread(MidiIn, Options, std::ref(MidiReceivedData), std::ref(MidiInData), std::ref(QuitFlag));
    std::thread RenderingThread(Rendering, Options, std::ref(MidiReceivedData), std::ref(MidiInData), std::ref(QuitFlag));

    // Wait thread exit
    MidiInThread.join();
    RenderingThread.join();
}
