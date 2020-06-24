//
// Created by Richard Marks on 6/24/20.
//

#ifndef DIXY_LINKER_RUNTIME_H
#define DIXY_LINKER_RUNTIME_H

#include "dixy-cli.h"

#include "dixy-linker.h"

namespace cli::rt {
  class LinkerRuntime : public Runtime {
  public:
    LinkerRuntime(std::string bin, std::vector<std::string> args) : Runtime(std::move(bin), std::move(args)) {
      std::cout
        << "DIXY Linker Runtime v"
        << LINKER_VERSION
        << "\n"
        << COPYRIGHT_AND_URL
        << std::endl;
      // dixy link input.prg input2.prg output.dxy
      std::vector<std::string> sourceFiles;
      std::string outputFile = "output.dxy";

//        int n = 0;
      for (auto& arg : programArguments) {
        std::string ext = arg.substr(arg.size() - 3, 3);
        if (ext == LINKER_INPUT_EXTENSION) {
          sourceFiles.push_back(arg);
        } else if (ext == LINKER_OUTPUT_EXTENSION) {
          outputFile = arg;
        }
//          std::cout << n << " " << arg << std::endl;
//          n++;
      }

      if (sourceFiles.empty()) {
        throw std::runtime_error("Nothing to Link! An input file is required!");
      }

      std::cout << "Source Files:\n";
      for (auto& filename : sourceFiles) {
        std::cout << filename << std::endl;
      }

      std::cout << "Output File: " << outputFile << std::endl;

      dixy::Linker linker;
      // TODO - parse command line flag to toggle explain mode
      linker.explain();
      linker.link(sourceFiles, outputFile);
    }
  };
}

#endif // DIXY_LINKER_RUNTIME_H
