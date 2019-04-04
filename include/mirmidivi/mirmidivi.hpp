/*
 * mirmidivi.hpp
 *
 * Copyright (C) 2015-2019 Shun Terabayashi <shunonymous@gmail.com>
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

#ifndef MIRMIDIVI_HPP
#define MIRMIDIVI_HPP

#include <string>
#include <chrono>
#include <unordered_map>
#include <filesystem>

namespace mirmidivi
{
    enum RenderApi{CURSES, TEXT};
    enum FluidSynthMode{SYNTH, PLAYER};

    // Store options from command-line args (or other)
    class Option
    {
    private:
	// bool RealTime;
	bool EnableAudio = false;
	float FramePerSecond = 60.0;
//	int VertexResolution;
//	int HorizonalResolution;
	// bool FullScreen;
	std::string AudioDriver;
	std::string MidiDriver;
	RenderApi RenderingApi;
	std::vector<std::filesystem::path> SoundFontsPath;
	std::vector<std::filesystem::path> SmfFilePath;
	FluidSynthMode FluidSynthMode;
    public:
	const std::unordered_map<RenderApi, std::string> RenderLibName = {
	    {CURSES, "curses"},
	    {TEXT, "PrintMessage"}
	};

	std::vector<std::filesystem::path> getSoundFontsPath() const { return SoundFontsPath; }
	std::vector<std::filesystem::path> getSmfFilePath() const { return SmfFilePath; }

	std::string getMidiDriver() const { return MidiDriver; }
	bool getAudioEnableFlag() const { return EnableAudio; }
	std::string getAudioDriver() const { return AudioDriver; }

	enum RenderApi getRenderingApi() const { return RenderingApi; }
	void setFramePerSecond(float fps) { FramePerSecond = fps; }
	float getFramePerSecond() const { return FramePerSecond; }
	enum FluidSynthMode getFluidSynthMode() const { return FluidSynthMode; }
	
	Option(int argc, char** argv);
    }; // class Option

    // aliases
    using sysclk = std::chrono::system_clock;

} // namespace mirmidivi

#endif
