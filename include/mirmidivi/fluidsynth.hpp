/*
 * fluidsynth.hpp - Using fluidsynth for mirmidivi
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

#include <memory>
#include <filesystem>
#include <vector>

#include <fluidsynth.h>

#include "mirmidivi/mirmidivi.hpp"

namespace mirmidivi
{
    namespace fluidsynth
    {
	class Synth
	{
	private:
	    fluid_settings_t* settings;
	    static std::vector<fluid_synth_t*> synth; // due to callback
	    fluid_midi_driver_t* midi_driver;
	    fluid_player_t* smf_player;
	    fluid_audio_driver_t* audio_driver;
	    int synth_id;
	    int sound_font_id;
	    static std::vector<fluid_midi_event_t*> event; // due to callback
	public:
	    void mkSettings(const Option& Options);
	    void launchSmfPlayer(const Option& Options);
	    void launchMidiDriver(const Option& Options);
	    void launchAudioDriver(const Option& Options);

	    fluid_midi_event_t* getEvent() const { return event[synth_id]; }
	    void setEvent(fluid_midi_event_t* e) { event[synth_id] = e; }
	    fluid_synth_t* getSynth() const { return synth[synth_id]; }

	    // Get message statuses
	    int getType() const { return fluid_midi_event_get_type(getEvent()); }
	    int getChannel() const { return fluid_midi_event_get_channel(getEvent()); }
	    int getKey() const { return fluid_midi_event_get_key(getEvent()); }
	    int getVelocity() const { return fluid_midi_event_get_velocity(getEvent()); }
	    int getControl() const { return fluid_midi_event_get_control(getEvent()); }
	    int getValue() const { return fluid_midi_event_get_value(getEvent()); }
	    int getProgram() const { return fluid_midi_event_get_program(getEvent()); }
	    int getPitchbend() const { return fluid_midi_event_get_pitch(getEvent()); }

	    // Hendle function calling when received message
	    static int handleEvent(void* Data, fluid_midi_event_t* Event);

	    // Constructor
	    Synth();
	    Synth(const Option& Options);
	    ~Synth();
	};
    }
}
