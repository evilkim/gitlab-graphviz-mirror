#!/usr/bin/env python3

import argparse
import json

from json2html import json2html


def main():
    parser = argparse.ArgumentParser(description="Converts a json file to a html file")
    parser.add_argument("input", type=argparse.FileType("rt"), help="Input file")
    parser.add_argument("output", type=argparse.FileType("wt"), help="Output file")
    args = parser.parse_args()

    json_input = json.load(args.input)
    html_output = json2html.convert(json=json_input, table_attributes='class="jsontable"')
    args.output.write(html_output)


if __name__ == "__main__":
    main()
