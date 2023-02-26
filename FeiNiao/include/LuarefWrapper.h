#pragma once

// 扩展 luabridge::LuaRef 类
struct LuaRefWrapper : public luabridge::LuaRef {
    using luabridge::LuaRef::LuaRef;
    
    // 添加 keys() 函数
    std::vector<std::string> keys() {
        std::vector<std::string> result;
        lua_State* L = state();
        // 确保值是一个 table
        if (lua_istable(L, -1)) {
            lua_pushnil(L);
            while (lua_next(L, -2) != 0) {
                // 如果键是字符串，则添加到结果列表中
                if (lua_type(L, -2) == LUA_TSTRING) {
                    const char* key = lua_tostring(L, -2);
                    result.emplace_back(key);
                }
                lua_pop(L, 1);
            }
        }
        return result;
    }
};

// 注册 LuaRefWrapper 类
luabridge::Namespace(L)
    .beginClass<LuaRefWrapper>("LuaRef")
        // 注册 keys() 函数
        .addFunction("keys", &LuaRefWrapper::keys)
    .endClass();
