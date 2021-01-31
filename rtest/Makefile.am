SUBDIRS = graphs linux.x86

check test rtest: $(top_builddir)/cmd/dot/dot_builtins $(top_builddir)/contrib/diffimg/diffimg
	DOT=$(top_builddir)/cmd/dot/dot_builtins DIFFIMG=$(top_builddir)/contrib/diffimg/diffimg $(top_srcdir)/rtest/rtest.py

EXTRA_DIST = graphs nshare rtest.py strps.awk tests.txt tests_subset.txt test_regression.py
