/*
 * PianoRoll.hpp
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

#ifndef MIRMIDIVI_RENDERING_PIANOROLL_HPP
#define MIRMIDIVI_RENDERING_PIANOROLL_HPP

#include <array>
#include <vector>
#include <thread>
#include <memory>
#include <chrono>
#include <filesystem>

#include "mirmidivi/fluidsynth.hpp"

namespace mirmidivi
{
    struct Note
    {
	sysclk::duration BeginTime;
	sysclk::duration EndTime;
	bool Ringing;
	int Scale;
	int Ch;
    };
    
    class PianoRoll
    {
    private:
	static int structPianoRoll(sysclk::duration NowDuration, fluid_midi_event_t* ev, std::vector<Note>& Notes);
	static std::size_t searchNoteOff(const std::vector<std::pair<sysclk::duration, fluid_midi_event_t*>>& EventBuffer, std::size_t note_on_index, std::size_t end);

	int task_id;
	std::shared_ptr<fluidsynth::Synth> Synth;
	std::vector<Note> Notes;
    public:
	std::vector<Note> getNotes(sysclk::time_point Begin) { return getNotes(Begin - Synth->getBeginTimePoint()); }
	std::vector<Note> getNotes(sysclk::time_point Begin, sysclk::time_point End) {
	    return getNotes(Begin - Synth->getBeginTimePoint(), End - Synth ->getBeginTimePoint()); }
	std::vector<Note> getNotes(sysclk::duration Begin) { return getNotes(Begin, sysclk::now() - Synth->getBeginTimePoint()); }
	std::vector<Note> getNotes(sysclk::duration Begin, sysclk::duration End);
	std::vector<Note> getNotes() {
//	    for(auto& Note : Notes)
//		if(Note.Ringing)
//		    Note.EndTime = sysclk::now() - Synth->getBeginTimePoint();
	    return Notes;
	};

	PianoRoll(const std::shared_ptr<mirmidivi::fluidsynth::Synth>& synth);
	PianoRoll(){}
	~PianoRoll();
    };
}

#endif

