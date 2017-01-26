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

#ifndef INCLUDE_PROTOCOLS_GOMOCUP_H_
#define INCLUDE_PROTOCOLS_GOMOCUP_H_

#include <string>

class RenjuProtocolGomocup {
 public:
    RenjuProtocolGomocup();
    ~RenjuProtocolGomocup();

    static bool beginSession(int argc, char const *argv[]);

 private:
    static void performAndWriteMove(char *gs_string, int time_limit);
    static void splitLine(const char *line, int *output);
    static void writeStdout(std::string str);
};

#endif  // INCLUDE_PROTOCOLS_GOMOCUP_H_
