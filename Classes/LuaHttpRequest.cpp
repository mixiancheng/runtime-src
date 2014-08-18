#include "LuaHttpRequest.h"
#include "CCLuaEngine.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

typedef unsigned short int uint16;
typedef unsigned long int uint32;
// 短整型大小端互换
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

//自动生成lua后需要将auto.cpp中的setResponseScriptCallback部分内容替换如下
//#include "CCLuaValue.h"
//if (argc == 1)
//{
//    LUA_FUNCTION handler = (toluafix_ref_function(tolua_S, 2, 0));
//    cobj->setResponseScriptCallback(handler);
//    return 0;
//}

//自动生成lua后需要将auto.cpp中的send部分内容替换如下
//if (!ok) { break; }
///////////////  by dong
//int   nLength = arg1;
//unsigned char* binaryArray = new unsigned char[nLength];
//if (NULL == binaryArray) {
//    return 0;
//}
//for (int i = 0; i < nLength; i++) {
//    binaryArray[i] = (unsigned char)tolua_tofieldnumber(tolua_S, 2, i+1, 0);
//}
//cobj->send(binaryArray, nLength);
//delete [] binaryArray;
//////////////
////            cobj->send(arg0, arg1);
///////////// end dong
//return 0;


#define INT_SIZE 4
LuaHttpRequest::LuaHttpRequest() :
mHandler(0)
{
    
}

LuaHttpRequest::~LuaHttpRequest()
{
}

LuaHttpRequest* LuaHttpRequest::create()
{
    return new LuaHttpRequest;
}

void LuaHttpRequest::open(string url, string type){
    setUrl(url.c_str());
    if (type.compare("post") == 0 || type.compare("POST") == 0){
        setRequestType(HttpRequest::Type::POST);
    }
    else{
        setRequestType(HttpRequest::Type::GET);
    }
}

void LuaHttpRequest::setTag(string tag){
    HttpRequest::setTag(tag.c_str());
}

void LuaHttpRequest::send(const std::string& stringData){
    int dataStringSize = (int)stringData.size();
    CCLOG("发送的数据包长度为 %d", dataStringSize);
    int requestDataSize = INT_SIZE+dataStringSize;
    
    char* requestData = (char*)malloc(requestDataSize);
    int dataSize = (int)t_htonl(dataStringSize);
    memcpy(&requestData[0],&dataSize,INT_SIZE);
    memcpy(&requestData[INT_SIZE],stringData.data() ,dataStringSize);
    
    setRequestData(requestData, requestDataSize);
    delete requestData;
//    HttpClient::getInstance()->setTimeoutForRead(20);
    HttpClient::getInstance()->send(this);
}

void LuaHttpRequest::send(const unsigned char* binaryData, unsigned int len) {
    CCLOG("发送的数据包长度为 %d", len);
    int requestDataSize = INT_SIZE+len;
    
    char* requestData = new char[requestDataSize];
    int dataSize = (int)t_htonl(len);
    memcpy((void*)requestData, &dataSize,INT_SIZE);
    memcpy((void*)(requestData+INT_SIZE), (void*)binaryData, len);
    
    setRequestData(requestData, requestDataSize);
    delete requestData;
//    HttpClient::getInstance()->setTimeoutForRead(20);
    HttpClient::getInstance()->send(this);
}

void LuaHttpRequest::setResponseScriptCallback(unsigned int aHandler)
{
    mHandler = aHandler;
    setResponseCallback(this, httpresponse_selector(LuaHttpRequest::responseScriptCallback));
}

void LuaHttpRequest::responseScriptCallback(cocos2d::network::HttpClient* apClient, cocos2d::network::HttpResponse* apResponse)
{
    if (!apResponse)
    {
        return;
    }
    
    if (!apResponse->isSucceed()) {
        return;
    }
    
    vector<char> *buffer = apResponse->getResponseData();
    char* data = &(*buffer)[0];
    
    int bufferStringSize = *(int*)data;
    int dataSize = (int)t_ntohl(bufferStringSize);
    CCLOG("接收的数据包长度为 %d", dataSize);
    
    auto engine = LuaEngine::getInstance();
    cocos2d::LuaStack* pStack =engine->getLuaStack();
    pStack->pushBoolean(apResponse->isSucceed());       //是否成功
    pStack->pushInt((int)apResponse->getResponseCode());    //状态码
    
    LuaValueArray array;
    for (int i = INT_SIZE ; i < dataSize+INT_SIZE; i++) {
        LuaValue value = LuaValue::intValue(data[i] & 0xff);
        array.push_back(value);
    }
    pStack->pushLuaValueArray(array);
    
    pStack->pushString(apResponse->getErrorBuffer());   //错误描述
    pStack->pushString(apResponse->getHttpRequest()->getTag()); //数据回传
    pStack->executeFunctionByHandler(mHandler, 5);
    pStack->clean();
}

bool LuaHttpRequest::mkdirs(std::string aDir)
{
    if (aDir.size() == 0)
    {
        return true;
    }
    
    CCLOG("creating dir:%s", aDir.c_str());
    bool isFirst = aDir[0] == '/';
    bool isLast = false;
    for (unsigned int i = 1; i < aDir.size(); i++)
    {
        if (aDir[i] == '/')
        {
            if (isLast)
            {
                continue;
            }
            isLast = true;
            
            if (isFirst)
            {
                isFirst = false;
                continue;
            }
            
            std::string path = aDir.substr(0, i);
            if (cocos2d::CCFileUtils::sharedFileUtils()->isFileExist(path.c_str()))
            {
                CCLOG("path:%s exist", path.c_str());
                continue;
            }
            int ret = mkdir(path.c_str(), 0700);
            if (ret && errno != EEXIST)
            {
                CCLOG("mkdir:%s failed", path.c_str());
                return false;
            }
            else
            {
                CCLOG("path:%s created", path.c_str());
            }
        }
        else
        {
            isLast = false;
        }
    }
    return true;
}