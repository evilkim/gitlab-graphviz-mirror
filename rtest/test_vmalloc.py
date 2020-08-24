# test ../lib/vmalloc

import os
import platform
import subprocess
import tempfile

def test_vmalloc():
    '''run the vmalloc unit tests'''

    # locate the vmalloc unit tests
    src = os.path.join(os.path.abspath(os.path.dirname(__file__)),
      '../lib/vmalloc/test.c')
    assert os.path.exists(src)

    # create a temporary directory to work in
    with tempfile.TemporaryDirectory() as tmp:

      # compile the unit tests
      dst = os.path.join(tmp, 'vmalloc-tests.exe')
      if platform.system() == 'Windows':
        subprocess.check_call(['cl', '-nologo', src, '-Fe:', dst])
      else:
        subprocess.check_call([os.environ.get('CC', 'cc'), '-Wall', '-Wextra',
          '-Werror', '-o', dst, src])

      # run the unit tests
      subprocess.check_call([dst])
