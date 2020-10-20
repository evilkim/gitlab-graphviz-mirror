import pytest
import platform
import signal
import subprocess
import os
import re
import tempfile

# The terminology used in rtest.py is a little inconsistent. At the
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
    subprocess.check_call(['python3', 'rtest.py', 'tests_subset.txt'])

# Secondly, run all tests but ignore differences and fail the test
# only if there is a crash. This will leave the differences for png
# output in rtest/nhtml/index.html for review.

def test_regression_failure():
    os.chdir(os.path.dirname(os.path.realpath(__file__)))
    result = subprocess.Popen(['python3', 'rtest.py'], stderr=subprocess.PIPE,
                              universal_newlines=True)
    text = result.communicate()[1]
    print(text)
    assert "Layout failures: 0" in text
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

def test_1314():
    '''
    test that a large font size that produces an overflow in Pango is rejected
    https://gitlab.com/graphviz/graphviz/-/issues/1314
    '''

    # locate our associated test case in this directory
    input = os.path.join(os.path.dirname(__file__), '1314.dot')
    assert os.path.exists(input), 'unexpectedly missing test case'

    # ask Graphviz to process it, which should fail
    try:
      subprocess.check_call(['dot', '-Tsvg', '-o', os.devnull, input])
    except subprocess.CalledProcessError:
      return

    # the execution did not fail as expected
    pytest.fail('dot incorrectly exited with success')

def test_1411():
    '''
    parsing strings containing newlines should not disrupt line number tracking
    https://gitlab.com/graphviz/graphviz/-/issues/1411
    '''

    # locate our associated test case in this directory
    input = os.path.join(os.path.dirname(__file__), '1411.dot')
    assert os.path.exists(input), 'unexpectedly missing test case'

    # process it with Graphviz (should fail)
    p = subprocess.Popen(['dot', '-Tsvg', '-o', os.devnull, input],
      stderr=subprocess.PIPE, universal_newlines=True)
    _, output = p.communicate()

    assert p.returncode != 0, 'Graphviz accepted broken input'

    assert 'syntax error in line 17 near \'\\\'' in output, \
      'error message did not identify correct location'

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

def test_1444():
    '''
    specifying 'headport' as an edge attribute should work regardless of what
    order attributes appear in
    https://gitlab.com/graphviz/graphviz/-/issues/1444
    '''

    # locate the first of our associated tests
    input1 = os.path.join(os.path.dirname(__file__), '1444.dot')
    assert os.path.exists(input1), 'unexpectedly missing test case'

    # ask Graphviz to process it
    p = subprocess.Popen(['dot', '-Tsvg', input1], stdout=subprocess.PIPE,
      stderr=subprocess.PIPE, universal_newlines=True)
    stdout1, stderr = p.communicate()

    assert p.returncode == 0, 'failed to process a headport edge'

    assert stderr.strip() == '', 'emitted an error for a legal graph'

    # now locate our second variant, that simply has the attributes swapped
    input2 = os.path.join(os.path.dirname(__file__), '1444-2.dot')
    assert os.path.exists(input2), 'unexpectedly missing test case'

    # process it identically
    p = subprocess.Popen(['dot', '-Tsvg', input2], stdout=subprocess.PIPE,
      stderr=subprocess.PIPE, universal_newlines=True)
    stdout2, stderr = p.communicate()

    assert p.returncode == 0, 'failed to process a headport edge'

    assert stderr.strip() == '', 'emitted an error for a legal graph'

    assert stdout1 == stdout2, \
      'swapping edge attributes altered the output graph'

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

def test_1676():
    '''
    https://gitlab.com/graphviz/graphviz/-/issues/1676
    '''

    # locate our associated test case in this directory
    input = os.path.join(os.path.dirname(__file__), '1676.dot')
    assert os.path.exists(input), 'unexpectedly missing test case'

    # run Graphviz with this input
    ret = subprocess.call(['dot', '-Tsvg', '-o', os.devnull, input])

    # this malformed input should not have caused Graphviz to crash
    assert ret != -signal.SIGSEGV, 'Graphviz segfaulted'

def test_1724():
    '''
    passing malformed node and newrank should not cause segfaults
    https://gitlab.com/graphviz/graphviz/-/issues/1724
    '''

    # locate our associated test case in this directory
    input = os.path.join(os.path.dirname(__file__), '1724.dot')
    assert os.path.exists(input), 'unexpectedly missing test case'

    # run Graphviz with this input
    ret = subprocess.call(['dot', '-Tsvg', '-o', os.devnull, input])

    assert ret != -signal.SIGSEGV, 'Graphviz segfaulted'

