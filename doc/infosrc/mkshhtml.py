#!/usr/bin/env python3
# Generates shapes.html. Takes path to compiled HTML grammar and a dot file
# showing HTML labels, to include as argv[1] and argv[2], and a shapelist on
# stdin.

import markupsafe
import sys
import templates

template = templates.env().get_template('shapes.html.j2')
print(template.render(
    html_grammar=markupsafe.Markup(open(sys.argv[1]).read()),
    html1_dot=markupsafe.Markup(open(sys.argv[2]).read()),
    shapes=[line.strip() for line in sys.stdin],
))
