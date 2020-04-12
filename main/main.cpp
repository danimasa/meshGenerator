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
#include "MeshWriter.hpp"
#include "AnsysMeshWriter.hpp"
#include "AreaMesh.hpp"
#include "MeshShapesGenerator.hpp"
#include "MeshGenerator.hpp"

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
    GeometryList geometries;

    auto p1 = Point(0, 0, 0);
    auto p2 = Point(3, 2, 0);
    auto p3 = Point(3, 4, 0);
    auto p4 = Point(2, 5, 0);

    // auto p5 = Point(1, 1, 0);
    // auto p6 = Point(0.3, 0, 0);
    // auto p5 = Point(1.9302474402, 2.8027895855, 0);
    // auto p6 = Point(0.2616991886, 1.2254422329, 0);

    auto kp1 = factory->createKeypoint(p1);
    auto kp2 = factory->createKeypoint(p2);
    auto kp3 = factory->createKeypoint(p3);
    auto kp4 = factory->createKeypoint(p4);

    geometries.add(kp1);
    geometries.add(kp2);
    geometries.add(kp3);
    geometries.add(kp4);
    // auto kp5 = factory->createKeypoint(p5);
    // auto kp6 = factory->createKeypoint(p6);

    // Vector v1(0, 1, 0);
    // Vector v2(1, 0, 0);
    // Vector v3(0.7456954357, 0.6662869631, 0);
    // auto l5 = factory->createStraightLine(kp1, kp6);
    // auto l6 = factory->createStraightLine(kp6, kp5);
    // auto l7 = factory->createStraightLine(kp5, kp2);
    // vector<Line*> pLines { l5, l6, l7 };

    auto l1 = factory->createStraightLine(kp1, kp2);
    // auto l1 = factory->createPolyline(kp1, kp2, pLines);
    auto l2 = factory->createStraightLine(kp2, kp3);
    auto l3 = factory->createStraightLine(kp3, kp4);
    // auto l3 = factory->createArcLine(kp4, kp3, &p5, &v3, &v2);
    auto l4 = factory->createStraightLine(kp4, kp1);
    // auto l4 = factory->createArcLine(kp1, kp4, &p6, &v1, &v3);

    // std::vector<Line*> lines;
    // lines.push_back(l1);
    // lines.push_back(l2);
    // lines.push_back(l3);
    // lines.push_back(l4);
    geometries.add(l1);
    geometries.add(l2);
    geometries.add(l3);
    geometries.add(l4);

    // AreaMesh area(lines, 0.1);
    // area.lines[1].qtdElements = area.east().qtdElements + 1;
    // area.lines[0].qtdElements = area.south().qtdElements + 1;

    // vector<MeshShapes::RelativeShapes> shapeList;
    // shapeList.push_back(MeshShapes::RelativeShapes::PPOO_270);
    // shapeList.push_back(MeshShapes::RelativeShapes::POPO_0);
    // shapeList.push_back(MeshShapes::RelativeShapes::POPO_0);
    // shapeList.push_back(MeshShapes::RelativeShapes::POPO_0);

    // AreaMesh area(lines, 0.4);
    // area.lines[2].qtdElements = 12;
    // MeshShapesGenerator gen;
    // auto mesh = gen.genMesh(shapeList, area);
    MeshGenerator generator(&geometries, 0.2);
    Mesh mesh = generator.generateMesh();

    // AreaMesh meshGenerator(0.5);
    // Mesh mesh = area.generateMesh();

    // gmshlib::MeshWriter writer(&mesh);
    ansyslib::MeshWriter writer(&mesh);
    cout << writer.getMshFile() << endl;
}

int main(int argc, char **argv) {
    // if(argc < 2) {
    //     help();
    //     return 1;
    // }

    // string filepath = argv[1];
    // leitura(filepath);
    try {
        testMeshGeneration();
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