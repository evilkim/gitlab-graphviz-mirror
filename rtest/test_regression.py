import pytest
import platform
import subprocess
import os
import re

# The terminology used in rtest.sh is a little inconsistent. At the
# end it reports the total number of tests, the number of "failures"
# (crashes) and the number of "changes" (which is the number of tests
# where the output file did not match the reference file). However,
# for each test that detects "changes", it prints an error message
# saying "== Failed ==" which thus is not counted as a failure at the
# end.

# First run a subset of all the tests that produces equal output files
# for all platforms and fail the test if there are differences.

def test_regression_subset_differences():
    os.chdir(os.path.dirname(os.path.realpath(__file__)))
    result = subprocess.Popen(['python3', 'rtest.py', 'tests_subset.txt'], stderr=subprocess.PIPE)
    text = result.communicate()[1]
    print(text)
    assert result.returncode == 0

# Secondly, run all tests but ignore differences and fail the test
# only if there is a crash. This will leave the differences for png
# output in rtest/nhtml/index.html for review.

def test_regression_failure():
    os.chdir(os.path.dirname(os.path.realpath(__file__)))
    result = subprocess.Popen(['python3', 'rtest.py'], stderr=subprocess.PIPE)
    text = result.communicate()[1]
    print(text)
    assert "Layout failures: 0" in str(text)
# FIXME: re-enable when all tests pass on all platforms
#    assert result.returncode == 0

def test_165():
    '''
    dot should be able to produce properly escaped xdot output
    https://gitlab.com/graphviz/graphviz/-/issues/165
    '''

    # locate our associated test case in this directory
    input = os.path.join(os.path.dirname(__file__), '165.dot')
    assert os.path.exists(input), 'unexpectedly missing test case'

    # ask Graphviz to translate it to xdot
    output = subprocess.check_output(['dot', '-Txdot', input],
      universal_newlines=True)

    # find the line containing the _ldraw_ attribute
    ldraw = re.search(r'^\s*_ldraw_\s*=(?P<value>.*?)$', output, re.MULTILINE)
    assert ldraw is not None, 'no _ldraw_ attribute in graph'

    # this should contain the label correctly escaped
    assert r'hello \\\" world' in ldraw.group('value'), \
      'unexpected ldraw contents'

def test_165_2():
    '''
    variant of test_165() that checks a similar problem for edges
    https://gitlab.com/graphviz/graphviz/-/issues/165
    '''

    # locate our associated test case in this directory
    input = os.path.join(os.path.dirname(__file__), '165_2.dot')
    assert os.path.exists(input), 'unexpectedly missing test case'

    # ask Graphviz to translate it to xdot
    output = subprocess.check_output(['dot', '-Txdot', input],
      universal_newlines=True)

    # find the lines containing _ldraw_ attributes
    ldraw = re.findall(r'^\s*_ldraw_\s*=(.*?)$', output, re.MULTILINE)
    assert ldraw is not None, 'no _ldraw_ attributes in graph'

    # one of these should contain the label correctly escaped
    assert any(r'hello \\\" world' in l for l in ldraw), \
      'unexpected ldraw contents'

def test_165_3():
    '''
    variant of test_165() that checks a similar problem for graph labels
    https://gitlab.com/graphviz/graphviz/-/issues/165
    '''

    # locate our associated test case in this directory
    input = os.path.join(os.path.dirname(__file__), '165_3.dot')
    assert os.path.exists(input), 'unexpectedly missing test case'

    # ask Graphviz to translate it to xdot
    output = subprocess.check_output(['dot', '-Txdot', input],
      universal_newlines=True)

    # find the lines containing _ldraw_ attributes
    ldraw = re.findall(r'^\s*_ldraw_\s*=(.*?)$', output, re.MULTILINE)
    assert ldraw is not None, 'no _ldraw_ attributes in graph'

    # one of these should contain the label correctly escaped
    assert any(r'hello \\\" world' in l for l in ldraw), \
      'unexpected ldraw contents'

def test_1436():
    '''
    test a segfault from https://gitlab.com/graphviz/graphviz/-/issues/1436 has
    not reappeared
    '''

    # locate our associated test case in this directory
    input = os.path.join(os.path.dirname(__file__), '1436.dot')
    assert os.path.exists(input), 'unexpectedly missing test case'

    # ask Graphviz to process it, which should generate a segfault if this bug
    # has been reintroduced
    subprocess.check_call(['dot', '-Tsvg', '-o', os.devnull, input])

def test_1449():
    '''
    using the SVG color scheme should not cause warnings
    https://gitlab.com/graphviz/graphviz/-/issues/1449
    '''

    # start Graphviz
    p = subprocess.Popen(['dot', '-Tsvg', '-o', os.devnull],
      stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
      universal_newlines=True)

    # pass it some input that uses the SVG color scheme
    stdout, stderr = p.communicate('graph g { colorscheme="svg"; }')

    assert p.returncode == 0, 'Graphviz exited with non-zero status'

    assert stderr.strip() == '', 'SVG color scheme use caused warnings'

# FIXME: Remove skip when
# https://gitlab.com/graphviz/graphviz/-/issues/1753 is fixed
@pytest.mark.skipif(
    os.environ.get('build_system') == 'cmake',
    reason='The Windows "CMake" installer does not install gvpr (#1753)'
)
def test_1594():
    '''
    GVPR should give accurate line numbers in error messages
    https://gitlab.com/graphviz/graphviz/-/issues/1594
    '''

    # locate our associated test case in this directory
# FIXME: remove cwd workaround when
# https://gitlab.com/graphviz/graphviz/-/issues/1780 is fixed
#    input = os.path.join(os.path.dirname(__file__), '1594.gvpr')
    input = '1594.gvpr'

    # run GVPR with our (malformed) input program
    p = subprocess.Popen(['gvpr', '-f', input], stdin=subprocess.PIPE,
      cwd=os.path.join(os.path.dirname(__file__)),
      stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
    _, stderr = p.communicate()

    assert p.returncode != 0, 'GVPR did not reject malformed program'

    assert 'line 3:' in stderr, \
      'GVPR did not identify correct line of syntax error'
