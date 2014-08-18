//
//  BTEventDispatcher.h
//  YingLing
//
//  Created by zhangwei on 13-4-28.
//
//

#ifndef __YingLing__BTEventDispatcher__
#define __YingLing__BTEventDispatcher__

#include <iostream>
//#include "CCScriptSupport.h"
#include "CCLuaEngine.h"

class BTEventDispatcher {
public:
    static BTEventDispatcher * getInstance() {
        if (instance == NULL) {
            instance = new BTEventDispatcher();
        }
        return instance;
    }
    
    bool callRPC(const std::string & callFlag, const std::string & rpcName, const unsigned char* binaryData,unsigned int len);
    bool dispatch(const std::string & callFlag);
    
    void addLuaHandler(const std::string & callFlag, int luaHandler, bool autoRemove);
    void addEventResult(const std::string & callFlag, std::string sendBuf);
    
    void removeEvent(const std::string & callFlag);
    void removeAll();
    int autoRemoveEventCount();
    
    bool hasHandlerByCallFlag(const std::string & callFlag);

    pthread_mutex_t m_pMutex; //add by zhangqi, 20130523, 用于网络连接线程操作 m_aResult 的互斥

	const static float FPS;

    cocos2d::CCArray * m_aResult; //add by zhangqi, 20130522, 用数组实现队列存放后端返回的数据，避免Dictionary出现重复key覆盖的情况
    
private:
    BTEventDispatcher();
    ~BTEventDispatcher();
    
    void callLuaFunc(const std::string & callbak, std::string _str, cocos2d::ScriptHandlerEntry * handlerEntry);
    
    static BTEventDispatcher * instance;
    
    cocos2d::CCDictionary * m_pHandler;
    std::map<std::string, bool> m_mapAutoRemove;
    
    cocos2d::LuaStack * m_pStack;
    
};

#endif /* defined(__YingLing__BTEventDispatcher__) */
