/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020, 2021, 2022 Volker Christian <me@vchrist.at>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CORE_SOCKET_STREAM_SOCKETCONNECTOR_H
#define CORE_SOCKET_STREAM_SOCKETCONNECTOR_H

#include "core/eventreceiver/ConnectEventReceiver.h"
#include "core/socket/stream/SocketConnectionFactory.h"

namespace core::socket {
    class SocketContextFactory;
} // namespace core::socket

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <any>
#include <functional>
#include <map>
#include <memory>
#include <string>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace core::socket::stream {

    template <typename SocketClientT, template <typename SocketT> class SocketConnectionT>
    class SocketConnector
        : protected core::eventreceiver::InitConnectEventReceiver
        , protected core::eventreceiver::ConnectEventReceiver {
    private:
        using SocketClient = SocketClientT;
        using PrimarySocket = typename SocketClient::Socket;

    protected:
        using SocketConnection = SocketConnectionT<PrimarySocket>;
        using SocketConnectionFactory = core::socket::stream::SocketConnectionFactory<SocketClient, SocketConnection>;

    public:
        using Config = typename SocketClient::Config;
        using SocketAddress = typename SocketClient::SocketAddress;

        SocketConnector() = delete;
        SocketConnector(const SocketConnector&) = delete;

        SocketConnector& operator=(const SocketConnector&) = delete;

        SocketConnector(const std::shared_ptr<core::socket::SocketContextFactory>& socketContextFactory,
                        const std::function<void(SocketConnection*)>& onConnect,
                        const std::function<void(SocketConnection*)>& onConnected,
                        const std::function<void(SocketConnection*)>& onDisconnect,
                        const std::map<std::string, std::any>& options)
            : core::eventreceiver::InitConnectEventReceiver("SocketConnector")
            , core::eventreceiver::ConnectEventReceiver("SocketConnector")
            , socketConnectionFactory(socketContextFactory, onConnect, onConnected, onDisconnect)
            , options(options) {
        }

        ~SocketConnector() override {
            if (socket != nullptr) {
                delete socket;
            }
        }

        void connect(const std::shared_ptr<Config>& clientConfig, const std::function<void(const SocketAddress&, int)>& onError) {
            this->config = clientConfig;
            this->onError = onError;

            InitConnectEventReceiver::publish();
        }

    private:
        void initConnectEvent() override {
            socket = new PrimarySocket();
            if (socket->open(PrimarySocket::Flags::NONBLOCK) < 0) {
                onError(config->getRemoteAddress(), errno);
                destruct();
            } else if (socket->bind(config->getLocalAddress()) < 0) {
                onError(config->getRemoteAddress(), errno);
                destruct();
            } else if (socket->connect(config->getRemoteAddress()) < 0 && !socket->connectInProgress(errno)) {
                onError(config->getRemoteAddress(), errno);
                destruct();
            } else {
                enable(socket->getFd());
            }
        }

        void connectEvent() override {
            int cErrno = -1;

            if ((cErrno = socket->getSockError()) >= 0) { //  >= 0->return valid : < 0->getsockopt failed errno = cErrno;
                if (!socket->connectInProgress(cErrno)) {
                    if (cErrno == 0) {
                        disable();
                        socketConnectionFactory.create(*socket, config);
                        errno = errno == 0 ? cErrno : errno;
                        onError(config->getRemoteAddress(), errno);
                    } else {
                        disable();
                        errno = errno == 0 ? cErrno : errno;
                        onError(config->getRemoteAddress(), errno);
                    }
                } else {
                    // Do nothing: connect() still in progress
                }
            } else {
                disable();
                onError(config->getRemoteAddress(), cErrno);
            }
        }

    protected:
        void destruct() {
            delete this;
        }

        std::shared_ptr<Config> config = nullptr;

    private:
        void unobservedEvent() override {
            destruct();
        }

        PrimarySocket* socket = nullptr;

        SocketConnectionFactory socketConnectionFactory;

    protected:
        std::function<void(const SocketAddress& socketAddress, int err)> onError;

        std::map<std::string, std::any> options;
    };

} // namespace core::socket::stream

#endif // CORE_SOCKET_STREAM_SOCKETCONNECTOR_H
