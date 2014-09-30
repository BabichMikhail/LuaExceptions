#include "LuaLoad.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lualib.h"
#include "lauxlib.h"
#include "sqlite3.h"
#include "lua.h"

int lua_callback(void *state, int argc, char **argv, char **names)
{
	int i, count;
	count = lua_tointeger(state, -1);
	lua_newtable(state);
	for (i = 0; i < argc; i++)
	{
	    lua_pushstring(state, names[i]);
	    lua_pushstring(state, argv[i]);
	    lua_settable(state, -3);
	}
	lua_settable(state, -3);
	count++;
	lua_pushinteger(state, count);
	return 0;
}

int lua_sqlite_open(lua_State *state)
{
	sqlite3 *db;
	char *file_name = lua_tostring(state, -1);
	if (sqlite3_open(file_name, &db))
    {
		lua_pushstring(state, sqlite3_errmsg(db));
		sqlite3_close(db);
		lua_error(state);
	}
    lua_pushlightuserdata(state, db);
    return 1;
}

int lua_sqlite_close(lua_State *state)
{
	sqlite3 *db = (sqlite3 *)lua_touserdata(state, 1);
	if (sqlite3_close(db))
    {
		luaL_error(state, "Can not close Database");
	}
	return 1;
}

int lua_sqlite_exec(lua_State *state)
{
	const char *query = lua_tostring(state, 2);
	sqlite3 *db = (sqlite3*)lua_touserdata(state, 1);
    char *zErrMsg;
    lua_newtable(state);
    lua_pushinteger(state, 1);
    if (sqlite3_exec(db, query, lua_callback, state, &zErrMsg))
    {
        lua_pushfstring(state, "%s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        lua_error(state);
    }
    lua_pop(state, 1);
	return 1;
}

