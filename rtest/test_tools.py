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
