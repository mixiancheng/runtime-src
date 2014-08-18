#ifndef LuaHttpRequest_H_
#define LuaHttpRequest_H_
#include "network/HttpClient.h"
USING_NS_CC;
using namespace network;
using namespace std;

class LuaHttpRequest: public HttpRequest
{
public:
    LuaHttpRequest();
    
    virtual ~LuaHttpRequest();
    
public:
    
    static LuaHttpRequest* create();
    
    static bool mkdirs(string aDir);
    
    void open(string url, string type);
    
    void setTag(string tag);
    
    void send(const std::string& stringData);
    
    void send(const unsigned char* binaryData, unsigned int len);
    /**
     * 设置一个用于回调的lua函数
     */
    void setResponseScriptCallback(unsigned int aHandler);
    
private:
    
    /**
     * 默认的用于c++的回调，由这里统一处理到lua的回调
     */
     void responseScriptCallback(HttpClient* apClient, HttpResponse* apResponse);
    
private:
    
    /**
     * 当前保存的handler
     */
    unsigned int mHandler;
};
#endif /* LuaHttpRequest_H_ */