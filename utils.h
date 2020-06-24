//
// Created by Richard Marks on 6/24/20.
//

#ifndef UTILS_H
#define UTILS_H

#include <stack>
#include <functional>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
#include <fstream>
#include <streambuf>
#include <stdexcept>

namespace utils {
  template <typename T> class Pipeline {
    std::stack<std::function<T(T)>> segments;
  public:
    void push(std::function<T(T)> pipe) {
      segments.push(pipe);
    }
    T start(T input) {
      while (!segments.empty()) {
        input = segments.top()(input);
        segments.pop();
      }
      return input;
    }
  };

  class TextUtils {
  public:

    static inline void ltrim(std::string& s) {
      s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](auto chr) {
        return !std::isspace(chr);
      }));
    }

    static inline void rtrim(std::string& s) {
      s.erase(std::find_if(s.rbegin(), s.rend(), [](auto chr) {
        return !std::isspace(chr);
      }).base(), s.end());
    }

    static inline void trim(std::string& s) {
      TextUtils::ltrim(s);
      TextUtils::rtrim(s);
    }

    static inline std::string trimmed(std::string s) {
      TextUtils::trim(s);
      return s;
    }

    static unsigned char getLiteralCharValue(const char &c) {
      if (c >= 0x30 && c <= 0x39) {
        // 0 - 9
        return (unsigned char) (c - 0x30);
      } else if (c >= 0x41 && c <= 0x46) {
        // A - F
        return (unsigned char) (0xa + (c - 0x41));
      } else if (c >= 0x61 && c <= 0x66) {
        // a - f
        return (unsigned char) (0xa + (c - 0x61));
      }
      return (unsigned char)0;
    }

    static unsigned char parseByteValue(const std::string& input) {
      std::string truncated = input.substr(0, 2);

      if (truncated.size() == 2) {
        // 00 - ff
        unsigned char high = TextUtils::getLiteralCharValue(input[0]);
        unsigned char low = TextUtils::getLiteralCharValue(input[1]);
        return (unsigned char) (low + (high * 0x10u));
      } else if (truncated.size() == 1) {
        // 0 - f
        return TextUtils::getLiteralCharValue(input[1]);
      }
      return (unsigned char)0;
    }

    static std::vector<std::string> split(const std::string &source) {
      std::vector<std::string> tokens;
      size_t p0 = 0;
      while (p0 != std::string::npos) {
        size_t p1 = source.find_first_of('\n', p0);
        if (p1 != p0) {
          std::string token = source.substr(p0, p1 - p0);
          tokens.push_back(token);
        }
        p0 = source.find_first_not_of('\n', p1);
      }
      return tokens;
    }
  };

  class FSUtils {
  public:
    static std::string source(const std::string &inputFilename) {
      std::string content;
      std::ifstream inStream(inputFilename);

      if (inStream.fail()) {
        throw std::runtime_error("Error Reading Input File");
      }

      inStream.seekg(0, std::ios::end);
      content.reserve(inStream.tellg());
      inStream.seekg(0, std::ios::beg);
      content.assign(std::istreambuf_iterator<char>(inStream), std::istreambuf_iterator<char>());
      inStream.close();

      return content;
    }

    static std::vector<unsigned char> sourceBytes(const std::string &inputFilename) {
      std::vector<unsigned char> content;
      std::ifstream inStream;
      inStream.open(inputFilename, std::ios::binary);

      if (inStream.fail()) {
        throw std::runtime_error("Error Reading Input File");
      }

      inStream.seekg(0, std::ios::end);
      content.reserve(inStream.tellg());
      inStream.seekg(0, std::ios::beg);
      content.assign(std::istreambuf_iterator<char>(inStream), std::istreambuf_iterator<char>());

      inStream.close();
      return content;
    }
  };
}

#endif // UTILS_H
