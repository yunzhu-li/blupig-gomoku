<?php
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

    // A simple PHP script as the HTTP interface of renju-parallel executable.
    // Receives data via GET and responds with stdout in the body.

    $cmdline = "bin/renju";

    if (isset($_GET['s'])) $cmdline .= ' -s ' . $_GET['s'];
    if (isset($_GET['p'])) $cmdline .= ' -p ' . $_GET['p'];
    if (isset($_GET['t'])) $cmdline .= ' -t ' . $_GET['t'];
    // if (isset($_GET['d'])) $cmdline .= ' -d ' . $_GET['d'];

    // Set search depth to 8
    $cmdline .= ' -d 8';

    // Allow access form any origin
    header("Access-Control-Allow-Origin: *");

    // Execute binary and print stdout
    echo(shell_exec($cmdline));
?>
