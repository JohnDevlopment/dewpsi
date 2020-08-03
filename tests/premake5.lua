IncludeDir = {}
IncludeDir["dewpsiroot"] = "../Dewpsi/src"
IncludeDir["spdlog"] = "../Dewpsi/vendor/spdlog/include"

LibDir = {}
LibDir["dewpsi"] = "../bin/Debug-linux-x86_64/dewpsi"
LibDir["spdlog"] = "../bin/Debug-linux-x86_64/spdlog"

workspace "tests"
    configurations { "Debug", "Release" }
    libdirs {
        "%{LibDir.dewpsi}",
        "%{LibDir.spdlog}"
    }
    includedirs {
        "%{IncludeDir.dewpsiroot}",
        "%{IncludeDir.spdlog}"
    }
    defines {
        "SPDLOG_COMPILED_LIB"
    }

project "stringtest"
    kind "ConsoleApp"
    language "C++"
    files {
        "stringtest_*.cc"
    }
    links {
        "dewpsi",
        "spdlog"
    }
    cppdialect "C++11"
    flags "MultiProcessorCompile"
    
filter "configurations:Debug"
    symbols "On"
    runtime "Debug"
    
filter "configurations:Release"
    optimize "On"
    runtime "Release"

project "macrotest"
    kind "ConsoleApp"
    language "C++"
    files {
        "macrotest_*.cc"
    }
    cppdialect "C++11"
    flags "MultiProcessorCompile"
    
project "mathtests"
    kind "ConsoleApp"
    language "C++"
    files {
        "mathtests_*.cc"
    }
    links {
        "dewpsi",
        "spdlog"
    }
    cppdialect "C++11"
    flags "MultiProcessorCompile"
    
filter "configurations:Debug"
    symbols "On"
    runtime "Debug"
    
filter "configurations:Release"
    optimize "On"
    runtime "Release"
