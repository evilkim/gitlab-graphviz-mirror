#!/usr/bin/env python3

"""
generator for large, meaningless graphs

When dealing with performance issues or trying to profile Graphviz, a recurring
blocker is that input graphs have non-portable performance characteristics. E.g.
a user may supply a graph that exhausts memory on their machine, but it runs
fine when the maintainers try to reproduce it on a larger machine. The converse
(and to be honest, much more common) situation is that a user supplies a long
running graph that cannot be processed on any machine the maintainers have
available. Profiling or optimizing a workload you cannot even run is, well,
challenging.

This script attempts to mitigate this problem. If you can distill the relevant
features of a user’s example, you can use this script to produce a more
constrained – but still locally expensive – representative graph. You can then
profile and optimize Graphviz on this generated graph, with the hope that it
will apply to the user’s original graph.

Some ideas for TODO:
  * It’s possible to give conflicting criteria such that the generator never
    converges. E.g. “2 node graph that uses 8GB memory.” Maybe we should detect
    this?
  * cluster support
  * subgraph support
  * This script can currently only generate DAGs. Maybe cyclic graphs are
    interesting?
"""

import argparse
from enum import Enum
import os
from pathlib import Path
import platform
import re
import subprocess
import sys
import tempfile
from typing import List, Optional

class Graph():
    """generative representation of a large graph"""

    def __init__(self, directed: bool, branching: int):
        self.nodes = 1
        self.directed = directed
        self.branching = branching
        self.edges = 0

    def grow(self) -> "Graph":
        """create a bigger graph"""
        g = Graph(self.directed, self.branching)
        g.nodes = self.nodes * 2
        g.edges = 1 if self.edges == 0 else (self.edges * 2)
        return g

    def shrink(self) -> "Graph":
        """create a smaller graph"""
        g = Graph(self.directed, self.branching)
        g.nodes = self.nodes // 2
        g.edges = self.edges // 2
        return g

    def bisect(self, g: "Graph") -> "Graph":
        """find something “half way” between us and the given graph"""
        n = Graph(self.directed, self.branching)
        n.nodes = (self.nodes + g.nodes) // 2
        n.edges = (self.edges + g.edges) // 2
        return n

    def serialize(self, out):
        """write this graph to the given output stream"""
        out.write(f'{"digraph" if self.directed else "graph"} {{\n')
        for i in range(self.nodes):
            out.write(f"  n{i}\n")
        for i in range(self.edges):
            src = i // self.branching
            for j in range(self.branching):
                dst = src + j + 1
                if dst >= self.nodes:
                    # Our branching factor and edge count exceeded the number of
                    # nodes we have available. Oh well, we do not get all the
                    # edges we asked for.
                    continue
                out.write(f'  n{src} {"->" if self.directed else "--"} n{dst}\n')
        out.write("}")

class Result():
    TIMEOUT = 124

    def __init__(self, status: int, runtime: int, rss: Optional[int] = 0):
        self.status = status   # exit status
        self.runtime = runtime # execution time
        self.rss = rss         # Resident Set Size (peak memory usage)

def run(args: List[str], timeout: Optional[int]) -> Result:
    """
    run a command, capping its execution time and measuring its memory usage
    """

    # copy to avoid modifying input array
    argv = args[:]

    # make command tell us some statistics
    argv = ["/usr/bin/time", "--verbose"] + argv

    # run it
    try:
        p = subprocess.run(argv, capture_output=True, timeout=timeout,
                           universal_newlines=True)
    except subprocess.TimeoutExpired:
        return Result(Result.TIMEOUT, timeout + 1)

    # extract runtime
    m = re.search(r"\bElapsed \(wall clock\) time \(h:mm:ss or m:ss\): "
                  r"(\d+:)?(\d+):(\d+)", p.stderr, re.MULTILINE)
    assert m is not None, "runtime information not found in output"
    runtime = int(m.group(2)) * 60 + int(m.group(3))
    if m.group(1) is not None:
        runtime += int(m.group(1)) * 60 * 60

    # extract memory usage
    m = re.search(r"\bMaximum resident set size \(kbytes\): (\d+)\s*$",
                  p.stderr, re.MULTILINE)
    assert m is not None, "memory usage information not found in output"
    rss = int(m.group(1))

    return Result(p.returncode, runtime, rss)

