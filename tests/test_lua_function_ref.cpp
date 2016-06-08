#include "lua_tinker.h"
#include"test.h"
extern std::map<std::string, std::function<bool()> > g_test_func_set;

lua_tinker::lua_function_ref<int> g_lua_func_ref;

void test_luafunction_ref(lua_State* L)
{
	g_test_func_set["test_lua_luafunction_ref_1"] = [L]()->bool
	{
		std::string luabuf =
			R"(function test_lua_luafunction_ref_1()
					local upval = 0;
					local localtest = function (intval)
						upval = upval + intval;
						return upval;
					end
					return localtest;
				end
			)";
		lua_tinker::dostring(L, luabuf.c_str());

		lua_tinker::lua_function_ref<int> lua_func = lua_tinker::call<decltype(lua_func)>(L, "test_lua_luafunction_ref_1");
		lua_func(7);
		return 14 == lua_func(7);
	};
	g_test_func_set["test_lua_luafunction_ref_2"] = [L]()->bool
	{
		std::string luabuf =
			R"(function test_lua_luafunction_ref_2_1()
					local upval = 0;
					local localtest = function (intval)
						upval = upval + intval;
						return upval;
					end
					return localtest;
				end
				function test_lua_luafunction_ref_2_2(func)
					func(7);
					return func(7);
				end
			)";
		lua_tinker::dostring(L, luabuf.c_str());

		lua_tinker::lua_function_ref<int> lua_func = lua_tinker::call<decltype(lua_func)>(L, "test_lua_luafunction_ref_2_1");
		return 14 == lua_tinker::call<int>(L, "test_lua_luafunction_ref_2_2", lua_func);
	};


	g_test_func_set["test_lua_luafunction_ref_3"] = [L]()->bool
	{
		std::string luabuf =
			R"( g_test_func_table={test=1,};
				function g_test_func_table.func1(val)
					return val + 1;
				end;
				
			)";
		lua_tinker::dostring(L, luabuf.c_str());
		lua_tinker::table_onstack func_table(L, "g_test_func_table");
		lua_tinker::lua_function_ref<int> lua_func = func_table.get<decltype(lua_func)>("func1");
		return 7 == lua_func(6);
	};


	std::string luabuf =
		R"( 	function g_lua_func_test(val)
					return val + 1;
				end;
				
			)";
	lua_tinker::dostring(L, luabuf.c_str());
	g_test_func_set["test_lua_luafunction_ref_4"] = [L]()->bool
	{
		if(g_lua_func_ref.empty())
			g_lua_func_ref = lua_tinker::get<decltype(g_lua_func_ref)>(L,"g_lua_func_test");

		return g_lua_func_ref(6) == 7;
	};

	g_test_func_set["test_lua_luafunction_ref_5"] = [L]()->bool
	{
		if (g_lua_func_ref.empty())
			g_lua_func_ref = lua_tinker::get<decltype(g_lua_func_ref)>(L, "g_lua_func_test");

		return g_lua_func_ref(8) == 9;
	};
}