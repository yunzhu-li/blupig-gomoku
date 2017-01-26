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

#include <protocols/cli.h>
#include <protocols/gomocup.h>
#include <cstring>

// Exclude main() if building with tests
#ifndef BLUPIG_TEST

int main(int argc, char const *argv[]) {
    if (argc <= 0) return 1;

    // Select Gomocup protocol if "pbrain' found in file name
    bool success;
    if (strstr(argv[0], "pbrain") != nullptr) {
        success = RenjuProtocolGomocup::beginSession(argc, argv);
    } else {
        success = RenjuProtocolCLI::beginSession(argc, argv);
    }
    return !success;
}

#endif  // BLUPIG_TEST
