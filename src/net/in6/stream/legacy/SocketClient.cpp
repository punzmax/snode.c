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

#include "net/in6/stream/legacy/SocketClient.h"

#include "net/in6/stream/SocketClient.hpp" // IWYU pragma: keep

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif // DOXYGEN_SHOULD_SKIP_THIS

template class net::in6::stream::SocketClient<net::in6::stream::legacy::config::ConfigSocketClient>;
template class net::stream::SocketClient<net::in6::stream::ClientSocket, net::in6::stream::legacy::config::ConfigSocketClient>;
template class net::SocketConfig<net::in6::stream::legacy::config::ConfigSocketClient>;
