/*
 * Msg2Txt.hpp
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

#ifndef MIRMIDIVI_RENDERING_MSG2TXT_HPP
#define MIRMIDIVI_RENDERING_MSG2TXT_HPP

#include <string>
#include <sstream>
#include <iomanip>

#include "mirmidivi/fluidsynth.hpp"
#include "mirmidivi/message.hpp"

namespace mirmidivi
{
    std::string Msg2Txt(const std::shared_ptr<mirmidivi::fluidsynth::Synth>& Synth);
} // namespace mirmidivi

#endif

