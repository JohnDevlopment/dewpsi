project "stb_image"
    kind "StaticLib"
    language "C"
    targetdir (targetdir_prefix .. "/%{prj.name}")
    objdir (objdir_prefix .. "/%{prj.name}")
    files "stb_image.cc"

filter "toolset:gcc"
    buildoptions {
        "-msse",
        "-msse2",
        "-fPIC"
    }
