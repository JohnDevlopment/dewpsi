-- add functions to "string"
string["char"] = function(self, idx)
    assert(type(idx) == "number", idx .. " is not a valid integer!")
    return self:sub(idx, idx)
end

string["append"] = function(self, str)
    self = self .. str
    return self
end

string["length"] = function(self)
    return #self
end

-- add functions to "io"
io["writef"] = function(out, fmt, ...)
    local str = string.format(fmt, ...)
    out:write(str)
end

io["cout"] = function(...)
    local stdout = io.stdout
    local nextarg
    local str = ""

    local n = select("#", ...)
    for i=1,n,1 do
        str = str .. tostring(select(i, ...))
    end
    io.write(str)
end

cout = io.cout
cin = io.read
fprintf = io.writef

function replaceChars(str, ch, with)
    local len = str:length()
    do
        local t = type(ch)
        assert(t ~= "table", "'ch' cannot be a table!")
        if t == "number" then
            ch = string.format("%c", ch)
        end

        if string.length(ch) > 1 then
            ch = ch:char(1)
        end
    end

    local retval = ""

    for i=1,len,1 do
        local c = str:char(i)
        if c == ch then
            retval = retval .. with
        else
            retval = retval .. c
        end
    end

    return retval
end

function removeIfExists(path)
    if os.isfile(path) then
        os.remove(path)
        print("Removed file: " .. path)
    elseif os.isdir(path) then
        os.rmdir(path)
        print("Removed dir: " .. path)
    else
        --print("Not removing " .. path)
    end
end

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
srcdir = "%{prj.location}/src"

targetdir_prefix = ("bin/" .. outputdir)
objdir_prefix = ("bin-int/" .. outputdir)

--[[
function staticLibPath(prefix, libname)
    local result = ""

    if (prefix) then
        result = string.format("%s/bin/%s/%s", prefix, outputdir, libname)
    else
        result = string.format("bin/%s/%s", outputdir_sub, libname)
    end

    return(result)
end
]]

newaction {
    trigger = "clean",
    description = "Clean the workspace",
    execute = function()
        local _dirs = {}
        _dirs[1] = "."
        _dirs[2] = "Dewpsi"
        _dirs[3] = "Dewpsi/vendor/imgui"
        _dirs[4] = "Dewpsi/vendor/glad"
        _dirs[5] = "Dewpsi/vendor/spdlog"
        _dirs[6] = "Dewpsi/vendor/getopt"
        _dirs[7] = "Dewpsi/vendor/glm"
        _dirs[8] = "Sandbox"
        for i=1,8,1 do
            local _path = _dirs[i]
            removeIfExists(path.join(_path, "Makefile"))
            removeIfExists(path.join(_path, "bin"))
            removeIfExists(path.join(_path, "bin-int"))
        end
        print("finished cleaning the project")
    end
}

newaction {
    trigger = "newheader",
    description = "Writes a new header",
    execute = function()
        local _subdir, _file, _id

        cout('Enter the relative path of the file: ')
        _subdir = io.stdin:read('l')

        cout('Enter the name of the file: ')
        _file = cin('l')

        -- open file; buffers until end-of-line
        _id = io.open(path.join('.', _subdir, _file), 'w')
        _id:setvbuf("line")

        -- header guard name
        local _fdefname = string.upper(_file)
        _fdefname = replaceChars(_fdefname, '.', '_')

        -- header include tables
        local _sysincs = {}
        local _localincs = {}

        do
            fprintf(_id, "#ifndef %s\n#define %s\n\n", _fdefname, _fdefname)

            local _locx, _sysx = 1, 1

            cout("\nHeaders: each local header is prefixed with a 'l_'. ")
            cout("Each system header is not.\n")

            local _done = false
            while not(done) do
                cout("Input: ")
                local _answer = cin('l')

                -- break loop
                if _answer == "" then break end

                -- local for system header?
                if _answer:sub(1, 2) == 'l_' then
                    _localincs[_locx] = _answer:sub(3)
                    _locx = _locx + 1
                else
                    _sysincs[_sysx] = _answer
                    _sysx = _sysx + 1
                end
            end
        end
        -- end do

        -- print each system header first
        for i,v in ipairs(_sysincs) do
            fprintf(_id, "#include <%s>\n", v)
        end

        -- print each local header
        if #_sysincs > 0 then
            fprintf(_id, "\n")
            for i,v in ipairs(_localincs) do
                fprintf(_id, "#include \"%s\"\n", v)
            end
        end

        fprintf(_id, "\n#endif /* %s */", _fdefname)

        _id:close()
    end
}

workspace "Dewpsi"
    configurations {
        "Debug",
        "Release",
        "Dist"
    }
    architecture "x64"
    startproject "sandbox"
    flags "MultiProcessorCompile"

