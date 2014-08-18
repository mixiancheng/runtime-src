//
//  BTNetClient.h
//  YingLing
//
//  Created by zhangwei on 13-5-3.
//
//

#ifndef __YingLing__BTNetClient__
#define __YingLing__BTNetClient__

#include <iostream>
#include <map>
#include "cocos2d.h"
#include "network/netlink/netlink.h"
extern void addExceptionEvent (const std::string & event, const std::string & infoKey, const std::string & info); //add by zhangqi, 20130722

class BTNetClient {
public:
    typedef std::map<std::string, NL::Socket*> SocketMap;
    typedef std::map<std::string, NL::Socket*>::iterator SocketMapIt;
    
    static BTNetClient * getInstance() {
        if (instance == NULL) {
            instance = new BTNetClient();
        }
        return instance;
    }
    //pthread_mutex_t m_mutexForSocketGroup;
    bool init(const std::string & ip, unsigned int port);
    bool connect();
    bool connectWithAddr(const std::string & strIp, unsigned int nPort);

    NL::Socket * getSocket() {
        if (m_pSockGroup) {
            try {
                return m_pSockGroup->get(0);
            } catch (NL::Exception e) {
                return NULL;
            }
        }
        return NULL;
    }
    //释放所有的socket以及连接记录
    void clearSockGroup() {
        //pthread_mutex_lock(&m_mutexForSocketGroup);
        if (m_pSockGroup) {
            for (int i = 0; i < m_pSockGroup->size(); i++) {
                m_pSockGroup->remove(i);
            }
        }
        m_mapSocket.clear();
        //pthread_mutex_unlock(&m_mutexForSocketGroup);
    }
    //释放指定socket以及对应的连接记录
    void removeSocket(NL::Socket * socket) {
        for (BTNetClient::SocketMapIt it = m_mapSocket.begin(); it != m_mapSocket.end(); it++) {
            if (it->second == socket) {
                m_mapSocket.erase(it);
                break;
            }
        }
        
        m_pSockGroup->remove(socket);
    }
    
private:
    BTNetClient();
    ~BTNetClient();
    
    static BTNetClient * instance;
    
    std::string m_ip;
    unsigned int m_port;
    
    static NL::SocketGroup * m_pSockGroup;
    NL::SocketGroupCmd * m_pReadHandler;
    NL::SocketGroupCmd * m_pDisconnectHandler;
    
    SocketMap m_mapSocket;
    
    int m_tryMaxCount;
    static void * listen_thread(void*);
};

#endif /* defined(__YingLing__BTNetClient__) */
