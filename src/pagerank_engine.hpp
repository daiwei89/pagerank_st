// Author: Dai Wei (wdai@cs.cmu.edu)
// Date: 2014.04.29

#pragma once

#include <vector>
#include <cstdint>
#include <string>

namespace pr {

struct Edge {
  uint32_t src;
  uint32_t dst;
  Edge() { }
  Edge(uint32_t s, uint32_t d) : src(s), dst(d) { }
};


// PagerankEngine solves Pagerank using algorithm 1 in
// http://www.ccs.northeastern.edu/home/daikeshi/notes/PageRank.pdf but scaled
// by N.
class PagerankEngine {
public:
  PagerankEngine();

  // Read data in the format of "src dst" on each line.
  void ReadData(const std::string& file);

  void Start();

private:
  // Damping factor. The canonical value is 0.85.
  float d_;

  std::vector<float> pr_;

  std::vector<Edge> edges_;

  std::vector<uint32_t> src_degrees_;

  std::vector<uint32_t> dangling_vertices_;

  int num_vertices_;
};

}   // namespace pr
