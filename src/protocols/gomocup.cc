/*
 * renju-parallel
 * Copyright (C) 2016 Yunzhu Li
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

#include <protocols/gomocup.h>
#include <api/renju_api.h>
#include <utils/globals.h>
#include <cstring>
#include <iostream>

bool RenjuProtocolGomocup::beginSession(int argc, char const *argv[]) {
    char line[256];
    char *gs_string = nullptr;
    bool errored = false;

    while (std::cin.getline(line, 256)) {
        // Commands
        if (strncmp(line, "START", 5) == 0) {
            // START
            unsigned int board_size = (unsigned int)atoi(&line[6]);
            if (board_size >= 15 && board_size <= 20) {
                g_board_size = board_size;
                g_gs_size = (unsigned int)g_board_size * g_board_size;

                // Initialize game state
                gs_string = new char[g_gs_size + 1];
                memset(gs_string, 0, g_gs_size + 1);
                memset(gs_string, '0', g_gs_size);

                // Write output
                writeStdout("OK");
            } else {
                writeStdout("ERROR");
                errored = true;
                break;
            }
        } else if (strncmp(line, "END", 3) == 0) {
            // END
            break;
        } else if (strncmp(line, "BEGIN", 5) == 0) {
            // BEGIN [SIZE]
            // Check board status
            if (gs_string == nullptr) {
                writeStdout("ERROR");
                errored = true;
                break;
            }

            // Reset board
            memset(gs_string, '0', g_gs_size);

            // Put a piece in center
            int move_r = g_board_size / 2, move_c = g_board_size / 2;
            gs_string[g_board_size * move_r + move_c] = '1';

            // Write output
            std::cout << move_c << "," << move_r << std::endl;

        } else if (strncmp(line, "BOARD", 5) == 0) {
            // BOARD
            // Check board status
            if (gs_string == nullptr) {
                writeStdout("ERROR");
                errored = true;
                break;
            }

            // Reset board
            memset(gs_string, '0', g_gs_size);

            while (std::cin.getline(line, 256)) {
                // [X],[Y],[field]
                if (strncmp(line, "DONE", 4) == 0) {
                    break;
                } else {
                    // Read piece
                    int values[3] = {-1, -1, -1};
                    splitLine(line, values);

                    if (values[2] == -1) {
                        writeStdout("ERROR");
                        errored = true;
                        break;
                    }

                    // Update board
                    gs_string[g_board_size * values[1] + values[0]] = '0' + static_cast<char>(values[2]);
                }
            }

            // Generate, perform a move and write to stdout
            performAndWriteMove(gs_string);

        } else if (strncmp(line, "TURN", 4) == 0) {
            // TURN [X],[Y]
            // Check board status
            if (gs_string == nullptr) {
                writeStdout("ERROR");
                errored = true;
                break;
            }

            // Read move
            int values[2] = {-1, -1};
            int move_r, move_c;
            splitLine(&line[5], values);
            move_c = values[0]; move_r = values[1];

            if (move_r == -1 || move_r >= g_board_size || move_c >= g_board_size) {
                writeStdout("ERROR");
                errored = true;
                break;
            }

            // Update board
            gs_string[g_board_size * move_r + move_c] = '2';

            // Generate, perform a move and write to stdout
            performAndWriteMove(gs_string);

        } else if (strncmp(line, "INFO", 4) == 0) {
        } else if (strncmp(line, "ABOUT", 5) == 0) {
            std::string build_datetime = __DATE__;
            build_datetime = build_datetime + " " + __TIME__;

            writeStdout("name=\"renju-parallel\", version=\"" + build_datetime + "\", author=\"Yunzhu Li\", country=\"China\"");
        } else {
            writeStdout("UNKNOWN");
        }
    }

    // Release memory
    if (gs_string != nullptr) delete[] gs_string;

    return !errored;
}

void RenjuProtocolGomocup::performAndWriteMove(char *gs_string) {
    // Generate move
    int move_r, move_c, winning_player, actual_depth;
    unsigned int node_count, eval_count;
    bool success = RenjuAPI::generateMove(gs_string, 1, -1, 7000, 1, &actual_depth, &move_r, &move_c,
                                          &winning_player, &node_count, &eval_count, nullptr);

    if (success) {
        // Write MESSAGE
        std::cout << "MESSAGE" <<
                     " d=" << actual_depth <<
                     " node_cnt=" << node_count <<
                     " eval_cnt=" << eval_count << std::endl;

        // Update board
        gs_string[g_board_size * move_r + move_c] = '1';

        // Write output
        std::cout << move_c << "," << move_r << std::endl;
    } else {
        writeStdout("ERROR");
    }
}

void RenjuProtocolGomocup::splitLine(const char *line, int *output) {
    // Copy input
    size_t in_length = strlen(line);
    char *_line = new char[in_length];
    memcpy(_line, line, in_length);

    int pos = 0, seg_idx = 0, seg_begin = 0;

    while (_line[pos] != 0) {
        if (_line[pos] == ',') {
            _line[pos] = 0;
            output[seg_idx++] = atoi(&_line[seg_begin]);
            seg_begin = pos + 1;
        }
        ++pos;
    }

    // Last one
    output[seg_idx] = atoi(&line[seg_begin]);

    delete[] _line;
}

void RenjuProtocolGomocup::writeStdout(std::string str) {
    std::cout << str << std::endl;
}
