/*
 * mirmidivi is rendering midi as visual
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
#include <thread>
#include <csignal>

#include "DynamicLoader/DynamicLoader.hpp"
#include "mirmidivi/Options.hpp"
#include "mirmidivi/MidiDataStructure.hpp"

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
    Environment CoreEnv;

    // Signal handling
    QuitFlag = false;
    std::signal(SIGINT, Quit);

    // Set options
    CoreEnv = parseOptions(argc, argv);

    Midi MidiInData;

    // Using MIDI API
    std::cout << "MIDI API:" << CoreEnv.CurrentMidiModule << std::endl;

    // Dynamic load MIDI-In Library
    DynamicLoadLibray MidiInLibrary;
    MidiInLibrary.setupLibrary(CoreEnv.CurrentMidiModule,"MidiIn");
    auto MidiIn = MidiInLibrary.Function<void>("MidiIn").alias<Environment, Midi&,bool&>();

    // Dynamic load rendering library
    DynamicLoadLibray RenderingLibrary;
    RenderingLibrary.setupLibrary(CoreEnv.CurrentRenderModule,"Rendering");
    auto Rendering = RenderingLibrary.Function<void>("Rendering").alias<Environment, Midi&, bool&>();

    // Launch threads
    std::thread MidiInThread(MidiIn, CoreEnv, std::ref(MidiInData), std::ref(QuitFlag));
    std::thread RenderingThread(Rendering, CoreEnv, std::ref(MidiInData), std::ref(QuitFlag));

    MidiInThread.join();
    RenderingThread.join();
}
