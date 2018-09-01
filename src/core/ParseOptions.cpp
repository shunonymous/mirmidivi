/*
 * ParseOptions.cpp
 *
 * This is a part of mirmidivi.
 *
 * Copyright (C) 2015-2016, 2018 Shun Terabayashi <shunonymous@gmail.com>
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
#include <boost/program_options.hpp>

#include "mirmidivi/mirmidivi.hpp"

namespace mirmidivi
{
    Option parseOptions(int argc, char** argv)
    {
	namespace po = boost::program_options;

	Option Options;
	bool ExitFlag = false;

	// Declare mirmidivi options 
	po::options_description Core("Options");
	po::options_description Midi("MIDI");
	po::options_description Render("Rendering");

	// Set options
	Core.add_options()
	    ("help,h", "Print this help and exit.")
	    ("rendering-api,g", po::value<std::string>(), "The name of the rendering API to use [text, curses].")
	    ;

	// About MIDI
	Midi.add_options()
	    ("midi-in-api,m", po::value<std::string>(), "API for input MIDI.\n")
	    ;

	// About render
	Render.add_options()
	    ("fps,f", po::value<float>(&Options.FramePerSecond), "Set frame per second.")
	    ;
	
	po::variables_map vm;

	// Merge options
	Core.add(Midi).add(Render);

	try{

	    // Store options
	    po::store(po::parse_command_line(argc, argv, Core), vm);
	    po::notify(vm);

	    // Print help message and exit
	    if(vm.count("help"))
	    {
		std::cout << Core << std::endl;
		exit(0);
	    }
	    
	    std::string API;
	    // Midi API
	    if(vm.count("midi-api"))
	    {
		API = vm["midi-api"].as<std::string>();
		if(API == "RtMidi")
		    Options.InputMidiAPI = "RtMidi";
		else
		{
		    std::cerr << "mirmidivi has not " << vm["midi-api"].as<std::string>() << " for midi-in." << std::endl;
		    ExitFlag = true;
		}
	    }else{
		// If nothing to set midi-api
		Options.InputMidiAPI = "RtMidi";
	    }

	    // Rendering API
	    if(vm.count("rend-api"))
	    {
		API = vm["rend-api"].as<std::string>();
		if(API == "text")
		    Options.RenderAPI = "PrintMessage";
		else if(API == "curses")
		    Options.RenderAPI = "curses";
		else
		{
		    std::cerr << "mirmidivi has not " << vm["rend-api"].as<std::string>() << " for rendering." << std::endl;
		    ExitFlag = true;
		}
	    }else{
		// If nothing to set rend-api
		Options.RenderAPI = "PrintMessage";
	    }
	} // try

	// Error handling
	catch(std::exception& e)
	{
	    std::cerr << "Error: " << e.what() << std::endl;
	    std::cerr << Core << std::endl;
	    ExitFlag = true;
	}
	
	catch(...)
	{
	    std::cerr << "Exception of unknown type." << std::endl;
	    ExitFlag = true;
	}

	if(ExitFlag == true)
	    exit(1);

	return Options;
    } // bool ParseOptions(int argc,char **argv)
} // namespace mirmidivi
