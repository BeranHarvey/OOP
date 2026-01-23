#include <iostream>
#include <stdexcept>
#include <vector>

#include "debug.h"
#include "pred_grad_time_course.h"

std::vector<float> compute_pred_grad (const std::vector<float>& parameters, const std::vector<float>& input_grad)
{
    int N = parameters.size() / 2;
    int T = input_grad.size();
    int k_amp = 0;
    int k_rc = 0;
    std::vector<float> amplitude (N, 0.0);
    std::vector<float> rate_const (N, 0.0);

    // extracting parameters to separate variables
    for (int i = 0; i < N*2; i++) {
       if (i % 2 == 0) {
          amplitude[k_amp] = parameters[i];
          k_amp += 1;
       } else {
          rate_const[k_rc] = parameters[i];
          k_rc += 1;
       }
    }

    std::vector<std::vector<float>> current (N, std::vector<float>(T, 0.0));
    std::vector<float> grad_prime (T, 0.0);

    // computing predicted time course
    for (int t = 1; t < T; t++) {
       float d_grad = input_grad[t] - input_grad[t-1];
       float sum = 0.0;

       for (int n = 0; n < N; n++) {
          current[n][t] = current[n][t-1] + d_grad - (rate_const[n] * current[n][t-1]);
          sum += amplitude[n] * current[n][t];
       }
       grad_prime[t] = input_grad[t] - sum;
    }

    return grad_prime;
}