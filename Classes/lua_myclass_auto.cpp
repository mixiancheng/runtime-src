#include "lua_myclass_auto.hpp"
#include "LuaHttpRequest.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"
#include "CCLuaValue.h"


int lua_myclass_LuaHttpRequest_setResponseScriptCallback(lua_State* tolua_S)
{
    int argc = 0;
    LuaHttpRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaHttpRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaHttpRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_myclass_LuaHttpRequest_setResponseScriptCallback'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1)
    {
        LUA_FUNCTION handler = (toluafix_ref_function(tolua_S, 2, 0));
        cobj->setResponseScriptCallback(handler);
        return 0;
    }

    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setResponseScriptCallback",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_myclass_LuaHttpRequest_setResponseScriptCallback'.",&tolua_err);
#endif

    return 0;
}
int lua_myclass_LuaHttpRequest_open(lua_State* tolua_S)
{
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "open",1, 2);
    int argc = 0;
    LuaHttpRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaHttpRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaHttpRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_myclass_LuaHttpRequest_open'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::string arg0;
        std::string arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        cobj->open(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "open",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_myclass_LuaHttpRequest_open'.",&tolua_err);
#endif

    return 0;
}
int lua_myclass_LuaHttpRequest_send(lua_State* tolua_S)
{
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "send",1, 1);
    int argc = 0;
    LuaHttpRequest* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaHttpRequest",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (LuaHttpRequest*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_myclass_LuaHttpRequest_send'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 2) {
            const unsigned char* arg0;
            #pragma warning NO CONVERSION TO NATIVE FOR unsigned char*;

            if (!ok) { break; }
            unsigned int arg1;
            ok &= luaval_to_uint32(tolua_S, 3,&arg1);

            if (!ok) { break; }
            /////////////  by dong 
            int   nLength = arg1;
            unsigned char* binaryArray = new unsigned char[nLength];
            if (NULL == binaryArray) {
                return 0;
            }
            for (int i = 0; i < nLength; i++) {
                binaryArray[i] = (unsigned char)tolua_tofieldnumber(tolua_S, 2, i+1, 0);
            }
            cobj->send(binaryArray, nLength);
            delete [] binaryArray;
            ////////////
//            cobj->send(arg0, arg1);
            /////////// end dong
            return 0;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 1) {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0);

            if (!ok) { break; }
            cobj->send(arg0);
            return 0;
        }
    }while(0);
    ok  = true;
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "send",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_myclass_LuaHttpRequest_send'.",&tolua_err);
#endif

    return 0;
}
int lua_myclass_LuaHttpRequest_setTag(lua_State* tolua_S)
{
    int argc = 0;
    LuaHttpRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaHttpRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaHttpRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_myclass_LuaHttpRequest_setTag'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setTag(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setTag",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_myclass_LuaHttpRequest_setTag'.",&tolua_err);
#endif

    return 0;
}
int lua_myclass_LuaHttpRequest_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"LuaHttpRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        LuaHttpRequest* ret = LuaHttpRequest::create();
        object_to_luaval<LuaHttpRequest>(tolua_S, "LuaHttpRequest",(LuaHttpRequest*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "create",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_myclass_LuaHttpRequest_create'.",&tolua_err);
#endif
    return 0;
}
int lua_myclass_LuaHttpRequest_mkdirs(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"LuaHttpRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        bool ret = LuaHttpRequest::mkdirs(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "mkdirs",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_myclass_LuaHttpRequest_mkdirs'.",&tolua_err);
#endif
    return 0;
}
int lua_myclass_LuaHttpRequest_constructor(lua_State* tolua_S)
{
    int argc = 0;
    LuaHttpRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj = new LuaHttpRequest();
        cobj->autorelease();
        int ID =  (int)cobj->_ID ;
        int* luaID =  &cobj->_luaID ;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"LuaHttpRequest");
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "LuaHttpRequest",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_myclass_LuaHttpRequest_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_myclass_LuaHttpRequest_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (LuaHttpRequest)");
    return 0;
}

int lua_register_myclass_LuaHttpRequest(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"LuaHttpRequest");
    tolua_cclass(tolua_S,"LuaHttpRequest","LuaHttpRequest","cc.network::HttpRequest",nullptr);

    tolua_beginmodule(tolua_S,"LuaHttpRequest");
        tolua_function(tolua_S,"setResponseScriptCallback",lua_myclass_LuaHttpRequest_setResponseScriptCallback);
        tolua_function(tolua_S,"open",lua_myclass_LuaHttpRequest_open);
        tolua_function(tolua_S,"send",lua_myclass_LuaHttpRequest_send);
        tolua_function(tolua_S,"setTag",lua_myclass_LuaHttpRequest_setTag);
        tolua_function(tolua_S,"new",lua_myclass_LuaHttpRequest_constructor);
        tolua_function(tolua_S,"create", lua_myclass_LuaHttpRequest_create);
        tolua_function(tolua_S,"mkdirs", lua_myclass_LuaHttpRequest_mkdirs);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(LuaHttpRequest).name();
    g_luaType[typeName] = "LuaHttpRequest";
    g_typeCast["LuaHttpRequest"] = "LuaHttpRequest";
    return 1;
}
TOLUA_API int register_all_myclass(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,nullptr,0);
	tolua_beginmodule(tolua_S,nullptr);

	lua_register_myclass_LuaHttpRequest(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

