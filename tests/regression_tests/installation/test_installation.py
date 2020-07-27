import pytest
import subprocess
import os

# FIXME: Remove skip when
# https://gitlab.com/graphviz/graphviz/-/issues/1745 is fixed
@pytest.mark.skipif(
    os.getenv('build_system') == 'msbuild',
    reason='Windows msbuild executables have the wrong version (#1745)'
)
def test_installation():
    expected_version = os.environ['GV_VERSION']
    actual_version_string = subprocess.check_output(
        [
            'dot',
            '-V',
        ],
        universal_newlines=True,
        stderr=subprocess.STDOUT,
    )
    try:
        actual_version = actual_version_string.split()[4]
    except IndexError:
        pytest.fail('Malformed version string: {0}'.format(actual_version_string))
    assert actual_version == expected_version
