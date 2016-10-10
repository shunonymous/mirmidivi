/*
 * ParseOptions.cpp
 *
 * This is a part of mirmidivi.
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
 *
 * Referenced from boost's sample codes(http://www.boost.org/doc/libs/1_59_0/libs/program_options/example)
 *
 */

#include <iostream>
#include <iterator>
#include <vector>
#include <boost/program_options.hpp>

#include "mirmidivi/Options.hpp"

namespace mirmidivi
{
    Environment parseOptions(int argc,char** argv)
    {
	namespace po = boost::program_options;

	Environment CoreEnv;
	bool ExitFlag = false;

	// Declare mirmidivi options 
	po::options_description Core("Options");
	po::options_description Midi("MIDI");
	po::options_description Render("Rendering");

	// Set options
	Core.add_options()
	    ("help,h","Print this help and exit.")
	    ("midi-api,m",po::value<std::string>(),"The name of the MIDI API to use [RtMidi].")
	    ("rend-api,g",po::value<std::string>(),"The name of the rendering API to use [opengl(WIP),text,ncurses(WIP)].")
	    // ("config-module,x",po::value<std::string>(),"Configure module. [MODULE:OPTIONS]")
	    ;
	// About MIDI
	Midi.add_options()
	    ("realtime,r","Enable realtime mode.")
	    ("buffer,b",po::value<unsigned int>(&CoreEnv.BufferMilliSecond),"Set buffer length in ms.")
	    ;
	// About render
	Render.add_options()
	    ("fps,f",po::value<float>(&CoreEnv.FramePerSecond),"Set frame per second.")
	    ("size,s",po::value<std::vector<int> >()->multitoken(),"Set drawing region size to use [(horizonal) (vertex)]")
	    ;
	
	po::variables_map vm;

	// Merge options
	Core.add(Midi).add(Render);

	try{

	    // Store options
	    po::store(po::parse_command_line(argc,argv,Core),vm);
	    po::notify(vm);

	    // Print help message and exit
	    if(vm.count("help"))
	    {
		std::cout << Core << std::endl;
		exit(0);
	    }
	    
	    // Realtime Mode
	    if(vm.count("realtime"))
		CoreEnv.RealTime = true;

	    // Drow region(flame) size
	    if(vm.count("size"))
	    {
		if(vm["size"].as<std::vector<int>>().size() == 2)
		{
		    CoreEnv.HorizonalResolution = vm["size"].as<std::vector<int> >()[0];
		    CoreEnv.VertexResolution = vm["size"].as<std::vector<int> >()[1];
		}
		else
		{
		    std::cerr << "Error: --size option requires only 2 arguments." << std::endl;
		}
	    }

	    std::string API;
	    // Midi API
	    if(vm.count("midi-api"))
	    {
		API = vm["midi-api"].as<std::string>();
		if(API == "RtMidi")
		    CoreEnv.CurrentMidiModule = "RtMidi";
		else
		{
		    std::cerr << "mirmidivi has not " << vm["midi-api"].as<std::string>() << " for midi-in." << std::endl;
		    ExitFlag = true;
		}
	    }else{
		// If nothing to set midi-api
		CoreEnv.CurrentMidiModule = "RtMidi";
	    }

	    // Rendering API
	    if(vm.count("rend-api"))
	    {
		API = vm["rend-api"].as<std::string>();
		if(API == "text")
		    CoreEnv.CurrentRenderModule = "PrintMessage";
		else
		{
		    std::cerr << "mirmidivi has not " << vm["rend-api"].as<std::string>() << " for rendering." << std::endl;
		    ExitFlag = true;
		}
	    }else{
		// If nothing to set rend-api
		CoreEnv.CurrentRenderModule = "PrintMessage";
	    }
	    // Fullscreen
	    if(vm.count("fullscreen"))
		CoreEnv.FullScreen = true;
	    
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

	return CoreEnv;
    } // bool ParseOptions(int argc,char **argv)
} // namespace mirmidivi
