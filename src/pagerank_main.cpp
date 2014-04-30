// Author: Dai Wei (wdai@cs.cmu.edu)
// Date: 2014.04.29

#include "pagerank_engine.hpp"
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <cstdio>
#include <cstdint>
#include <string>
#include <vector>

// All these are required command line inputs
DEFINE_string(data_file, " ", "path to doc file in libsvm format.");
DEFINE_int32(num_iterations, 10, "Number of iterations");
DEFINE_double(damping_factor, 0.85, "Damping factor in Pagerank.");

int main(int argc, char* argv[]) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  if (FLAGS_data_file == " ") {
    LOG(FATAL)
      << "usage: need data_file, vocab_file, output_file, and num_partitions";
  }

  pr::PagerankEngine pagerank_engine;

  pagerank_engine.ReadData(FLAGS_data_file);
  pagerank_engine.Start();

  return 0;
}
