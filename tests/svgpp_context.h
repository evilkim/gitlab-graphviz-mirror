#pragma once

#include <any>

#include <svgpp/svgpp.hpp>

class ISVGAnalyzer;

/**
 * @brief The SvgppContext class provides a context containing SVG element
 * callbacks for the SVG++ parser which is called from the SVG document
 * traverser and forwards these callbacks to the SVG analyzer. It's separated
 * from the SVG analyzer to avoid the very time consuming recompilation of the
 * SVG document traverser when then SVG analyzer header is changed, which is
 * expected to happen often as new functionality is added.
 *
 * Most of this is taken from
 * http://svgpp.org/lesson01.html#handling-shapes-geometry.
 */

class SvgppContext {
public:
  SvgppContext(ISVGAnalyzer *svgAnalyzer);
  void on_enter_element(svgpp::tag::element::svg e);
  void on_enter_element(svgpp::tag::element::g e);
  void on_enter_element(svgpp::tag::element::circle e);
  void on_enter_element(svgpp::tag::element::ellipse e);
  void on_enter_element(svgpp::tag::element::line e);
  void on_enter_element(svgpp::tag::element::path e);
  void on_enter_element(svgpp::tag::element::polygon e);
  void on_enter_element(svgpp::tag::element::polyline e);
  void on_enter_element(svgpp::tag::element::rect e);
  void on_enter_element(svgpp::tag::element::title e);
  void on_enter_element(svgpp::tag::element::any e);
  void on_exit_element();
  void path_move_to(double x, double y, svgpp::tag::coordinate::absolute);
  void path_line_to(double x, double y, svgpp::tag::coordinate::absolute);
  void path_cubic_bezier_to(double x1, double y1, double x2, double y2,
                            double x, double y,
                            svgpp::tag::coordinate::absolute);
  void path_quadratic_bezier_to(double x1, double y1, double x, double y,
                                svgpp::tag::coordinate::absolute);
  void path_elliptical_arc_to(double rx, double ry, double x_axis_rotation,
                              bool large_arc_flag, bool sweep_flag, double x,
                              double y, svgpp::tag::coordinate::absolute);
  void path_close_subpath();
  void path_exit();
  void set(svgpp::tag::attribute::cy cy, const double v);
  void set(svgpp::tag::attribute::cx cx, const double v);
  void set(svgpp::tag::attribute::r r, const double v);
  void set(svgpp::tag::attribute::rx rx, const double v);
  void set(svgpp::tag::attribute::ry ry, const double v);
  void set(svgpp::tag::attribute::x1 x1, const double v);
  void set(svgpp::tag::attribute::y1 y1, const double v);
  void set(svgpp::tag::attribute::x2 x2, const double v);
  void set(svgpp::tag::attribute::y2 y2, const double v);
  template <typename Range>
  void set(svgpp::tag::attribute::points points, const Range &range) {
    set_impl(points, range);
  }
  void set(svgpp::tag::attribute::x a, const double v);
  void set(svgpp::tag::attribute::y y, const double v);
  void set(svgpp::tag::attribute::width width, const double v);
  void set(svgpp::tag::attribute::height height, const double v);
  template <class Range> void set_text(const Range &range) {
    set_text_impl(range);
  }

private:
  void set_impl(svgpp::tag::attribute::points &points, const std::any &range);
  void set_text_impl(const std::any &range);

private:
  ISVGAnalyzer *m_svgAnalyzer = nullptr;
};
