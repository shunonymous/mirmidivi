/*
 * PianoRoll.cpp
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
#include <functional>
#include <thread>
#include <algorithm>
#include <mutex>
#include <chrono>

#include "mirmidivi/fluidsynth.hpp"
#include "mirmidivi/message.hpp"
#include "mirmidivi/sleep.hpp"
#include "mirmidivi/rendering/PianoRoll.hpp"

namespace mirmidivi
{
    int PianoRoll::structPianoRoll(sysclk::duration NowDuration, fluid_midi_event_t* ev, std::vector<Note>& Notes)
    {
	int Type = fluidsynth::Synth::getType(ev);
	int Ch = fluidsynth::Synth::getChannel(ev);
	int Scale = fluidsynth::Synth::getKey(ev);
	if(Type == NOTE_ON)
	{
	    Note Note;
	    Note.Ch = Ch;
	    Note.Scale = Scale;
	    Note.BeginTime = NowDuration;
	    Note.EndTime = NowDuration;
	    Note.Ringing = true;
	    Notes.push_back(Note);
	} else if(Type == NOTE_OFF)
	{
	    bool Paired = false;
	    if(!Notes.empty())
		for(std::size_t i = Notes.size() - 1; (i >= 0) and !Paired; --i)
		    if((Ch == Notes[i].Ch) and (Scale == Notes[i].Scale))
		    {
			Notes[i].EndTime = NowDuration;
			Notes[i].Ringing = false;
			Paired = true;
		    }
	} else if(Type == ALL_NOTES_OFF)
	    for(auto& Note : Notes)
		if(Note.Ringing)
		{
		    Note.EndTime = NowDuration;
		    Note.Ringing = false;
		}
	return 0;
    }

    std::vector<Note> PianoRoll::getNotes(sysclk::duration Begin, sysclk::duration End)
    {
	std::vector<Note> Result;
	sysclk::duration NowDuration = sysclk::now() - Synth->getBeginTimePoint();
	for(auto& Note : Notes)
	{
	    if(Note.Ringing)
		Note.EndTime = NowDuration;

	    if(Note.EndTime >= Begin and
	       Note.BeginTime <= End)
		Result.push_back(Note);		
	}

	return Result;
    }

    PianoRoll::PianoRoll(const std::shared_ptr<mirmidivi::fluidsynth::Synth>& synth)
    {
	Synth = synth;
	std::function<int(sysclk::duration, fluid_midi_event_t*)> func = std::bind(structPianoRoll, std::placeholders::_1, std::placeholders::_2, std::ref(Notes));
	task_id = Synth->addTask(func);
	Notes.reserve(10'000);
    }

    PianoRoll::~PianoRoll()
    {
	Synth->dropTask(task_id);
    }
}
