#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include "Line.hpp"
#include "FileReader.hpp"
#include "AnsysFileReaderFactory.hpp"
#include "AnsysMacroWriter.hpp"
#include "LineAnalysis.hpp"

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

// void print_area(const Area *area) {
//     cout << "(" << area->getID() << ") - ";
//     for(auto line : area->lines) {
//         cout << line->getID() << ", ";
//     }
//     cout << endl;
// }

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
    /*cout << "Size: " << geometry->size() << endl;
    auto kplist = geometry->getListOf(GeometryType::Keypoint);
    cout << "Keypoints: " << kplist.size() << endl;
    for(auto p : kplist) {
        auto keypoint = dynamic_cast<KeyPoint*>(p);
        print_Keypoint(keypoint);
    }

    auto lineList = geometry->getListOf(GeometryType::Line);
    cout << "Linhas: " << lineList.size() << endl;
    for(auto l : lineList) {
        auto linha = dynamic_cast<Line*>(l);
        print_Line(linha);
    }

    auto areaList = geometry->getListOf(GeometryType::Area);
    cout << "Areas: " << areaList.size() << endl;
    for(auto a : areaList) {
        auto area = dynamic_cast<Area*>(a);
        print_area(area);
    }*/
	cout << "------------------------------" << endl;
	cout << "Before Analyser" << endl;
	cout << "------------------------------" << endl;

	ansyslib::AnsysMacroWriter initWriter(geometry);
	string initmacro = initWriter.getMacro();
	cout << initmacro << endl;

    processlib::LineAnalysis analyser(geometry);
    analyser.findSingularities();

    cout << "------------------------------" << endl;
    cout << "Ansys Macro" << endl;
    cout << "------------------------------" << endl;

    ansyslib::AnsysMacroWriter writer(geometry);
    string macro = writer.getMacro();
    cout << macro << endl;
}

void help() {
    cout << "******* PROTOTIPO - Leitor de Keypoints de um arquivo de texto ********" << endl;
    cout << "" << endl;
    cout << "Forma de utilizacao:" << endl;
    cout << "meshGenerator [filepath]" << endl; 
}