workspace "Dewpsi"
    configurations {
        "Debug",
        "Release",
        "Dist"
    }
    architecture "x64"
    startproject "dewpsi"
    flags "MultiProcessorCompile"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

sdl2dir = os.findlib("SDL2")
sndiodir = os.findlib("sndio")

srcdir = "%{prj.location}/src"

IncludeDir = {}
IncludeDir["spdlog"] = "Dewpsi/vendor/spdlog/include"
IncludeDir["imgui"] = "Dewpsi/vendor/imgui"
IncludeDir["glad"] = "Dewpsi/vendor/glad/include"

group "Dependencies"
    include "Dewpsi/vendor/imgui"
    include "Dewpsi/vendor/glad"
group ""

-- Main engine project --
project "dewpsi"
    location "Dewpsi"
    kind "SharedLib"
    language "C++"
    cppdialect "C++14"
--    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    files {
        (srcdir .. "/*.cc"),
        (srcdir .. "/events/*.cc"),
        (srcdir .. "/ImGui/*.cc"),
        
        (srcdir .. "/*.h"),
        (srcdir .. "/os/*.h"),
        (srcdir .. "/events/*.h"),
        (srcdir .. "/ImGui/*.h"),
    }
    pchheader "pdpch.h"
    pchsource "Dewpsi/src/pdpch.cpp"
    
    defines {
        "SPDLOG_COMPILED_LIB",
        "_REENTRANT"
    }
    
    includedirs {
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.glad}",
        (srcdir),
        (srcdir .. "/events"),
        (srcdir .. "/os"),
        (srcdir .. "/ImGui")
    }
    libdirs {
        (sdl2dir),
        (sndiodir)
    }
    links {
        "spdlog",
        "sndio",
        "m",
        "ImGui",
        "Glad"
    }
    
    postbuildcommands {
        ("{COPY} " .. srcdir .. "/*.h ../Sandbox/src/dewpsi-include"),
        ("{COPY} " .. srcdir .. "/events/*.h ../Sandbox/src/dewpsi-include"),
        ("{COPY} " .. srcdir .. "/os/*.h  ../Sandbox/src/dewpsi-include")
    }
    
-- Linux
filter "system:linux"
    defines {
        "PD_PLATFORM_LINUX",
        "PD_BUILD_SOLIB"
    }
    includedirs {
        (srcdir .. "/platform/sdl")
    }
    links {
        "dl"
    }
    files {
        (srcdir .. "/platform/sdl/Dewpsi_*.cc"),
        (srcdir .. "/platform/sdl/Dewpsi_*.h")
    }
    
    postbuildcommands {
        ("{COPY} " .. srcdir .. "/platform/sdl/*.h ../Sandbox/src/dewpsi-include")
    }

-- GCC compiler
filter "toolset:gcc"
    linkoptions {
        "-Wl,--enable-new-dtags",
        "-Wl,-rpath,/usr/local/lib",
        "-Wl,--enable-new-dtags",
        "-Wl,--no-undefined",
        "-pthread",
        "-lSDL2",
        "-z undefs"
    }

-- different configurations
filter "configurations:Debug"
    defines {
        "PD_DEBUG",
        "PD_ENABLE_ASSERTS"
    }
    symbols "On"
    runtime "Debug"

filter "configurations:Release"
    defines {
        "NDEBUG",
        "PD_RELEASE"
    }
    optimize "On"
    runtime "Release"

filter "configurations:Dist"
    defines {
        "NDEBUG",
        "PD_DIST"
    }
    optimize "On"
    runtime "Release"
---------------------------

-- project sandbox
project "sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++14"
    staticruntime "On"
    
    links "dewpsi"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    files {
        "%{prj.location}/src/**.cc",
        "%{prj.location}/src/**.h"
    }
    includedirs {
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.imgui}",
        "%{prj.location}/src",
        "%{prj.location}/src/dewpsi-include"
    }
    
    pchheader "pdpch.h"
    pchsource "pdpch.cpp"
    
    defines "SPDLOG_COMPILED_LIB"
    
-- Linux build
filter "system:linux"
    defines "PD_PLATFORM_LINUX"

-- different configurations    
filter "configurations:Debug"
    defines {
        "PD_DEBUG",
        "PD_ENABLE_ASSERTS"
    }
    symbols "On"
    runtime "Debug"

filter "configurations:Release"
    defines {
        "NDEBUG",
        "PD_RELEASE"
    }
    optimize "On"
    runtime "Release"

filter "configurations:Dist"
    defines {
        "NDEBUG",
        "PD_DIST"
    }
    optimize "On"
    runtime "Release"
---------------------------

-- project spdlog, vendor, external static library
project "spdlog"
    location "Dewpsi/vendor/spdlog"
    kind "StaticLib"
    language "C++"
    cppdialect "C++11"
    staticruntime "On"
    runtime "Release"
    optimize "On"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files {
        "%{prj.location}/src/*.cpp"
    }
    includedirs {
        "%{prj.location}/include"
    }
    
    defines "SPDLOG_COMPILED_LIB"
    
filter "toolset:gcc"
    buildoptions "-fPIC"

