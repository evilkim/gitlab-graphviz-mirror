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
import argparse

from datetime import datetime

parser = argparse.ArgumentParser(description='Generate Graphviz version.')
parser.add_argument('--committer-date-iso',
                    action='store_true',
                    help='Print ISO formatted committer date in UTC instead of version'
)

args = parser.parse_args()

major_version, minor_version, patch_version = version.split('.')

if not patch_version.isnumeric() or args.committer_date_iso:
    os.environ['TZ'] = 'UTC'
    try:
        committer_date = datetime.strptime(
            subprocess.check_output(
                [
                    'git',
                    'log',
                    '-n',
                    '1',
                    '--format=%cd',
                    '--date=format-local:%Y-%m-%d %H:%M:%S'
                ],
                universal_newlines=True,
            ).strip(),
            '%Y-%m-%d %H:%M:%S',
        )
    except (subprocess.CalledProcessError, FileNotFoundError):
        print('Warning: build not started in a Git clone, or Git is not installed: setting version date to 0.', file=sys.stderr)

        committer_date = '0'

if args.committer_date_iso:
    print(committer_date)
else:
    if not patch_version.isnumeric():
        # Non-numerical patch version; add committer date
        patch_version += '.' + committer_date.strftime('%Y%m%d.%H%M')
    print('{0}.{1}.{2}'.format(major_version, minor_version, patch_version))
