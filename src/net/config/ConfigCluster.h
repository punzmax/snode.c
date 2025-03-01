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

#ifndef NET_CONFIG_CONFIGCLUSTER_H
#define NET_CONFIG_CONFIGCLUSTER_H

#include "net/config/ConfigBase.h" // IWYU pragma: export

namespace CLI {
    class App;
    class Option;
} // namespace CLI

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace net::config {

    class ConfigCluster : virtual public ConfigBase {
    public:
        ConfigCluster();

        enum MODE { NONE, PRIMARY, SECONDARY, PROXY };

        int getClusterMode() const;

    private:
        CLI::App* clusterSc = nullptr;
        CLI::Option* modeOpt = nullptr;

        MODE mode = MODE::NONE;
    };

} // namespace net::config

#endif // NET_CONFIG_CONFIGCLUSTER_H
