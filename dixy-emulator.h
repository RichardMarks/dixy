//
// Created by Richard Marks on 6/24/20.
//

#ifndef DIXY_EMULATOR_H
#define DIXY_EMULATOR_H

#include <vector>
#include <string>

namespace dixy {

  constexpr int MEMORY_SIZE = 0x1000;

  class Emulator {
    int data{0};
    int instruction{0};
    unsigned char registerX{0};
    unsigned char registerY{0};
    unsigned char memory[MEMORY_SIZE]{0};

    unsigned char getMemory();
    void setMemory(unsigned char value);
    void reset();
    static std::vector<unsigned char> decompress(const std::vector<unsigned char> &compressedBytes);
    static bool identifyProgram(const std::vector<unsigned char> &compressedBytes);
    void runProgram(const std::vector<unsigned char> &decompressedBytes);
  public:
    Emulator();
    void run(const std::string &programFilename);
    void dumpState();
  };
}

#endif // DIXY_EMULATOR_H
