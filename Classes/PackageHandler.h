//
//  PackageHandler.h
//  CocosLuaGameNet
//
//  Created by 米宪成 on 14-8-18.
//
//

#ifndef __CocosLuaGameNet__PackageHandler__
#define __CocosLuaGameNet__PackageHandler__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;
class PackageHandler {
public:
    typedef struct{
        int PackLenth;                         //包长度
        char isVerifyData[1];                  //是否有校验数据
        char isZip[1];                         //是否压缩
        char Temp[2];                          //预留数据
    } Header;
    static std::string buildPackage(const std::string & callFlag, const std::string & rpcName, const  char* binaryData,unsigned int len);
    static void parsePackage(const char * buffer_body, size_t data_len);
    static void setToken(const std::string & newTocken);
    static std::string getToken();
    static void setZip(bool zip);
    static bool getZip();
    private :
    PackageHandler();
    ~PackageHandler();
    static std::string m_strToken;
    //    static ReadState m_state;
    static bool m_bZip;
};
#endif /* defined(__CocosLuaGameNet__PackageHandler__) */
