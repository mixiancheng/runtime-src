#include "lua_ccocos2dx_NetScoket_auto.hpp"
#include "NetUtil.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"
#include "CCLuaValue.h"


int lua_cocos2dx_NetScoket_NetUtil_connect(lua_State* tolua_S)
{
    int argc = 0;
    NetUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetUtil*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_NetScoket_NetUtil_connect'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::string arg0;
        unsigned int arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        ok &= luaval_to_uint32(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        bool ret = cobj->connect(arg0, arg1);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "connect",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_NetScoket_NetUtil_connect'.",&tolua_err);
#endif

    return 0;
}
int lua_cocos2dx_NetScoket_NetUtil_init(lua_State* tolua_S)
{
    int argc = 0;
    NetUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetUtil*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_NetScoket_NetUtil_init'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->init();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "init",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_NetScoket_NetUtil_init'.",&tolua_err);
#endif

    return 0;
}
int lua_cocos2dx_NetScoket_NetUtil_addLuaHandler(lua_State* tolua_S)
{
    int argc = 0;
    NetUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetUtil*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_NetScoket_NetUtil_addLuaHandler'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        std::string arg0;
        int arg1;
        bool arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

//        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1);
        LUA_FUNCTION handler = (toluafix_ref_function(tolua_S, 3, 0));


        ok &= luaval_to_boolean(tolua_S, 4,&arg2);
        if(!ok)
            return 0;
        cobj->addLuaHandler(arg0, handler, arg2);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "addLuaHandler",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_NetScoket_NetUtil_addLuaHandler'.",&tolua_err);
#endif

    return 0;
}
int lua_cocos2dx_NetScoket_NetUtil_callRPC(lua_State* tolua_S)
{
    int argc = 0;
    NetUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetUtil*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_NetScoket_NetUtil_callRPC'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 4) 
    {
        std::string arg0;
        std::string arg1;
//        const unsigned char* arg2;
        unsigned int arg3;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        #pragma warning NO CONVERSION TO NATIVE FOR unsigned char*;
        /////////////  by dong
        
        ok &= luaval_to_uint32(tolua_S, 5,&arg3);
        /////////////  by dong
        int   nLength = arg3;
        unsigned char* binaryArray = new unsigned char[nLength];
        if (NULL == binaryArray) {
            return 0;
        }
        for (int i = 0; i < nLength; i++) {
            binaryArray[i] = (unsigned char)tolua_tofieldnumber(tolua_S, 4, i+1, 0);
            CCLOG("s------>%c",binaryArray[i]);
        }

        if(!ok)
            return 0;
        bool ret = cobj->callRPC(arg0, arg1, binaryArray, arg3);
        delete [] binaryArray;
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "callRPC",argc, 4);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_NetScoket_NetUtil_callRPC'.",&tolua_err);
#endif

    return 0;
}
int lua_cocos2dx_NetScoket_NetUtil_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"NetUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        NetUtil* ret = NetUtil::create();
        object_to_luaval<NetUtil>(tolua_S, "NetUtil",(NetUtil*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "create",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_NetScoket_NetUtil_create'.",&tolua_err);
#endif
    return 0;
}
int lua_cocos2dx_NetScoket_NetUtil_constructor(lua_State* tolua_S)
{
    int argc = 0;
    NetUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj = new NetUtil();
        cobj->autorelease();
        int ID =  (int)cobj->_ID ;
        int* luaID =  &cobj->_luaID ;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"NetUtil");
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "NetUtil",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_NetScoket_NetUtil_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_cocos2dx_NetScoket_NetUtil_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (NetUtil)");
    return 0;
}

int lua_register_cocos2dx_NetScoket_NetUtil(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"NetUtil");
    tolua_cclass(tolua_S,"NetUtil","NetUtil","cc.Ref",nullptr);

    tolua_beginmodule(tolua_S,"NetUtil");
        tolua_function(tolua_S,"connect",lua_cocos2dx_NetScoket_NetUtil_connect);
        tolua_function(tolua_S,"init",lua_cocos2dx_NetScoket_NetUtil_init);
        tolua_function(tolua_S,"addLuaHandler",lua_cocos2dx_NetScoket_NetUtil_addLuaHandler);
        tolua_function(tolua_S,"callRPC",lua_cocos2dx_NetScoket_NetUtil_callRPC);
        tolua_function(tolua_S,"new",lua_cocos2dx_NetScoket_NetUtil_constructor);
        tolua_function(tolua_S,"create", lua_cocos2dx_NetScoket_NetUtil_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(NetUtil).name();
    g_luaType[typeName] = "NetUtil";
    g_typeCast["NetUtil"] = "NetUtil";
    return 1;
}
TOLUA_API int register_all_cocos2dx_NetScoket(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,nullptr,0);
	tolua_beginmodule(tolua_S,nullptr);

	lua_register_cocos2dx_NetScoket_NetUtil(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

