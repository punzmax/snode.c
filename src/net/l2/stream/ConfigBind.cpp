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

#include "net/l2/stream/ConfigBind.h"

#include "utils/Config.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include "utils/CLI11.hpp"

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace net::l2::stream {

    ConfigBind::ConfigBind(CLI::App* baseSc) {
        bindSc = baseSc->add_subcommand("bind");
        bindSc->description("Bind options");
        bindSc->configurable();
        bindSc->required();

        bindHostOpt = bindSc->add_option("-a,--host", bindHost, "Bluetooth address");
        bindHostOpt->type_name("[bluetooth address]");
        bindHostOpt->default_val("00:00:00:00:00:00");
        bindHostOpt->configurable();

        bindPsmOpt = bindSc->add_option("-p,--psm", bindPsm, "Protocol service multiplexer");
        bindPsmOpt->type_name("[uint16_t]");
        bindPsmOpt->default_val(0);
        bindPsmOpt->configurable();
    }

    const std::string& ConfigBind::getBindHost() const {
        return bindHost;
    }

    uint16_t ConfigBind::getBindPsm() const {
        return bindPsm;
    }

    SocketAddress ConfigBind::getBindAddress() const {
        return net::l2::SocketAddress(bindHost, bindPsm);
    }

    void ConfigBind::required(bool req) const {
        utils::Config::instance().required(bindSc, req);
        utils::Config::instance().required(bindPsmOpt, req);
    }

} // namespace net::l2::stream
