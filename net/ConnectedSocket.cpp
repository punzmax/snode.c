#include <iostream>
#include <string.h>

#include "ConnectedSocket.h"
#include "SocketMultiplexer.h"
#include "ServerSocket.h"
#include "FileReader.h"


ConnectedSocket::ConnectedSocket(int csFd, Server* ss) : Socket(csFd), serverSocket(ss) {
}


ConnectedSocket::~ConnectedSocket() {
}


InetAddress& ConnectedSocket::getRemoteAddress() {
    return remoteAddress;
}


void ConnectedSocket::setRemoteAddress(const InetAddress& remoteAddress) {
    this->remoteAddress = remoteAddress;
}


void ConnectedSocket::send(const char* buffer, int size) {
    writeBuffer.append(buffer, size);
    SocketMultiplexer::instance().getWriteManager().manageSocket(this);
}


void ConnectedSocket::send(const std::string& junk) {
    writeBuffer += junk;
    SocketMultiplexer::instance().getWriteManager().manageSocket(this);
}


void ConnectedSocket::sendFile(const std::string& file) {
    FileReader::read(file,
                     [&] (char* data, int length) -> void {
                         if (length > 0) {
                             this->ConnectedSocket::send(data, length);
                         }
                     },
                     [&] (int err) -> void {
                         std::cout << "Error: " << strerror(err) << std::endl;
                         this->end();
                     });
}


void ConnectedSocket::end() {
    SocketMultiplexer::instance().getReadManager().unmanageSocket(this);
}


void ConnectedSocket::clearReadBuffer() {
    readBuffer.clear();
}


void ConnectedSocket::writeEvent() {
    ssize_t ret = ::send(this->getFd(), writeBuffer.c_str(), (writeBuffer.size() < 4096) ? writeBuffer.size() : 4096, MSG_DONTWAIT | MSG_NOSIGNAL);
    
    if (ret >= 0) {
        writeBuffer.erase(0, ret);
        if (writeBuffer.empty()) {
            SocketMultiplexer::instance().getWriteManager().unmanageSocket(this);
        }
    } else if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR) {
        SocketMultiplexer::instance().getWriteManager().unmanageSocket(this);
    }
}
