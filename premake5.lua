workspace "Dewpsi"
    configurations { "Debug", "Release", "Dist" }
    architecture "x64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Dewpsi"
    kind "SharedLib"
    language "C++"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    files {
        "src/**.cc",
        "src/**.h"
    }
    includedirs {
        "./vendor/spdlog/include",
        "./src"
    }

filter "system:windows"
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"
    defines {
        "PD_PLATFORM_WINDOWS",
        "PD_BUILD_DLL",
        "_WINDLL"
    }
    postbuildcommands {
        ("{COPY} %{cfg.buildtarget.relpath} ")
    }

filter "configurations:Debug"
    defines "DEBUG"
    symbols "On"

filter "configurations:Release"
    defines "NDEBUG"
    optimize "On"

