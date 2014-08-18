//
//  BTEventDispatcher.cpp
//  YingLing
//
//  Created by zhangwei on 13-4-28.
//
//
#include "PackageHandler.h"
#include "BTEventDispatcher.h"
#include "BTNetClient.h"
NL_NAMESPACE_USE
using namespace cocos2d;
const float BTEventDispatcher::FPS = 0.1f;
typedef unsigned short int uint16;
typedef unsigned long int uint32;
#define INT_SIZE 4
//// 短整型大小端互换
#define BigLittleSwap16(A)  ((((uint16)(A) & 0xff00) >> 8) | \
(((uint16)(A) & 0x00ff) << 8))
// 长整型大小端互换

#define BigLittleSwap32(A)  ((((uint32)(A) & 0xff000000) >> 24) | \
(((uint32)(A) & 0x00ff0000) >> 8) | \
(((uint32)(A) & 0x0000ff00) << 8) | \
(((uint32)(A) & 0x000000ff) << 24))

// 本机大端返回1，小端返回0
int checkCPUendian()
{
    union{
        unsigned long int i;
        unsigned char s[4];
    }c;
    
    c.i = 0x12345678;
    return (0x12 == c.s[0]);
}

// 模拟htonl函数，本机字节序转网络字节序
unsigned long int t_htonl(unsigned long int h)
{
    // 若本机为大端，与网络字节序同，直接返回
    // 若本机为小端，转换成大端再返回
    return checkCPUendian() ? h : BigLittleSwap32(h);
}

// 模拟ntohl函数，网络字节序转本机字节序
unsigned long int t_ntohl(unsigned long int n)
{
    // 若本机为大端，与网络字节序同，直接返回
    // 若本机为小端，网络数据转换成小端再返回
    return checkCPUendian() ? n : BigLittleSwap32(n);
}

// 模拟htons函数，本机字节序转网络字节序
unsigned short int t_htons(unsigned short int h)
{
    // 若本机为大端，与网络字节序同，直接返回
    // 若本机为小端，转换成大端再返回
    return checkCPUendian() ? h : BigLittleSwap16(h);
}

// 模拟ntohs函数，网络字节序转本机字节序
unsigned short int t_ntohs(unsigned short int n)
{
    // 若本机为大端，与网络字节序同，直接返回
    // 若本机为小端，网络数据转换成小端再返回
    return checkCPUendian() ? n : BigLittleSwap16(n);
}

BTEventDispatcher * BTEventDispatcher::instance = NULL;

BTEventDispatcher::BTEventDispatcher() {
    m_pHandler =__Dictionary::create();
    m_aResult = __Array::create(); //change by zhangqi, 20130522
    m_aResult->retain();
    m_pHandler->retain();
    pthread_mutex_init(&m_pMutex, NULL); //add by zhangqi, 20130523
    
    m_pStack = LuaEngine::defaultEngine()->getLuaStack();
}
BTEventDispatcher::~BTEventDispatcher() {
    //change by zhangqi, 20130522
    CC_SAFE_DELETE(m_pHandler);
    CC_SAFE_DELETE(m_aResult);
}
void BTEventDispatcher::addLuaHandler(const std::string & callFlag, int luaHandler, bool autoRemove) {
//    printf("BTEventDispatcher::addLuaHandler..........................callFlag: %s\n", callFlag.c_str());
    ScriptHandlerEntry * handlerEntry = ScriptHandlerEntry::create(luaHandler);
    m_pHandler->setObject(handlerEntry, callFlag);
    m_mapAutoRemove.insert(make_pair(callFlag, autoRemove));
}
void BTEventDispatcher::addEventResult(const std::string & callFlag, std::string ret) {
    //pthread_mutex_lock(&m_pMutex);
//    ret->retain();
//    std::map<std::string, std::string> SocketMap=new;
    /* change by zhangqi, 20130522 */
    CCString *_str=CCString::create(ret);
    _str->retain();
    CCDictionary * retDict = new CCDictionary();
    retDict->setObject(_str, callFlag);
    
    //add by zhangqi, 20130523, 多线程中，修改事件队列前加锁
    
    m_aResult->addObject(retDict);
    
    retDict->release(); //这样m_aResult remove retDict时就会delete掉
    /* change by zhangqi, 20130522 */
    
    CCLog("in %s", __FUNCTION__);
    _str->release();
    //pthread_mutex_unlock(&m_pMutex);
}
void BTEventDispatcher::removeEvent(const std::string & callFlag) {
    //change by zhangqi, 20130529, 把自动删除的逻辑判断移出方法，方便其他对象直接调用
    m_pHandler->removeObjectForKey(callFlag);
    m_mapAutoRemove.erase(callFlag);
    CCLog("removeEvent: %s", callFlag.c_str());
}
void BTEventDispatcher::removeAll() {
    m_pHandler->removeAllObjects();
    
    //add by zhangqi, 20130523, 多线程中，修改事件队列前加锁
    //pthread_mutex_lock(&m_pMutex);
    m_aResult->removeAllObjects();//change by zhagnqi, 20130522
    //pthread_mutex_unlock(&m_pMutex);
    CCLog("............................BTEventDispatcher::removeAll");
    m_mapAutoRemove.clear();
}

