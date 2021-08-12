#include <string_view>

#include <catch2/catch.hpp>

#include "svg_analyzer.h"
#include <cgraph++/AGraph.h>
#include <gvc++/GVContext.h>
#include <gvc++/GVLayout.h>
#include <gvc++/GVRenderData.h>

TEST_CASE(
    "Rendering an SVG from a graph without any nodes outputs an SVG containing "
    "a single (transparent) polygon") {
  const auto demand_loading = false;
  auto gvc = GVC::GVContext(lt_preloaded_symbols, demand_loading);

  auto dot = "digraph {}";
  auto g = CGraph::AGraph(dot);

  const auto layout = GVC::GVLayout(std::move(gvc), std::move(g), "dot");

  const auto result = layout.render("svg");
  SVGAnalyzer svgAnalyzer{result.c_str()};
  REQUIRE(svgAnalyzer.num_svgs() == 1);
  REQUIRE(svgAnalyzer.num_groups() == 1);
  REQUIRE(svgAnalyzer.num_circles() == 0);
  REQUIRE(svgAnalyzer.num_ellipses() == 0);
  REQUIRE(svgAnalyzer.num_lines() == 0);
  REQUIRE(svgAnalyzer.num_paths() == 0);
  REQUIRE(svgAnalyzer.num_polygons() == 1);
  REQUIRE(svgAnalyzer.num_polylines() == 0);
  REQUIRE(svgAnalyzer.num_rects() == 0);
  REQUIRE(svgAnalyzer.num_titles() == 0);
  REQUIRE(svgAnalyzer.num_unknowns() == 0);
}

TEST_CASE("Rendering an SVG from a graph with a single node outputs an SVG "
          "containing an ellipse") {
  const auto demand_loading = false;
  auto gvc = GVC::GVContext(lt_preloaded_symbols, demand_loading);

  auto dot = "digraph {a}";
  auto g = CGraph::AGraph(dot);

  const auto layout = GVC::GVLayout(std::move(gvc), std::move(g), "dot");

  const auto result = layout.render("svg");
  SVGAnalyzer svgAnalyzer{result.c_str()};
  REQUIRE(svgAnalyzer.num_svgs() == 1);
  REQUIRE(svgAnalyzer.num_groups() == 2);
  REQUIRE(svgAnalyzer.num_ellipses() == 1);
  REQUIRE(svgAnalyzer.num_circles() == 0);
  REQUIRE(svgAnalyzer.num_lines() == 0);
  REQUIRE(svgAnalyzer.num_paths() == 0);
  REQUIRE(svgAnalyzer.num_polygons() == 1);
  REQUIRE(svgAnalyzer.num_polylines() == 0);
  REQUIRE(svgAnalyzer.num_rects() == 0);
  REQUIRE(svgAnalyzer.num_titles() == 1);
  REQUIRE(svgAnalyzer.num_unknowns() == 0);
}

TEST_CASE("Rendering an SVG from a graph with two nodes outputs an SVG "
          "containing two ellipses") {
  const auto demand_loading = false;
  auto gvc = GVC::GVContext(lt_preloaded_symbols, demand_loading);

  auto dot = "digraph {a b}";
  auto g = CGraph::AGraph(dot);

  const auto layout = GVC::GVLayout(std::move(gvc), std::move(g), "dot");

  const auto result = layout.render("svg");
  SVGAnalyzer svgAnalyzer{result.c_str()};
  REQUIRE(svgAnalyzer.num_svgs() == 1);
  REQUIRE(svgAnalyzer.num_groups() == 3);
  REQUIRE(svgAnalyzer.num_ellipses() == 2);
  REQUIRE(svgAnalyzer.num_circles() == 0);
  REQUIRE(svgAnalyzer.num_lines() == 0);
  REQUIRE(svgAnalyzer.num_paths() == 0);
  REQUIRE(svgAnalyzer.num_polygons() == 1);
  REQUIRE(svgAnalyzer.num_polylines() == 0);
  REQUIRE(svgAnalyzer.num_rects() == 0);
  REQUIRE(svgAnalyzer.num_titles() == 2);
  REQUIRE(svgAnalyzer.num_unknowns() == 0);
}

TEST_CASE("Rendering an SVG from a graph with two nodes and one edge outputs "
          "an SVG containing two polygons, two ellipses and one path") {
  const auto demand_loading = false;
  auto gvc = GVC::GVContext(lt_preloaded_symbols, demand_loading);

  auto dot = "digraph {a -> b}";
  auto g = CGraph::AGraph(dot);

  const auto layout = GVC::GVLayout(std::move(gvc), std::move(g), "dot");

  const auto result = layout.render("svg");
  SVGAnalyzer svgAnalyzer{result.c_str()};
  REQUIRE(svgAnalyzer.num_svgs() == 1);
  REQUIRE(svgAnalyzer.num_groups() == 4);
  REQUIRE(svgAnalyzer.num_ellipses() == 2);
  REQUIRE(svgAnalyzer.num_circles() == 0);
  REQUIRE(svgAnalyzer.num_lines() == 0);
  REQUIRE(svgAnalyzer.num_paths() == 1);
  REQUIRE(svgAnalyzer.num_polygons() == 2);
  REQUIRE(svgAnalyzer.num_polylines() == 0);
  REQUIRE(svgAnalyzer.num_rects() == 0);
  REQUIRE(svgAnalyzer.num_titles() == 3);
  REQUIRE(svgAnalyzer.num_unknowns() == 0);
}
