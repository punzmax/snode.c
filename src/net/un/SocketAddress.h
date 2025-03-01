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

#ifndef NET_UN_SOCKETADDRESS_H
#define NET_UN_SOCKETADDRESS_H

#include "net/SocketAddress.h"

// IWYU pragma: no_include "net/SocketAddress.hpp"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <exception>
#include <string>
#include <sys/un.h>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace net::un {

    class bad_sunpath : public std::exception {
    public:
        explicit bad_sunpath(const std::string& sunPath);

        const char* what() const noexcept override;

    private:
        std::string message;
    };

    class SocketAddress : public net::SocketAddress<sockaddr_un> {
    private:
        using Super = net::SocketAddress<sockaddr_un>;

    public:
        using net::SocketAddress<sockaddr_un>::SocketAddress;

        SocketAddress();
        explicit SocketAddress(const std::string& sunPath);

        void setSunPath(const std::string& sunPath);

        std::string address() const override;
        std::string toString() const override;
    };

} // namespace net::un

#endif // NET_UN_SOCKETADDRESS_H
