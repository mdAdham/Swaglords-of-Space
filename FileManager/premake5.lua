include "Dependencies.lua"

project "FileManager"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.hpp",
		"src/**.cpp",
		"**.h",
		"**.lua",
		"**.rc",
		"**.aps",

		"**.ttf",
		"**.png",
		"**.jpg",
		"**.ico",
		"**.htm",
		"**.ogg",
		"**.wav"
	}

	includedirs
	{
		"%{wks.location}/vendor/SFML/include",
		"%{wks.location}/vendor/imgui",
		"%{wks.location}/vendor/spdlog/include",
		"%{wks.location}/Swaglords of Space/src"
	}

	libdirs
	{
		"%{wks.location}/vendor/SFML/lib"
	}

	links
	{
		"%{Library.opengl32}",
		"%{Library.openal32}",
		"%{Library.winmm}",
		"%{Library.gdi32}",
		"%{Library.flac}",
		"%{Library.vorbisenc}", 
		"%{Library.vorbis}",
		"%{Library.vorbisfile}",
		"%{Library.ogg}",
		"%{Library.ws2_32}", 
		"%{Library.freetype}",
		"%{Library.sfml_main}"
	}

	defines
	{
		"SFML_STATIC"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines
		{
			"SWAG_DEBUG"
		}

		links
		{
			"%{Library.sfml_window_s_d}",
			"%{Library.sfml_graphics_s_d}",
			"%{Library.sfml_system_s_d}",
			"%{Library.sfml_audio_s_d}"
		}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines
		{
			"SWAG_RELEASE"
		}

		links
		{
			"%{Library.sfml_window_s}",
			"%{Library.sfml_graphics_s}",
			"%{Library.sfml_system_s}",
			"%{Library.sfml_audio_s}"
		}
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines
		{
			"SWAG_RELEASE"
		}

		links
		{
			"%{Library.sfml_window_s}",
			"%{Library.sfml_graphics_s}",
			"%{Library.sfml_system_s}",
			"%{Library.sfml_audio_s}"
		}
		runtime "Release"
		optimize "on"