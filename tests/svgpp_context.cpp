#include <any>

#include "svg_analyzer_interface.h"
#include "svgpp_context.h"

SvgppContext::SvgppContext(ISVGAnalyzer *svgAnalyzer)
    : m_svgAnalyzer(svgAnalyzer){};

void SvgppContext::on_enter_element(svgpp::tag::element::svg e) {
  (void)e;
  m_svgAnalyzer->on_enter_element_svg();
}

void SvgppContext::on_enter_element(svgpp::tag::element::g e) {
  (void)e;
  m_svgAnalyzer->on_enter_element_g();
}

void SvgppContext::on_enter_element(svgpp::tag::element::circle e) {
  (void)e;
  m_svgAnalyzer->on_enter_element_circle();
}

void SvgppContext::on_enter_element(svgpp::tag::element::ellipse e) {
  (void)e;
  m_svgAnalyzer->on_enter_element_ellipse();
}

void SvgppContext::on_enter_element(svgpp::tag::element::line e) {
  (void)e;
  m_svgAnalyzer->on_enter_element_line();
}

void SvgppContext::on_enter_element(svgpp::tag::element::path e) {
  (void)e;
  m_svgAnalyzer->on_enter_element_path();
}

void SvgppContext::on_enter_element(svgpp::tag::element::polygon e) {
  (void)e;
  m_svgAnalyzer->on_enter_element_polygon();
}

void SvgppContext::on_enter_element(svgpp::tag::element::polyline e) {
  (void)e;
  m_svgAnalyzer->on_enter_element_polyline();
}

void SvgppContext::on_enter_element(svgpp::tag::element::rect e) {
  (void)e;
  m_svgAnalyzer->on_enter_element_rect();
}

void SvgppContext::on_enter_element(svgpp::tag::element::title e) {
  (void)e;
  m_svgAnalyzer->on_enter_element_title();
}

void SvgppContext::on_enter_element(svgpp::tag::element::any) {
  m_svgAnalyzer->on_enter_element_unknown();
}

void SvgppContext::on_exit_element() {}

void SvgppContext::path_move_to(double x, double y,
                                svgpp::tag::coordinate::absolute c) {
  (void)x;
  (void)y;
  (void)c;
}

void SvgppContext::path_line_to(double x, double y,
                                svgpp::tag::coordinate::absolute c) {
  (void)x;
  (void)y;
  (void)c;
}

void SvgppContext::path_cubic_bezier_to(double x1, double y1, double x2,
                                        double y2, double x, double y,
                                        svgpp::tag::coordinate::absolute c) {
  (void)x1;
  (void)y1;
  (void)x2;
  (void)y2;
  (void)x;
  (void)y;
  (void)c;
}

void SvgppContext::path_quadratic_bezier_to(
    double x1, double y1, double x, double y,
    svgpp::tag::coordinate::absolute c) {
  (void)x1;
  (void)y1;
  (void)x;
  (void)y;
  (void)c;
}

void SvgppContext::path_elliptical_arc_to(double rx, double ry,
                                          double x_axis_rotation,
                                          bool large_arc_flag, bool sweep_flag,
                                          double x, double y,
                                          svgpp::tag::coordinate::absolute c) {
  (void)rx;
  (void)ry;
  (void)x_axis_rotation;
  (void)large_arc_flag;
  (void)sweep_flag;
  (void)x;
  (void)y;
  (void)c;
}

void SvgppContext::path_close_subpath() {}

void SvgppContext::path_exit() {}

void SvgppContext::set(svgpp::tag::attribute::cy a, const double v) {
  (void)a;
  (void)v;
}

void SvgppContext::set(svgpp::tag::attribute::cx a, const double v) {
  (void)a;
  (void)v;
}

void SvgppContext::set(svgpp::tag::attribute::r a, const double v) {
  (void)a;
  (void)v;
}

void SvgppContext::set(svgpp::tag::attribute::rx a, const double v) {
  (void)a;
  (void)v;
}

void SvgppContext::set(svgpp::tag::attribute::ry a, const double v) {
  (void)a;
  (void)v;
}

void SvgppContext::set(svgpp::tag::attribute::x1 a, const double v) {
  (void)a;
  (void)v;
}

void SvgppContext::set(svgpp::tag::attribute::y1 a, const double v) {
  (void)a;
  (void)v;
}

void SvgppContext::set(svgpp::tag::attribute::x2 a, const double v) {
  (void)a;
  (void)v;
}

void SvgppContext::set(svgpp::tag::attribute::y2 a, const double v) {
  (void)a;
  (void)v;
}

void SvgppContext::set(svgpp::tag::attribute::x a, const double v) {
  (void)a;
  (void)v;
}

void SvgppContext::set(svgpp::tag::attribute::y a, const double v) {
  (void)a;
  (void)v;
}

void SvgppContext::set(svgpp::tag::attribute::width a, const double v) {
  (void)a;
  (void)v;
}

void SvgppContext::set(svgpp::tag::attribute::height a, const double v) {
  (void)a;
  (void)v;
}

void SvgppContext::set_impl(svgpp::tag::attribute::points &points,
                            const std::any &range) {
  (void)points;
  (void)range;
  // ignore for now
}

void SvgppContext::set_text_impl(const std::any &range) {
  (void)range;
  // ignore for now
}
