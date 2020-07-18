# tests that examples provided with Graphviz compile correctly

import os
import platform
import pytest
import shutil
import subprocess

def c_compiler():
    '''find the system's C compiler'''
    if platform.system() == 'Windows':
      return "cl"
    else:
      return os.environ.get('CC', 'cc')

@pytest.mark.parametrize('src', ['demo.c', 'dot.c', 'example.c', 'neatopack.c',
  'simple.c'])
def test_compile_example(src):
    '''try to compile the example'''

    # construct an absolute path to the example
    filepath = os.path.join(os.path.abspath(os.path.dirname(__file__)),
      '..', 'dot.demo', src)

    cc = c_compiler()

    libs = ('cgraph', 'gvc')

    # ensure the C compiler can build this without error
    if platform.system() == 'Windows':
      subprocess.check_call([cc, filepath, '-nologo', '-link']
        + ['{}.lib'.format(l) for l in libs])
    else:
      subprocess.check_call([cc, '-o', os.devnull, filepath]
        + ['-l{}'.format(l) for l in libs])

@pytest.mark.parametrize('src', ['addrings', 'attr', 'bbox', 'bipart',
  'chkedges', 'clustg', 'collapse', 'cycle', 'deghist', 'delmulti', 'depath',
  'flatten', 'group', 'indent', 'path', 'scale', 'span', 'treetoclust',
  'addranks', 'anon', 'bb', 'chkclusters', 'cliptree', 'col', 'color',
  'dechain', 'deledges', 'delnodes', 'dijkstra', 'get-layers-list', 'knbhd',
  'maxdeg', 'rotate', 'scalexy', 'topon'])
def test_gvpr_example(src):
    '''check GVPR can parse the given example'''

    # skip this test if GVPR is unavailable
    if shutil.which('gvpr') is None:
      pytest.skip('GVPR not available')

    # construct an absolute path to the example
    path = os.path.join(os.path.abspath(os.path.dirname(__file__)),
      '../cmd/gvpr/lib', src)

    # run GVPR with the given script
    with open(os.devnull, 'rt') as nul:
      subprocess.check_call(['gvpr', '-f', path], stdin=nul)
