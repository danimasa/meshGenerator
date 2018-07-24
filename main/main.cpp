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

// void print_Keypoint(const KeyPoint &point) {
//     cout << "(" << point.x << ", " << point.y << ", " << point.z << ")" << endl;
// };

// void print_Line(const Line &line) {
//    cout << "Initial Point: ";
//    print_Keypoint(*line.init_point);
//    cout << "Final Point: ";
//    print_Keypoint(*line.final_point);
// };

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
    shared_ptr<GeometryList> geometry = reader->read(arquivo);
    cout << "Size: " << geometry->size() << endl;
    // for(auto p : database.keyPoints) {
    //     print_Keypoint(p);
    // }
}

void help() {
    cout << "******* PROTOTIPO - Leitor de Keypoints de um arquivo de texto ********" << endl;
    cout << "" << endl;
    cout << "Forma de utilizacao:" << endl;
    cout << "meshGenerator [filepath]" << endl; 
}