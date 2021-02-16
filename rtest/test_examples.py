# tests that examples provided with Graphviz compile correctly

import os
from pathlib import Path
import platform
import pytest
import shutil
import subprocess
import tempfile

def c_compiler():
    '''find the system's C compiler'''
    if platform.system() == 'Windows':
      return "cl"
    else:
      return os.environ.get('CC', 'cc')

@pytest.mark.parametrize('src', ['demo.c', 'dot.c', 'example.c', 'neatopack.c',
  'simple.c'])
# FIXME: Remove skip when
# https://gitlab.com/graphviz/graphviz/-/issues/1777 is fixed
@pytest.mark.skipif(
    os.getenv('build_system') == 'msbuild',
    reason='Windows MSBuild release does not contain any header files (#1777)'
)
def test_compile_example(src):
    '''try to compile the example'''

    # construct an absolute path to the example
    filepath = Path(__file__).parent.resolve() / '..' / 'dot.demo' / src

    cc = c_compiler()

    libs = ('cgraph', 'gvc')

    # create some scratch space to work in
    with tempfile.TemporaryDirectory() as tmp:

      # compile our test code
      exe = Path(tmp) / 'a.exe'
      if platform.system() == 'Windows':
        debug = os.environ.get('configuration') == 'Debug'
        rt_lib_option = '-MDd' if debug else '-MD'
        subprocess.check_call([cc, filepath, '-Fe:', exe, '-nologo',
          rt_lib_option, '-link'] + [f'{l}.lib' for l in libs])
      else:
        subprocess.check_call([cc, '-std=c99', '-o', exe, filepath]
          + [f'-l{l}' for l in libs])

      # FIXME: Remove skip of execution of neatopack.c example when
      # https://gitlab.com/graphviz/graphviz/-/issues/1800 has been fixed
      if src == 'neatopack.c':
        pytest.skip('Executing neatopack gives segmentation fault (#1800)')

      # run the example
      args = ['-Kneato'] if src in ['demo.c', 'dot.c'] else [];

      p = subprocess.Popen(
          [exe] + args,
          stdin=subprocess.PIPE,
          universal_newlines=True,
      )
      p.communicate(input='graph {a -- b}')
      print(f'returncode: {p.returncode} = 0x{p.returncode:08x}')
      assert p.returncode == 0

@pytest.mark.parametrize('src', ['addrings', 'attr', 'bbox', 'bipart',
  'chkedges', 'clustg', 'collapse', 'cycle', 'deghist', 'delmulti', 'depath',
  'flatten', 'group', 'indent', 'path', 'scale', 'span', 'treetoclust',
  'addranks', 'anon', 'bb', 'chkclusters', 'cliptree', 'col', 'color',
  'dechain', 'deledges', 'delnodes', 'dijkstra', 'get-layers-list', 'knbhd',
  'maxdeg', 'rotate', 'scalexy', 'topon'])
@pytest.mark.skipif(shutil.which('gvpr') is None, reason='GVPR not available')
def test_gvpr_example(src):
    '''check GVPR can parse the given example'''

# FIXME: remove when https://gitlab.com/graphviz/graphviz/-/issues/1784 is fixed
    if ((os.environ.get('build_system') == 'msbuild' and \
      os.environ.get('configuration') == 'Debug') or
      (platform.system() == 'Windows' and \
      os.environ.get('build_system') == 'cmake')) and \
      src in ['bbox', 'col']:
      pytest.skip('GVPR tests "bbox" and "col" hangs on Windows MSBuild Debug '
                  'builds and Windows CMake builds (#1784)')

    # construct a relative path to the example because gvpr on Windows does not
    # support absolute paths (#1780)
    path = Path('cmd/gvpr/lib') / src
    wd = Path(__file__).parent.parent.resolve()

    # run GVPR with the given script
    with open(os.devnull, 'rt') as nul:
      subprocess.check_call(['gvpr', '-f', path], stdin=nul, cwd=wd)

@pytest.mark.skipif(shutil.which('gvpr') is None, reason='GVPR not available')
# FIXME: Remove skip when
# https://gitlab.com/graphviz/graphviz/-/issues/1882 is fixed
@pytest.mark.skipif(
    platform.system() == 'Windows' and \
    os.getenv('build_system') == 'cmake' and \
    platform.machine() in ('AMD64', 'x86_64'),
    reason='test_gvpr_clustg fails with 64-bit gvpr on Windows (#1882)'
)
def test_gvpr_clustg():
    '''check cmd/gvpr/lib/clustg works'''

    # construct a relative path to clustg because gvpr on Windows does not
    # support absolute paths (#1780)
    path = Path('cmd/gvpr/lib/clustg')
    wd = Path(__file__).parent.parent.resolve()

    # some sample input
    input = 'digraph { N1; N2; N1 -> N2; N3; }'

    # run GVPR on this input
    p = subprocess.Popen(['gvpr', '-f', path], stdin=subprocess.PIPE,
      stdout=subprocess.PIPE, cwd=wd, universal_newlines=True)
    output, _ = p.communicate(input)

    assert p.returncode == 0, 'GVPR exited with non-zero status'

    assert output.strip() == 'strict digraph "clust%1" {\n' \
                             '\tnode [_cnt=0];\n' \
                             '\tedge [_cnt=0];\n' \
                             '\tN1 -> N2\t[_cnt=1];\n' \
                             '\tN3;\n' \
                             '}', 'unexpected output'
