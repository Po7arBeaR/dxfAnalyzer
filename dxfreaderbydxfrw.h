
#ifndef DXFREADERBYDXFRW_H
#define DXFREADERBYDXFRW_H
#include "dx_data.h"
#include "dx_iface.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>

class dxfReaderBydxfrw
{
public:
    dxfReaderBydxfrw();
    bool extractText(std::string inName){
        bool badState = false;
        //verify if input file exist
        std::ifstream ifs;
        ifs.open (inName.c_str(), std::ifstream::in);
        badState = ifs.fail();
        ifs.close();
        if (badState) {
            std::cout << "Error can't open " << inName << std::endl;
            return false;
        }

        dx_data fData;
        dx_iface *input = new dx_iface();
        badState = input->fileImport( inName, &fData );
        if (!badState) {
            std::cout << "Error reading file " << inName << std::endl;
            return false;
        }
        delete input;

        return badState;
    }
};

#endif // DXFREADERBYDXFRW_H
