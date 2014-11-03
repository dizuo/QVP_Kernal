#ifndef GRAPH_ALGORITHMS_HXX
# define GRAPH_ALGORITHMS_HXX

#include <queue>
#include <stack>

using namespace std;

namespace graph
{
	/** Return all node with no predecessor */
	template <class NodeLabel>
	list< Node<NodeLabel>* > Graph<NodeLabel>::get_roots()
	{
		t_list_nodes	res; res.clear();

		for (typename map< t_nodelabel, t_node* >::const_iterator i = _nodes.begin();
			i != _nodes.end(); i++)
		{
			t_node *node = i->second;
			if (node->get_predecessors().size() == 0)
				res.push_back(node);
		}
		return res;
	}

	/** topological sort, give the result in good way
	** at first, push nodes which don't have predecessors
	** then visit others nodes and mark them when they're visited
	*/
	template <class NodeLabel>
	list< Node<NodeLabel>* > Graph<NodeLabel>::topological_sort()
	{
		map<t_node*, size_t> nodeInDegree;	nodeInDegree.clear();
		stack<t_node*> nodeStack;	
		list< Node<NodeLabel>* > res; res.clear();

		/*compute each node's in degree*/
		for (typename map< t_nodelabel, t_node* >::const_iterator iter = _nodes.begin();
			iter != _nodes.end(); 
			iter++)
		{
			t_node* node = iter->second;
			size_t indegree = node->get_predecessors().size();

			nodeInDegree[node] = indegree;
			if (0==indegree)
				nodeStack.push(node);
		}

		/* compute */
		while (!nodeStack.empty())
		{
			t_node *node = nodeStack.top();
			nodeStack.pop();
			
			res.push_back(node);		//push the node to result list

			/* treat the node's all successor */
			t_list_nodes	succes = node->get_successors();
			for (t_list_nodes_citer i = succes.begin(); i != succes.end(); i++)
			{
				if ( !(--nodeInDegree[*i]) )
					nodeStack.push(*i);
			}// for
		}// while
		
		if ( res.size() < _nodes.size() )	// detect cycle !_!
			res.clear();

		return res;
	}

	/** detect cycle
	** use the topological_sort method to detect cycle
	*/
	template <class NodeLabel>
	bool		Graph<NodeLabel>::detect_cycle()
	{
		map<t_node*, size_t> nodeInDegree;	nodeInDegree.clear();
		stack<t_node*> nodeStack;	
		list< Node<NodeLabel>* > res; res.clear();

		/*compute each node's in degree*/
		for (typename map< t_nodelabel, t_node* >::const_iterator iter = _nodes.begin();
			iter != _nodes.end(); 
			iter++)
		{
			t_node* node = iter->second;
			size_t indegree = node->get_predecessors().size();

			nodeInDegree[node] = indegree;
			if (0==indegree)
				nodeStack.push(node);
		}

		/* compute */
		while (!nodeStack.empty())
		{
			t_node *node = nodeStack.top();
			nodeStack.pop();

			res.push_back(node);		//push the node to result list

			/* treat the node's all successor */
			t_list_nodes	succes = node->get_successors();
			for (t_list_nodes_citer i = succes.begin(); i != succes.end(); i++)
			{
				if ( !(--nodeInDegree[*i]) )
					nodeStack.push(*i);
			}// for
		}// while

		if ( res.size() < _nodes.size() )	// detect circle !_!
			return true;

		return false;
	}

	/** \brief test if all node are connected in same component
	** considere the graph like a undirected graph, if algorithm
	** can find only one component, return true, false else.
	*/
	template <class NodeLabel>
	bool		Graph<NodeLabel>::one_component()
	{
		if (_nodes.size() == 0)
			return false;

		set< t_node* >	visited; visited.clear();
		queue<t_node *>	Q;
		t_node *node = _nodes.begin()->second;

		Q.push(node);
		visited.insert(node);
		while (!Q.empty())
		{
			t_node *node = Q.front();
			Q.pop();

			t_list_nodes	preds = node->get_predecessors();
			for (t_list_nodes_citer i = preds.begin();
				i != preds.end(); i++)
				if (visited.find(*i) == visited.end())
				{
					visited.insert(*i);
					Q.push(*i);
				}

				t_list_nodes succs = node->get_successors();
				for (t_list_nodes_citer i = succs.begin();
					i != succs.end(); i++)
					if (visited.find(*i) == visited.end())
					{
						visited.insert(*i);
						Q.push(*i);
					}
		}
		if (visited.size() == _nodes.size())
			return true;
		return false;
	}
}

#endif /* GRAPH_ALGORITHMS_HXX */
