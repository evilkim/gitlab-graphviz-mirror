#!/usr/bin/env python3
# Generates colors.html. Takes color files as args.

import markupsafe
import sys
import templates

template = templates.env().get_template('colors.html.j2')
print(template.render(
    x11_colors=markupsafe.Markup(open(sys.argv[1]).read()),
    svg_colors=markupsafe.Markup(open(sys.argv[2]).read()),
    brewer_colors=markupsafe.Markup(open(sys.argv[3]).read()),
))
