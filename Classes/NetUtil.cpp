//
//  NetUtil.cpp
//  CocosLuaGameNet
//
//  Created by 米宪成 on 14-8-14.
//
//

#include "NetUtil.h"
#include "BTNetClient.h"
#include "BTEventDispatcher.h"
NetUtil::NetUtil(){
    
}
NetUtil::~NetUtil(){
    
}
bool NetUtil::init(){
    return true;
}
bool NetUtil::connect(const std::string &strIp, unsigned int nPort){
    BTNetClient::getInstance()->connectWithAddr(strIp, nPort);
    return false;
}
bool NetUtil::callRPC(const std::string & callFlag, const std::string & rpcName, const unsigned char* binaryData,unsigned int len){
    BTEventDispatcher::getInstance()->callRPC(callFlag, rpcName, binaryData,len);
    return false ;
}
void NetUtil::addLuaHandler(const std::string &callFlag, int luaHandler, bool autoRemove){
    BTEventDispatcher::getInstance()->addLuaHandler(callFlag, luaHandler, autoRemove);

}
