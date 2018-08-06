#include <vector>

#include "AnsysFileReaderFactory.hpp"
#include "FileReader.hpp"
#include "KeypointInterpreter.hpp"

using namespace geomlib;

namespace ansyslib {

FileReader* AnsysFileReaderFactory::createReader() {
    std::vector<Interpreter*> interpreters {
       new KeypointInterpreter()
    };
    auto builder = new GeometryBuilder(interpreters);
    return new FileReader(builder);
}

}
