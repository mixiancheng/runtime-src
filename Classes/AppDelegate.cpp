#include "AppDelegate.h"
#include "CCLuaEngine.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "Runtime.h"
#include "ConfigParser.h"
#include "PackageHandler.h"
//#include "lua_cocos2dx_NetUtil_auto.hpp"
//#include "lua_myclass_auto.hpp"
//#include "lua_cocos2dx_myclass_auto.hpp"
#include "lua_ccocos2dx_NetScoket_auto.hpp"
//#include "lua_cocos2dx_custom.hpp"
//#include "lua_myclass_auto.hpp"
using namespace CocosDenshion;
#include "BTEventDispatcher.h"
USING_NS_CC;
using namespace std;
extern "C"{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
    int luaopen_pb (lua_State *L);
}

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    
#if (COCOS2D_DEBUG>0)
    initRuntime();
#endif
    
    if (!ConfigParser::getInstance()->isInit()) {
        ConfigParser::getInstance()->readConfig();
    }
    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        Size viewSize = ConfigParser::getInstance()->getInitViewSize();
        string title = ConfigParser::getInstance()->getInitViewName();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
        extern void createSimulator(const char* viewName, float width, float height,bool isLandscape = true, float frameZoomFactor = 1.0f);
        bool isLanscape = ConfigParser::getInstance()->isLanscape();
        createSimulator(title.c_str(),viewSize.width,viewSize.height,isLanscape);
#else
        glview = GLView::createWithRect(title.c_str(), Rect(0,0,viewSize.width,viewSize.height));
        director->setOpenGLView(glview);
#endif
    }
    // turn on display FPS
    director->setDisplayStats(true);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
//    std::string name="nameTest";
//    const  char *arrCharTemp =name.c_str();
//    PackageHandler::buildPackage("", "", arrCharTemp, name.size());
//    if (true){ return true ;};
    auto engine = LuaEngine::getInstance();
    luaopen_pb(engine->getLuaStack()->getLuaState());
    ScriptEngineManager::getInstance()->setScriptEngine(engine);
    LuaStack* stack = engine->getLuaStack();
    auto state = stack->getLuaState();
    register_all_cocos2dx_NetScoket(state);
    cocos2d::Scheduler *_pScheduler=Director::getInstance()->getScheduler();
    _pScheduler->schedule(schedule_selector(AppDelegate::dealNetWorkEvent), CCNode::create(), 0.1f, false);
#if (COCOS2D_DEBUG>0)
    if (startRuntime())
//    BTNetClient::getInstance()->connectWithAddr("localhost", 1235);
//    BTNetClient::getInstance()->getSocket()->send("hellosever",10);
    return true;
#endif
    
    engine->executeScriptFile(ConfigParser::getInstance()->getEntryFile().c_str());
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
    
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
    
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
void AppDelegate::dealNetWorkEvent (float args)
{
    BTEventDispatcher::getInstance()->dispatch("");
//    CCLOG("update");
}


