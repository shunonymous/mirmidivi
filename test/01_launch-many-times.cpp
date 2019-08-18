/*
 * 01_launch-many-times.cpp
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
#include <memory>
#include <csignal>
#include <algorithm>
#include <thread>

#include "mirmidivi/mirmidivi.hpp"
#include "mirmidivi/sleep.hpp"

#include "dlldr.hpp"

// Handle SIGINT signal (Ctrl+C), for quit all loop thread. 
bool QuitFlag;

void Quit(int Signal)
{
    QuitFlag = true;
    std::cout << "Interrupt." << std::endl;
}

void DelayFlagSwitch(bool& Flag)
{
    using namespace mirmidivi;
    Flag = false;
    sleep(2s);
    Flag = true;
}

int main(int argc, char** argv)
{
    try
    {
	using namespace mirmidivi;
	using namespace dlldr;
	for(int i = 0; i < 10; ++i)
	{
	    std::cout << "Passed " << i << "time(s)." << std::endl;
	    Option Options(argc, argv);

	    // Signal handling
	    QuitFlag = false;
	    std::signal(SIGINT, Quit);
	    std::thread DelayFlagSwitchThread(DelayFlagSwitch,std::ref(QuitFlag));

	    bool LibraryNotFound = std::find(UiToolKitList.begin(), UiToolKitList.end(), Options.getUiToolKit()) == UiToolKitList.end();
	    // When not unsafe mode, GUI Toolkit API must listed
	    if(!Options.getUnsafeMode() and LibraryNotFound)
		throw "mirmidivi has not " + Options.getRenderingApi();
    
	    // Load UI ToolKit
	    shared_library UiToolKitLibrary("mirmidivi_" + Options.getUiToolKit(), dlldr_mode);
	    auto UiToolKit = UiToolKitLibrary.get_if<void(Option&, bool&)>("InitUiToolKit");
	    UiToolKit(std::ref(Options), std::ref(QuitFlag));
	    DelayFlagSwitchThread.join();
	    UiToolKitLibrary.reset();
	}
    }
    catch(std::exception& e)
    {
	std::cerr << "Error:" << e.what() << std::endl;
    }
    std::cout << "End of mirmidivi" << std::endl;
}
