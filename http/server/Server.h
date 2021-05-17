#ifndef HTTP_SERVER_SERVERT_H
#define HTTP_SERVER_SERVERT_H

#include "http/server/http/HTTPServerContext.hpp"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <any>
#include <cstddef>
#include <functional>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <string>

#endif // DOXYGEN_SHOULD_SKIP_THIS

namespace http::server {

    template <typename SocketServerT, typename RequestT, typename ResponseT>
    class Server {
    public:
        using SocketServer = SocketServerT;
        using SocketConnection = typename SocketServer::SocketConnection;
        using SocketAddress = typename SocketConnection::SocketAddress;
        using Request = RequestT;
        using Response = ResponseT;

        Server(const std::function<void(const SocketAddress&, const SocketAddress&)>& onConnect,
               const std::function<void(SocketConnection*)>& onConnected,
               const std::function<void(Request& req, Response& res)>& onRequestReady,
               const std::function<void(SocketConnection*)>& onDisconnect,
               const std::map<std::string, std::any>& options = {{}})
            : socketServer(
                  [onConnect](const SocketAddress& localAddress,
                              const SocketAddress& remoteAddress) -> void { // OnConnect
                      onConnect(localAddress, remoteAddress);
                  },
                  [onConnected, onRequestReady](SocketConnection* socketConnection) -> void { // onConnected.
                      socketConnection->setSocketProtocol(new HTTPServerContext<Request, Response>(socketConnection, onRequestReady));

                      onConnected(socketConnection);
                  },
                  [onDisconnect](SocketConnection* socketConnection) -> void { // onDisconnect
                      onDisconnect(socketConnection);

                      delete socketConnection->getSocketProtocol();
                  },
                  [](SocketConnection* socketConnection, const char* junk, std::size_t junkLen) -> void { // onRead
                      static_cast<ServerContext*>(socketConnection->getSocketProtocol())->take(junk, junkLen);
                  },
                  [](SocketConnection* socketConnection, int errnum) -> void { // onReadError
                      static_cast<ServerContext*>(socketConnection->getSocketProtocol())->onReadError(errnum);
                  },
                  [](SocketConnection* socketConnection, int errnum) -> void { // onWriteError
                      static_cast<ServerContext*>(socketConnection->getSocketProtocol())->onWriteError(errnum);
                  },
                  options) {
        }

        void listen(uint16_t port, const std::function<void(int err)>& onError) const {
            socketServer.listen(SocketAddress(port), 5, onError);
        }

        void listen(const std::string& ipOrHostname, uint16_t port, const std::function<void(int err)>& onError) const {
            socketServer.listen(SocketAddress(ipOrHostname, port), 5, onError);
        }

    protected:
        SocketServer socketServer;
    };

} // namespace http::server

#endif // HTTP_SERVER_SERVERT_H
