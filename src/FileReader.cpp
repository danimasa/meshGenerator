#include "FileReader.h"

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "stringUtils.h"

KeyPoint readPoint(const string &line) {
    vector<string> strs;
    boost::split(strs, line, boost::is_any_of("\t "), boost::token_compress_on);
    double x = boost::lexical_cast<double>(strs[1]);
    double y = boost::lexical_cast<double>(strs[2]);
    double z = boost::lexical_cast<double>(strs[3]);
    KeyPoint p (x, y, z);
    return p;
}

vector<KeyPoint> readKeyPoints(ifstream &file) {
    vector<KeyPoint> points;
    string line;
    // Descartando a quantidade de pontos
    getline(file, line);
    // Lendo os KeyPoints
    while(getline(file, line)){
        trim(line);
        if(line == "-9876.") return points;
        KeyPoint p = readPoint(line);
        points.push_back(p);
    }
    return points;
}

vector<KeyPoint> readBlock(ifstream &file) {
    vector<KeyPoint> points;
    string line;
    getline(file, line);
    trim(line);
    if(line == "10.") {
        points = readKeyPoints(file);
    }
    return points;
}

vector<KeyPoint> FileReader::read() {
    vector<KeyPoint> keyPoints;

    string line;
    ifstream file (pvr_file);
    if(file.is_open()) {
        while(getline(file, line)) {
            trim(line);
            if(line == "9999.") {
                auto kps = readBlock(file);
                keyPoints.insert(keyPoints.begin(), kps.begin(), kps.end());
            }
        }
        file.close();
    }
    else cout << "Incapaz de encontrar o arquivo especificado!";
    
    return keyPoints;
}