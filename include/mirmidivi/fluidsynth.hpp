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

#ifndef MIRMIDIVI_FLUIDSYNTH_HPP
#define MIRMIDIVI_FLUIDSYNTH_HPP

#include <memory>
#include <filesystem>
#include <chrono>
#include <vector>
#include <functional>
#include <map>
#include <mutex>

#include <fluidsynth.h>

#include "mirmidivi/mirmidivi.hpp"

namespace mirmidivi
{
    namespace fluidsynth
    {
	class Synth;
	class Player;
	struct Controller;

	using Task = std::function<int(sysclk::duration, fluid_midi_event_t*)>;
	
	class Synth
	{
	private:
	    fluid_settings_t* settings;
	    static std::vector<fluid_synth_t*> synth_callback; // due to callback
	    static int synth_count;
	    fluid_synth_t* synth;
	    fluid_midi_driver_t* midi_driver;
	    fluid_audio_driver_t* audio_driver;
	    int synth_id;
	    int sound_font_id;
	    static std::vector<fluid_midi_event_t*> event_callback; // due to callback
	    fluid_midi_event_t* event;
	    std::map<int, Task> tasks;
	    int task_id_count = 0;
	    sysclk::time_point begin;
	public:
	    std::mutex mtx_callback;
	    void mkSettings(const Option& Options);
	    void launchSmfPlayer(const Option& Options);
	    void launchMidiDriver(const Option& Options);
	    void launchAudioDriver(const Option& Options);

	    fluid_midi_event_t* getEvent() const { return event; }
	    void setEvent(fluid_midi_event_t* e) { event = e; }
	    
	    fluid_synth_t* getSynth() const { return synth; }

	    // Get message statuses
	    int getType() const { return fluid_midi_event_get_type(getEvent()); }
	    int getChannel() const { return fluid_midi_event_get_channel(getEvent()); }
	    int getKey() const { return fluid_midi_event_get_key(getEvent()); }
	    int getVelocity() const { return fluid_midi_event_get_velocity(getEvent()); }
	    int getControl() const { return fluid_midi_event_get_control(getEvent()); }
	    int getValue() const { return fluid_midi_event_get_value(getEvent()); }
	    int getProgram() const { return fluid_midi_event_get_program(getEvent()); }
	    int getPitchbend() const { return fluid_midi_event_get_pitch(getEvent()); }

	    static int getType(fluid_midi_event_t* ev) { return fluid_midi_event_get_type(ev); }
	    static int getChannel(fluid_midi_event_t* ev) { return fluid_midi_event_get_channel(ev); }
	    static int getKey(fluid_midi_event_t* ev) { return fluid_midi_event_get_key(ev); }
	    static int getVelocity(fluid_midi_event_t* ev) { return fluid_midi_event_get_velocity(ev); }
	    static int getControl(fluid_midi_event_t* ev) { return fluid_midi_event_get_control(ev); }
	    static int getValue(fluid_midi_event_t* ev) { return fluid_midi_event_get_value(ev); }
	    static int getProgram(fluid_midi_event_t* ev) { return fluid_midi_event_get_program(ev); }
	    static int getPitchbend(fluid_midi_event_t* ev) { return fluid_midi_event_get_pitch(ev); }

	    // Hendle function calling when received message
	    static int handleEvent(void* Data, fluid_midi_event_t* Event);

	    // Tasks
	    int addTask(Task t) {
		tasks.insert({task_id_count, t});
		return task_id_count++;
	    }
	    void dropTask(int id) { tasks.erase(id); }
	    Task getTask(int id) { return tasks.at(id); }
	    std::map<int, Task> getTasks() { return tasks; }

	    sysclk::time_point getBeginTimePoint() { return begin; };

	    // Constructor
	    Synth();
	    Synth(const Option& Options);
	    ~Synth();

	    Synth& operator()(const Option& Options) { return *this; }
	    Synth& operator=(Synth&& Synth) { return *this; }

	    friend Player;
	};

	class Player
	{
	private:
	    std::shared_ptr<Synth> Synth;
	    void launchSmfPlayer(const Option& Options);

	    fluid_player_t* smf_player;
	public:
	    std::shared_ptr<class Synth>& getSynth() { return Synth; }

	    void addToQueue(std::filesystem::path File) { fluid_player_add(smf_player, File.c_str()); }
	    void play() { fluid_player_play(smf_player); }
	    void stop() { fluid_player_stop(smf_player); }
	    void waitForEnd() { fluid_player_join(smf_player); }
	    void setTempo(int Tempo) { fluid_player_set_midi_tempo(smf_player, Tempo); }
	    void setBpm(int Bpm) { fluid_player_set_bpm(smf_player, Bpm); }
	    fluid_player_status getStatus() { return static_cast<fluid_player_status>(fluid_player_get_status(smf_player)); }
	    int getCurrentTick() { return fluid_player_get_current_tick(smf_player); }
	    int getTotalTicks() { return fluid_player_get_total_ticks(smf_player); }
	    int getBpm() { return fluid_player_get_bpm(smf_player); }
	    int getTempo() { return fluid_player_get_midi_tempo(smf_player); }
	    void seek(int Tick) { fluid_player_seek(smf_player, Tick); }

	    Player();
	    Player(const Option& Options);
	    ~Player(){}
	};
    }
}

#endif
