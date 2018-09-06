/*
 * mirmidivi is rendering midi as visual
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
#include <thread>
#include <csignal>

#include "mirmidivi/mirmidivi.hpp"
#include "mirmidivi/midi.hpp"

#include "DynamicLoader/DynamicLoader.hpp"


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
    using namespace DynamicLoader;
    Option Options(argc, argv);

    // Signal handling
    QuitFlag = false;
    std::signal(SIGINT, Quit);

    // Midi Data
    MidiReceiver MidiReceivedData;
    MidiUtils MidiInData;

    // Using MIDI API
    std::cout << "MIDI API:" << Options.getMidiInApi()  << std::endl;

    // Dynamic loading MIDI-In Library
    DynamicLoadLibray MidiInLibrary;
    MidiInLibrary.setupLibrary("mirmidivi_" + Options.getMidiInApi(), "MidiIn");
    auto MidiIn = MidiInLibrary.Function<void>("MidiIn").alias<Option, MidiReceiver&, MidiUtils&, bool&>();

    // Dynamic loading rendering library
    DynamicLoadLibray RenderingLibrary;
    RenderingLibrary.setupLibrary("mirmidivi_" + Options.getRenderingApi(), "Rendering");
    auto Rendering = RenderingLibrary.Function<void>("Rendering").alias<Option, MidiReceiver&, MidiUtils&, bool&>();

    // Launch threads
    std::thread MidiInThread(MidiIn, Options, std::ref(MidiReceivedData), std::ref(MidiInData), std::ref(QuitFlag));
    std::thread RenderingThread(Rendering, Options, std::ref(MidiReceivedData), std::ref(MidiInData), std::ref(QuitFlag));

    // Wait thread exit
    MidiInThread.join();
    RenderingThread.join();
}
