#include "li.h"
#include "util.h"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

static lua_State *li_lua = NULL;

void li_init() {
	li_lua = OOM_CHECK(luaL_newstate());

	luaL_openlibs(li_lua);

	luaL_loadfile(li_lua, "config.lua");

	lua_pcall(li_lua, 0, LUA_MULTRET, 0);

}


void li_deinit() {
	lua_close(li_lua);
}

