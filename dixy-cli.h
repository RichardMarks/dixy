//
// Created by Richard Marks on 6/24/20.
//

#ifndef DIXY_CLI_H
#define DIXY_CLI_H

#include <iostream>
#include <memory>
#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>
#include <string>
#include <iomanip>
#include <unordered_map>
#include <fstream>
#include <streambuf>
#include <algorithm>
#include <cctype>
#include <locale>

#include "constants.h"

namespace cli::rt {
  enum RuntimeMode {
    RT_ASSEMBLER,
    RT_LINKER,
    RT_EMULATOR,
    RT_DISASSEMBLER,
    RT_REFERENCE,
    RT_HELP,
    RT_VERSION
  };

  class Runtime {
  protected:
    std::string programExecutable;
    std::vector<std::string> programArguments;
  public:
    Runtime(std::string bin, std::vector<std::string> args) {
      programExecutable = std::move(bin);
      programArguments = std::move(args);
    }
  };

}

#endif // DIXY_CLI_H
