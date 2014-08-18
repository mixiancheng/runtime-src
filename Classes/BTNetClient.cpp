//
//  BTNetClient.cpp
//  YingLing
//
//  Created by zhangwei on 13-5-3.
//
//

#include "BTNetClient.h"
#include "PackageHandler.h"
#include <signal.h>
#include "cocos2d.h"
#include "BTEventDispatcher.h"
#define MAX_DATA_LEN 1024*128 //zhanqi, 20130620

NL_NAMESPACE_USE

USING_NS_CC;

using namespace std;

void addExceptionEvent (const string & event, const string & infoKey, const string & info) {
    CCLOG("event==%s,infoKey==%s,info==%s",event.c_str(),infoKey.c_str(),info.c_str());
    CCString * dict = new CCString(info);
    BTEventDispatcher::getInstance()->addEventResult(event, dict->getCString());
    dict->release();
}

class ReceiveDeal : public NL::SocketGroupCmd {
public:
    virtual void exec(NL::Socket* socket, NL::SocketGroup* group, void* reference) {
        unsigned int data_len = 0;//包长度
        
        char buffer[8];
        memset(buffer, 0, 8); //add by zhangqi, 20130605
        socket->read(buffer, 8);
        for (int i=0; i<8; i++) {
            CCLOG("c====%d",buffer[i]);
        }
        for (int i = 0; i < 4; i++) {
            data_len <<= 8;
            data_len |= (0xff & buffer[i]);
        }
        
        PackageHandler::setZip((buffer[5] != 0));//是否有Base64编码
        
        int multi = buffer[6];
//        PackageHandler::setMulti(multi);//是否多个数据
        
        CCLOG("nonReadStat data_len = %d", data_len);
        if (data_len > MAX_DATA_LEN || data_len <= 0) { //zhangqi, 20130605, 判断如果包长大于128K或小于等于0，则视为异常情况直接丢弃不解析
            CCLOG("len=========%d",data_len);
            CCLOG("Error: data length from Server is illegal.\n");
//            return;
        }
        CCLOG("----------------->3333");
        char * buffer_body = (char*)malloc(data_len);
        if (buffer_body == NULL) { //zhangqi, 20130620, 检查动态分配如果失败则直接返回
            CCLOG("Error: malloc %ul    ", data_len);
            return;
        }
        CCLOG("----------------->2222");
        memset(buffer_body, 0, data_len);
        socket->read(buffer_body, data_len);
        CCLOG("----------------->111");
        PackageHandler::parsePackage(buffer_body, data_len);
        
        free(buffer_body);
//        BTEventDispatcher::getInstance()->addEventResult("user.userLogin", "------>");
    }
};

class DisconnectDeal : public NL::SocketGroupCmd {
public:
    DisconnectDeal(BTNetClient::SocketMap & sMap):m_mapSocket(sMap){};
    
    virtual void exec(NL::Socket* socket, NL::SocketGroup* group, void* reference) {
        CCLOG("on disconnect");
        
        //删除全局的连接记录
        BTNetClient::getInstance()->removeSocket(socket);
   
        // add failed event
        addExceptionEvent("failed", "NetWork", "Server Disconnected");
    }
    
private:
    BTNetClient::SocketMap & m_mapSocket;
};

BTNetClient * BTNetClient::instance = NULL;
BTNetClient::BTNetClient()
{
    m_tryMaxCount = 3;
    //pthread_mutex_init(&m_mutexForSocketGroup, NULL);
}
bool BTNetClient::init(const std::string & ip, unsigned int port) {
    if (ip.empty())
        return false;
    m_ip = ip;
    m_port = port;
    return true;
}

pthread_t g_pid = 0;//add by zhangqi, 20130720

void * BTNetClient::listen_thread(void * arg) {    
    /*
     * add by zhangqi, 20130522, 设置线程退出后由系统释放相关资源
     * 据说放在pthread_create()后紧跟着调用会出错，所以放在线程内
     */
    pthread_detach(pthread_self());
    
    CCLog("pthread[%d]: listen", pthread_self());
    
    //add by zhangqi, 20130720, 添加异常处理，服务器断开连接后子线程自动退出
    while (true) {
        pthread_mutex_lock(&(BTEventDispatcher::getInstance()->m_pMutex));
        try {
                BTNetClient::m_pSockGroup->listen(100);
        } catch (NL::Exception e) {
            CCLOG("socket init error: %s", e.what());
            BTNetClient::getInstance()->clearSockGroup();//zhangqi, 20131219, 如果监听抛出select异常, 清除socket group
            addExceptionEvent("exception", "NL::Exception", string("Listen: ").append(e.what()));
        } catch (exception e) {
            CCLOG("connect other error: %s", e.what());
        } catch (...) {
			CCLOG("unknown exception");
		}
		bool shouldSleep = false;
		if(BTEventDispatcher::getInstance()->m_aResult->count())
		{
			shouldSleep= true;
		}
        pthread_mutex_unlock(&(BTEventDispatcher::getInstance()->m_pMutex));
		if(shouldSleep) 
		{
			usleep(BTEventDispatcher::FPS * 1000000 + 5000);
		}
    }
    
    return NULL;
}

