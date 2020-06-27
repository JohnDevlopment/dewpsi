-- clean.lua

premake.modules.lua = {}

local m = premake.modules.lua

local p = premake

newaction {
    trigger = "clean",
    description = "Clean all files",
    
    onStart = function()
        print("Cleaning files")
    end,
    
    onProject = function(prj)
        printf("Cleaning generated file(s) for project '%s'", prj.name)
    end
}

return m


