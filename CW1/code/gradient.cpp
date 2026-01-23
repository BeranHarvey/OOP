#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>

#include "debug.h"
#include "gradient.h"

std::vector<float>  load_gradient (const std::string& filename)
{
    debug::log ("loading parameters file \"" + filename + "\"...");

    std::ifstream infile (filename);
    if (!infile) {
      throw std::runtime_error ("unable to open file \"" + filename + "\"");
    }
    float val_grad;
    std::vector<float> data_grad;

    while (infile >> val_grad) {
      data_grad.push_back (val_grad);
    }

    debug::log ("task file \"" + filename + "\" loaded OK");
    return data_grad;
}