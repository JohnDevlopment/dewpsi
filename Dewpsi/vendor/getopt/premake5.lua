project "getopt"
    kind "StaticLib"
    language "C"
    staticruntime "On"
    runtime "Release"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.location}/src/*.c",
        "%{prj.location}/src/include/*.h"
    }
    includedirs "%{prj.location}/include"
    defines "GETOPT_API_EXPORT"

filter "toolset:gcc"
    buildoptions "-fPIC"
    cdialect "gnu99"

filter "system:windows"
    systemversion "latest"
    cdialect "C99"

filter "configurations:Debug"
    runtime "Debug"
    symbols "on"

filter "configurations:Release"
    runtime "Release"
    optimize "on"
