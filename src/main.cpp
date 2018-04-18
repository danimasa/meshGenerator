#include <iostream>
#include <memory>
#include "Line.h"
using namespace std;

void print_Keypoint(const KeyPoint &point) {
    cout << "(" << point.x << ", " << point.y << ", " << point.z << ")" << endl;
};

void print_Line(const Line &line) {
   cout << "Initial Point: ";
   print_Keypoint(*line.init_point);
   cout << "Final Point: ";
   print_Keypoint(*line.final_point);
};

int main() {
    shared_ptr<KeyPoint> p1(new KeyPoint(0, 0, 0));
    shared_ptr<KeyPoint> p2(new KeyPoint(500, 0, 0));
    unique_ptr<Line> line(new Line(p1, p2));
    print_Line(*line);

    return 0;
}