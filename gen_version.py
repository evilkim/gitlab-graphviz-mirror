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
import subprocess

major_version, minor_version, patch_version = version.split('.')

try:
    int(patch_version)
except ValueError:
    # Non-numerical patch version; add committer date
    os.environ['TZ'] = 'UTC'
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
    ).stdout.decode('utf-8').strip()
    patch_version += '.' + committer_date

print(f'{major_version}.{minor_version}.{patch_version}')
