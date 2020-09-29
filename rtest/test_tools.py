import os
import platform
import pytest
import re
import subprocess

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

    tools_not_built_with_cmake = [
        'circo',
        'cluster',
        'diffimg',
        'dot2gxl',
        'dot_builtins',
        'dotty',
        'edgepaint',
        'fdp',
        'gc',
        'gv2gml',
        'gv2gxl',
        'gvedit',
        'gvmap',
        'gvmap.sh',
        'gvpr',
        'gxl2dot',
        'lefty',
        'lneato',
        'mingle',
        'neato',
        'osage',
        'patchwork',
        'prune',
        'sfdp',
        'smyrna',
        'twopi',
        'vimdot',
    ]

    tools_not_built_with_msbuild = [
        'circo',
        'cluster',
        'dot2gxl',
        'dot_builtins',
        'fdp',
        'gv2gxl',
        'gvedit',
        'gvmap.sh',
        'gxl2dot',
        'neato',
        'osage',
        'patchwork',
        'sfdp',
        'twopi',
        'vimdot',
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
    # https://gitlab.com/graphviz/graphviz/-/issues/1839 is fixed
    if tool == 'dot_builtins' and os_id in ['centos', 'fedora']:
      pytest.skip('dot_builtins is not installed for ' + os_id + ' (#1839)')

    # FIXME: Remove skip when
    # https://gitlab.com/graphviz/graphviz/-/issues/1753 and
    # https://gitlab.com/graphviz/graphviz/-/issues/1836 is fixed
    if os.getenv('build_system') == 'cmake':
      if tool in tools_not_built_with_cmake:
        pytest.skip(tool + ' is not built with CMake (#1753 & #1836)')

    # FIXME: Remove skip when
    # https://gitlab.com/graphviz/graphviz/-/issues/1837 is fixed
    if os.getenv('build_system') == 'msbuild':
      if tool in tools_not_built_with_msbuild:
        pytest.skip(tool + ' is not built with MSBuild (#1837)')

    # FIXME: Remove skip when
    # https://gitlab.com/graphviz/graphviz/-/issues/1838 is fixed
    if tool == 'gvpack' and platform.system() != 'Windows':
        if os.getenv('build_system') == 'cmake':
            pytest.skip('gvpack does not find libgvplugin_neato_layout.so.6'
                        'when built with CMake (#1838)')

    # Test usage
    output = subprocess.check_output(
        [tool, '-?'],
        stdin=subprocess.DEVNULL,
        stderr=subprocess.STDOUT,
        universal_newlines=True,
    )
    assert re.match('usage', output, flags=re.IGNORECASE) is not None, \
      tool +' -? did not show usage'
