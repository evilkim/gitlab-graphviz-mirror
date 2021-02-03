import json
import subprocess

def test_json_node_order():
    '''
    test that nodes appear in JSON output in the same order as they were input
    '''

    # a simple graph with some nodes
    input = 'digraph G {\n'             \
            '  ordering=out;\n'         \
            '  {rank=same;"1";"2"};\n'  \
            '  "1"->"2";\n'             \
            '  {rank=same;"4";"5";};\n' \
            '  "4"->"5";\n'             \
            '  "7"->"5";\n'             \
            '  "7"->"4";\n'             \
            '  "6"->"1";\n'             \
            '  "3"->"6";\n'             \
            '  "6"->"4";\n'             \
            '  "3"->"8";\n'             \
            '}'

    # turn it into JSON
    output = subprocess.check_output(['dot', '-Tjson'], input=input,
      universal_newlines=True)

    # parse this into a data structure we can inspect
    data = json.loads(output)

    # extract the nodes, filtering out subgraphs
    nodes = [n['name'] for n in data['objects'] if 'nodes' not in n]

    # the nodes should appear in the order in which they were seen in the input
    assert nodes == ['1', '2', '4', '5', '7', '6', '3', '8']

def test_json_edge_order():
    '''
    test that edges appear in JSON output in the same order as they were input
    '''

    # a simple graph with some edges
    input = 'digraph G {\n'             \
            '  ordering=out;\n'         \
            '  {rank=same;"1";"2"};\n'  \
            '  "1"->"2";\n'             \
            '  {rank=same;"4";"5";};\n' \
            '  "4"->"5";\n'             \
            '  "7"->"5";\n'             \
            '  "7"->"4";\n'             \
            '  "6"->"1";\n'             \
            '  "3"->"6";\n'             \
            '  "6"->"4";\n'             \
            '  "3"->"8";\n'             \
            '}'

    # turn it into JSON
    output = subprocess.check_output(['dot', '-Tjson'], input=input,
      universal_newlines=True)

    # parse this into a data structure we can inspect
    data = json.loads(output)

    # the edges should appear in the order in which they were seen in the input
    expected = [('1', '2'), ('4' ,'5'), ('7', '5'), ('7', '4'), ('6', '1'),
                ('3', '6'), ('6', '4'), ('3', '8')]
    edges = [(data['objects'][e['tail']]['name'],
              data['objects'][e['head']]['name']) for e in data['edges']]
    assert edges == expected
