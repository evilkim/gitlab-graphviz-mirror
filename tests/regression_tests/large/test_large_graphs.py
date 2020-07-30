import pytest

import subprocess
import os
import sys
import platform


class TestLargeGraphs():
	@pytest.mark.skipif(platform.system() == 'Windows' and platform.architecture()[0] == '32bit', reason='https://gitlab.com/graphviz/graphviz/-/issues/1710')
	def test_long_chain(self):
		"""
		This test will fail on 32bit Windows machine if compiled with stack size < 16MB.
		long_chain input file generated using code below:		
			from graphviz import Digraph

			graph = Digraph(format='svg')

			prev = "start"
			graph.node("start", label="start")

			for i in range(33000):
				new_node = str(i)    
				graph.node(new_node, label=new_node, shape='rectangle')
				
				graph.edge(prev, new_node)
				
				prev = new_node

			graph.render("long_chain")
		"""
		subprocess.check_call([
			'dot', '-Tsvg', '-O', os.devnull
		])
