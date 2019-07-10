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

#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>
#include <filesystem>

#include "dlldr.hpp"

namespace mirmidivi
{
    enum FluidSynthMode{SYNTH, PLAYER};

    const std::vector<std::string_view> RenderingLibrariesList = {
	"text",
	"curses"
    };

    const std::vector<std::string_view> UiToolKitList = {
	"no_toolkit",
	"glfw"
    };

    // Dynamic loading Libraries
    const auto dlldr_mode =
	dlldr::shared_library::add_decorations +
	dlldr::shared_library::search_system_directories;

    // Store options from command-line args (or other)
    class Option
    {
    private:
	float FramePerSecond = 60.0;

	// Fluidsynth driver
	std::string AudioDriver;
	std::string MidiDriver;

	// Misc dor fluidsynth
	bool EnableAudio = false;
	std::vector<std::filesystem::path> SoundFontsPath;
	std::vector<std::filesystem::path> SmfFilePath;
	FluidSynthMode FluidSynthMode;

	// Library for loading
	std::string RenderingApi = RenderingLibrariesList[0].data();
	std::string UiToolKit = UiToolKitList[0].data();

	// Library uncheck flag
	bool Unsafe = false;
    public:
	float getFramePerSecond() const { return FramePerSecond; }

	std::string getAudioDriver() const { return AudioDriver; }
	std::string getMidiDriver() const { return MidiDriver; }

	bool getAudioEnableFlag() const { return EnableAudio; }
	std::vector<std::filesystem::path> getSoundFontsPath() const { return SoundFontsPath; }
	std::vector<std::filesystem::path> getSmfFilePath() const { return SmfFilePath; }
	enum FluidSynthMode getFluidSynthMode() const { return FluidSynthMode; }

	std::string getRenderingApi() const { return RenderingApi; }
	std::string getUiToolKit() const { return UiToolKit; }

	bool getUnsafeMode() const { return Unsafe; }

	Option(int argc, char** argv);
    }; // class Option

    // aliases
    using sysclk = std::chrono::system_clock;

} // namespace mirmidivi

#endif
