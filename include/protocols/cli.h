/*
 * blupig
 * Copyright (C) 2016-2017 Yunzhu Li
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INCLUDE_PROTOCOLS_CLI_H_
#define INCLUDE_PROTOCOLS_CLI_H_

#include <string>
#include <unordered_map>

class RenjuProtocolCLI {
 public:
    RenjuProtocolCLI();
    ~RenjuProtocolCLI();

    static bool beginSession(int argc, char const *argv[]);

    // Generate move and responds in json
    static std::string generateMove(const char *gs_string, int ai_player_id, int search_depth,
                                    int time_limit, int num_threads);

 private:
    // Validates a string and parses into an integer
    static bool parseIntegerArgument(const char *str, int max_length, int *result);

    // Validates a string and returns the length.
    // If fails validation, -1 is returned.
    static int validateString(const char *str, int max_length);

    // Generate json response
    static std::string generateResultJson(const std::unordered_map<std::string, std::string> *data,
                                          const std::string &message);
};

#endif  // INCLUDE_PROTOCOLS_CLI_H_
