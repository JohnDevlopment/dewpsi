project "Glad"
    kind "StaticLib"
    language "C"
    staticruntime "On"
    runtime "Release"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
    }
    includedirs "include"
    defines "GLAD_GLAPI_EXPORT"
    
filter "toolset:gcc"
    buildoptions "-fPIC"

filter "system:windows"
    systemversion "latest"

filter "configurations:Debug"
    runtime "Debug"
    symbols "on"

filter "configurations:Release"
    runtime "Release"
    optimize "on"
