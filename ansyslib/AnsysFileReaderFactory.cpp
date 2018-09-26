#include <vector>

#include "AnsysFileReaderFactory.hpp"
#include "FileReader.hpp"
#include "KeypointInterpreter.hpp"
#include "LineInterpreter.hpp"

using namespace geomlib;

namespace ansyslib {

FileReader* AnsysFileReaderFactory::createReader() {
    GeometryList* geomList = new GeometryList();
    std::vector<Interpreter*> interpreters {
       new KeypointInterpreter(),
       new LineInterpreter(geomList)
    };
    auto builder = new GeometryBuilder(geomList, interpreters);
    return new FileReader(builder);
}

}
