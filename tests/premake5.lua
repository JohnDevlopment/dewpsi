workspace "tests"
    configurations { "Debug", "Release" }

project "stringtest"
    kind "ConsoleApp"
    language "C++"
    files {
        "stringtest_*.cc"
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
    
filter "configurations:Debug"
    symbols "On"
    runtime "Debug"
    
filter "configurations:Release"
    optimize "On"
    runtime "Release"
