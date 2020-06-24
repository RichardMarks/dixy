//
// Created by Richard Marks on 6/24/20.
//

#ifndef DIXY_DISASSEMBLER_RUNTIME_H
#define DIXY_DISASSEMBLER_RUNTIME_H

#include "dixy-cli.h"

#include "dixy-assembler.h"

namespace cli::rt {
  class DisassemblerRuntime : public Runtime {
  public:
    DisassemblerRuntime(std::string bin, std::vector<std::string> args) : Runtime(std::move(bin), std::move(args)) {
      std::cout
        << "DIXY Disassembler Runtime v"
        << DISASSEMBLER_VERSION
        << "\n"
        << COPYRIGHT_AND_URL
        << std::endl;

      std::vector<std::string> sourceFiles;
      std::string outputFile = "disassembled.das";

//        int n = 0;
      for (auto& arg : programArguments) {
        std::string ext = arg.substr(arg.size() - 3, 3);
        if (ext == DISASSEMBLER_INPUT_EXTENSION) {
          sourceFiles.push_back(arg);
        } else if (ext == DISASSEMBLER_OUTPUT_EXTENSION) {
          outputFile = arg;
        }
//          std::cout << n << " " << arg << std::endl;
//          n++;
      }

      if (sourceFiles.empty()) {
        throw std::runtime_error("Nothing to Disassemble! An input file is required!");
      }

      std::cout << "Source File: " << sourceFiles[0] << std::endl;
      std::cout << "Output File: " << outputFile << std::endl;

      // TODO - create disassembler class
      dixy::Assembler assembler;
      // TODO - parse command line flag to toggle explain mode
      assembler.explain();
      assembler.disassemble(sourceFiles[0], outputFile);
    }
  };
}

#endif // DIXY_DISASSEMBLER_RUNTIME_H
