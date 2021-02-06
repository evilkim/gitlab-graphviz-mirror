import os
import platform
import pytest
import re
import shutil
import subprocess
import shutil

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
