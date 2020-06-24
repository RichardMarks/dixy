//
// Created by Richard Marks on 6/24/20.
//

#ifndef DIXY_LINKER_H
#define DIXY_LINKER_H

#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <string>
#include <fstream>

#include "utils.h"

namespace dixy {
  class Linker {
    bool shouldExplain{false};
    std::vector<unsigned char> bytes;

    static std::vector<unsigned char> compress(const std::vector<unsigned char>& uncompressedBytes);
    void createFile(const std::string &outputFilename, const std::vector<unsigned char> &assembledBytes) const;
  public:
    void explain();
    void link(const std::vector<std::string>& sourceFiles, const std::string& outputFile);
    void link(const char *source, const std::function<void(std::vector<unsigned char>)> &onComplete);
  };
}

#endif // DIXY_LINKER_H
