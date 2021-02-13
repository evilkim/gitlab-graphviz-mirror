import os
from pathlib import Path
import platform
import pytest
import re
import shutil
import subprocess
import shutil

try:
  import pylint
except ImportError:
  pylint = None

@pytest.mark.parametrize('tool', [
    'acyclic',
    'bcomps',
    'ccomps',
    'circo',
    'cluster',
    'diffimg',
    'dijkstra',
    'dot',
    'dot2gxl',
    'dot_builtins',
    'dotty',
    'edgepaint',
    'fdp',
    'gc',
    'gml2gv',
    'graphml2gv',
    'gv2gml',
    'gv2gxl',
    'gvcolor',
    'gvedit',
    'gvgen',
    'gvmap',
    'gvmap.sh',
    'gvpack',
    'gvpr',
    'gxl2dot',
    'gxl2gv',
    'lefty',
    'lneato',
    'mingle',
    'mm2gv',
    'neato',
    'nop',
    'osage',
    'patchwork',
    'prune',
    'sccmap',
    'sfdp',
    'smyrna',
    'tred',
    'twopi',
    'unflatten',
    'vimdot',
])
def test_tools(tool):

    if shutil.which(tool) is None:
      pytest.skip(f'{tool} not available')

    # FIXME: Remove skip when
    # https://gitlab.com/graphviz/graphviz/-/issues/1829 is fixed
    if tool == 'smyrna' and os.getenv('build_system') == 'msbuild':
      pytest.skip('smyrna fails to start because of missing DLLs in Windows MSBuild builds (#1829)')

    # FIXME: Remove skip when
    # https://gitlab.com/graphviz/graphviz/-/issues/1838 is fixed
    if tool == 'gvpack' and platform.system() != 'Windows':
        if os.getenv('build_system') == 'cmake':
            pytest.skip('gvpack does not find libgvplugin_neato_layout.so.6'
                        'when built with CMake (#1838)')

    # Ensure that X fails to open display
    environ_copy = os.environ.copy()
    environ_copy.pop('DISPLAY', None)

    # Test usage
    output = subprocess.check_output(
        [tool, '-?'],
        env=environ_copy,
        stdin=subprocess.DEVNULL,
        stderr=subprocess.STDOUT,
        universal_newlines=True,
    )
    assert re.match('usage', output, flags=re.IGNORECASE) is not None, \
      f'{tool} -? did not show usage'

    # Test unsupported option
    returncode = subprocess.call(
        [tool, '-$'],
        env=environ_copy,
    )

    assert returncode != 0, f'{tool} accepted unsupported option -$'

# root directory of this checkout
ROOT = Path(__file__).parent.parent.resolve()

# find all Python files
pys = set()
for (prefix, _, files) in os.walk(ROOT):
  for name in files:
    if Path(name).suffix.lower() == '.py':
      pys.add(Path(prefix) / name)

@pytest.mark.parametrize('py', pys)
@pytest.mark.skipif(pylint is None, reason='pylint unavailable')
def test_pylint_errors(py: Path):
  '''
  Check all Python scripts for errors
  '''

  # files that may fail because they have an import that may not be installed
  WAIVER = frozenset(ROOT / x for x in (
    # files expecting to import 'gv' or using Python 2 syntax
    'dot.demo/gv_test.py',
    'tclpkg/gv/examples/layout.py',
    'tclpkg/gv/examples/simple.py',
    'tclpkg/gv/demo/modgraph.py',
    'tclpkg/gv/gv.py',
    'tclpkg/gv/test.py',

    'doc/infosrc/jconvert.py', # expects to imporrt 'json2html'
    'doc/infosrc/templates.py', # expects to import 'jinja2'
  ))

  # ensure this test fails if one of the above files is moved/deleted, to prompt
  # developers to update the list
  assert all(x.exists() for x in WAIVER), 'missing file in WAIVER list'

  import pylint.epylint

  # scan the given file
  errors, _ = pylint.epylint.py_run(f'"{py}" --errors-only', return_std=True)

  # do not check pass/fail if this file is waived
  if py in WAIVER:
    pytest.skip(f'pylint errors waived in {py}')

  # otherwise, expect it to succeed
  assert errors.getvalue() == ''
