workspace "ParticalSimulation"
    architecture "x86_64"
    configurations { "Debug", "Release" }

project "ParticalSimulation"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin-int/%{cfg.buildcfg}"

    files {
        "src/**.h",
        "src/**.cpp",
        "src/shaders/**.cpp",
        "src/renderer/**.cpp",
        "libs/glad/src/glad.c"
    }

    includedirs {
        "src",
        "src/shaders",
        "libs/glad/include",
        "/usr/include",
    }

    links {
        "SDL2",
        "GL",
        "dl",
        "pthread"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
