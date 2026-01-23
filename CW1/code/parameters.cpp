#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "debug.h"
#include "parameters.h"

std::vector<float>  load_parameters (const std::string& filename)
{
    debug::log ("loading parameters file \"" + filename + "\"...");

    std::ifstream infile (filename);
    if (!infile) {
      throw std::runtime_error ("unable to open file \"" + filename + "\"");
    }
    float val_param;
    std::vector<float> data_param;

    while (infile >> val_param) {
      data_param.push_back (val_param);
    }
    if (data_param.size() % 2 != 0) {
      throw std::runtime_error ("Expected sets of amplitude and rate constant values");
    }

    debug::log ("task file \"" + filename + "\" loaded OK");
    return data_param;
}