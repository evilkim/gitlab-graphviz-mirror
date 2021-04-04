#include <memory>

#include <catch2/catch.hpp>

#include <cgraph++/AGraph.h>
#include <gvc++/GVContext.h>
#include <gvc++/GVLayout.h>

TEST_CASE("Layout of a graph can use on-demand loaded plugins") {
  auto gvc = std::make_shared<GVC::GVContext>();

  auto dot = "graph {a}";
  auto g = std::make_shared<CGraph::AGraph>(dot);

  const auto layout = GVC::GVLayout(gvc, g, "dot");
}

TEST_CASE("Layout of a graph can use built-in plugins") {
  const auto demand_loading = false;
  auto gvc =
      std::make_shared<GVC::GVContext>(lt_preloaded_symbols, demand_loading);

  auto dot = "graph {a}";
  auto g = std::make_shared<CGraph::AGraph>(dot);

  const auto layout = GVC::GVLayout(gvc, g, "dot");
}

TEST_CASE("Layout with an unknown engine throws an exception") {
  auto dot = "digraph {}";
  auto g = std::make_shared<CGraph::AGraph>(dot);

  const auto demand_loading = false;
  auto gvc =
      std::make_shared<GVC::GVContext>(lt_preloaded_symbols, demand_loading);

  REQUIRE_THROWS_AS(GVC::GVLayout(gvc, g, "UNKNOWN_ENGINE"),
                    std::runtime_error);
}
