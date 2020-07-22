# Generate version
#
# Stable release entry format     : <major>.<minor>.>patch>
# Development release entry format: <major>.<minor>.<patch>-dev
#
# Stable release output format     : <major>.<minor>.<patch>
# Development release output format: <major>.<minor>.<patch>-dev.<YYYYmmdd.HHMM>

# The patch version of a development release should be the next stable
# release patch number followed by "-dev". The committer date will be
# added with a period separator.
#
# Example sequence:
#
# Entry          Output
# 2.44.1      => 2.44.1
# 2.44.2-dev  => 2.44.2-dev.20200704.1652
# 2.44.2      => 2.44.2
# 2.44.3-dev  => 2.44.3-dev.20200824.1337

version = '2.44.2-dev'

import os
import sys
import subprocess

major_version, minor_version, patch_version = version.split('.')

try:
    int(patch_version)
except ValueError:
    # Non-numerical patch version; add committer date
    os.environ['TZ'] = 'UTC'
    try:
        committer_date = subprocess.run(
            [
                'git',
                'log',
                '-n',
                '1',
                '--format=%cd',
                '--date=format-local:%Y%m%d.%H%M'
            ],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            check=True,
        ).stdout.decode('utf-8').strip()
        patch_version += '.' + committer_date
    except (subprocess.CalledProcessError, FileNotFoundError):
        print('Warning: build not started in a Git clone, or Git is not installed: setting version date to 0.', file=sys.stderr)

        patch_version += '.0'

print(f'{major_version}.{minor_version}.{patch_version}')
