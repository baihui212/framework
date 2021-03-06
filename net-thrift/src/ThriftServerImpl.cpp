// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.
/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file ThriftServerImpl.cpp
* @brief thrift服务器实现文件
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2015-11-25
*/

#include "ThriftServerImpl.h"

/**
* @brief thrift自动生成的代码，用于接收客户端发送的消息
*/
class RPCMessageServiceHandler : virtual public RPCMessageServiceIf 
{
  public:
    RPCMessageServiceHandler(ThriftServerImpl* thriftServer)
    {
        // Your initialization goes here

        m_thriftServer = thriftServer;
    }

    void sendMessage(std::string& _return, const std::string& message)
    {
        // Your implementation goes here
        
        Message* msg = new Message;
        msg->deserializeSelf(message);
        
        if (m_thriftServer->m_messageCallback != NULL)
        {
            Message* retMsg = new Message;
            retMsg->m_messageType = msg->m_messageType;
            m_thriftServer->m_messageCallback(msg, retMsg);
            if (retMsg != NULL)
            {
                _return = retMsg->serializeSelf();
                delete retMsg;
                retMsg = NULL;
            }
        }
        else
        {
            std::cout << "Message callback function is NULL" << std::endl;
        }

        if (msg != NULL)
        {
            delete msg;
            msg = NULL;
        }
    }

  private:
    ThriftServerImpl*       m_thriftServer;
};

ThriftServerImpl::ThriftServerImpl()
    : m_messageCallback(NULL),
    m_threadedServer(NULL),
    m_thread(NULL),
    m_port(9090)
{
    // Do nothing.
}

ThriftServerImpl::~ThriftServerImpl()
{
    deinit();
}

void ThriftServerImpl::init(const unsigned int& port)
{
    m_port = port;
}

bool ThriftServerImpl::start()
{
    try
    {
        if (m_thread == NULL)
        {
            m_thread = new std::thread(serverStart, this);
        }
    } catch (TException& e)
    {
        std::cout << "Start server failed, error: " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool ThriftServerImpl::stop()
{
    try
    {
        if (m_threadedServer != NULL)
        {
            m_threadedServer->stop();

            if (m_thread != NULL)
            {
                m_thread->join();

                delete m_thread;
                m_thread = NULL;
            }

            delete m_threadedServer;
            m_threadedServer = NULL;
        }
    } catch (TException& e)
    {
        std::cout << "Stop server failed, error: " << e.what() << std::endl;
        return false;
    }

    return true;
}

void ThriftServerImpl::deinit()
{
    // Do nothing.
}

void ThriftServerImpl::serverStart(ThriftServerImpl* server)
{
    shared_ptr<RPCMessageServiceHandler> handler(new RPCMessageServiceHandler(server));
    shared_ptr<TProcessor> processor(new RPCMessageServiceProcessor(handler));
    shared_ptr<TServerTransport> serverTransport(new TServerSocket(server->m_port));
    shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    server->m_threadedServer = new TThreadedServer(processor, serverTransport, transportFactory, protocolFactory);

    try
    {
        server->m_threadedServer->serve();
    } catch (TException& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void ThriftServerImpl::setMessageCallback(MESSAGE_CALLBACK func)
{
    if (func != NULL)
    {
        m_messageCallback = func;
    }
}
