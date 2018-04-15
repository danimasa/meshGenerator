#include <iostream>
#include "Line.h"
using namespace std;

void print_Keypoint(KeyPoint* point) {
    cout << "(" << point->x << ", " << point->y << ", " << point->z << ")" << endl;
};

void print_Line(Line* line) {
   cout << "Initial Point: ";
   print_Keypoint(line->init_point);
   cout << "Final Point: ";
   print_Keypoint(line->final_point);
};

int main() {
    auto p1 = new KeyPoint(0, 0, 0);
    auto p2 = new KeyPoint(500, 0, 0);
    auto line = new Line(p1, p2);
    print_Line(line);

    delete line;
    delete p1;
    delete p2;

    return 0;
}