static int connect_fang_test(const char *ip, unsigned short port)
{
    int sockfd1 = 0;
    struct sockaddr_in sa;
    struct hostent* hp;
    hp = gethostbyname(ip);
    if(!hp){
        return -1;
    }
    memset(&sa, 0, sizeof(sa));
    memcpy((char*)&sa.sin_addr, hp->h_addr, hp->h_length);
    sa.sin_family = hp->h_addrtype;
    sa.sin_port = htons(port);
    
    sockfd1 = socket(sa.sin_family, SOCK_STREAM, 0);
    
    if(sockfd1 < 0) {
        return -1;
    }
    
    // 设置非阻塞模式
    int flags = fcntl(sockfd1, F_GETFL,0);
    fcntl(sockfd1, F_SETFL, flags | O_NONBLOCK);
    
    int ret = connect(sockfd1, (sockaddr*)&sa, sizeof(sa));
    if (ret < 0 && errno != EINPROGRESS) {
//        perror("connect");
//        printf("errno: %d\n", errno);
        close(sockfd1);
        return -1;
    }
    fd_set          fdwrite;
    struct timeval  tvSelect;
    
    FD_ZERO(&fdwrite);
    FD_SET(sockfd1, &fdwrite);
    tvSelect.tv_sec = 4;
    tvSelect.tv_usec = 0;
    int retval = select(sockfd1 + 1,NULL, &fdwrite, NULL, &tvSelect);
    if(retval < 0)
    {
        if ( errno == EINTR )
        {
            perror("select error");
            close(sockfd1);
            return -1;
        }
        else
        {
            perror("error");
            close(sockfd1);
            return -1;
        }
    }
    else if(retval == 0)
    {
        perror("select timeout........");
        close(sockfd1);
        return -2;
    }
    
    // 设置阻塞模式
//    flags = fcntl(sockfd1, F_GETFL,0);
//    flags &= ~ O_NONBLOCK;
//    fcntl(sockfd1,F_SETFL, flags);
    
    //设置不被SIGPIPE信号中断，物理链路损坏时才不会导致程序直接被Terminate
    //在网络异常的时候如果程序收到SIGPIRE是会直接被退出的。
//    struct sigaction sig;
//    sig.sa_handler = SIG_IGN;
//    sigaction( SIGPIPE, &sig, 0 );
    
    close(sockfd1);
    return sockfd1;
}

NL::SocketGroup * BTNetClient::m_pSockGroup = NULL;
bool BTNetClient::connectWithAddr(const string & strIp, unsigned int nPort) {
    //检查 strIp 和 nPort 对应的socket是否已创建
//    int status = connect_fang_test(strIp.c_str(), nPort);
//    if (status < 0) {
//        return false;
//    }
    ostringstream ostr(strIp);
    ostr << ":" << nPort;
    CCLOG("addr string is : %s", ostr.str().c_str());

    try {
        clearSockGroup();//20131219
        NL::init();
        
        //设置不被SIGPIPE信号中断，物理链路损坏时才不会导致程序直接被Terminate
        //在网络异常的时候如果程序收到SIGPIPE是会直接被退出的。
        struct sigaction sig;
        sig.sa_handler = SIG_IGN;
        sigaction( SIGPIPE, &sig, 0 );
        
        if (m_pSockGroup == NULL) {
            m_pSockGroup = new SocketGroup();
            m_pSockGroup->setCmdOnRead(new ReceiveDeal());
            m_pSockGroup->setCmdOnDisconnect(new DisconnectDeal(m_mapSocket));
        }

        //zhangqi,20131219, 确认socket group 和监听线程已经创建成功后才创建socket
        Socket * _socket = new Socket(strIp, nPort);
        
        //pthread_mutex_lock(&m_mutexForSocketGroup);
        m_pSockGroup->add(_socket);
        CCLOG("socket(%s, %d) init OK", strIp.c_str(), nPort);
        m_mapSocket[ostr.str()] = _socket;//保存连接记录
        //pthread_mutex_unlock(&m_mutexForSocketGroup);

        if(g_pid==0 || pthread_kill(g_pid, 0) != 0) {// 如果网络监听线程不存在才创建新的监听线程
            g_pid = 0;
            int ret = pthread_create(&g_pid, NULL, BTNetClient::listen_thread, NULL);
            if (ret != 0) {
                string msg("create listen_thread error: ");
                msg.append(strerror(ret));
                CCLOG(msg.c_str());
                throw NL::Exception(NL::Exception::ERROR_ALLOC, msg); // add by zhangqi, 20130722，创建监听线程失败抛出异常
            }
        }
        
        return true;
    } catch (NL::Exception e) {
        CCLOG("socket init error: %s", e.what());
        addExceptionEvent("exception", "NL::Exception", e.what());
        return false;
    } catch (exception e) {
        CCLOG("connect other error: %s", e.what());
        addExceptionEvent("exception", "std::exception", e.what());
        return false;
    }
    
    return true;
}
// this function is deprecated.
bool BTNetClient::connect()
{
    return false;
}
