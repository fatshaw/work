#include <string>
#include <fstream>
#include <iostream>

#include "AccessControler.h"
#include "AccessControlerUtils.h"

int main(int arg, char* argv[])
{
    if (arg < 2) {
        std::cout << "too few params" << std::endl;
        return 0;
    }

    std::string dst_file = argv[1];

    if (0 > AccessControler::loadVisitorConfigure()) {
        std::cout << "load config fail!" << std::endl;
        return 0;
    }

    fstream fout(dst_file.c_str(), ios_base::out);
    if (fout.fail()) {
        std::cout << "open failed!" << std::endl;
        return 0;
    }

    if (0 > AccessControler::dumpAccessTableToFile(fout)) {
        std::cout << "fail to dump to file!" << std::endl;
        return 0;
    }

    return 0;
}
