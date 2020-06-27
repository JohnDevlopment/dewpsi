workspace "Dewpsi"
    configurations { "Debug", "Release", "Dist" }
    architecture "x64"
    startproject "dewpsi"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

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
        "%{prj.location}/src/**.cc",
        "%{prj.location}/src/**.h"
    }
    
    defines "SPDLOG_COMPILED_LIB"
    includedirs {
        "%{prj.location}/vendor/spdlog/include",
        "%{prj.location}/src",
        "%{prj.location}/src/events"
    }

    pchheader "pdpch.h"
    pchsource "Dewpsi/src/pdpch.cpp"
    
    links {
        "spdlog",
        "SDL2"
    }
    libdirs "/usr/local/lib"
    
filter "system:linux"
    defines {
        "PD_PLATFORM_LINUX",
        "PD_BUILD_SOLIB"
    }

filter "toolset:gcc"
    linkoptions {
        "-Wl,--enable-new-dtags",
        "-pthread"
    }

filter "system:windows"
    cppdialect "C++17"
    systemversion "latest"
    defines {
        "PD_PLATFORM_WINDOWS",
        "PD_BUILD_DLL",
        "_WINDLL"
    }

filter "configurations:Debug"
    defines "PD_DEBUG"
    symbols "On"
    runtime "Debug"
    postbuildcommands {
        "{COPY} %{prj.location}/src/*.h ../Sandbox/src/dewpsi-include"
    }

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


project "sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++14"
    staticruntime "On"
    
    links {
        "dewpsi",
        "pthread"
    }
    
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
    
    defines "SPDLOG_COMPILED_LIB"
    
filter "system:windows"
    cppdialect "C++17"
    systemversion "latest"
    defines "PD_PLATFORM_WINDOWS"

filter "configurations:Debug"
    defines "PD_DEBUG"
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
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files {
        "%{prj.location}/src/*.cpp"
    }
    includedirs {
        "%{prj.location}/include"
    }
    buildoptions "-fPIC"
    defines "SPDLOG_COMPILED_LIB"

