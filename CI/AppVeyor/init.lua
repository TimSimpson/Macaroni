 properties =
 {
    bjam_options='-q -d+2 -j8',
    boost =
    {
        version = "1.58",
        ["1.58"] =
        {
            include=[[C://Libraries//boost_1_58_0]],
            lib=[[C://Libraries//boost_1_58_0/stage/lib]],
            path=[[C://Libraries//boost_1_58_0]]
        }
    },
    lpeg = {
        ["0.12"] = {
            bin = "C:/Libraries/lpeg/lpeg-0.12/bin",
            include = "C:/Libraries/lpeg/lpeg-0.12",
            source = "C:/Libraries/lpeg/lpeg-0.12"
        },
    },
    lua = {
        ["5.2.3"] =
        {
            bin = "C:/Libraries/lua/lua-5.2.3/bin",
            include = "C:/Libraries/lua/lua-5.2.3/src",
            source = "C:/Libraries/lua/lua-5.2.3/src",
        }
    },
    luafilesystem = {
        ["1.6.2"] = {
            source = "C:/Libraries/luafilesystem/luafilesystem-1_6_2/src",
        },
    },
    simple_project = { run_sphinx = true },
}
