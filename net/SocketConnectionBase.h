#ifndef BASECONNECTEDSOCKET_H
#define BASECONNECTEDSOCKET_H

#include "InetAddress.h"
#include "SocketConnectionInterface.h"

class FileReader;
class SocketServerInterface;

template<typename R, typename W>
class SocketConnectionBase : public SocketConnectionInterface, public R, public W
{
public:
    SocketConnectionBase(int csFd,
                     SocketServerInterface* serverSocket,
                     const std::function<void (SocketConnectionInterface* cs, const char* junk, ssize_t n)>& readProcessor,
                     const std::function<void (int errnum)>& onReadError,
                     const std::function<void (int errnum)>& onWriteError
    );
    
    virtual ~SocketConnectionBase();

    void setContext(void* context) {
        this->context = context;
    }

    void* getContext() {
        return this->context;
    }

    virtual void send(const char* puffer, int size);
    virtual void send(const std::string& junk);
    virtual void sendFile(const std::string& file, const std::function<void (int ret)>& onError);
    virtual void end();

    virtual InetAddress& getRemoteAddress();
    virtual void setRemoteAddress(const InetAddress& remoteAddress);

protected:
    SocketServerInterface* serverSocket;
    
    void* context;
    
    InetAddress remoteAddress;

    FileReader* fileReader;
};

#endif // BASECONNECTEDSOCKET_H
