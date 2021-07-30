"""test ../lib/cgraph/itos.h"""

import os
from pathlib import Path
import sys

sys.path.append(os.path.dirname(__file__))
from gvtest import run_c # pylint: disable=wrong-import-position

def test_itos():
  """run the itos unit tests"""

  # locate the itos unit tests
  src = Path(__file__).parent.resolve() / "../lib/cgraph/test_itos.c"
  assert src.exists()

  # locate lib directory that needs to be in the include path
  lib = Path(__file__).parent.resolve() / "../lib"

  # extra C flags this compilation needs
  cflags = ['-I', lib]

  ret, _, _ = run_c(src, cflags=cflags)

  assert ret == 0
