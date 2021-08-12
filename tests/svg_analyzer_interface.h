#pragma once

/**
 * @brief The ISVGAnalyzer class is an interface class declaring
 * callbacks that can be implemented by an SVGAnalyzer class. Its
 * purpose is to isolate the SVGAnalyzer class from the SvgppContext
 * class to avoid recompiling the SvgppContext class when the
 * SVGAnalyzer class is changed, which is expected to happen
 * frequently.
 */

class ISVGAnalyzer {
public:
  virtual ~ISVGAnalyzer() = default;
  virtual void on_enter_element_svg() = 0;
  virtual void on_enter_element_g() = 0;
  virtual void on_enter_element_circle() = 0;
  virtual void on_enter_element_ellipse() = 0;
  virtual void on_enter_element_line() = 0;
  virtual void on_enter_element_path() = 0;
  virtual void on_enter_element_polygon() = 0;
  virtual void on_enter_element_polyline() = 0;
  virtual void on_enter_element_rect() = 0;
  virtual void on_enter_element_title() = 0;
  virtual void on_enter_element_unknown() = 0;
};
