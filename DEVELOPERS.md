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

See [`gen_version.py`](https://gitlab.com/graphviz/graphviz/-/blob/master/gen_version.py).

### Instructions

1. Check that the
[master pipeline](https://gitlab.com/graphviz/graphviz/-/pipelines?ref=master)
is green

1. Create a local branch and name it e.g. `stable-release-<version>`

   Example: `stable-release-2.44.1`

1. Edit `gen_version.py` according to instructions in that file.

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

1. The “deployment” CI task will automatically create a release on the
   [Gitlab releases tab](https://gitlab.com/graphviz/graphviz/-/releases). If a
   release is not created, check that your modifications to `gen_version.py`
   correctly set a version conforming to the regular expression `\d+\.\d+\.\d+`.
   The “deployment” CI task will also create a Git tag for the version, e.g.
   `2.44.1`.

1. Create a new local branch and name it e.g. `return-to-<version>-dev`

   Example: `return-to-2.45-dev`

1. Edit `gen_version.py` again according to instructions in that file.

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

## Performance and profiling

The runtime and memory usage of Graphviz is dependent on the user’s graph. It is
easy to create “expensive” graphs without realizing it using only moderately
sized input. For this reason, users regularly encounter performance bottlenecks
that they need help with. This situation is likely to persist even with hardware
advances, as the size and complexity of the graphs users construct will expand
as well.

This section documents how to build performance-optimized Graphviz binaries and
how to identify performance bottlenecks. Note that this information assumes you
are working in a Linux environment.

### Building an optimized Graphviz

The first step to getting an optimized build is to make sure you are using a
recent compiler. If you have not upgraded your C and C++ compilers for a while,
make sure you do this first.

The simplest way to change flags used during compilation is by setting the
`CFLAGS` and `CXXFLAGS` environment variables:

```sh
env CFLAGS="..." CXXFLAGS="..." ./configure
```

You should use the maximum optimization level for your compiler. E.g. `-O3` for
GCC and Clang. If your toolchain supports it, it is recommended to also enable
link-time optimization (`-flto`).

You can further optimize compilation for the machine you are building on with
`-march=native -mtune=native`. Be aware that the resulting binaries will no
longer be portable (may not run if copied to another machine). These flags are
also not recommended if you are debugging a user issue, because you will end up
profiling and optimizing different code than what may execute on their machine.

Most profilers need a symbol table and/or debugging metadata to give you useful
feedback. You can enable this on GCC and Clang with `-g`.

Putting this all together:

```sh
env CFLAGS="-O3 -flto -march=native -mtune=native -g" \
  CXXFLAGS="-O3 -flto -march=native -mtune=native -g" ./configure
```

### Profiling

#### [Callgrind](https://valgrind.org/docs/manual/cl-manual.html)

Callgrind is a tool of [Valgrind](https://valgrind.org/) that can measure how
many times a function is called and how expensive the execution of a function is
compared to overall runtime. When you have built an optimized binary according
to the above instructions, you can run it under Callgrind:

```sh
valgrind --tool=callgrind dot -Tsvg test.dot
```

This will produce a file like callgrind.out.2534 in the current directory. You
can use [Kcachegrind](https://kcachegrind.github.io/) to view the results by
running it in the same directory with no arguments:

```sh
kcachegrind
```

If you have multiple trace results in the current directory, Kcachegrind will
load all of them and even let you compare them to each other. See the
Kcachegrind documentation for more information about how to use this tool.

Be aware that execution under Callgrind will be a lot slower than a normal run.
If you need to see instruction-level execution costs, you can pass
`--dump-instr=yes` to Valgrind, but this will further slow execution and is
usually not necessary. To profile with less overhead, you can use a statistical
profiler like Linux Perf.

#### [Linux Perf](https://perf.wiki.kernel.org/index.php/Main_Page)

TODO

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