def test_1767():
    '''
    using the Pango plugin multiple times should produce consistent results
    https://gitlab.com/graphviz/graphviz/-/issues/1767
    '''

    # FIXME: Remove skip when
    # https://gitlab.com/graphviz/graphviz/-/issues/1777 is fixed
    if os.getenv('build_system') == 'msbuild':
      pytest.skip('Windows MSBuild release does not contain any header files (#1777)')

    # find co-located test source
    c_src = os.path.abspath(os.path.join(os.path.dirname(__file__), '1767.c'))
    assert os.path.exists(c_src), 'missing test case'

    # create some scratch space to work in
    with tempfile.TemporaryDirectory() as tmp:

      # compile our test code
      exe = os.path.join(tmp, 'a.exe')
      rt_lib_option = '-MDd' if os.environ.get('configuration') == 'Debug' else '-MD'

      if platform.system() == 'Windows':
          subprocess.check_call(['cl', c_src, '-Fe:', exe, '-nologo',
            rt_lib_option, '-link', 'cgraph.lib', 'gvc.lib'])
      else:
          cc = os.environ.get('CC', 'cc')
          subprocess.check_call([cc, c_src, '-o', exe, '-lcgraph', '-lgvc'])

      # find our co-located dot input
      dot = os.path.abspath(os.path.join(os.path.dirname(__file__), '1767.dot'))
      assert os.path.exists(dot), 'missing test case'

      # run the test
      stdout = subprocess.check_output([exe, dot], universal_newlines=True)

      assert stdout == 'Loaded graph:clusters\n' \
                       'cluster_0 contains 5 nodes\n' \
                       'cluster_1 contains 1 nodes\n' \
                       'cluster_2 contains 3 nodes\n' \
                       'cluster_3 contains 3 nodes\n' \
                       'Loaded graph:clusters\n' \
                       'cluster_0 contains 5 nodes\n' \
                       'cluster_1 contains 1 nodes\n' \
                       'cluster_2 contains 3 nodes\n' \
                       'cluster_3 contains 3 nodes\n'

def test_1783():
    '''
    Graphviz should not segfault when passed large edge weights
    https://gitlab.com/graphviz/graphviz/-/issues/1783
    '''

    # locate our associated test case in this directory
    input = os.path.join(os.path.dirname(__file__), '1783.dot')
    assert os.path.exists(input), 'unexpectedly missing test case'

    # run Graphviz with this input
    ret = subprocess.call(['dot', '-Tsvg', '-o', os.devnull, input])

    assert ret != 0, 'Graphviz accepted illegal edge weight'

    assert ret != -signal.SIGSEGV, 'Graphviz segfaulted'

# FIXME: Remove skip when
# https://gitlab.com/graphviz/graphviz/-/issues/1857 and
# https://gitlab.com/graphviz/graphviz/-/issues/1816 is fixed
@pytest.mark.skipif(
    os.environ.get('build_system') == 'cmake' or
    platform.system() == 'Darwin' or
    platform.system() == 'Windows',
    reason='gvedit is not built for macOS or Windows or using CMake (#1816 & #1857)'
)
def test_1813():
    '''
    gvedit -? should show usage
    https://gitlab.com/graphviz/graphviz/-/issues/1813
    '''

    environ_copy = os.environ.copy()
    environ_copy.pop('DISPLAY', None)
    output = subprocess.check_output(['gvedit', '-?'],
      env=environ_copy,
      universal_newlines=True)

    assert 'Usage' in output, 'gvedit -? did not show usage'

# FIXME: Remove skip when
# https://gitlab.com/graphviz/graphviz/-/issues/1753 is fixed
@pytest.mark.skipif(
    os.environ.get('build_system') == 'cmake',
    reason='The Windows "CMake" installer does not install lefty (#1753)'
)
def test_1818():
    '''
    lefty -? should show usage
    https://gitlab.com/graphviz/graphviz/-/issues/1813
    '''

    output = subprocess.check_output(['lefty', '-?'],
      stderr=subprocess.STDOUT,
      universal_newlines=True)

    assert 'Usage' in output, 'lefty -? did not show usage'

def test_1845():
    '''
    rendering sequential graphs to PS should not segfault
    https://gitlab.com/graphviz/graphviz/-/issues/1845
    '''

    # locate our associated test case in this directory
    input = os.path.join(os.path.dirname(__file__), '1845.dot')
    assert os.path.exists(input), 'unexpectedly missing test case'

    # generate a multipage PS file from this input
    subprocess.check_call(['dot', '-Tps', '-o', os.devnull, input])
