//
//  PackageHandler.cpp
//  CocosLuaGameNet
//
//  Created by 米宪成 on 14-8-18.
//
//
#include <signal.h>
#include "PackageHandler.h"
#include "BTEventDispatcher.h"
std::string PackageHandler::buildPackage(const std::string &callFlag, const std::string &rpcName, const  char *binaryData, unsigned int binaryDataLen){
    Header _header;
    memset(&_header, 0, sizeof(Header));
    _header.isVerifyData[0] = 1;
    _header.isZip[0] = 0;
    _header.Temp[0] = 1;
    _header.Temp[1] = 1;
    _header.PackLenth =binaryDataLen;
    
    std::ostringstream _sendData;
    char buffer[4];
    int len = _header.PackLenth;
    
    
    for (int i =3; i>=0; i--) {
        buffer[i] = len & 0xff;
        len >>= 8;
    }
    
    for (int i =0; i<=3; i++ ) {
        _sendData<<buffer[i];
    }
    
    _sendData<<_header.isVerifyData[0];
    _sendData<<_header.isZip[0];
    _sendData<<_header.Temp[0];
    _sendData<<_header.Temp[1];
    
    std::ostringstream _dataTemp;
//    _dataTemp << "BabelTime"; //加入扰码
//    //转换为char
////    const char *arrCharTemp = binaryData;
//    //扰码计算
//    char buffer1[1];
//    for (int j = 0; j < _header.PackLenth ; j++) {
//        if (j != (_header.PackLenth  - 1)) {
//            buffer1[0] = (binaryData[j]^binaryData[j+1]);
//        } else {
//            buffer1[0] = (binaryData[j]^_header.PackLenth );
//        }
//        _dataTemp << buffer1[0];
//    }
    for (int i = 0; i < binaryDataLen; i++) {
        _sendData << binaryData[i];
    }
    std::string out=_sendData.str();
    const char* temp=out.c_str();
//    for (int i=0; i<binaryDataLen+8; i++) {
//        CCLOG("char===>%c",temp[i]);
//    }
    return _sendData.str();
}
void PackageHandler::parsePackage(const char *buffer_body,size_t dataLen){
    CCLOG("parsePackage");
    std::ostringstream _dataTemp;
    for (int i = 0; i < dataLen; i++) {
        _dataTemp << buffer_body[i];
//        CCLOG("char----->%c",buffer_body[i]);
    }
    std::string out=_dataTemp.str();
    CCLOG("user.userLogin>>>>>>>>>>getMes====>%s",out.c_str());
    BTEventDispatcher::getInstance()->addEventResult("user.userLogin", out);
    }
std::string PackageHandler::m_strToken = "0";
bool PackageHandler::m_bZip = false;

std::string PackageHandler::getToken() {
    return m_strToken;
}
void PackageHandler::setToken(const std::string & newTocken) {
    m_strToken = newTocken;
}

void PackageHandler::setZip(bool zip) {
    m_bZip = zip;
}
bool PackageHandler::getZip() {
    return m_bZip;
}
