#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include <exception>
#include "Line.hpp"
#include "FileReader.hpp"
#include "AnsysFileReaderFactory.hpp"
#include "AnsysMacroWriter.hpp"
#include "LineAnalysis.hpp"
#include "GeometryFactory.hpp"
#include "meshlib.hpp"
#include "AnsysMeshWriter.hpp"
#include "AreaMesh.hpp"
#include "MeshShapesGenerator.hpp"
#include "MeshGenerator.hpp"
#include "AnsysMeshWriter.hpp"

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

void testMeshGeneration(const string &arquivo) {
    /* auto factory = GeometryFactory::getDefaultInstance();
    GeometryList geometries;

    auto p1 = Point(0, 0, 0);
    auto p2 = Point(1, 0, 0);
    auto p3 = Point(1, 2, 0);
    auto p4 = Point(0, 2, 0);
    auto p5 = Point(2, 0, 0);
    auto p6 = Point(2, 1.3, 0);
    auto p7 = Point(1, 1, 0);
    auto p8 = Point(3, 0, 0);
    auto p9 = Point(3, 0.5, 0);

    auto kp1 = factory->createKeypoint(p1);
    auto kp2 = factory->createKeypoint(p2);
    auto kp3 = factory->createKeypoint(p3);
    auto kp4 = factory->createKeypoint(p4);
    auto kp5 = factory->createKeypoint(p5);
    auto kp6 = factory->createKeypoint(p6);
    auto kp7 = factory->createKeypoint(p7);
    auto kp8 = factory->createKeypoint(p8);
    auto kp9 = factory->createKeypoint(p9);

    geometries.add(kp1);
    geometries.add(kp2);
    geometries.add(kp3);
    geometries.add(kp4);
    geometries.add(kp5);
    geometries.add(kp6);
    geometries.add(kp7);
    geometries.add(kp8);
    geometries.add(kp9);

    auto l1 = factory->createStraightLine(kp1, kp2);
    auto l2 = factory->createStraightLine(kp2, kp3);
    auto l3 = factory->createStraightLine(kp3, kp4);
    auto l4 = factory->createStraightLine(kp4, kp1);

    std::vector<Line*> lines ({l1, l2, l3, l4});

    geometries.add(l1);
    geometries.add(l2);
    geometries.add(l3);
    geometries.add(l4);

    Area::Loop loop (lines);
    std::vector<Area::Loop*> loops ({ &loop });
    auto a1 = factory->createArea(loops);
    geometries.add(a1);

    auto l5 = factory->createStraightLine(kp2, kp5);
    auto l6 = factory->createStraightLine(kp5, kp6);
    auto l7 = factory->createStraightLine(kp6, kp7);
    auto l8 = factory->createStraightLine(kp7, kp2);

    geometries.add(l5);
    geometries.add(l6);
    geometries.add(l7);
    geometries.add(l8);

    std::vector<Line*> lines2 ({l5, l6, l7, l8});
    Area::Loop loop2 (lines2);
    std::vector<Area::Loop*> loops2 ({ &loop2 });
    auto a2 = factory->createArea(loops2);
    geometries.add(a2);

    auto l9 = factory->createStraightLine(kp5, kp8);
    auto l10 = factory->createStraightLine(kp8, kp9);
    auto l11 = factory->createStraightLine(kp9, kp6);

    geometries.add(l9);
    geometries.add(l10);
    geometries.add(l11);

    std::vector<Line*> lines3 ({ l9, l10, l11, l6 });
    Area::Loop loop3 (lines3);
    std::vector<Area::Loop*> loops3 ({ &loop3 });
    auto a3 = factory->createArea(loops3);
    geometries.add(a3);*/

    ansyslib::AnsysFileReaderFactory factory {};
    geomlib::FileReader *reader = factory.createReader();
    GeometryList* geometry = reader->read(arquivo);

    double elementSize = 0.3;

    // AreaMesh area(lines, 0.4);
    // area.lines[2].qtdElements = 12;
    // MeshShapesGenerator gen;
    // auto mesh = gen.genMesh(shapeList, area);
    MeshGenerator generator(geometry, 0.15);
    Mesh mesh = generator.generateMesh();

    // AreaMesh meshGenerator(0.5);
    // Mesh mesh = area.generateMesh();

    ansyslib::MeshWriter mWriter(&mesh);
    cout << mWriter.getMshFile() << endl;
}

int main(int argc, char **argv) {
    // if(argc < 2) {
    //     help();
    //     return 1;
    // }

    string filepath = argv[1];
    // leitura(filepath);
    try {
        testMeshGeneration(filepath);
    } catch(std::exception& e) {
        cerr << "Exception: " << e.what();
    }

    return 0;
}

void leitura(const string &arquivo) {
    cout << "Lendo arquivo de Keypoints " << arquivo << " ..." << endl;

    // ansyslib::AnsysFileReaderFactory factory {};
    // geomlib::FileReader *reader = factory.createReader();
    // GeometryList* geometry = reader->read(arquivo);

    // processlib::LineAnalysis analyser(geometry);
    // analyser.findSingularities();
    // double elementSize = 0.3;

    // auto areaList = geometry->getListOf(GeometryType::Area);
    // vector<AreaMesh*> quadAreas;
    // for(auto areaGeom : areaList) {
    //     auto area = dynamic_cast<Area*>(areaGeom);
    //     auto quadArea = new AreaMesh(area->loops[0]->lines, elementSize);
    //     quadAreas.push_back(quadArea);
    // }

	// cout << "------------------------------" << endl;
	// cout << "Before Analyser" << endl;
	// cout << "------------------------------" << endl;

	// ansyslib::AnsysMacroWriter initWriter(geometry);
	// string initmacro = initWriter.getMacro();
	// cout << initmacro << endl;

    // processlib::LineAnalysis analyser(geometry);
    // analyser.findSingularities();

    // cout << "------------------------------" << endl;
    // cout << "Ansys Macro" << endl;
    // cout << "------------------------------" << endl;

    // ansyslib::AnsysMacroWriter writer(geometry);
    // string macro = writer.getMacro();
    // cout << macro << endl;
}

void help() {
    cout << "******* PROTOTIPO - Leitor de Keypoints de um arquivo de texto ********" << endl;
    cout << "" << endl;
    cout << "Forma de utilizacao:" << endl;
    cout << "meshGenerator [filepath]" << endl; 
}