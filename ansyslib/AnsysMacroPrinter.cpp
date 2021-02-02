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

void AnsysMacroPrinter::printNode(const Vertex* node) {
	out << "N," << node->getID() << "," 
		<< node->x << ","
		<< node->y << ","
		<< node->z << endl;
}

void AnsysMacroPrinter::printNodes(const vector<Vertex*> nodes) {
	for (auto node : nodes) printNode(node);
}

void AnsysMacroPrinter::printElement(const Quadrilateral* element) {
	out << "E," << element->v1->getID() << ","
		<< element->v2->getID() << ","
		<< element->v3->getID() << ","
		<< element->v4->getID() << "    !E"
		<< element->getID() << endl;
}

void AnsysMacroPrinter::printElements(const vector<Quadrilateral*> elements) {
	out << "ET,1,SHELL181" << endl;

	for (auto element : elements) printElement(element);
}

std::string AnsysMacroPrinter::getString() {
	return out.str();
}

}