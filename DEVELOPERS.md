# Developer's Guide

## Table of contents

[[_TOC_]]

## How to make a release

### A note about the examples below

The examples below are for the 2.44.1 release. Modify the version
number for the actual release.

### Using a fork or a clone of the original repo

The instructions below can be used from a fork (recommended) or from a
clone of the master repo.

### Deciding the release version number

This project adheres to
[Semantic Versioning](https://semver.org/spec/v2.0.0.html).
Before making the release, it must be decided if it is a *major*, *minor* or
*patch* release.

#### Stable release versions and development versions numbering convention

See gen-version.py.

### Instructions

1. Check that the
[master pipeline](https://gitlab.com/graphviz/graphviz/-/pipelines?ref=master)
is green

1. Create a local branch and name it e.g. `stable-release-<version>`

   Example: `stable-release-2.44.1`

1. Edit `gen_version.py` according to instructions in that file.

1. Edit `windows/include/builddate.h` (if https://gitlab.com/graphviz/graphviz/-/issues/1745 isn't fixed)

   Set version, date and time in UTC. Time will be approximate and the
   minutes part should be `00` to not indicate any exactness.

   Example (from
   https://gitlab.com/graphviz/graphviz/-/commit/5e0d3b1841b7e358274c916b52276d251eabef3d#2dcbe62b02ff1b46c3e5dc995a0a86f993cb6eca):

    ```diff
    -#define BUILDDATE "20090106.0545"
    +#define BUILDDATE "20200629.0800"
    ```

1. Edit `windows/include/config.h` (if https://gitlab.com/graphviz/graphviz/-/issues/1745 isn't fixed)

   Set version, date and time in UTC. Time will be approximate and the
   minutes part should be `00` to not indicate any exactness.

   Example (from
   https://gitlab.com/graphviz/graphviz/-/commit/5e0d3b1841b7e358274c916b52276d251eabef3d#5299703d1b79f96afe7c4bf2fc8bd368da39d5e2):

    ```diff
     /* Define to the full name and version of this package. */
    -#define PACKAGE_STRING "graphviz 2.39.20160612.1140"
    +#define PACKAGE_STRING "graphviz 2.44.1 (20200629.0800)" /* 08:00
    is approximate */
    ```
    ```diff
     /* Define to the version of this package. */
    -#define PACKAGE_VERSION "2.39.20160612.1140"
    +#define PACKAGE_VERSION "2.44.1"
    ```

1. Edit `CHANGELOG.md`

    Add the new version between `[Unreleased]` and the previous
    version.

    At the bottom of the file, add an entry for the new version. These
    entries are not visible in the rendered page, but are essential
    for the version links to the GitLab commit comparisons to work.

    Example (from
    https://gitlab.com/graphviz/graphviz/-/commit/5e0d3b1841b7e358274c916b52276d251eabef3d#ab09011fa121d0a2bb9fa4ca76094f2482b902b7):

    ```diff
     ## [Unreleased]
     
    +## [2.44.1] - 2020-06-29
    +
    ```
    ```diff
    -[Unreleased]: https://gitlab.com/graphviz/graphviz/compare/2.44.0...master
    +[Unreleased]: https://gitlab.com/graphviz/graphviz/compare/2.44.1...master
    +[2.44.1]: https://gitlab.com/graphviz/graphviz/compare/2.44.0...2.44.1
     [2.44.0]: https://gitlab.com/graphviz/graphviz/compare/2.42.4...2.44.0
     [2.42.4]: https://gitlab.com/graphviz/graphviz/compare/2.42.3...2.42.4
     [2.42.3]: https://gitlab.com/graphviz/graphviz/compare/2.42.2...2.42.3
    ```

1. Commit:

   `git add -p`

   `git commit -m "Stable Release 2.44.1"`

1. Push:

   Example: `git push origin stable-release-2.44.1`

1. Wait until the pipeline has run for your branch and check that it's green

1. Create a merge request

1. Merge the merge request

1. Wait for the
[master pipeline](https://gitlab.com/graphviz/graphviz/-/pipelines?ref=master)
  to run for the new commit and check that it's green

1. Create a release at [GitHub releases](https://gitlab.com/graphviz/graphviz/-/releases)

   Fill in the `Tag name`, `Message` and `Release notes` fields. The
   `Tag name` shall be a pure numeric new tag on the form
   `<major>.<minor>.<patch>`. The `Message` field shall have the text
   `Stable Release <major>.<minor>.<patch>`. The `Release Notes` field
   shall have the text `See the [CHANGELOG.md](<link to version in
   CHANGELOG.md>)`.

   Example:
   * **Tag name:** `2.44.1`
   * **Message:** `Stable Release 2.44.1`
   * **Release notes:** `See the [CHANGELOG](https://gitlab.com/graphviz/graphviz/-/blob/master/CHANGELOG.md#2441-2020-06-29).`

1. Create a new local branch and name it e.g. `return-to-<version>-dev`

   Example: `return-to-2.45-dev`

1. Edit `gen_version.py` again according to instructions in that file.

1. Edit `windows/include/builddate.h` again (if https://gitlab.com/graphviz/graphviz/-/issues/1745 isn't fixed)

    Set version, date and time. Date and time will be fixed for every
    build and everything except the year should be question marks (`?´)
    to not indicate any exactness.

    Example (from
    https://gitlab.com/graphviz/graphviz/-/commit/2bd021b3ef38ddcc7e7f9445f26026fadf441a52#2dcbe62b02ff1b46c3e5dc995a0a86f993cb6eca):

    ```diff
     /* Define to the full name and version of this package. */
    -#define PACKAGE_STRING "graphviz 2.44.1 (20200629.0800)" /* 08:00 is approximate */
    +#define PACKAGE_STRING "graphviz 2.45.2020????.???? (2020????.????)" /* FIXME: https://gitlab.com/graphviz/graphviz/-/issues/1745 */
    ```
    ```diff
     /* Define to the version of this package. */
    -#define PACKAGE_VERSION "2.44.1"
    +#define PACKAGE_VERSION "2.45.2020????.????" /* FIXME: https://gitlab.com/graphviz/graphviz/-/issues/1745 */
    ```

1. Edit `windows/include/config.h` again (if https://gitlab.com/graphviz/graphviz/-/issues/1745 isn't fixed)

    Set version, date and time. Date and time will be fixed for every
    build and everything except the year should be question marks
    (`?´) to not indicate any exactness.

    Example (from
    https://gitlab.com/graphviz/graphviz/-/commit/2bd021b3ef38ddcc7e7f9445f26026fadf441a52#5299703d1b79f96afe7c4bf2fc8bd368da39d5e2)

    ```diff
    -#define BUILDDATE "20200629.0800"
    +#define BUILDDATE "2020????.????"  /* FIXME: https://gitlab.com/graphviz/graphviz/-/issues/1745 */
    ```

1. Commit:

    `git add -p`

    If patch version was incremented:

    Example: `git commit -m "Move back to 2.45 development series"`

    else (if major or minor version was incremented):

    Example: `git commit -m "Start 2.47 development series"`

1. Push:

   Example: `git push origin return-to-2.45-dev`

1. Wait until the pipeline has run for your new branch and check that it's green

1. Create a merge request

1. Merge the merge request

## TODO with this guide

* Update with new example commits after next stable release.

## How to update this guide

### Markdown flavor used

This guide uses
[GitLab Flavored Markdown (GFM)](https://docs.gitlab.com/ce/user/markdown.html#gitlab-flavored-markdown-gfm]).

### Rendering this guide locally

This guide can be rendered locally with e.g. [Pandoc](https://pandoc.org/):

`pandoc DEVELOPERS.md --from=gfm -t html -o DEVELOPERS.html`

### Linting this guide locally

[markdownlint-cli](https://github.com/igorshubovych/markdownlint-cli)
can be used to lint it.
