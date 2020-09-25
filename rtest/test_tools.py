import os
import pytest
import subprocess

@pytest.mark.parametrize('tool', [
    'acyclic',
    'bcomps',
    'ccomps',
    'diffimg',
    'dijkstra',
    'dot',
    'dotty',
    'edgepaint',
    'gc',
    'gml2gv',
    'graphml2gv',
    'gv2gml',
    'gvcolor',
    'gvgen',
    'gvmap',
    'gvpack',
    'gvpr',
    'gxl2gv',
    'lefty',
    'lneato',
    'mingle',
    'mm2gv',
    'nop',
    'prune',
    'sccmap',
    'smyrna',
    'tred',
    'unflatten',
])
def test_tools(tool):

    tools_not_built_with_cmake = [
        'diffimg',
        'dotty',
        'edgepaint',
        'gc',
        'gv2gml',
        'gvmap',
        'gvpr',
        'lefty',
        'lneato',
        'mingle',
        'prune',
        'smyrna',
    ]

    os_id = os.getenv('OS_ID')

    # FIXME: Remove skip when
    # https://gitlab.com/graphviz/graphviz/-/issues/1829 is fixed
    if tool == 'smyrna' and os.getenv('build_system') == 'msbuild':
      pytest.skip('smyrna fails to start because of missing DLLs in Windows MSBuild builds (#1829)')

    # FIXME: Remove skip when
    # https://gitlab.com/graphviz/graphviz/-/issues/1834 is fixed
    if tool == 'smyrna' and os_id == 'centos':
      pytest.skip('smyrna is not built for Centos (#1834)')

    # FIXME: Remove skip when
    # https://gitlab.com/graphviz/graphviz/-/issues/1835 is fixed
    if tool == 'mingle' and os_id in ['ubuntu', 'centos']:
      pytest.skip('mingle is not built for ' + os_id + ' (#1835)')

    # FIXME: Remove skip when
    # https://gitlab.com/graphviz/graphviz/-/issues/1753 and
    # https://gitlab.com/graphviz/graphviz/-/issues/1836 is fixed
    if os.getenv('build_system') == 'cmake':
      if tool in tools_not_built_with_cmake:
        pytest.skip(tool + ' is not built with CMake (#1753 & #1836)')

    subprocess.check_call(
        [tool, '-?'],
        stdin=subprocess.DEVNULL,
    )
