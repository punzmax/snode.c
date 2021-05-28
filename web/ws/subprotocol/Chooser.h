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

#ifndef WEB_WS_SUBPROTOCOL_CHOOSER_H
#define WEB_WS_SUBPROTOCOL_CHOOSER_H

#include "web/ws/WSProtocol.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <map>
#include <string>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace web::ws::subprotocol {

    struct SubProtocol {
        void* handle;
        const char* (*name)();
        web::ws::WSProtocol* (*create)();
        void (*destroy)(web::ws::WSProtocol* echo);
        web::ws::WSProtocol::Role (*role)();
    };

    class Chooser {
    public:
        Chooser();
        ~Chooser();

        WSProtocol* select(const std::string& subProtocol, web::ws::WSProtocol::Role role);

    public:
        void loadSubprotocols();

    protected:
        std::map<std::string, SubProtocol> serverSubprotocols;
        std::map<std::string, SubProtocol> clientSubprotocols;
    };

} // namespace web::ws::subprotocol

#endif // WEB_WS_SUBPROTOCOL_CHOOSER_H