def process(args: List[str], g: Graph, timeout: Optional[int]) -> Result:
    """
    stage a graph to be consumed by Graphviz and then run it under
    instrumentation
    """

    # setup scratch space
    with tempfile.TemporaryDirectory() as tmp:
        src = Path(tmp) / "input.dot"

        # write our test input to a file
        with open(src, "wt") as f:
            g.serialize(f)

        # process this graph
        return run(args + [src], timeout)


def main(args: List[str]) -> int:

    # parse command line arguments
    parser = argparse.ArgumentParser(description="Graphviz graph generator")
    parser.add_argument("--branching", type=int, default=1,
                        help="desired outgoing edges per node")
    parser.add_argument("--digraph", action="store_true",
                        help="generate a directed graph")
    parser.add_argument("--dot", default="dot", help="path to Graphviz binary")
    parser.add_argument("--dot-arg", action="append", default=[],
                        help="extra command line arguments to Graphviz")
    parser.add_argument("--graph", dest="digraph", action="store_false",
                        help="generate an undirected graph")
    parser.add_argument("--max-mem", type=int,
                        help="maximum memory usage in KB")
    parser.add_argument("--max-runtime", type=int,
                        help="maximum runtime in seconds")
    parser.add_argument("--min-mem", type=int,
                        help="minimum memory usage in KB")
    parser.add_argument("--min-runtime", type=int,
                        help="minimum runtime in seconds")
    parser.add_argument("--output", "-o", type=argparse.FileType("wt"),
                        default=sys.stdout,
                        help="output file to write final graph to")
    options = parser.parse_args(args[1:])

    if platform.system() != "Linux":
        sys.stderr.write("warning: not on Linux; this probably will not work\n")

    # validate we can run Graphviz
    subprocess.check_call([options.dot, "-V"], stdout=subprocess.DEVNULL,
                          stderr=subprocess.DEVNULL)

    # some possible taste test outcomes
    (TOO_COLD,   # this porridge sucks
     JUST_RIGHT, # OK, I guess
     TOO_HOT     # this porridge also sucks
    ) = list(range(3))

    # define our criteria for success
    def judge(g: Graph, r: Result) -> int:
        if r.status == Result.TIMEOUT:
            return TOO_HOT
        if options.max_runtime is not None and options.max_runtime < r.runtime:
            return TOO_HOT
        if options.min_runtime is not None and options.min_runtime > r.runtime:
            return TOO_COLD
        if options.max_mem is not None and options.max_mem < r.rss:
            return TOO_HOT
        if options.min_mem is not None and options.min_mem > r.rss:
            return TOO_COLD
        return JUST_RIGHT

    # construct our Graphviz invocation
    argv = [options.dot, "-Tsvg", "-o", os.devnull] + options.dot_arg

    # construct our starting point
    graph = Graph(options.digraph, options.branching)

    # run this to get a baseline
    result = process(argv, graph, options.max_runtime)

    j = judge(graph, result)

    # only do the rest if we do not immediately hit the constraints
    if j != JUST_RIGHT:

        if j == TOO_COLD:
            lb = graph
            ub = None
        else:
            lb = None
            ub = graph

        # run in a loop until we hit the required constraints
        while True:

            # derive a new candidate
            if ub is None:
                c = lb.grow()
            elif lb is None:
                c = ub.shrink()
            else:
                c = lb.bisect(ub)

            # take it for a test drive
            r = process(argv, c, options.max_runtime)
            print(f"graph with {c.nodes} nodes and {c.edges} used {r.rss}KB")

            # what did we learn?
            j = judge(c, r)
            if j == TOO_COLD:
                lb = c
            elif j == JUST_RIGHT:
                graph = c
                break
            else:
                ub = c

    # yield the final graph to the user
    graph.serialize(options.output)

    return 0

if __name__ == "__main__":
    sys.exit(main(sys.argv))
