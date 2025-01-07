workspace "Graphics"
	architecture "x86"
	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}/%{cfg.system}-%{cfg.architecture}"

project "Graphics"
	location "Graphics"
	kind "consoleApp"
	language "C++"
	cppdialect "C++14"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp"
	}

	includedirs
	{
		"Libraries\glew-2.2.0\include\GL",
		"Libraries\glm",
		"Libraries\SDL2-2.28.3\include"
	}

	libdirs
	{
		"Libraries\SDL2-2.28.3\lib\x86",
		"Libraries\glew-2.2.0\lib\Release\Win32"
	}

	links
	{
		"SDL2.lib",
		"SDL2main.lib",
		"glew32s.lib",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "on"

	filter "configurations:Realease"
		optimize "on"