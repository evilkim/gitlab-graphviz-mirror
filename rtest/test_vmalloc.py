# test ../lib/vmalloc

import os
from pathlib import Path
import platform
import subprocess
import tempfile

def test_vmalloc():
    '''run the vmalloc unit tests'''

    # locate the vmalloc unit tests
    src = Path(__file__).parent.resolve() / '../lib/vmalloc/test.c'
    assert src.exists()

    # locate lib directory that needs to be in the include path
    lib = Path(__file__).parent.resolve() / '../lib'

    # create a temporary directory to work in
    with tempfile.TemporaryDirectory() as tmp:

      # compile the unit tests
      dst = Path(tmp) / 'vmalloc-tests.exe'
      if platform.system() == 'Windows':
        subprocess.check_call(['cl', '-I', lib, '-nologo', src, '-Fe:', dst])
      else:
        subprocess.check_call([os.environ.get('CC', 'cc'), '-std=gnu99',
          '-Wall', '-Wextra', '-Werror', '-I', lib, '-o', dst, src])

      # run the unit tests
      subprocess.check_call([dst])
