#include "AnsysFileReaderFactory.hpp"
#include "AnsysFileReader.hpp"

FileReader* AnsysFileReaderFactory::createReader(const string filePath) {
    return new AnsysFileReader(filePath);
}