#!/usr/bin/env python3

"""Graphviz CI script for compilation on Windows"""

import argparse
import os
from pathlib import Path
import shutil
import subprocess
import sys
from typing import List

def run(args: List[str], cwd: Path): #pylint: disable=C0116
  print(f"+ {' '.join(str(x) for x in args)}")
  subprocess.check_call(args, cwd=cwd)

def main(args: List[str]) -> int: #pylint: disable=C0116

  parser = argparse.ArgumentParser(description="Graphviz CI script for "
                                   "compilation on Windows")
  parser.add_argument("--build-system", choices=("cmake", "msbuild"),
                      required=True, help="build system to run")
  parser.add_argument("--configuration", choices=("Debug", "Release"),
                      required=True, help="build configuration to select")
  parser.add_argument("--platform", choices=("Win32", "x64"),
                      required=True, help="target platform")
  options = parser.parse_args(args[1:])

  # find the repository root directory
  root = Path(__file__).resolve().parent.parent

  if options.build_system == "cmake":
    build = root / "build"
    if build.exists():
      shutil.rmtree(build)
    os.makedirs(build)
    run(["cmake", "-G", "Visual Studio 16 2019", "-A", options.platform, "-D",
         "with_cxx_api=ON", ".."],
        build)
    run(["cmake", "--build", ".", "--config", options.configuration], build)
    run(["cpack", "-C", options.configuration], build)

  else:
    run(["msbuild.exe", f"-p:Configuration={options.configuration}",
         f"-p:Platform={options.platform}", "graphviz.sln"], root)
    if options.configuration == "Release":
      for filename in os.listdir(root / "Release" / "Graphviz" / "bin"):
        src = root / "Release" / "Graphviz" / "bin" / filename
        if src.suffix in (".lastcodeanalysissucceeded", ".iobj", ".ipdb",
                          ".ilk"):
          print(f"deleting {src}")
          src.unlink()

  return 0

if __name__ == "__main__":
  sys.exit(main(sys.argv))
