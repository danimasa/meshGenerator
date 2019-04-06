#include "AnsysMacroWriter.hpp"

#include <iostream>
#include <sstream>
#include <vector>

namespace ansyslib {

string AnsysMacroWriter::getMacro() {
  ostringstream out;

  out << "/clear" << endl;
  out << "/prep7" << endl;
  out << endl;

  auto keypoints = geometries->getListOf(GeometryType::Keypoint);

  if(keypoints.size() > 0) {
    for(auto rawKp : keypoints) {
      auto kp = dynamic_cast<KeyPoint*>(rawKp);
      out << "K, " << kp->getID() 
        << ", " << kp->x
        << ", " << kp->y
        << ", " << kp->z << endl;
    }
    out << endl;
  }

  auto lines = geometries->getListOf(GeometryType::Line);
  if(lines.size() > 0) {
    for(auto rawLine : lines) {
      auto line = dynamic_cast<Line*>(rawLine);
      out << "L, " << line->init_point->getID()
        << ", " << line->final_point->getID() << endl;
    }
    out << endl;
  }

  return out.str();
}

}
