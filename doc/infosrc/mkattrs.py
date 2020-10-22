#!/usr/bin/env python3
# Generator taking a list of attribute descriptions as argv[1] and attribute
# type descriptions as argv[2] and producing a summary table, followed by a
# list of attribute descriptions and attribute types. Uses
# `templates/attrs.html.j2`.
# See `attrs` and `types` files for format documentation.

from dataclasses import dataclass
import markupsafe
import sys
from typing import List, Dict
import templates

##
# Parse `attrs` file
##

@dataclass
class Attribute:
    name: str
    # use string : this is a string formed of G,N,C,E
    used_by: str
    kinds: List[str]
    flags: List[str]
    html_description: str
    defaults: List[str]
    minimums: List[str]
    # Anchor (<a name="#">) for definition
    d_anchor: str = ""
    # Anchor (<a name="#">) for table at the top
    a_anchor: str = ""


attrs: List[Attribute] = []

with open(sys.argv[1]) as attrs_in:
    for line in attrs_in:
        # Skip comment lines.
        if line.startswith('#'):
            continue

        if line.startswith(':'):
            # This is a header line. Grab out the values.
            #    :name:used_by:kind[:dflt[:minv]];  [notes]
            headers, _, flags = line.rpartition(';')
            parts = headers.split(':')

            attr = Attribute(
                name=parts[1],
                used_by=parts[2],
                kinds=parts[3].split('/'),
                flags=[flag for flag in flags.strip().split(',') if flag],
                # Set an empty string html description, ready to append to.
                html_description='',
                # Remaining fields are optional.
                # First, check for default value: this has format :dflt1[/dflt2]*
                defaults=parts[4].split('/') if len(parts) >= 5 else [],
                # Check for minimum value: this has format /min1[/min2]*
                minimums=parts[5].split('/') if len(parts) >= 6 else [],
            )
            attrs.append(attr)

        else:
            # This is an HTML line, possibly a continuation of a previous HTML line.
            attr.html_description += line


attrs.sort(key=lambda attr: attr.name)

for attr in attrs:
    attr.html_description = markupsafe.Markup(attr.html_description)

# The specification allows items with the same attribute name.
# This creates unique anchor keys, which are n copies of 'a' and 'd'.
a_anchors_used = set()
d_anchors_used = set()
for attr in attrs:
    a_key = 'a'
    d_key = 'd'
    a_anchor = a_key + ':' + attr.name
    d_anchor = d_key + ':' + attr.name
    while a_anchor in a_anchors_used:
        a_key += 'a'
        a_anchor = a_key + ':' + attr.name
    while d_anchor in d_anchors_used:
        d_key += 'd'
        d_anchor = d_key + ':' + attr.name
    a_anchors_used.add(a_anchor)
    d_anchors_used.add(d_anchor)
    attr.a_anchor = a_anchor
    attr.d_anchor = d_anchor

##
# Parse `types` file
##


@dataclass
class Type:
    name: str
    html_description: str


types: List[Type] = []

with open(sys.argv[2]) as types_in:
    for line in types_in:
        # Skip comment lines.
        if line.startswith('#'):
            continue
        if line.startswith(':'):
            # This is a header line. Grab out the values.
            types.append(Type(
                name=line[1:].rstrip(),
                html_description=''
            ))
        else:
            # This is an HTML line, possibly a continuation of a previous HTML line.
            t = types[-1]
            t.html_description += line

types.sort(key=lambda t: t.name)

for t in types:
    t.html_description = markupsafe.Markup(t.html_description)

##
# Output HTML
##

template = templates.env().get_template('attrs.html.j2')
print(template.render(attrs=attrs, types=types))
