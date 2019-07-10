/*
 * no-toolkit.cpp
 *
 * Copyright (C) 2019 Shun Terabayashi <shunonymous@gmail.com>
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
#include <algorithm>

#include "mirmidivi/mirmidivi.hpp"
#include "mirmidivi/fluidsynth.hpp"
#include "mirmidivi/sleep.hpp"

#include "dlldr.hpp"

extern "C" void InitUiToolKit(mirmidivi::Option& Options, bool& QuitFlag)
{
    std::cout << "no-toolkit loaded" << std::endl;

    using namespace mirmidivi;
    using namespace dlldr;

    // When not unsafe mode, GUI Toolkit API must listed
    if(!Options.getUnsafeMode() and
       std::find(RenderingLibrariesList.begin(), RenderingLibrariesList.end(), Options.getRenderingApi()) == RenderingLibrariesList.end())
	throw "mirmidivi has not " + Options.getRenderingApi();

    // Dynamic loading Libraries
    const auto dlldr_mode =
	shared_library::add_decorations +
	shared_library::search_system_directories;

    std::thread RenderingThread;
    shared_library RenderingLibrary("mirmidivi_" + Options.getRenderingApi(), dlldr_mode);
    if(Options.getFluidSynthMode() == SYNTH)
    {
	std::shared_ptr<fluidsynth::Synth> Synth(new fluidsynth::Synth(Options));
    
	auto Rendering = RenderingLibrary.get_if<void(Option&, std::shared_ptr<fluidsynth::Synth>, bool&)>("SynthRendering");
	RenderingThread = std::thread(Rendering, std::ref(Options), Synth, std::ref(QuitFlag));
    }
    else if(Options.getFluidSynthMode() == PLAYER)
    {
	std::shared_ptr<fluidsynth::Player> Player(new fluidsynth::Player(Options));
    
	auto Rendering = RenderingLibrary.get_if<void(Option&, std::shared_ptr<fluidsynth::Player>, bool&)>("PlayerRendering");
	RenderingThread = std::thread(Rendering, std::ref(Options), Player, std::ref(QuitFlag));
    }

    RenderingThread.join();
}
