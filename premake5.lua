workspace "Swag Space"
	architecture "x64"
	startproject "Swaglords of Space"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Swaglords of Space"
include "Swaglords of Space Launcher"