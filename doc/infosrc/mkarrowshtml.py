#!/usr/bin/env python3
# Generates arrows.html.

import markupsafe
import sys
import templates

template = templates.env().get_template('arrows.html.j2')
print(template.render(
    arrow_grammar=markupsafe.Markup(open(sys.argv[1]).read()),
    arrow_table=markupsafe.Markup(open(sys.argv[2]).read()),
))
