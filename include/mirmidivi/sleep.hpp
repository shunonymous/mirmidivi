/*
 * sleep.hpp
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
 *
 */

#ifndef MIRMIDIVI_SLEEP_HPP
#define MIRMIDIVI_SLEEP_HPP

#include <chrono>
#include <thread>

namespace mirmidivi
{
    // For using units of time. (s, ms, us, etc...)
    using namespace std::chrono_literals;

    inline void sleep(std::chrono::duration<double> time)
    {
	return std::this_thread::sleep_for(time);
    }
}

#endif