bool BTEventDispatcher::callRPC (const std::string & callFlag, const std::string & rpcName, const unsigned char* binaryData,unsigned int len) {
#ifdef COCOS2D_DEBUG
    CCLog("BTEventDispatcher::callRPC %s 发给服务器数据 begin **************************", rpcName.c_str());
//    ClassInfo::description(args, 2);
    CCLog("BTEventDispatcher::callRPC %s 发给服务器数据 end **************************", rpcName.c_str());
#endif
    Socket * pSocket = BTNetClient::getInstance()->getSocket();
    if (pSocket == NULL) {
        CCLog("getSocket == NULL");
        addExceptionEvent("failed", "NetWork", "Server Disconnected");
        return false;
    }
    CCLOG("发送的数据包长度为 %d", len);
//    int requestDataSize = INT_SIZE+len;
    char* requestData = new char[len];
//    int dataSize = (int)t_htonl(len);
//    char* name="test";
//    memcpy((void*)requestData, len,INT_SIZE);
    memcpy((void*)(requestData), (void*)binaryData, len);
    std::string _str=PackageHandler::buildPackage(callFlag, rpcName, requestData, len);
    try {
        pSocket->send(_str.c_str(), _str.size());
//        pSocket->send(binaryData, len);
//        delete requestData;
    } catch (...) {
        CCLog("send error");
    }
    
    
    return true;
}

int BTEventDispatcher::autoRemoveEventCount() {
    int autoRemove = 0;
    std::map<string, bool>::const_iterator it = m_mapAutoRemove.begin();
    for (; it != m_mapAutoRemove.end(); it++) {
        if (it->second) {
            autoRemove++;
        }
    }
    return autoRemove;
}

bool BTEventDispatcher::hasHandlerByCallFlag(const std::string & callFlag) {
//    CCLog("..........................BTEventDispatcher::hasHandlerByCallFlag");
//    std::map<std::string, bool>::iterator itr;
//    for (itr=m_mapAutoRemove.begin(); itr != m_mapAutoRemove.end(); itr++){
//        printf(".....................name: %s, value: %d\n", itr->first.c_str(), itr->second);
//    }
    
    std::map<string, bool>::iterator it = m_mapAutoRemove.find(callFlag.c_str());
    return (it != m_mapAutoRemove.end());
}

void BTEventDispatcher::callLuaFunc(const string & callbak, std::string _str, ScriptHandlerEntry * handlerEntry) {
//    if (dictRet->count() <= 0) {//20130525
//        return;
//    }
    m_pStack->pushString(callbak.c_str());
    m_pStack->pushString(_str.c_str());
//    m_pStack->pushLuaValue(BTUtil::transformCCObjectToLua(dictRet));
//
//    string strErr;
//    if (dictRet) {
//        strErr = ((CCString *)dictRet->objectForKey("err"))->m_sString;
//    } else {
//        strErr = "error";
//        CCLog("invalid result of rpc: %s", callbak.c_str());
//    }
//    bool bErr = (strErr == "OK" || strErr == "ok");
    m_pStack->pushBoolean(true); //"ok"表示返回的数据本身是有效的，可以做进一步的逻辑处理
//    CCLog("dispatch:rpc = %s 4.5", callbak.c_str());
//    
    m_pStack->executeFunctionByHandler(handlerEntry->getHandler(), 3);
    CCLog("dispatch:rpc = %s 5", callbak.c_str());
}
/*后台数据监听*/
bool BTEventDispatcher::dispatch(const std::string & callFlag) {
//    CCLOG("dispatch---->");
    CCObject* pResult = NULL;
    //add by zhangqi, 20130523, 多线程中，修改事件队列前加锁
    int ret = pthread_mutex_trylock(&m_pMutex);
    if (ret != 0) {
		return false;
	}
//     CCLOG("dispatch---->11");
    //pthread_mutex_lock(&m_pMutex);

    CCARRAY_FOREACH(m_aResult, pResult) {
//         CCLOG("dispatch---->22");[]
        CCDictionary * dict = dynamic_cast<CCDictionary*>(pResult);//zhangqi, 20130903
        
        DictElement* pElement = NULL;
        CCDICT_FOREACH(dict, pElement)
        {
            const char * pCallback = pElement->getStrKey();
            CCString* ret = (CCString*)pElement->getObject();
            CCLOG("name========>%s",ret->getCString());
            CCLOG("key====%s,ret===%s",pCallback,ret->getCString());
            if (strcasecmp(pCallback, "exception") == 0) {
//                CCString * key = (CCString*)ret->allKeys()->objectAtIndex(0);
//                CCString * what = (CCString*) ret->objectForKey(key->getCString());
//                CCLog("found exception: %s", what->getCString());
                CCLog("found exception:");
            }
            CCLog("dispatch:rpc = %s 1", pCallback);
            
            if (strcasecmp(pCallback, "failed") == 0) {
//                CCString * key = (CCString*)ret->allKeys()->objectAtIndex(0);
//                CCString * what = (CCString*) ret->objectForKey(key->getCString());
//                CCLog("found failed: %s", what->getCString());
                CCLog("found failed:");
            }
            CCLog("dispatch:rpc = %s 2", pCallback);
            
            ScriptHandlerEntry * handlerEntry = (ScriptHandlerEntry *)m_pHandler->objectForKey(pCallback);
            if (handlerEntry == NULL) {
                CCLog("no handler of rpc: %s", pCallback);
                continue;
            }
            CCLog("dispatch:rpc = %s 3,str===%s", pCallback,ret->getCString());
//            string name="test--------->";

            callLuaFunc(pCallback, ret->getCString(), handlerEntry);
            
            std::map<std::string, bool>::iterator it = m_mapAutoRemove.find(pCallback);
            if ( it != m_mapAutoRemove.end() && it->second == true) {
                removeEvent(pCallback);
            }
            
            CCLog("dispatch:rpc = %s OK", pCallback);
        }
    }
    m_aResult->removeAllObjects(); //zhangqi, 20130903, 遍历完m_aResult再删除所有event。遍历过程中删除，会改变array的内存结构，出现野指针导致崩溃
    pthread_mutex_unlock(&m_pMutex);
    return true;
}
