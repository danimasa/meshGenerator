#pragma once

#include <string>
#include <sstream>
#include <iostream>

#include "KeyPoint.hpp"
#include "Line.hpp"
#include "Polyline.hpp"

#include "Mesh.hpp"

using namespace std;
using namespace geomlib;
using namespace meshlib;

namespace ansyslib {

class AnsysMacroPrinter {
public:
	AnsysMacroPrinter() {
		out << "/clear" << endl;
		out << "/prep7" << endl;
		out << endl;
	}

	void printKeypoint(KeyPoint* kp);
	void printSimpleLine(Line* line);
	void printPolyline(Polyline* polyline);
	void printBlankLine();
	void printDeleteLine(Line* line);
	void printDeleteKeypoint(KeyPoint* kp);

	void printNode(const Vertex* node);
	void printNodes(const vector<Vertex*> nodes);
	void printElement(const Quadrilateral* element);
	void printElements(const vector<Quadrilateral*> elements);

	string getString();

private:
	ostringstream out;
};

}