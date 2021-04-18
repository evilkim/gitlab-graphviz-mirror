# test ../lib/vmalloc

import os
from pathlib import Path
import platform
import sys

sys.path.append(os.path.dirname(__file__))
from gvtest import run_c

def test_vmalloc():
    """run the vmalloc unit tests"""

    # locate the vmalloc unit tests
    src = Path(__file__).parent.resolve() / "../lib/vmalloc/test.c"
    assert src.exists()

    # locate lib directory that needs to be in the include path
    lib = Path(__file__).parent.resolve() / "../lib"

    # extra C flags this compilation needs
    cflags = ["-I", lib]
    if platform.system() != "Windows":
      cflags += ["-std=gnu99", "-Wall", "-Wextra", "-Werror"]

    ret, _, _ = run_c(src, cflags=cflags)

    assert ret == 0
