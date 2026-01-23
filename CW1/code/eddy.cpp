#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

#include "debug.h"
#include "parameters.h"
#include "gradient.h"
#include "pred_grad_time_course.h"
#include "terminal_graphics.h"

// Command line input: 
// 'oop_build && ./eddy ../data/parameters.txt ../data/gradient.txt ../data/output.txt -n x' 
//                                                                     [optional add-ons] 
// This function contains our program's core functionality:

void run (std::vector<std::string>& args)
{
  debug::verbose = std::erase (args, "-v");

  bool output_file = false;
  
  // setting number of iterations
  int iterations = 10;
  auto iter = std::ranges::find (args, "-n");
  if (iter != args.end()) {
    if (std::distance (iter, args.end()) != 2 ) {
      throw std::runtime_error ("invalid input to '-n' option (expected '-n x')");
    }
    iterations = std::stoi (*(iter + 1));
    if (iterations < 1) {
      throw std::runtime_error ("number of iterations entered is invalid"); 
    } else if (iterations > 100) {
      throw std::runtime_error ("number of iterations entered exceeds limit of 100");
    }
    if (args.size() > 5) {
      output_file = true;
    }
  } else if (args.size() > 3) {
    output_file = true;
  }

  if (args.size() < 3) {
    throw std::runtime_error ("missing arguments - expected at least 2 arguments");
  }
  
  // settings input files to variables
  auto parameters = load_parameters (args[1]);
  auto desired_grad = load_gradient (args[2]);
  auto input_grad = desired_grad;
  int T = desired_grad.size();

  // iterative prediction gradient solver
  for (int it = 1; it < iterations + 1; it++) {
    auto predict_grad = compute_pred_grad (parameters, input_grad);

    // find maximum absolute deviation
    std::vector<float> deviation;
    for (int i = 0; i < T; i++) {
        deviation.push_back (std::abs (desired_grad[i] - predict_grad[i]));
    }
    float max_dev = *std::max_element(deviation.begin(), deviation.end());
    std::cerr << "iteration number: " << it << ", maximum absolute deviation: " << max_dev << "\n";

    // plot input and prediction time courses
    TG::plot (1024, 256) 
       .add_line(input_grad)
       .add_line(predict_grad, 3);

    // saving final input waveform to file
    if ((it == iterations) && output_file) {
      std::ofstream outfile (args[3]);
      if (!outfile) {
        throw std::runtime_error ("unable to find file \"" + args[3] + "\" to store final time course");
      }
      for (const auto &val : input_grad) {
        outfile << val << "\n";
      }
    }

    // creating next input waveform 
    for (int i = 0; i < T; i++) {
      input_grad[i] += (desired_grad[i] - predict_grad[i]);
    }
  }
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
