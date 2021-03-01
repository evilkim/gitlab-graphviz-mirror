#!/usr/bin/env python3
# Generates lang.html. Takes path to a dot language grammar to include as argv[1].

import markupsafe
import sys
import templates

template = templates.env().get_template('lang.html.j2')
print(template.render(
    dot_grammar=markupsafe.Markup(open(sys.argv[1]).read()),
))
