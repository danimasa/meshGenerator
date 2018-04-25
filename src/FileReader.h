#pragma once

#include <fstream>
#include <string>
#include <vector>
#include "KeyPoint.h"

using namespace std;

class FileReader {
private:
    string pvr_file;

public:
    FileReader(const string &file) : pvr_file(file) { }

    vector<KeyPoint> read();
};