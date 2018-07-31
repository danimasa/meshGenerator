#include "AnsysFileReaderFactory.hpp"
#include "AnsysFileStructure.hpp"
#include "FileReader.hpp"

namespace ansyslib {

FileReader* AnsysFileReaderFactory::createReader() {
    FileStructure *structure = createFileStructure();
    return new FileReader(structure);
}

FileStructure* AnsysFileReaderFactory::createFileStructure() {
    return new AnsysFileStructure();
}

}
