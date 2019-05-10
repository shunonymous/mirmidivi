/*
 * fluidsynth.cpp - Using fluidsynth for mirmidivi
 *
 * Copyright (C) 2018-2019 Shun Terabayashi <shunonymous@gmail.com>
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

#include <string>
#include <iostream>

#include <fluidsynth.h>

#include "mirmidivi/mirmidivi.hpp"
#include "mirmidivi/fluidsynth.hpp"

namespace mirmidivi
{
    namespace fluidsynth
    {
	// Initialize static values.
	std::vector<fluid_synth_t*> Synth::synth = {};
	std::vector<fluid_midi_event_t*> Synth::event = {};

	void Synth::mkSettings(const Option& Options)
	{
	    // Settings for fluidsynth
	    settings = new_fluid_settings();

	    // Midi driver
	    if(!Options.getMidiDriver().empty())
		fluid_settings_setstr(settings, "midi.driver", Options.getMidiDriver().c_str());
	    fluid_settings_setstr(settings, "midi.portname", "fluidsynth for mirmidivi");
	    fluid_settings_setint(settings, "midi.autoconnect", 1);

	    // Audio Driver
	    if(!Options.getAudioDriver().empty())
		fluid_settings_setstr(settings, "audio.driver", Options.getAudioDriver().c_str());
	    else {
		// WORKAROUND:
		// fluidplayer not working without audio driver,
		// so create for output to null device
		fluid_settings_setstr(settings, "audio.driver", "file");
#if defined (__unix)
		fluid_settings_setstr(settings, "audio.file.name", "/dev/null");
#elif defined (_WIN32)
		fluid_settings_setstr(settings, "audio.file.name", "nul");
#endif
	    }
	}

	int Synth::handleEvent(void* Data, fluid_midi_event_t* Event)
	{
	    Synth* synth = static_cast<Synth*>(Data);
	    fluid_synth_handle_midi_event(synth->getSynth(), Event);
	    synth->setEvent(Event);
	    for(const auto& f : synth->getTasks())
		f.second(sysclk::now() - synth->getBeginTimePoint(), Event);
	    return 0;
	}

	void Synth::launchMidiDriver(const Option& Options)
	{
	    midi_driver = new_fluid_midi_driver(settings, handleEvent, this);
	}

	void Synth::launchAudioDriver(const Option& Options)
	{
	    audio_driver = new_fluid_audio_driver(settings, synth[synth_id]);
	    for(const auto& sf : Options.getSoundFontsPath())
	    {
		std::cout << sf.c_str() << std::endl;
		fluid_synth_sfload(synth[synth_id], sf.c_str(), 1);
	    }
	}

	Synth::Synth(const Option& Options)
	{
	    synth_id = event.size();
	    event.push_back(new_fluid_midi_event());

	    // Settings part
	    mkSettings(Options);
	    synth.push_back(new_fluid_synth(settings));

	    // Midi Driver
	    if(Options.getFluidSynthMode() == SYNTH)
		launchMidiDriver(Options);


	    // Audio Driver
//	    if(Options.getAudioEnableFlag()) // Comment out due to workaround
		launchAudioDriver(Options);

	    begin = sysclk::now();
	}

	Synth::~Synth()
	{
	    /*
	    if(settings)
		delete_fluid_settings(settings);
	    if(synth)
		delete_fluid_synth(synth);
	    if(midi_driver)
		delete_fluid_midi_driver(midi_driver);
	    if(smf_player)
		delete_fluid_player(smf_player);
	    if(audio_driver)
		delete_fluid_audio_driver(audio_driver);
	    */
	}

	void Player::launchSmfPlayer(const Option& Options)
	{
	    smf_player = new_fluid_player(getSynth()->getSynth());
	    fluid_player_set_playback_callback(smf_player, Synth::handleEvent, getSynth().get());
	    for(const auto& p : Options.getSmfFilePath())
		fluid_player_add(smf_player, p.c_str());
	}

	Player::Player(const Option& Options)
	{
	    Synth = std::shared_ptr<class Synth>(new class Synth(Options));

	    // SMF Player
	    if(Options.getFluidSynthMode() == PLAYER)
		launchSmfPlayer(Options);

	    // SMF Player play
	    if(Options.getFluidSynthMode() == PLAYER)
		fluid_player_play(smf_player);
	}
    }
}
