#pragma once

#include <string>

#include "image.h"

Image<int> load_pgm (const std::string& filename);

Image<int> create_mask (const Image<int>& slice);