//
//  NetUtil.h
//  CocosLuaGameNet
//
//  Created by 米宪成 on 14-8-14.
//
//

#ifndef __CocosLuaGameNet__NetUtil__
#define __CocosLuaGameNet__NetUtil__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;
class NetUtil :public Ref {
public:
     NetUtil();
     ~NetUtil();
     bool init();
     bool callRPC(const std::string & callFlag, const std::string & rpcName, const unsigned char* binaryData,unsigned int len);
     void addLuaHandler(const std::string & callFlag, int luaHandler, bool autoRemove);
     bool connect(const std::string & strIp, unsigned int nPort);
     CREATE_FUNC(NetUtil);
};
#endif /* defined(__CocosLuaGameNet__NetUtil__) */
