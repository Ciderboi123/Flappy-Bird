#pragma once

#include "iostream"
#include <fstream>
#include "vector"
#include "stack"
#include "array"
#include "map"
#include "string"
#include "ostream"
#include "memory"
#include <cstdarg>
#include <stdexcept>
#include <stdarg.h>

#include <unistd.h>

#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"

#include "nlohmann/json.hpp"

// nlohmann::json getConfig();
// nlohmann::json getJson(std::string path);

// nlohmann::json getConfig() {
//   std::ifstream i("content/Config/config.json");
//   nlohmann::json j;
//   i >> j;

//   i.close();

//   return j;
// }

// nlohmann::json getJson(std::string path) {
//   std::ifstream i(path);
//   nlohmann::json j;
//   i >> j;

//   i.close();
//   return j;
// }

class Helper {
public:
  static std::string format(const std::string fmt, ...) {
    int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
    std::string str;
    va_list ap;
    while (1) {     // Maximum two passes on a POSIX system...
      str.resize(size);
      va_start(ap, fmt);
      int n = vsnprintf((char*)str.data(), size, fmt.c_str(), ap);
      va_end(ap);
      if (n > -1 && n < size) {  // Everything worked
        str.resize(n);
        return str;
      }
      if (n > -1)  // Needed size returned
        size = n + 1;   // For null char
      else
        size *= 2;      // Guess at a larger size (OS specific)
    }
    return str;
  }
};
