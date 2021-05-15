// XML-escaping behavior, as implemented by xml.c
//
// The following is intended to be used in combination with an X Macro¹
// definition to avoid repeating XML translation in gv_xml_esc. The assumption
// is that we have in scope:
//
//   * A gv_xml_esc_options called options
//   * A pointer to the current location within the input string, p
//   * A pointer to the start of the input string, s
//
// The parameters to the macro are:
//
//   1. A guard conditioning defining when this translation is enabled
//   2. The input character to translate
//   3. The escape sequence to output
//
// Note that encoding of non-ASCII Unicode characters is done separately in
// gv_xml_esc.
//
// ¹ https://en.wikipedia.org/wiki/X_Macro

X(options.raw, '\n', "&#10;");
X(options.raw, '\r', "&#13;");
X(options.nbsp && s != p && p[-1] == ' ', ' ', "&#160;");
  /* Inkscape does not recognize &nbsp; */
X(options.raw || !is_xml_entity(p), '&', "&amp;");
X(options.dash, '-', "&#45;");
X(true, '<', "&lt;");
X(true, '>', "&gt;");
X(true, '"', "&#34;");
X(true, '\'', "&#39;");
