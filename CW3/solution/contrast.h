#pragma once

#include <string>
#include <vector>

struct contrast {
  std::string name;
  float dose;
};

contrast load_contrast (const std::string& filename);