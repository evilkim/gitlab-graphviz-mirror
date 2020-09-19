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
    subprocess.check_call(
        [tool, '-?'],
        stdin=subprocess.DEVNULL,
    )
