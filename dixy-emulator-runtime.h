//
// Created by Richard Marks on 6/24/20.
//

#ifndef DIXY_EMULATOR_RUNTIME_H
#define DIXY_EMULATOR_RUNTIME_H

#include "dixy-cli.h"

#include "dixy-emulator.h"

namespace cli::rt {
  class EmulatorRuntime : public Runtime {
  public:
    EmulatorRuntime(std::string bin, std::vector<std::string> args) : Runtime(std::move(bin), std::move(args)) {
      std::cout
        << "DIXY Emulator Runtime v"
        << EMULATOR_VERSION
        << "\n"
        << COPYRIGHT_AND_URL
        << std::endl;

      std::vector<std::string> programFiles;
      for (auto& arg : programArguments) {
        std::string ext = arg.substr(arg.size() - 3, 3);
        if (ext == EMULATOR_INPUT_EXTENSION) {
          programFiles.push_back(arg);
        }
      }

      if (programFiles.empty()) {
        throw std::runtime_error("Nothing to Run! An input file is required!");
      }

//      std::cout << "Running " << programFiles[0] << std::endl;

      dixy::Emulator emulator;
      emulator.run(programFiles[0]);
    }
  };
}

#endif // DIXY_EMULATOR_RUNTIME_H
