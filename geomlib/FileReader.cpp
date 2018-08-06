#include "FileReader.hpp"

#include <iostream>
#include <fstream>

namespace geomlib {

GeometryList* FileReader::read(const string filePath) {
    string line;
    ifstream file (filePath);
    if(file.is_open()) {
        while(getline(file, line)) {
            builder->readFileLine(line);
        }
    }
    return builder->getGeometryList();
}

}