 properties =
 {
    bjam_options='-q toolset=gcc',
    boost =
    {
        version = "1.58",
        ["1.58"] =
        {
            include=[[/usr/include/boost]],
            -- These next two values are bologna.
            lib=[[/usr/lib]],
            path=[[/usr/bin]]
        }
    },
    lpeg = {
        ["0.12"] = {
            bin = "~HOME~/MacDeps/lpeg/lpeg-0.12/bin",
            include = "~HOME~/MacDeps/lpeg/lpeg-0.12",
            source = "~HOME~/MacDeps/lpeg/lpeg-0.12"
        },
    },
    lua = {
        ["5.2.3"] =
        {
            bin = "~HOME~/MacDeps/lua/lua-5.2.3/bin",
            include = "~HOME~/MacDeps/lua/lua-5.2.3/src",
            source = "~HOME~/MacDeps/lua/lua-5.2.3/src",
        }
    },
    luafilesystem = {
        ["1.6.2"] = {
            source = "~HOME~/MacDeps/luafilesystem/luafilesystem-1_6_2/src",
        },
    },
    simple_project = { run_sphinx = false },
}
