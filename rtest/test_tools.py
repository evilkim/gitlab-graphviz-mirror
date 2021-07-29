"""
Graphviz tools tests

The test cases in this file are sanity checks on the various tools in the
Graphviz suite. A failure of one of these indicates that some basic functional
property of one of the tools has been broken.
"""

import os
import platform
import re
import shutil
import subprocess
import pytest

@pytest.mark.parametrize("tool", [
    "acyclic",
    "bcomps",
    "ccomps",
    "circo",
    "cluster",
    "diffimg",
    "dijkstra",
    "dot",
    "dot2gxl",
    "dot_builtins",
    "dotty",
    "edgepaint",
    "fdp",
    "gc",
    "gml2gv",
    "graphml2gv",
    "gv2gml",
    "gv2gxl",
    "gvcolor",
    "gvedit",
    "gvgen",
    "gvmap",
    "gvmap.sh",
    "gvpack",
    "gvpr",
    "gxl2dot",
    "gxl2gv",
    "lefty",
    "lneato",
    "mingle",
    "mm2gv",
    "neato",
    "nop",
    "osage",
    "patchwork",
    "prune",
    "sccmap",
    "sfdp",
    "smyrna",
    "tred",
    "twopi",
    "unflatten",
    "vimdot",
])
def test_tools(tool):
  """
  check the help/usage output of the given tool looks correct
  """

  if shutil.which(tool) is None:
    pytest.skip(f"{tool} not available")

  # FIXME: Remove skip when
  # https://gitlab.com/graphviz/graphviz/-/issues/1829 is fixed
  if tool == "smyrna" and os.getenv("build_system") == "msbuild":
    pytest.skip("smyrna fails to start because of missing DLLs in Windows MSBuild builds (#1829)")

  # FIXME: Remove skip when
  # https://gitlab.com/graphviz/graphviz/-/issues/1838 is fixed
  if tool == "gvpack" and platform.system() != "Windows":
    if os.getenv("build_system") == "cmake":
      pytest.skip("gvpack does not find libgvplugin_neato_layout.so.6"
                  "when built with CMake (#1838)")

  # Ensure that X fails to open display
  environ_copy = os.environ.copy()
  environ_copy.pop("DISPLAY", None)

  # Test usage
  p = subprocess.Popen([tool, "-?"], env=environ_copy, stdin=subprocess.DEVNULL,
                       stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                       universal_newlines=True)
  output, _ = p.communicate()
  ret = p.returncode

  # FIXME: https://gitlab.com/graphviz/graphviz/-/issues/1934
  # cope with flaky failures, while also failing if this flakiness has been
  # fixed
  if tool == "lneato" and os.getenv("build_system") == "msbuild":
    has_pass = ret == 0
    has_fail = ret != 0
    for _ in range(100):
      if has_pass and has_fail:
        break
      p = subprocess.Popen([tool, "-?"], env=environ_copy,
                           stdin=subprocess.DEVNULL, stdout=subprocess.PIPE,
                           stderr=subprocess.STDOUT, universal_newlines=True)
      out, _ = p.communicate()
      if p.returncode == 0:
        has_pass = True
        ret = p.returncode
        output = out
      else:
        has_fail = True
    assert has_pass, "could not find passing execution"
    assert has_fail, "could not find failing execution (#1934 fixed?)"

  assert ret == 0, f"`{tool} -?` failed. Output was: {output}"

  assert re.match("usage", output, flags=re.IGNORECASE) is not None, \
    f"{tool} -? did not show usage. Output was: {output}"

  # Test unsupported option
  returncode = subprocess.call(
      [tool, "-$"],
      env=environ_copy,
  )

  assert returncode != 0, f"{tool} accepted unsupported option -$"

@pytest.mark.skipif(shutil.which("edgepaint") is None,
                    reason="edgepaint not available")
@pytest.mark.parametrize("arg", ("-accuracy=0.01", "-angle=15",
                                 "-random_seed=42", "-random_seed=-42",
                                 "-lightness=0,70", "-share_endpoint",
                                 f"-o {os.devnull}", "-color_scheme=accent7",
                                 "-v"))
def test_edgepaint_options(arg: str):
  """
  edgepaint should correctly understand all its command line flags
  """

  # a basic graph that edgepaint can process
  input =                                                                      \
    'digraph {\n'                                                              \
    '  graph [bb="0,0,54,108"];\n'                                             \
    '  node [label="\\N"];\n'                                                  \
    '  a       [height=0.5,\n'                                                 \
    '           pos="27,90",\n'                                                \
    '           width=0.75];\n'                                                \
    '  b       [height=0.5,\n'                                                 \
    '           pos="27,18",\n'                                                \
    '           width=0.75];\n'                                                \
    '  a -> b  [pos="e,27,36.104 27,71.697 27,63.983 27,54.712 27,46.112"];\n' \
    '}'

  # run edgepaint on this
  args = ["edgepaint"] + arg.split(" ")
  p = subprocess.Popen(args, stdin=subprocess.PIPE, universal_newlines=True)
  p.communicate(input)

  assert p.returncode == 0, f"edgepaint rejected command line option '{arg}'"
