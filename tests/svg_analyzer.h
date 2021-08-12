#pragma once

#include <cstddef>

#include "svg_analyzer_interface.h"

/**
 * @brief The SVGAnalyzer class analyzes the contents of an SVG document.
 */

class SVGAnalyzer : public ISVGAnalyzer {
public:
  SVGAnalyzer(char *text);
  void on_enter_element_svg() override;
  void on_enter_element_g() override;
  void on_enter_element_circle() override;
  void on_enter_element_ellipse() override;
  void on_enter_element_line() override;
  void on_enter_element_path() override;
  void on_enter_element_polygon() override;
  void on_enter_element_polyline() override;
  void on_enter_element_rect() override;
  void on_enter_element_title() override;
  void on_enter_element_unknown() override;
  std::size_t num_svgs() const { return m_num_svgs; };
  std::size_t num_groups() const { return m_num_groups; };
  std::size_t num_circles() const { return m_num_circles; };
  std::size_t num_ellipses() const { return m_num_ellipses; };
  std::size_t num_lines() const { return m_num_lines; };
  std::size_t num_paths() const { return m_num_paths; };
  std::size_t num_polygons() const { return m_num_polygons; };
  std::size_t num_polylines() const { return m_num_polylines; };
  std::size_t num_rects() const { return m_num_rects; };
  std::size_t num_titles() const { return m_num_titles; };
  std::size_t num_unknowns() const { return m_num_unknowns; };

private:
  std::size_t m_num_svgs = 1; // the top level svg is implicit. See
                              // https://github.com/svgpp/svgpp/issues/98
  std::size_t m_num_groups = 0;
  std::size_t m_num_circles = 0;
  std::size_t m_num_ellipses = 0;
  std::size_t m_num_lines = 0;
  std::size_t m_num_paths = 0;
  std::size_t m_num_polygons = 0;
  std::size_t m_num_polylines = 0;
  std::size_t m_num_rects = 0;
  std::size_t m_num_titles = 0;
  std::size_t m_num_unknowns = 0;
};
