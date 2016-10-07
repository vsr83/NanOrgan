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

