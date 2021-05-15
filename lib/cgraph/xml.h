#pragma once

#ifdef _WIN32
#ifdef EXPORT_CGRAPH
#define CGRAPH_API __declspec(dllexport)
#else
#define CGRAPH_API __declspec(dllimport)
#endif
#else
#define CGRAPH_API /* nothing */
#endif

// options to configure the behavior of gv_xml_esc
struct gv_xml_esc_options {

  /** raw mode
   *
   * Assume the input contains no XML escape sequences itself. That is, do not
   * try to recognize '&'-prefixed content in the input as XML entities.
   */
  unsigned raw : 1;

  /** escape-guard double spaces
   *
   * Some environments like HTML coalesce consecutive spaces. Set this option to
   * escape consecutive spaces to suppress this behavior.
   */
  unsigned nbsp : 1;

  /** escape '-'
   *
   * Dashes do not usually need to be escaped in XML. However, if the output may
   * eventually be emitted within an XML content, dashes are meaningful and may
   * need to be escaped.
   */
  unsigned dash : 1;
};

/** escape text, such that it can be printed to an XML document
 *
 * @param s String to process
 * @param options Settings to tweak behavior
 * @return XML-escaped string. The caller should later free this pointer.
 */
CGRAPH_API char *gv_xml_esc(const char *s, struct gv_xml_esc_options options);
