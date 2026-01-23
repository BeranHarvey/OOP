#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>
#include <format>

#include "debug.h"
#include "contrast.h"

contrast load_contrast (const std::string& filename)
{
    debug::log ("loading contrast info file \"" + filename + "\"...");

    contrast info;
    std::ifstream infile (filename);
    if (!infile) {
      throw std::runtime_error ("unable to open file \"" + filename + "\"");
    }

    infile >> info.name >> info.dose;

    debug::log ("contrast info file \"" + filename + "\" loaded OK");

    return info;
}