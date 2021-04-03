#!/usr/bin/env python3

"""
command wrapper for counting occurrences of "warning:"
"""

import argparse
import io
import os
import re
import subprocess
import sys
from threading import Thread
from typing import Dict, List

def counter(expr: str, input, output, result: Dict[str, int]) -> int:
    """
    Count occurrences of "warning:" in input stream and return the result in
    result["count"]. Pass through all content to the output steam.
    """

    # turn the expression into bytes so we can match against a byte stream
    expr = bytes(expr, 'utf-8')

    buffer = io.BytesIO()
    while True:
        c = input.read(1)

        if c != b"":
            buffer.write(c)

        if c in (b"", b"\n", b"\r"):
            if re.search(expr, buffer.getvalue()) is not None:
                result["count"] += 1

            output.write(buffer.getvalue())
            buffer = io.BytesIO()

        if c == b'':
            break

def main(args: List[str]) -> int:

    # parse command line arguments
    parser = argparse.ArgumentParser(
      description='count occurences of "warning:" in the output of a command')
    parser.add_argument("--expression", default="warning",
      help="regular expression to count")
    parser.add_argument("--output", "-o", type=argparse.FileType("wt"),
      default=sys.stdout, help="file to write final count to")
    parser.add_argument("command", help="command to run")
    parser.add_argument("args", nargs="*", default=[],
      help="arguments to command")
    options = parser.parse_args(args[1:])

    # start our child process to monitor
    p = subprocess.Popen([options.command] + options.args,
      stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # setup a thread to drain and count stdout
    stdout_count = {"count":0}
    stdout = Thread(target=counter,
                    args=(options.expression, p.stdout, sys.stdout.buffer,
                          stdout_count))
    stdout.start()

    # setup a thread to drain and count stderr
    stderr_count = {"count":0}
    stderr = Thread(target=counter,
                    args=(options.expression, p.stderr, sys.stderr.buffer,
                          stderr_count))
    stderr.start()

    # wait for the command’s output to cease
    stdout.join()
    stderr.join()

    # if we have a CI job name, prefix the count with this
    if "CI_JOB_NAME" in os.environ:
        options.output.write(f"{os.environ['CI_JOB_NAME']}-warnings ")

    # dump the count
    options.output.write(f"{stdout_count['count'] + stderr_count['count']}\n")

    # clean up the subprocess
    return p.wait()

if __name__ == "__main__":
    sys.exit(main(sys.argv))
