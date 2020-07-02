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

project "dewpsi"
    location "Dewpsi"
    kind "SharedLib"
    language "C++"
    cppdialect "C++14"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    files {
        (srcdir .. "/**.cc"),
        (srcdir .. "/**.h")
    }
    pchheader "pdpch.h"
    pchsource "Dewpsi/src/pdpch.cpp"
    
    defines {
        "SPDLOG_COMPILED_LIB",
        "_REENTRANT"
    }
    
    includedirs {
        "%{IncludeDir.spdlog}",
        (srcdir),
        (srcdir .. "/events"),
        (srcdir .. "/os")
    }
    libdirs {
        (sdl2dir),
        (sndiodir)
    }
    links {
        "spdlog",
        "sndio",
        "m"
    }
    
    postbuildcommands {
        ("{COPY} " .. srcdir .. "/*.h ../Sandbox/src/dewpsi-include"),
        ("{COPY} " .. srcdir .. "/events/*.h ../Sandbox/src/dewpsi-include"),
        ("{COPY} " .. srcdir .. "/os/*.h  ../Sandbox/src/dewpsi-include")
    }
    
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
    files (srcdir .. "/platform/sdl/sdlwindow.cc")
    
    postbuildcommands {
        ("{COPY} " .. srcdir .. "/platform/sdl/*.h ../Sandbox/src/dewpsi-include")
    }

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

--filter "system:windows"
--    cppdialect "C++17"
--    systemversion "latest"
--    defines {
--        "PD_PLATFORM_WINDOWS",
--        "PD_BUILD_DLL",
--        "_WINDLL"
--    }


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
        "%{prj.location}/src",
        "%{prj.location}/src/dewpsi-include"
    }
    
    pchheader "pdpch.h"
    pchsource "pdpch.cpp"
    
    defines "SPDLOG_COMPILED_LIB"
    
filter "system:linux"
    defines "PD_PLATFORM_LINUX"
    
--filter "system:windows"
--    cppdialect "C++17"
--    systemversion "latest"
--    defines "PD_PLATFORM_WINDOWS"

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

