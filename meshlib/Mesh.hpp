#pragma once

#include <vector>
#include <memory>
#include "KeyPoint.hpp"
#include "Line.hpp"

using namespace std;

class Mesh {
public:
   vector<shared_ptr<KeyPoint>> Points;
   vector<shared_ptr<Line>> Lines; 
};