#include <stdio.h>
#include <stdlib.h>
#include "lua.h"
#include "LuaLoad.h"
#include "lauxlib.h"

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("Missing argument\n");
        return 1;
    }
    lua_State *state;
    if (!(state = luaL_newstate()))
        return 1;

    luaL_openlibs(state);

    lua_register(state, "sqlite_open", lua_sqlite_open);
    lua_register(state, "sqlite_exec", lua_sqlite_exec);
    lua_register(state, "sqlite_close", lua_sqlite_close);

    if (luaL_dofile(state, argv[1]))
        printf("Lua error: %s\n", lua_tostring(state, -1));

	lua_close(state);
	return 0;
}
