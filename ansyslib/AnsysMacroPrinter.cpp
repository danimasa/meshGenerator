#include "AnsysMacroPrinter.hpp"

namespace ansyslib {

void AnsysMacroPrinter::printKeypoint(KeyPoint* kp) {
	out << "K, " << kp->getID()
		<< ", " << kp->x
		<< ", " << kp->y
		<< ", " << kp->z << endl;
}

void AnsysMacroPrinter::printSimpleLine(Line* line) {
	out << "!# " << line->getID() << endl;
	out << "LSTR, " << line->init_point->getID()
		<< ", " << line->final_point->getID() << endl;
}

void AnsysMacroPrinter::printPolyline(Polyline* polyline) {
	printDeleteLine(polyline);
	out << "LCCAT";
	for (auto subLine : polyline->get_lines())
		out << "," << subLine->getID();
	out << endl;
}

void AnsysMacroPrinter::printDeleteKeypoint(KeyPoint* kp) {
	out << "KDELE," << kp->getID() << endl;
}

void AnsysMacroPrinter::printDeleteLine(Line* line) {
	out << "LDELE," << line->getID() << endl;
}

void AnsysMacroPrinter::printBlankLine() {
	out << endl;
}

std::string AnsysMacroPrinter::getString() {
	return out.str();
}

}