-- inclusion directories
IncludeDir = {}
IncludeDir["spdlog"] = "Dewpsi/vendor/spdlog/include"
IncludeDir["imgui"] = "Dewpsi/vendor/imgui"
IncludeDir["glad"] = "Dewpsi/vendor/glad/include"
IncludeDir["getopt"] = "Dewpsi/vendor/getopt/include"
IncludeDir["glm"] = "Dewpsi/vendor/glm/glm"

group "Dependencies"
    include "Dewpsi/vendor/imgui"
    include "Dewpsi/vendor/glad"
    include "Dewpsi/vendor/getopt"
group ""

-- Main engine project --
project "dewpsi"
    location "Dewpsi"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"

    targetdir (targetdir_prefix .. "/%{prj.name}")
    objdir (objdir_prefix .. "/%{prj.name}")
    files {
        (srcdir .. "/*.cc"),
        (srcdir .. "/events/*.cc"),
        (srcdir .. "/ImGui/*.cc"),
        (srcdir .. "/ImGui/imguibuild.cpp"),
        (srcdir .. "/Renderer/*.cc"),

        (srcdir .. "/*.h"),
        (srcdir .. "/debug/*.h"),
        (srcdir .. "/events/*.h"),
        (srcdir .. "/ImGui/*.h"),
        (srcdir .. "/matrices/*.h"),
        (srcdir .. "/Renderer/*.h"),
        (srcdir .. "/os/*.h"),
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
        "%{IncludeDir.getopt}",
        "%{IncludeDir.glm}",
        (srcdir),
        (srcdir .. "/debug"),
        (srcdir .. "/events"),
        (srcdir .. "/ImGui"),
        (srcdir .. "/os"),
        (srcdir .. "/Renderer")
    }
    links {
        "spdlog",
        "ImGui",
        "Glad",
        "getopt"
    }
    postbuildcommands {
        "{MKDIR} ../Sandbox/src/dewpsi-include/glad",
        "{MKDIR} ../Sandbox/src/dewpsi-include/bits",
        "{MKDIR} ../Sandbox/src/dewpsi-include/imgui",

        ("{COPY} " .. srcdir .. "/*.h ../Sandbox/src/dewpsi-include"),
        ("{COPY} " .. srcdir .. "/debug/*.h  ../Sandbox/src/dewpsi-include"),
        ("{COPY} " .. srcdir .. "/events/*.h ../Sandbox/src/dewpsi-include"),
        ("{COPY} " .. srcdir .. "/ImGui/*.h  ../Sandbox/src/dewpsi-include"),
        ("{COPY} " .. srcdir .. "/os/*.h  ../Sandbox/src/dewpsi-include"),
        ("{COPY} " .. srcdir .. "/Renderer/Dewpsi_RenderContext.h ../Sandbox/src/dewpsi-include"),

        "{COPY} %{prj.location}/vendor/glad/include/glad/glad.h ../Sandbox/src/dewpsi-include/glad/glad.h",
        "{COPY} %{prj.location}/vendor/getopt/include/my_getopt.h ../Sandbox/src/dewpsi-include/my_getopt.h"
    }

-- Windows
filter "system:windows"
    defines {
        "PD_PLATFORM_WINDOWS",
        "PD_BUILD_DLL",
        "PD_EXPORT_DLL"
    }
    links {
        "SDL2:static",
        "SDL2main:static"
    }

-- Linux, GCC
filter {"system:linux", "toolset:gcc"}
    defines {
        "PD_PLATFORM_LINUX",
        "PD_BUILD_SOLIB"
    }
    includedirs {
        (srcdir .. "/platform/sdl")
    }
    links {
        "dl",
        "sndio",
        "m:static",
        "SDL2:static"
    }
    files {
        (srcdir .. "/platform/sdl/Dewpsi_*.cc"),
        (srcdir .. "/platform/sdl/Dewpsi_*.h")
    }
    linkoptions {
        "-Wl,--enable-new-dtags",
        "-pthread"
    }
    postbuildcommands {
        ("{COPY} " .. srcdir .. "/platform/sdl/*.h ../Sandbox/src/dewpsi-include"),
        ("{COPY} " .. srcdir .. "/Renderer/Dewpsi_OpenGLContext.h ../Sandbox/src/dewpsi-include")
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

    links { "dewpsi" }

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    files {
        "%{prj.location}/src/**.cc",
        "%{prj.location}/src/**.h"
    }
    includedirs {
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.getopt}",
        "%{IncludeDir.glm}",
        "%{prj.location}/src",
        "%{prj.location}/src/dewpsi-include"
    }
    pchheader "pdpch.h"
    pchsource "pdpch.cpp"
    defines {
        "SPDLOG_COMPILED_LIB",
        "PD_IMPORT_ALIB",
    }

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
