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

    double elementSize = 0.15;
    AppParams params {elementSize};
    GeometryFactory::init(params);
    auto factory = GeometryFactory::getDefaultInstance();

    GeometryList geometries;

    auto p1 = Point(0, 0, 0);
    auto p2 = Point(2, 0, 0);
    auto p3 = Point(2, 2, 0);
    auto p4 = Point(0, 2, 0);
    auto p5 = Point(0.5, 2, 0);
    auto p6 = Point(1.5, 2, 0);
    auto p7 = Point(1.5, 3, 0);
    auto p8 = Point(0.5, 3, 0);
    auto p9 = Point(1.1, -1, 0);
    auto p10 = Point(3.1, -1, 0);
    auto p11 = Point(3.1, 0, 0);
    auto p12 = Point(1.1, 0, 0);
    auto p13 = Point(0, 0.5, 1);
    auto p14 = Point(0, 0.5, -1);
    auto p15 = Point(-1, 0.5, -1);
    auto p16 = Point(-1, 0.5, 1);
    auto p17 = Point(0.5, 0, 0);
    auto p18 = Point(0.5, 0, 1);
    auto p19 = Point(0.5, -1, 1);
    auto p20 = Point(0.5, -1, 0);
    auto p21 = Point(0, 0.5, 0);

    auto kp1 = factory->createKeypoint(p1);
    auto kp2 = factory->createKeypoint(p2);
    auto kp3 = factory->createKeypoint(p3);
    auto kp4 = factory->createKeypoint(p4);
    auto kp5 = factory->createKeypoint(p5);
    auto kp6 = factory->createKeypoint(p6);
    auto kp7 = factory->createKeypoint(p7);
    auto kp8 = factory->createKeypoint(p8);
    auto kp9 = factory->createKeypoint(p9);
    auto kp10 = factory->createKeypoint(p10);
    auto kp11 = factory->createKeypoint(p11);
    auto kp12 = factory->createKeypoint(p12);
    auto kp13 = factory->createKeypoint(p13);
    auto kp14 = factory->createKeypoint(p14);
    auto kp15 = factory->createKeypoint(p15);
    auto kp16 = factory->createKeypoint(p16);
    auto kp17 = factory->createKeypoint(p17);
    auto kp18 = factory->createKeypoint(p18);
    auto kp19 = factory->createKeypoint(p19);
    auto kp20 = factory->createKeypoint(p20);
    auto kp21 = factory->createKeypoint(p21);

    geometries.add(kp1);
    geometries.add(kp2);
    geometries.add(kp3);
    geometries.add(kp4);
    geometries.add(kp5);
    geometries.add(kp6);
    geometries.add(kp7);
    geometries.add(kp8);
    geometries.add(kp9);
    geometries.add(kp10);
    geometries.add(kp11);
    geometries.add(kp12);
    geometries.add(kp13);
    geometries.add(kp14);
    geometries.add(kp15);
    geometries.add(kp16);
    geometries.add(kp17);
    geometries.add(kp18);
    geometries.add(kp19);
    geometries.add(kp20);
    geometries.add(kp21);

    auto l1 = factory->createStraightLine(kp1, kp17);
    auto l2 = factory->createStraightLine(kp17, kp12);
    auto l3 = factory->createStraightLine(kp12, kp2);
    auto l4 = factory->createStraightLine(kp2, kp3);
    auto l5 = factory->createStraightLine(kp3, kp6);
    auto l6 = factory->createStraightLine(kp6, kp5);
    auto l7 = factory->createStraightLine(kp5, kp4);
    auto l8 = factory->createStraightLine(kp4, kp21);
    auto l9 = factory->createStraightLine(kp21, kp1);
    
    std::vector<Line*> pLines1 ({l1, l2, l3});
    auto pl1 = factory->createPolyline(kp1, kp2, pLines1);

    std::vector<Line*> pLines2 ({l5, l6, l7});
    auto pl2 = factory->createPolyline(kp3, kp4, pLines2);

    std::vector<Line*> pLines3 ({l8, l9});
    auto pl3 = factory->createPolyline(kp4, kp1, pLines3);

    geometries.add(l1);
    geometries.add(l2);
    geometries.add(l3);
    geometries.add(l4);
    geometries.add(l5);
    geometries.add(l6);
    geometries.add(l7);
    geometries.add(l8);
    geometries.add(l9);
    geometries.add(pl1);
    geometries.add(pl2);
    geometries.add(pl3);

    std::vector<Line*> l_loop_1 ({pl1, l4, pl2, pl3});
    Area::Loop a_loop_1 (l_loop_1);
    std::vector<Area::Loop*> f_loop_1 ({ &a_loop_1 });
    auto a1 = factory->createArea(f_loop_1);
    geometries.add(a1);

    auto l10 = factory->createStraightLine(kp6, kp7);
    auto l11 = factory->createStraightLine(kp7, kp8);
    auto l12 = factory->createStraightLine(kp8, kp5);

    geometries.add(l10);
    geometries.add(l11);
    geometries.add(l12);

    std::vector<Line*> l_loop_2 ({l10, l11, l12, l6});
    Area::Loop a_loop_2 (l_loop_2);
    std::vector<Area::Loop*> f_loop_2 ({ &a_loop_2 });
    auto a2 = factory->createArea(f_loop_2);
    geometries.add(a2);

    auto l13 = factory->createStraightLine(kp9, kp10);
    auto l14 = factory->createStraightLine(kp10, kp11);
    auto l15 = factory->createStraightLine(kp11, kp2);
    auto l16 = factory->createStraightLine(kp12, kp9);

    std::vector<Line*> pLines4 ({l15, l3});
    auto pl4 = factory->createPolyline(kp11, kp12, pLines4);

    geometries.add(l13);
    geometries.add(l14);
    geometries.add(l15);
    geometries.add(l16);
    geometries.add(pl4);

    std::vector<Line*> l_loop_3 ({ l13, l14, pl4, l16 });
    Area::Loop a_loop_3 (l_loop_3);
    std::vector<Area::Loop*> f_loop_3 ({ &a_loop_3 });
    auto a3 = factory->createArea(f_loop_3);
    geometries.add(a3);

    auto l17 = factory->createStraightLine(kp17, kp18);
    auto l18 = factory->createStraightLine(kp18, kp19);
    auto l19 = factory->createStraightLine(kp19, kp20);
    auto l20 = factory->createStraightLine(kp20, kp17);

    geometries.add(l17);
    geometries.add(l18);
    geometries.add(l19);
    geometries.add(l20);

    std::vector<Line*> l_loop_4 ({ l17, l18, l19, l20 });
    Area::Loop a_loop_4 (l_loop_4);
    std::vector<Area::Loop*> f_loop_4 ({ &a_loop_4 });
    auto a4 = factory->createArea(f_loop_4);
    geometries.add(a4);

    auto l21 = factory->createStraightLine(kp13, kp21);
    auto l22 = factory->createStraightLine(kp21, kp14);
    auto l23 = factory->createStraightLine(kp14, kp15);
    auto l24 = factory->createStraightLine(kp15, kp16);
    auto l25 = factory->createStraightLine(kp16, kp13);

    std::vector<Line*> pLines5 ({l21, l22});
    auto pl5 = factory->createPolyline(kp13, kp14, pLines5);

    geometries.add(l21);
    geometries.add(l22);
    geometries.add(l23);
    geometries.add(l24);
    geometries.add(l25);
    geometries.add(pl5);

    std::vector<Line*> l_loop_5 ({ pl5, l23, l24, l25 });
    Area::Loop a_loop_5 (l_loop_5);
    std::vector<Area::Loop*> f_loop_5 ({ &a_loop_5 });
    auto a5 = factory->createArea(f_loop_5);
    geometries.add(a5);

    // ansyslib::AnsysFileReaderFactory factory {};
    // geomlib::FileReader *reader = factory.createReader();
    // GeometryList* geometry = reader->read(arquivo);

    // processlib::LineAnalysis analyser(geometry);
    // analyser.findSingularities();

    // AreaMesh area(lines, 0.4);
    // area.lines[2].qtdElements = 12;
    // MeshShapesGenerator gen;
    // auto mesh = gen.genMesh(shapeList, area);
    MeshGenerator generator(&geometries, params);
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