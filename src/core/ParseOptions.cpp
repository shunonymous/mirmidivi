/*
 * ParseOptions.cpp
 *
 * This is a part of mirmidivi.
 *
 * Copyright (C) 2015-2016, 2018-2019 Shun Terabayashi <shunonymous@gmail.com>
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
 *
 * Referenced from boost's sample codes(http://www.boost.org/doc/libs/1_59_0/libs/program_options/example)
 *
 */

#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <filesystem>

#include <boost/program_options.hpp>

#include "mirmidivi/mirmidivi.hpp"

namespace mirmidivi
{
    Option::Option(int argc, char** argv)
    {
	namespace po = boost::program_options;

	bool ExitFlag = false;

	// Declare mirmidivi options 
	po::options_description Base("Options");
	po::options_description FluidSynth("FluidSynth");
	po::options_description Render("Rendering");

	// Set options
	Base.add_options()
	    ("help,h", "Print this help and exit.")
	    ("rendering-api,g", po::value<std::string>(), "The name of the rendering API to use [text, curses].")
	    ;

	// About MIDI
	FluidSynth.add_options()
	    ("midi-driver", po::value<std::string>(&MidiDriver),
	     "Select MIDI driver. \n"
	     "[alsa_raw, alsa_seq, coremidi, jack, midishare, oss, winmidi]")
	    ("enable-audio,a", "Enable audio.")
	    ("audio-driver", po::value<std::string>(&AudioDriver),
	     "Select Audio driver.\n"
	     "[alsa, coreaudio, dart, dsound, file, jack, oss, portaudio, pulseaudio, sdl2, sndman, waveout]")
	    ("soundfont", po::value<std::vector<std::string>>()->multitoken(),
	     "Set soundfont(s) path")
	    ("smf", po::value<std::vector<std::filesystem::path>>(&SmfFilePath)->multitoken(), "MIDI file path for playing. (WIP)")
	    ;

	// About render
	Render.add_options()
	    ("fps,f", po::value<float>(&FramePerSecond), "Set frame per second.")
	    ;
	
	po::variables_map vm;

	// Merge options
	Base.add(FluidSynth).add(Render);

	try{
	    // Store options
	    po::store(po::parse_command_line(argc, argv, Base), vm);
	    po::notify(vm);

	    // Print help message and exit
	    if(vm.count("help"))
	    {
		std::cout << Base << std::endl;
		exit(0);
	    }
	    
	    if(vm.count("enable-audio") or vm.count("audio-driver"))
		EnableAudio = true;

	    if(vm.count("soundfont"))
		for(const auto& s : vm["soundfont"].as<std::vector<std::string>>())
		    SoundFontsPath.push_back(std::filesystem::path(s));

	    if(vm.count("smf"))
		FluidSynthMode = PLAYER;
	    else
		FluidSynthMode = SYNTH;

	    // Rendering API
	    if(vm.count("rendering-api"))
	    {
		std::string API;

		API = vm["rendering-api"].as<std::string>();
		if(API == "text")
		    RenderingApi = TEXT;
		else if(API == "curses")
		    RenderingApi = CURSES;
		else
		{
		    std::cerr << "mirmidivi has not " << vm["rend-api"].as<std::string>() << " for rendering." << std::endl;
		    ExitFlag = true;
		}
	    } else
		// If nothing to set rend-api
		RenderingApi = TEXT;
	} // try

	// Error handling
	catch(std::exception& e)
	{
	    std::cerr << "Error: " << e.what() << std::endl;
	    std::cerr << Base << std::endl;
	    ExitFlag = true;
	}
	
	catch(...)
	{
	    std::cerr << "Exception of unknown type." << std::endl;
	    ExitFlag = true;
	}

	if(ExitFlag == true)
	    exit(1);
    } // bool ParseOptions(int argc,char **argv)
} // namespace mirmidivi
