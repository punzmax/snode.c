/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020, 2021 Volker Christian <me@vchrist.at>
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

#ifndef NET_SOCKET_STREAM_SOCKETREADER_H
#define NET_SOCKET_STREAM_SOCKETREADER_H

#include "net/ReadEventReceiver.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <cstddef> // for std::size_t
#include <functional>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#ifndef MAX_READ_JUNKSIZE
#define MAX_READ_JUNKSIZE 16384
#endif

namespace net::socket::stream {

    template <typename SocketT>
    class SocketReader
        : public ReadEventReceiver
        , virtual public SocketT {
        SocketReader() = delete;

    public:
        using Socket = SocketT;

    protected:
        explicit SocketReader(const std::function<void(const char* junk, std::size_t junkLen)>& onRead,
                              const std::function<void(int errnum)>& onError)
            : onRead(onRead)
            , onError(onError) {
        }

        virtual ~SocketReader() = default;

        void shutdown() {
            if (!isEnabled()) {
                Socket::shutdown(Socket::shut::RD);
            } else {
                markShutdown = true;
            }
        }

    private:
        virtual ssize_t read(char* junk, std::size_t junkLen) = 0;

        void readEvent() override {
            // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays, modernize-avoid-c-arrays)
            static char junk[MAX_READ_JUNKSIZE];

            ssize_t ret = read(junk, MAX_READ_JUNKSIZE);

            if (ret > 0) {
                onRead(junk, static_cast<std::size_t>(ret));
            } else if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR) {
                ReadEventReceiver::disable();
                if (markShutdown) {
                    Socket::shutdown(Socket::shutdown::RD);
                }
                onError(getError());
            }
        }

        virtual int getError() = 0;

        std::function<void(const char* junk, std::size_t junkLen)> onRead;
        std::function<void(int errnum)> onError;

        bool markShutdown = false;
    };

} // namespace net::socket::stream

#endif // NET_SOCKET_STREAM_SOCKETREADER_H
