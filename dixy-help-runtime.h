//
// Created by Richard Marks on 6/24/20.
//

#ifndef DIXY_HELP_RUNTIME_H
#define DIXY_HELP_RUNTIME_H

#include "dixy-cli.h"

namespace cli::rt {
  class HelpRuntime : public Runtime {
  public:
    HelpRuntime(std::string bin, std::vector<std::string> args) : Runtime(std::move(bin), std::move(args)) {
      std::cout
        << "DIXY CLI v" << CLI_VERSION << " - "
        << COPYRIGHT_AND_URL
        << std::endl;

      auto numArgs = programArguments.size();

      if (numArgs == 0) {
        showUsage();
        showAvailableTopics();
      } else {
        std::string topic = programArguments[0];
        showTopic(topic);
      }
    }
  private:
    std::vector<const char *> topics{
      "assemble",
      "disassemble",
      "link",
      "run",
      "formats",
      "tutorial"
    };

    std::unordered_map<std::string, std::vector<const char*>> topicContents{
      {
        // TOPIC NAME
        "assemble",
        // TOPIC CONTENT
        {
          // topic description shown on the topics list
          "assembling DIXY programs",
          // content shown on topic display
          "Assembling DIXY Programs\n\n",
          "Use the assemble command to assemble DIXY Assembly Source .das files into DIXY program .prg files.\n\n",
          "$ dixy assemble input.das output.prg\n"
        }
      },

      {
        // TOPIC NAME
        "disassemble",
        // TOPIC CONTENT
        {
          // topic description shown on the topics list
          "disassembling DIXY binaries",
          // content shown on topic display
          "Disassembling DIXY Binaries\n\n",
          "Use the disassemble command to disassemble DIXY Binary .dxy files into DIXY Assembly Source .das files.\n\n",
          "$ dixy disassemble input.dxy output.das\n"
        }
      },

      {
        // TOPIC NAME
        "link",
        // TOPIC CONTENT
        {
          // topic description shown on the topics list
          "linking DIXY binaries",
          // content shown on topic display
          "Linking DIXY Binaries\n\n",
          "Use the link command to turn DIXY Program .prg files into DIXY Binary .dxy files.\n\n",
          "$ dixy link input.prg output.dxy\n"
        }
      },

      {
        // TOPIC NAME
        "run",
        // TOPIC CONTENT
        {
          // topic description shown on the topics list
          "running DIXY binaries",
          // content shown on topic display
          "Running DIXY Binaries\n\n",
          "Use the run command to launch a DIXY Binary *.dxy file with the emulator.\n\n",
          "$ dixy run input.dxy\n"
        }
      },

      {
        // TOPIC NAME
        "formats",
        // TOPIC CONTENT
        {
          // topic description shown on the topics list
          "DIXY file formats",
          // content shown on topic display
          "All About DIXY File Formats\n\n",
          "DIXY Assembly Source Files *.das\n\n",
          "These are plain text files which are easily created with any plain text editor.\n",
          "Write out programs in the DIXY Assembly language instruction set mnemonics into .das files.\n\n",
          "DIXY Program Files *.prg\n\n",
          "These are plain text files which are created by the DIXY Assembler from *.das files.\n",
          "Nothing stops you from writing *.prg files by hand, however it is easier and faster to write .das\n\n",
          "DIXY Binary Files *.dxy\n\n",
          "These are compressed binary files which are created by the DIXY Linker and are executed by the DIXY emulator.\n"
        }
      },

      {
        // TOPIC NAME
        "tutorial",
        // TOPIC CONTENT
        {
          // topic description shown on the topics list
          "writing DIXY programs",
          // content shown on topic display
          "DIXY Programming Tutorial\n",
          "\n*** Tutorial Content Is Under Construction ***\n"
        }
      }
    };

    void showUsage() {
      std::cout
        << "Usage: "
        << programExecutable
        << " {command} [{help topic}] [{input files}] [{output file}]\n\n"
        << "  {command} can be any of the following commands:\n\n    assemble, disassemble, link, run, syntax, help, and version\n\n"
        << "  {help topic} is used by the help command to show additional information\n"
        << "  {input files} depends on the command being used\n"
        << "  {output file} depends on the command being used\n"
        << std::endl;
    }

    void showAvailableTopics() {
      std::cout << "  Detailed Help Topics:\n" << std::endl;
      int numTopics = topics.size();
      for (int i = 0; i < numTopics; i++) {
        auto& topic = topics[i];
        auto& topicDescription = topicContents[topic][0];
        std::cout << std::setw(14) << topic << " - Learn more about " << topicDescription << "\n";
      }

      std::cout
        << "\n* Get more information by passing a help topic after the help command. *\n\nFor example: "
        << programExecutable
        << " help tutorial\n"
        << std::endl;
    }

    void showTopic(const std::string& topic) {
      auto unknownTopic = topicContents.find(topic) == topicContents.end();

      if (unknownTopic) {
        throw std::runtime_error("Unknown Topic. No Help Available.");
      }

      auto& topicContent = topicContents[topic];
      auto numLines = topicContent.size();

      for (int i = 1; i < numLines; i++) {
        std::cout << topicContent[i];
      }
      std::cout << std::endl;
    }
  };
}

#endif // DIXY_HELP_RUNTIME_H
