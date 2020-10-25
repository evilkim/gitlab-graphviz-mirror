#!/usr/bin/env python3

'''
test cases that are only relevant to run in CI
'''

import pytest
import shutil
import subprocess
import os

@pytest.mark.parametrize('binary', [
  'dot_builtins',
  'mingle',
  'smyrna',
])
def test_existence(binary: str):
  '''
  check that a given binary was built and is on $PATH
  '''

  tools_not_built_with_cmake = [
    'circo',
    'cluster',
    'diffimg',
    'dot2gxl',
    'dot_builtins',
    'dotty',
    'edgepaint',
    'fdp',
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

  os_id = os.getenv('OS_ID')

  # FIXME: Remove skip when
  # https://gitlab.com/graphviz/graphviz/-/issues/1839 is fixed
  if os_id in ['centos', 'fedora'] and binary == 'dot_builtins':
    check_that_tool_does_not_exist(binary, os_id)
    pytest.skip('dot_builtins is not installed for ' + os_id + ' (#1839)')

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

  # FIXME: Remove skip when
  # https://gitlab.com/graphviz/graphviz/-/issues/1753 and
  # https://gitlab.com/graphviz/graphviz/-/issues/1836 is fixed
  if os.getenv('build_system') == 'cmake':
    if binary in tools_not_built_with_cmake:
      check_that_tool_does_not_exist(binary, os_id)
      pytest.skip(binary + ' is not built with CMake (#1753 & #1836)')

  assert shutil.which(binary) is not None

def check_that_tool_does_not_exist(tool, os_id):
    assert shutil.which(tool) is None, '{} has been resurrected in the {} ' \
    'build on {}. Please remove skip.'.format(
        tool,
        os.getenv('build_system'),
        os_id
    )
