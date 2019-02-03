/*
 * mirmidivi is rendering midi as visual
 *
 * Copyright (C) 2016-2019 Shun Terabayashi <shunonymous@gmail.com>
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
#include "mirmidivi/fluidsynth.hpp"

#include "dlldr.hpp"

// Handle SIGINT signal (Ctrl+C), for quit all loop thread. //
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

    // Dynamic loading Libraries
    auto dlldr_mode =
	shared_library::add_decorations +
	shared_library::search_system_directories;
	
    fluidsynth::Synth Synth(Options);

    // Load rendering library
    shared_library RenderingLibrary("mirmidivi_" + Options.RenderLibName.at(Options.getRenderingApi()), dlldr_mode);
    auto Rendering = RenderingLibrary.get_if<void(Option, fluidsynth::Synth&, bool&)>("Rendering");

    // launch thread
    std::thread RenderingThread(Rendering, Options, std::ref(Synth), std::ref(QuitFlag));

    // Wait thread exit
    RenderingThread.join();
}
