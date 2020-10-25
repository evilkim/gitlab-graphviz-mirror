#!/usr/bin/env python3

'''
test cases that are only relevant to run in CI
'''

import pytest
import shutil
import subprocess
import os

@pytest.mark.parametrize('binary', [
  'mingle',
  'smyrna',
])
def test_existence(binary: str):
  '''
  check that a given binary was built and is on $PATH
  '''

  os_id = os.getenv('OS_ID')

  # FIXME: Remove skip when
  # https://gitlab.com/graphviz/graphviz/-/issues/1835 is fixed
  if os_id in ['ubuntu', 'centos'] and binary == 'mingle':
    check_that_tool_does_not_exist(binary, os_id)
    pytest.skip('mingle is not built for ' + os_id + ' (#1835)')

  # FIXME: Remove skip when
  # https://gitlab.com/graphviz/graphviz/-/issues/1834 is fixed
  if os_id == 'centos' and binary == 'smyrna':
    check_that_tool_does_not_exist(binary, os_id)
    pytest.skip('smyrna is not built for Centos (#1834)')

  assert shutil.which(binary) is not None

def check_that_tool_does_not_exist(tool, os_id):
    assert shutil.which(tool) is None, '{} has been resurrected in the {} ' \
    'build on {}. Please remove skip.'.format(
        tool,
        os.getenv('build_system'),
        os_id
    )
