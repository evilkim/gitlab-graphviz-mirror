"""test ../lib/cgraph/sprint.c"""

import os
from pathlib import Path
import sys

sys.path.append(os.path.dirname(__file__))
from gvtest import run_c #pylint: disable=C0413

def test_sprint():
  """run the sprint unit tests"""

  # locate the sprint unit tests
  src = Path(__file__).parent.resolve() / "../lib/cgraph/test_sprint.c"
  assert src.exists()

  # locate lib directory that needs to be in the include path
  lib = Path(__file__).parent.resolve() / "../lib"

  # extra C flags this compilation needs
  cflags = ['-I', lib]

  ret, _, _ = run_c(src, cflags=cflags)

  assert ret == 0
