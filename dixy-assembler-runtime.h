//
// Created by Richard Marks on 6/24/20.
//

#ifndef DIXY_ASSEMBLER_RUNTIME_H
#define DIXY_ASSEMBLER_RUNTIME_H

#include "dixy-cli.h"

#include "dixy-assembler.h"

namespace cli::rt {

  class AssemblerRuntime : public Runtime {
  public:
    AssemblerRuntime(std::string bin, std::vector<std::string> args) : Runtime(std::move(bin), std::move(args)) {
      std::cout
        << "DIXY Assembler Runtime v"
        << ASSEMBLER_VERSION
        << "\n"
        << COPYRIGHT_AND_URL
        << std::endl;

      std::vector<std::string> sourceFiles;
      std::string outputFile = "output.prg";

//        int n = 0;
      for (auto& arg : programArguments) {
        std::string ext = arg.substr(arg.size() - 3, 3);
        if (ext == ASSEMBLER_INPUT_EXTENSION) {
          sourceFiles.push_back(arg);
        } else if (ext == ASSEMBLER_OUTPUT_EXTENSION) {
          outputFile = arg;
        }
//          std::cout << n << " " << arg << std::endl;
//          n++;
      }

      if (sourceFiles.empty()) {
        throw std::runtime_error("Nothing to Assemble! An input file is required!");
      }

      std::cout << "Source Files:\n";
      for (auto& filename : sourceFiles) {
        std::cout << filename << std::endl;
      }

      std::cout << "Output File: " << outputFile << std::endl;

      dixy::Assembler assembler;
      // TODO - parse command line flag to toggle explain mode
      assembler.explain();
      assembler.assemble(sourceFiles, outputFile);
    }
  };
}

#endif // DIXY_ASSEMBLER_RUNTIME_H
