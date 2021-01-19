#!/usr/bin/env python3

# Generate version
#
# Release version entry format : <major>.<minor>.<patch>
#
# Stable release version output format     : <major>.<minor>.<patch>
# Development release version output format: <major>.<minor>.<patch>~dev.<YYYYmmdd.HHMM>

# The patch version of a development release should be the same as the
# next stable release patch number. The string "~dev." and the
# committer date will be added.
#
# Example sequence:
#
# Entry version   Entry collection          Output
# 2.44.1          stable                 => 2.44.1
# 2.44.2          development            => 2.44.2~dev.20200704.1652
# 2.44.2          stable                 => 2.44.2
# 2.44.3          development            => 2.44.3~dev.20200824.1337

collection = 'development'

version = '2.46.1'

import os
import sys
import subprocess
import argparse

from datetime import datetime

graphviz_date_format = '%Y%m%d.%H%M'
iso_date_format = '%Y-%m-%d %H:%M:%S'

parser = argparse.ArgumentParser(description='Generate Graphviz version.')
parser.add_argument('--committer-date-iso',
                    dest='date_format',
                    action='store_const',
                    const=iso_date_format,
                    help='Print ISO formatted committer date in UTC instead of version'
)
parser.add_argument('--committer-date-graphviz',
                    dest='date_format',
                    action='store_const',
                    const=graphviz_date_format,
                    help='Print graphviz special formatted committer date in UTC '
                    'instead of version'
)
parser.add_argument('--collection',
                    action='store_true',
                    help='Print collection ("stable" or "development") '
                    'instead of version'
)

args = parser.parse_args()

date_format = args.date_format or graphviz_date_format

assert collection in ('stable', 'development'), \
    'The collection is not "stable" or "development"'
assert len(version.split('.')) == 3, 'Wrong number of version elements'
assert all(part.isnumeric() for part in version.split('.')), \
    'All version elements are not numeric'

if collection == 'development':
    version += '~dev'

major_version, minor_version, patch_version = version.split('.')

if not patch_version.isnumeric() or args.date_format:
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
                cwd=os.path.abspath(os.path.dirname(__file__)),
                universal_newlines=True,
            ).strip(),
            '%Y-%m-%d %H:%M:%S',
        ).strftime(date_format)
    except (subprocess.CalledProcessError, FileNotFoundError):
        print('Warning: build not started in a Git clone, or Git is not installed: setting version date to 0.', file=sys.stderr)
        committer_date = '0'

if args.date_format:
    print(committer_date)
elif args.collection:
    print(collection)
else:
    if not patch_version.isnumeric():
        # Non-numerical patch version; add committer date
        patch_version += '.' + committer_date
    print('{0}.{1}.{2}'.format(major_version, minor_version, patch_version))
