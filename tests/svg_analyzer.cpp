#include "svg_analyzer.h"
#include "svgpp_context.h"
#include "svgpp_document_traverser.h"

SVGAnalyzer::SVGAnalyzer(char *text) {
  SvgppContext context{this};
  traverseDocumentWithSvgpp(context, text);
}

void SVGAnalyzer::on_enter_element_svg() { m_num_svgs++; }

void SVGAnalyzer::on_enter_element_g() { m_num_groups++; }

void SVGAnalyzer::on_enter_element_circle() { m_num_circles++; }

void SVGAnalyzer::on_enter_element_ellipse() { m_num_ellipses++; }

void SVGAnalyzer::on_enter_element_line() { m_num_lines++; }

void SVGAnalyzer::on_enter_element_path() { m_num_paths++; }

void SVGAnalyzer::on_enter_element_polygon() { m_num_polygons++; }

void SVGAnalyzer::on_enter_element_polyline() { m_num_polylines++; }

void SVGAnalyzer::on_enter_element_rect() { m_num_rects++; }

void SVGAnalyzer::on_enter_element_title() { m_num_titles++; }

void SVGAnalyzer::on_enter_element_unknown() { m_num_unknowns++; }
