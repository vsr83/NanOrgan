/*  NanOrgan - Simple Organ Synthesizer
 *
 *  Copyright (C) 2016 Ville Räisänen <vsr at vsr.name>
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <stdlib.h>
#include <string>
#include "Manager.h"

int
main(int argc, char **argv)
{
    unsigned int port;
    if (argc < 2) {
        std::cerr << "Usage " << argv[0] << " portnumber" << std::endl;
        return 1;
    } else {
        port = atoi(argv[1]);
        std::cout << "Using port " << port << std::endl;
    }
    Manager *manager = new Manager(port);

    std::cout << "Press Enter to quit." << std::endl;
    std::string foo;
    std::cin >> foo;

    delete manager;
    return 0;
}

