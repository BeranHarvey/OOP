#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

#include "debug.h"
#include "contrast.h"
#include "pgm.h"
#include "dataset.h"
#include "terminal_graphics.h"

// Command line input (in the solution folder workspace):
// 'oop_build && ./mri ../data/contrast_info.txt ../data/mri-*.pgm'

// This function contains our program's core functionality:

void run (std::vector<std::string>& args)
{
  debug::verbose = std::erase (args, "-v");

  if (args.size() < 3)
    throw std::runtime_error ("missing arguments - expected at least 2 arguments");

  // reading in contrast info 
  auto contrastinfo = load_contrast (args[1]);

  // reading in MRI PGMs to dataset
  Dataset<int> data ({ args.begin()+2, args.end() });

  // get lvbp timecourse information
  std::vector<float> lvbp_timecourse = data.get_lvbp_timecourse();
  float peak_lvbp = *std::max_element (lvbp_timecourse.begin(), lvbp_timecourse.end());

  auto it = std::find (lvbp_timecourse.begin(),lvbp_timecourse.end(),peak_lvbp);
  int peak_frame = std::distance (lvbp_timecourse.begin(),it);

  // get lvbp temporal gradient information
  auto temporal_gradient = data.get_temporal_grad(lvbp_timecourse);
  float contrast_signal = lvbp_timecourse[temporal_gradient.thresh_frame];  

  // compute temporal gradient during contrast uptake
  float G = (peak_lvbp - contrast_signal)/(peak_frame - temporal_gradient.thresh_frame);

  // plot information to terminal
  debug::log ("Pasting information to the terminal...\n");

  std::cerr << "Image at peak contrast concentration: \n";
  TG::imshow(TG::magnify(data[peak_frame],2),0,255);

  std::cerr << "Signal timecourse within ROI: \n";
  TG::plot().set_grid(2,20)
            .add_line (lvbp_timecourse);

  std::cerr << "Gradient of signal timecourse within ROI: \n";
  TG::plot().set_grid(2,20)
            .add_line(temporal_gradient.timecourse,3);

  std::cerr << "Contrast Agent: " << contrastinfo.name 
            << ", dose: " << contrastinfo.dose << "\n";

  std::cerr << "Contrast arrival occurs at frame " << temporal_gradient.thresh_frame 
            << ", with signal intensity: " << contrast_signal << "\n";

  std::cerr << "Peak contrast concentration occurs at frame " << peak_frame 
            << ", with signal intensity " << peak_lvbp << "\n";

  std::cerr << "Temporal gradient of signal during contrast update: " 
            << G << "\n";
  
}




// skeleton main() function, whose purpose is now to pass the arguments to
// run() in the expected format, and catch and handle any exceptions that may
// be thrown.

int main (int argc, char* argv[])
{
  try {
    std::vector<std::string> args (argv, argv+argc);
    run (args);
  }
  catch (std::exception& excp) {
    std::cerr << "ERROR: " << excp.what() << " - aborting\n";
    return 1;
  }
  catch (...) {
    std::cerr << "ERROR: unknown exception thrown - aborting\n";
    return 1;
  }

  return 0;
}
