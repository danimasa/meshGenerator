#pragma once

#include <fstream>
#include <string>
#include "GeomDatabase.hpp"

using namespace std;

class FileReader {
private:
    string pvr_file;

public:
    FileReader(const string &file) : pvr_file(file) { }

    GeomDatabase read();
};