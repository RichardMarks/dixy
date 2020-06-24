//
// Created by Richard Marks on 6/21/20.
//

#include "dixy-cli.h"
#include "dixy-assembler-runtime.h"
#include "dixy-disassembler-runtime.h"
#include "dixy-linker-runtime.h"
#include "dixy-emulator-runtime.h"
#include "dixy-reference-runtime.h"
#include "dixy-version-runtime.h"
#include "dixy-help-runtime.h"

int main(int argc, char* argv[]) {
  std::string bin = argv[0];
  std::vector<std::string> args;

  if (argc > 1) {
    args.reserve(argc - 1);
    for (int i = 1; i < argc; i++) {
      args.emplace_back(argv[i]);
    }
  }

  cli::rt::RuntimeMode runtimeMode = cli::rt::RT_HELP;

  if (args.empty()) {
    runtimeMode = cli::rt::RT_HELP;
  } else {
    std::string& arg = args.at(0);
    if (arg == "version") {
      runtimeMode = cli::rt::RT_VERSION;
    } else if (arg == "assemble") {
      runtimeMode = cli::rt::RT_ASSEMBLER;
    } else if (arg == "link") {
      runtimeMode = cli::rt::RT_LINKER;
    } else if (arg == "disassemble") {
      runtimeMode = cli::rt::RT_DISASSEMBLER;
    } else if (arg == "run") {
      runtimeMode = cli::rt::RT_EMULATOR;
    } else if (arg == "syntax") {
      runtimeMode = cli::rt::RT_REFERENCE;
    } else if (arg == "help") {
      runtimeMode = cli::rt::RT_HELP;
    }
    args.erase(args.begin());
  }

  try {
    switch (runtimeMode) {
      case cli::rt::RT_ASSEMBLER: {
        cli::rt::AssemblerRuntime rt(bin, args);
      }
        break;
      case cli::rt::RT_LINKER: {
        cli::rt::LinkerRuntime rt(bin, args);
      }
        break;
      case cli::rt::RT_DISASSEMBLER: {
        cli::rt::DisassemblerRuntime rt(bin, args);
      }
        break;
      case cli::rt::RT_EMULATOR: {
        cli::rt::EmulatorRuntime rt(bin, args);
      }
        break;
      case cli::rt::RT_REFERENCE: {
        cli::rt::ReferenceRuntime rt(bin, args);
      }
        break;
      case cli::rt::RT_VERSION: {
        cli::rt::VersionRuntime rt(bin, args);
      }
        break;
      default: {
        cli::rt::HelpRuntime rt(bin, args);
      }
        break;
    }
  } catch (std::exception& ex) {
    std::cerr << "Mistakes Were Made...\n" << ex.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}