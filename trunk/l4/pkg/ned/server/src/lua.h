#pragma once


#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

namespace Lua {

static char const *const package = "L4";
static luaL_Reg const empty_reg[]  = {{NULL, NULL}};

inline void
register_method(lua_State *l, char const *name, lua_CFunction f)
{
  lua_pushcfunction(l, f);
  lua_setfield(l, -2, name);
}

class Lib
{
public:
  enum Prio
  {
    P_cap_type,
    P_env
  };

  explicit Lib(Prio);
  virtual ~Lib() {}

  virtual void init(lua_State *) = 0;
  Prio prio() const { return _prio; }

  static void run_init(lua_State *);

private:
  Prio _prio;
  Lib *_next;
};

}

