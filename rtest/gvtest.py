"""common Graphviz test functionality"""

import os
from pathlib import Path
import platform
import subprocess
import tempfile
from typing import List, Optional, Tuple

def compile_c(src: Path, cflags: List[str] = [], link: List[str] = [],
            dst: Optional[Path] = None) -> Path:
  """compile a C program"""

  # include compiler flags from both the caller and the environment
  cflags = os.environ.get("CFLAGS", "").split() + cflags
  ldflags = os.environ.get("LDFLAGS", "").split()

  # if the user did not give us destination, use a temporary path
  if dst is None:
    _, dst = tempfile.mkstemp(".exe")

  if platform.system() == "Windows":
    # determine which runtime library option we need
    rtflag = "-MDd" if os.environ.get("configuration") == "Debug" else "-MD"

    # construct an invocation of MSVC
    args = ["cl", src, "-Fe:", dst, "-nologo", rtflag] + cflags
    if len(link) > 0:
      args += ["-link"] + [f"{l}.lib" for l in link] + ldflags

  else:
    # construct an invocation of the default C compiler
    cc = os.environ.get("CC", "cc")
    args = [cc, "-std=c99", src, "-o", dst] + cflags
    if len(link) > 0:
      args += [f"-l{l}" for l in link] + ldflags

  # dump the command being run for the user to observe if the test fails
  print(f'+ {" ".join(str(x) for x in args)}')

  # compile the program
  try:
    subprocess.check_call(args)
  except subprocess.CalledProcessError:
    dst.unlink(missing_ok=True)
    raise

  return dst

def run_c(src: Path, args: List[str] = [], input: str = "",
        cflags: List[str] = [], link: List[str] = []) -> Tuple[int, str, str]:
  """compile and run a C program"""

  # create some temporary space to work in
  with tempfile.TemporaryDirectory() as tmp:

    # output filename to write our compiled code to
    exe = Path(tmp) / "a.exe"

    # compile the program
    compile_c(src, cflags, link, exe)

    # dump the command being run for the user to observe if the test fails
    print(f'+ {exe} {" ".join(str(x) for x in args)}')

    # run it
    p = subprocess.run([exe] + args, input=input, stdout=subprocess.PIPE,
      stderr=subprocess.PIPE, universal_newlines=True)

    return p.returncode, p.stdout, p.stderr
