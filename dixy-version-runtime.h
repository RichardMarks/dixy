//
// Created by Richard Marks on 6/24/20.
//

#ifndef DIXY_VERSION_RUNTIME_H
#define DIXY_VERSION_RUNTIME_H

#include "dixy-cli.h"

namespace cli::rt {
  class VersionRuntime : public Runtime {
  public:
    VersionRuntime(std::string bin, std::vector<std::string> args) : Runtime(std::move(bin), std::move(args)) {
      std::cout
        << "DIXY CLI          v" << CLI_VERSION << "\n"
        << "DIXY Assembler    v" << ASSEMBLER_VERSION << "\n"
        << "DIXY Disassembler v" << DISASSEMBLER_VERSION << "\n"
        << "DIXY Linker       v" << LINKER_VERSION << "\n"
        << "DIXY Emulator     v" << EMULATOR_VERSION << "\n\n"
        << "Build Date: " << __DATE__ << "\n\n"
        << COPYRIGHT_AND_URL
        << std::endl;
    }
  };
}

#endif // DIXY_VERSION_RUNTIME_H
