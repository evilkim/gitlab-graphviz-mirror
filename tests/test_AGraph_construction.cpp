#include <stdexcept>

#include <catch2/catch.hpp>

#include <cgraph++/AGraph.h>

TEST_CASE("AGraph can be constructed from DOT source and has an underlying C "
          "data structure") {
  CGraph::AGraph g{"graph {a}"};
  REQUIRE(g.c_struct() != nullptr);
}
