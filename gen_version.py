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

version = '2.47.1'

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
parser.add_argument('--major',
                    dest='component',
                    action='store_const',
                    const='major',
                    help='Print major version')
parser.add_argument('--minor',
                    dest='component',
                    action='store_const',
                    const='minor',
                    help='Print minor version')
parser.add_argument('--patch',
                    dest='component',
                    action='store_const',
                    const='patch',
                    help='Print patch version')
parser.add_argument('--definition',
                    action='store_true',
                    help='Print a C-style preprocessor #define')

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

if not patch_version.isnumeric():
    # Non-numerical patch version; add committer date
    patch_version += '.' + committer_date

if args.date_format:
    if args.definition:
        print(f'#define BUILDDATE "{committer_date}"')
    else:
        print(committer_date)
elif args.collection:
    if args.definition:
        print(f'#define COLLECTION "{collection}"')
    else:
        print(collection)
elif args.component == 'major':
    if args.definition:
        print(f'#define VERSION_MAJOR "{major_version}"')
    else:
        print(major_version)
elif args.component == 'minor':
    if args.definition:
        print(f'#define VERSION_MINOR "{minor_version}"')
    else:
        print(minor_version)
elif args.component == 'patch':
    if args.definition:
        print(f'#define VERSION_PATCH "{patch_version}"')
    else:
        print(patch_version)
else:
    if args.definition:
        print(f'#define VERSION "{major_version}.{minor_version}.'
              f'{patch_version}"')
    else:
        print(f'{major_version}.{minor_version}.{patch_version}')
