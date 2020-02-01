#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include "Line.hpp"
#include "FileReader.hpp"
#include "AnsysFileReaderFactory.hpp"
#include "AnsysMacroWriter.hpp"
#include "LineAnalysis.hpp"
#include "GeometryFactory.hpp"
#include "meshlib.hpp"
#include "MeshWriter.hpp"
#include "AreaMesh.hpp"

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

void testMeshGeneration() {
    auto factory = GeometryFactory::getDefaultInstance();

    auto p1 = Point(0, 0, 0);
    auto p2 = Point(5, 2, 3);
    auto p3 = Point(4, 6, 2);
    auto p4 = Point(1, 5, 1);
    auto p5 = Point(2, 6, 3);
    auto p6 = Point(2.5, 1, 5);
    auto p7 = Point(1.5, 0.5, 1);
    auto p8 = Point(4, 2, 3);

    auto kp1 = factory->createKeypoint(p1);
    auto kp2 = factory->createKeypoint(p2);
    auto kp3 = factory->createKeypoint(p3);
    auto kp4 = factory->createKeypoint(p4);
    auto kp5 = factory->createKeypoint(p5);
    auto kp6 = factory->createKeypoint(p6);
    auto kp7 = factory->createKeypoint(p7);
    auto kp8 = factory->createKeypoint(p8);

    Vector v1(0, 0, 1);
    Vector v2(0, 0, -1);

    auto l5 = factory->createStraightLine(kp1, kp7);
    auto l6 = factory->createStraightLine(kp7, kp6);
    auto l7 = factory->createStraightLine(kp6, kp8);
    auto l8 = factory->createStraightLine(kp8, kp2);
    vector<Line*> polyList { l5, l6, l7, l8 };

    auto l1 = factory->createPolyline(kp1, kp2, polyList);
    auto l2 = factory->createStraightLine(kp2, kp3);
    auto l3 = factory->createArcLine(kp3, kp4, kp5, &v1, &v2);
    auto l4 = factory->createStraightLine(kp4, kp1);

    std::vector<Line*> lines;
    lines.push_back(l1);
    lines.push_back(l2);
    lines.push_back(l3);
    lines.push_back(l4);

    QuadArea area(lines);

    AreaMesh meshGenerator(0.2);
    Mesh mesh = meshGenerator.generateMesh(&area);
    // Mesh regMesh = meshlib::generateRegGrid(20, 20);
    // auto transfMesh = meshlib::transfiniteMapping(regMesh, area);

    gmshlib::MeshWriter writer(&mesh);
    cout << writer.getMshFile() << endl;
}

int main(int argc, char **argv) {
    // if(argc < 2) {
    //     help();
    //     return 1;
    // }

    // string filepath = argv[1];
    // leitura(filepath);
    testMeshGeneration();

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