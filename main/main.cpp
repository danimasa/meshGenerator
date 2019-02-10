#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include "Line.hpp"
#include "FileReader.hpp"
#include "AnsysFileReaderFactory.hpp"
using namespace std;

void help();
void leitura(const string &filepath);

void print_Keypoint(const KeyPoint *point) {
    cout << "(" << point->x << ", " << point->y << ", " << point->z << ")" << endl;
};

string lineName(LineType type) {
    switch (type)
    {
        case LineType::StraightLine:
            return "Straight Line";
        case LineType::ArcLine:
            return "Arc Line";
        case LineType::Polyline:
            return "Polyline";
        case LineType::UnspecifiedLine:
            return "Unspecified Line";
        default:
            return "";
    }
}

void print_Line(const Line *line) {
   cout << "(" << line->getID() << ") type: " << lineName(line->getLineType()) << endl;
};

int main(int argc, char **argv) {
    if(argc < 2) {
        help();
        return 1;
    }

    string filepath = argv[1];
    leitura(filepath);

    return 0;
}

void leitura(const string &arquivo) {
    cout << "Lendo arquivo de Keypoints " << arquivo << " ..." << endl;

    ansyslib::AnsysFileReaderFactory factory {};
    geomlib::FileReader *reader = factory.createReader();
    GeometryList* geometry = reader->read(arquivo);
    cout << "Size: " << geometry->size() << endl;
    auto kplist = geometry->getListOf("keypoint");
    cout << "Keypoints: " << kplist.size() << endl;
    for(auto p : kplist) {
        auto keypoint = dynamic_cast<KeyPoint*>(p);
        print_Keypoint(keypoint);
    }

    auto lineList = geometry->getListOf("line");
    cout << "Linhas: " << lineList.size() << endl;
    for(auto l : lineList) {
        auto linha = dynamic_cast<Line*>(l);
        print_Line(linha);
    }
}

void help() {
    cout << "******* PROTOTIPO - Leitor de Keypoints de um arquivo de texto ********" << endl;
    cout << "" << endl;
    cout << "Forma de utilizacao:" << endl;
    cout << "meshGenerator [filepath]" << endl; 
}