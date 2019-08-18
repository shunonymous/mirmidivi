/*
 * render-selecter.hpp
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

#ifndef MIRMIDIVI_RENDER_SELECTER_HPP
#define MIRMIDIVI_RENDER_SELECTER_HPP

#include <algorithm>
#include <thread>

#include "dlldr.hpp"

#include "mirmidivi.hpp"
#include "fluidsynth.hpp"

namespace mirmidivi
{
    void RenderSelecter(std::thread& RenderingThread, Option& Options, bool& QuitFlag)
    {
	using namespace dlldr;

        bool LibraryNotFound = std::find(RenderingLibrariesList.begin(), RenderingLibrariesList.end(), Options.getRenderingApi()) == RenderingLibrariesList.end();

	// When not unsafe mode, GUI Toolkit API must listed
	if(!Options.getUnsafeMode() and LibraryNotFound)
	{
	    throw "mirmidivi has not " + Options.getRenderingApi();
	}
    
	static shared_library RenderingLibrary("mirmidivi_" + Options.getRenderingApi(), dlldr_mode);
	if(Options.getFluidSynthMode() == SYNTH)
	{
	    static std::shared_ptr<fluidsynth::Synth> Synth(new fluidsynth::Synth(Options));
    
	    auto Rendering = RenderingLibrary.get_if<void(Option&, std::shared_ptr<fluidsynth::Synth>, bool&)>("SynthRendering");
	    RenderingThread = std::thread(Rendering, std::ref(Options), std::static_pointer_cast<fluidsynth::Synth>(Synth), std::ref(QuitFlag));
	}
	else if(Options.getFluidSynthMode() == PLAYER)
	{
	    static std::shared_ptr<fluidsynth::Player> Player(new fluidsynth::Player(Options));
    
	    auto Rendering = RenderingLibrary.get_if<void(Option&, std::shared_ptr<fluidsynth::Player>, bool&)>("PlayerRendering");
	    RenderingThread = std::thread(Rendering, std::ref(Options), std::static_pointer_cast<fluidsynth::Player>(Player), std::ref(QuitFlag));
	}
    }
}
#endif
