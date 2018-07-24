#include "AnsysFileReaderFactory.hpp"
#include "FileReader.hpp"

namespace ansyslib {

FileReader* AnsysFileReaderFactory::createReader() {
    return new FileReader(nullptr);
}

}
