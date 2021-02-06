import pytest
import subprocess
import os
from pathlib import Path

def test_installation():
    expected_version = os.environ.get('GV_VERSION')

    # If $GV_VERSION is not set, run the CI step that derives it. This will fail
    # if the user is in a snapshot directory without Git installed, but assume
    # they can live with that.
    if expected_version is None:
        ROOT = Path(__file__).parent.parent.parent.parent
        expected_version = subprocess.check_output(['python3',
          'gen_version.py'], cwd=ROOT, universal_newlines=True).strip()

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
        pytest.fail(f'Malformed version string: {actual_version_string}')
    assert actual_version == expected_version
