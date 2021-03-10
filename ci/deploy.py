#!/usr/bin/env python3

'''
steps for deploying a new release (see ../.gitlab-ci.yml)

This is based on Gitlab’s generic package example,
https://gitlab.com/gitlab-org/release-cli/-/tree/master/docs/examples/release-assets-as-generic-package
'''

import argparse
import hashlib
import json
import logging
import os
import re
import shutil
import stat
import subprocess
import sys
from typing import Generator, List, Optional

# logging output stream, setup in main()
log = None

def upload(version: str, path: str, name: Optional[str] = None) -> str:
  '''
  upload a file to the Graphviz generic package with the given version
  '''

  # use the path as the name if no other was given
  if name is None:
    name = path

  # Gitlab upload file_name field only allows letters, numbers, dot, dash, and
  # underscore
  safe = re.sub(r'[^a-zA-Z0-9.\-]', '_', name)
  log.info(f'escaped name {name} to {safe}')

  target = f'{os.environ["CI_API_V4_URL"]}/projects/' \
    f'{os.environ["CI_PROJECT_ID"]}/packages/generic/graphviz-releases/' \
    f'{version}/{safe}'

  log.info(f'uploading {path} to {target}')
  # calling Curl is not the cleanest way to achieve this, but Curl takes care of
  # encodings, headers and part splitting for us
  proc = subprocess.run(['curl',
    '--silent',     # no progress bar
    '--include',    # include HTTP response headers in output
    '--verbose',    # more connection details
    '--retry', '3', # retry on transient errors
    '--header', f'JOB-TOKEN: {os.environ["CI_JOB_TOKEN"]}',
    '--upload-file', path, target], stdout=subprocess.PIPE,
    stderr=subprocess.STDOUT, universal_newlines=True)
  log.info('Curl response:')
  for i, line in enumerate(proc.stdout.split('\n'), 1):
    log.info(f' {i:3}: {line}')
  proc.check_returncode()

  resp = proc.stdout.split('\n')[-1]
  if json.loads(resp)['message'] != '201 Created':
    raise Exception(f'upload failed: {resp}')

  return target

def checksum(path: str) -> Generator[str, None, None]:
  '''
  generate checksum(s) for the given file
  '''

  assert os.path.exists(path)

  log.info(f'MD5 summing {path}')
  check = f'{path}.md5'
  with open(check, 'wt') as f:
    with open(path, 'rb') as data:
      f.write(f'{hashlib.md5(data.read()).hexdigest()}  {path}\n')
  yield check

  log.info(f'SHA256 summing {path}')
  check = f'{path}.sha256'
  with open(check, 'wt') as f:
    with open(path, 'rb') as data:
      f.write(f'{hashlib.sha256(data.read()).hexdigest()}  {path}\n')
  yield check

def is_macos_artifact(path: str) -> bool:
  '''
  is this a deployment artifact for macOS?
  '''
  return re.search(r'\bDarwin\b', path) is not None

def is_windows_artifact(path: str) -> bool:
  '''
  is this a deployment artifact for Windows?
  '''
  return re.search(r'\bwindows\b', path) is not None

def main(args: List[str]) -> int:

  # setup logging to print to stderr
  global log
  ch = logging.StreamHandler()
  log = logging.getLogger('deploy.py')
  log.addHandler(ch)

  # parse command line arguments
  parser = argparse.ArgumentParser(description='Graphviz deployment script')
  parser.add_argument('--version', help='Override version number used to '
    'create a release. Without this, the contents of the VERSION file will be '
    'used.')
  parser.add_argument('--force', action='store_true', help='Force creating a '
    'Gitlab release, even if the version number does not match \\d+.\\d+.\\d+.')
  parser.add_argument('--verbose', action='store_true', help='Print more '
    'diagnostic information.')
  options = parser.parse_args(args[1:])

  if options.verbose:
    log.setLevel(logging.INFO)

  if os.environ.get('CI') is None:
    log.error('CI environment variable unset; refusing to run')
    return -1

  # echo some useful things for debugging
  log.info(f'os.uname(): {os.uname()}')
  if os.path.exists('/etc/os-release'):
    with open('/etc/os-release') as f:
      log.info('/etc/os-release:')
      for i, line in enumerate(f, 1):
        log.info(f' {i}: {line[:-1]}')

  # bail out early if we do not have release-cli to avoid uploading assets that
  # become orphaned when we fail to create the release
  if not shutil.which('release-cli'):
    log.error('release-cli not found')
    return -1

  # retrieve version name left by prior CI tasks
  log.info('reading VERSION')
  with open('VERSION') as f:
    gv_version = f.read().strip()
  log.info(f'VERSION == {gv_version}')

  # if we were not passed an explicit version, use the one from the VERSION file
  if options.version is None:
    options.version = gv_version

  # the generic package version has to be \d+.\d+.\d+ but it does not need to
  # correspond to the release version (which may not conform to this pattern if
  # this is a dev release)
  if re.match(r'\d+\.\d+\.\d+$', options.version) is None:
    # generate a compliant version
    package_version = f'0.0.{int(os.environ["CI_COMMIT_SHA"], 16)}'
  else:
    # we can use a version corresponding to the release version
    package_version = options.version
  log.info(f'using generic package version {package_version}')

  # list of assets we have uploaded
  assets: List[str] = []

  for tarball in (f'graphviz-{gv_version}.tar.gz',
                  f'graphviz-{gv_version}.tar.xz'):

    if not os.path.exists(tarball):
      log.error(f'source {tarball} not found')
      return -1

    # accrue the source tarball and accompanying checksum(s)
    assets.append(upload(package_version, tarball))
    for check in checksum(tarball):
      assets.append(upload(package_version, check))

  for stem, _, leaves in os.walk('Packages'):
    for leaf in leaves:
      path = os.path.join(stem, leaf)

      # get existing permissions
      mode = os.stat(path).st_mode

      # fixup permissions, o-rwx g-wx
      os.chmod(path, mode & ~stat.S_IRWXO & ~stat.S_IWGRP & ~stat.S_IXGRP)

      assets.append(upload(package_version, path, path[len('Packages/'):]))

      # if this is a standalone Windows or macOS package, also provide
      # checksum(s)
      if is_macos_artifact(path) or is_windows_artifact(path):
        for c in checksum(path):
          assets.append(upload(package_version, c, c[len('Packages/'):]))

  # we only create Gitlab releases for stable version numbers
  if not options.force:
    if re.match(r'\d+\.\d+\.\d+$', options.version) is None:
      log.warning(f'skipping release creation because {options.version} is not '
        'of the form \\d+.\\d+.\\d+')
      return 0

  # construct a command to create the release itself
  cmd = ['release-cli', 'create', '--name', options.version, '--tag-name',
    options.version, '--description', 'See the [CHANGELOG](https://gitlab.com/'
    'graphviz/graphviz/-/blob/master/CHANGELOG.md).']
  for a in assets:
    name = os.path.basename(a)
    url = a
    cmd += ['--assets-link', json.dumps({'name':name, 'url':url})]

  # create the release
  subprocess.check_call(cmd)

  return 0

if __name__ == '__main__':
  sys.exit(main(sys.argv))
