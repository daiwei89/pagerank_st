
#include "pagerank_engine.hpp"
#include "context.hpp"
#include "high_resolution_timer.hpp"
#include <glog/logging.h>
#include <cmath>

namespace pr {

PagerankEngine::PagerankEngine() : num_vertices_(0) {
  util::Context context = util::Context::get_instance();
  d_ = context.get_double("damping_factor");
}

void PagerankEngine::ReadData(const std::string& file) {
  util::HighResolutionTimer timer;
  char *line = NULL, *ptr = NULL, *endptr = NULL;
  size_t num_bytes;
  FILE *data_stream = fopen(file.c_str(), "r");
  CHECK_NOTNULL(data_stream);
  LOG(INFO) << "Reading from data file " << file;
  int base = 10;

  int num_edges = 0;
  while (getline(&line, &num_bytes, data_stream) != -1) {
    int src = strtol(line, &endptr, base);
    ptr = endptr;
    while (*ptr == ' ') ++ptr; // goto next non-space char
    int dst = strtol(ptr, &endptr, base);
    int max_node_id = std::max(src, dst);
    if (max_node_id >= num_vertices_) {
      src_degrees_.resize(max_node_id + 1);
      num_vertices_ = max_node_id + 1;
    }
    ++src_degrees_[src];
    edges_.push_back(Edge(src, dst));
    ++num_edges;
  }

  for (int i = 0; i < num_vertices_; ++i) {
    if (src_degrees_[i] == 0) {
      dangling_vertices_.push_back(i);
    }
  }
  LOG(INFO) << "Done reading " << num_edges << " edges in "
    << timer.elapsed() << " seconds. Found " << num_vertices_
    << " vertices. " << dangling_vertices_.size()
    << " of which are dangling vertices";
}

void PagerankEngine::Start() {
  // Initialize page rank to all 1's.
  pr_.resize(num_vertices_);
  for (auto& rank : pr_) {
    rank = 1.;
  }

  util::Context context = util::Context::get_instance();
  int num_iterations = context.get_int32("num_iterations");
  util::HighResolutionTimer total_timer;
  for (int iter = 0; iter < num_iterations; ++iter) {
    util::HighResolutionTimer iter_timer;
    std::vector<float> pr_new(num_vertices_, (1 - d_));
    // Page rank
    for (int i = 0; i < dangling_vertices_.size(); ++i) {
      int d_vertex = dangling_vertices_[i];
      pr_new[d_vertex] += d_ * pr_[d_vertex];
    }

    for (auto& e : edges_) {
      pr_new[e.dst] += d_ * pr_[e.src] / src_degrees_[e.src];
    }

    // Check convergence.
    double diff = 0.;
    double pr_sum = 0.;
    for (int i = 0; i < num_vertices_; ++i) {
      diff += std::abs(pr_new[i] - pr_[i]);
      pr_sum += pr_new[i];
    }
    LOG(INFO) << "Iter " << iter << ".\taverage diff: "
      << diff / num_vertices_ << "\tsum of pr: " << pr_sum
      << ".\tTime(iter): " << iter_timer.elapsed() << "\tTotal Time: "
      << total_timer.elapsed() << " sec.";

    pr_ = pr_new;
  }
}

}  // namespace pr
