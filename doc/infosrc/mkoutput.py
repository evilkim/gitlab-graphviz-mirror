#!/usr/bin/env python3

import re
import sys
from typing import List
from dataclasses import dataclass, asdict
import yaml

@dataclass
class Output:
    # List of command-line-params for an output format, e.g. ('jpg', 'jpeg', 'jpe')
    params: List[str]
    # Full name of the output format
    name: str
    # HTML description string
    html_description: str

HEADER_RE = re.compile(r'^:(?P<params>[^:]+):(?P<format>.*)')

outputs : List[Output] = []

for line in sys.stdin:
    # Skip comment lines.
    if line.startswith('#'):
        continue

    m = HEADER_RE.match(line)
    if m:
        # This is a header line. Grab out the values.
        outputs.append(Output(
            # Command-line formats are slash-separated.
            params = m.group('params').split('/'),
            # Full format name is plain text
            name = m.group('format'),
            # Set an empty string html description, ready to append to.
            html_description = '',
        ))

    else:
        # This is an HTML line, possibly a continuation of a previous HTML line.
        outputs[-1].html_description += line


for o in outputs:
    filename = o.params[0]
    with open("_formats/" + filename + ".html", 'w') as f:
        html_description = o.html_description
        d = asdict(o)
        del d["html_description"]

        print("---", file=f)
        print(yaml.dump(d), end='', file=f)
        print("---", file=f)
        print(html_description, end='', file=f)
