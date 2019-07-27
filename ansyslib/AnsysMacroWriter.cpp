#include "AnsysMacroWriter.hpp"
#include "AnsysMacroPrinter.hpp"
#include "Polyline.hpp"
#include "GeometryFactory.hpp"

#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>

namespace ansyslib {

double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

KeyPoint* AnsysMacroWriter::generateGhostPoint() {
	int initX = round(greaterX) + 1;
	int initY = round(greaterY) + 1;
	int initZ = round(greaterZ) + 1;

	double pX = fRand(initX, initX + 10.0);
	double pY = fRand(initY, initY + 10.0);
	double pZ = fRand(initZ, initZ + 10.0);

	Point p(pX, pY, pZ);

	auto factory = GeometryFactory::getDefaultInstance();
	return factory->createKeypoint(p);
}

Line* AnsysMacroWriter::generateGhostLine() {
	auto p1 = generateGhostPoint();
	auto p2 = generateGhostPoint();
	
	auto factory = GeometryFactory::getDefaultInstance();
	return factory->createStraightLine(p1, p2);
}

string AnsysMacroWriter::getMacro() {
  AnsysMacroPrinter printer;

  auto keypoints = geometries->getListOf(GeometryType::Keypoint);

  if(keypoints.size() > 0) {
    for(auto rawKp : keypoints) {
      auto kp = dynamic_cast<KeyPoint*>(rawKp);
	  if (kp->x > greaterX) greaterX = kp->x;
	  if (kp->y > greaterY) greaterY = kp->y;
	  if (kp->z > greaterZ) greaterZ = kp->z;
	  printer.printKeypoint(kp);
    }
	printer.printBlankLine();
  }

  auto lines = geometries->getListOf(GeometryType::Line);
  vector<Polyline*> polylines;
  vector<Line*> ghostLines;
  if(lines.size() > 0) {
	int id = 1;
    for(auto rawLine : lines) {
      auto line = dynamic_cast<Line*>(rawLine);

	  while (id != line->getID()) {
		auto ghostLine = generateGhostLine();
		ghostLine->setID(id);
		printer.printKeypoint(ghostLine->init_point);
		printer.printKeypoint(ghostLine->final_point);
		printer.printSimpleLine(ghostLine);
		ghostLines.push_back(ghostLine);
		id++;
	  }

	  printer.printSimpleLine(line);
	  if (line->getLineType() == LineType::Polyline) {
		  auto polyline = dynamic_cast<Polyline*>(line);
		  polylines.push_back(polyline);
	  }
	  id++;
    }

	for (auto polyline : polylines) {
		printer.printPolyline(polyline);
	}

	for (auto ghostLine : ghostLines) {
		printer.printDeleteLine(ghostLine);
		printer.printDeleteKeypoint(ghostLine->init_point);
		printer.printDeleteKeypoint(ghostLine->final_point);
	}
	printer.printBlankLine();
  }

  return printer.getString();
}

}
