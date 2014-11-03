#ifndef GRAPH_HXX
# define GRAPH_HXX

using namespace std;

namespace graph
{
	template<class NodeLabel>
	Graph<NodeLabel>::Graph()
	{
	}

	template<class NodeLabel>
	Graph<NodeLabel>::~Graph()
	{
		for (t_map_nodes_iter it=_nodes.begin();
			it != _nodes.end();
			it++)
		{
			delete it->second;
			it->second = 0;
		}
		
		_nodes.clear();
	}

  template <class NodeLabel>
  Node<NodeLabel>*	Graph<NodeLabel>::add_node(t_nodelabel label)
  {
    t_node* node = new t_node(label);
    _nodes[label] = node;
    return node;
  }

  template <class NodeLabel>
  void	Graph<NodeLabel>::del_node(t_nodelabel label)
  {
    if (!contains_node(label))
      return ;
    t_node* node = get_node(label);
    for (t_map_nodes_iter i = _nodes.begin(); i != _nodes.end(); i++)
      {
	i->second->del_succ_all(node);
	i->second->del_pred_all(node);
      }
    delete _nodes[label];
    _nodes.erase(label);
  }

  template <class NodeLabel>
  Node<NodeLabel>*	Graph<NodeLabel>::get_node(t_nodelabel label)
  {
    if (!contains_node(label))
      return NULL;
    return _nodes[label];
  }

  template <class NodeLabel>
  bool	Graph<NodeLabel>::contains_node(t_nodelabel label) const
  {
    if (_nodes.end() != _nodes.find(label))
      return true;
    return false;
  }

  template <class NodeLabel>
  bool	Graph<NodeLabel>::contains_node(t_node* node) const
  {
    t_nodelabel label = node->get_label();
    return contains_node(label);
  }

  template <class NodeLabel>
  list< Node<NodeLabel>* > Graph<NodeLabel>::succ_of(t_node* node)
  {
    return node->get_successors();
  }

  template <class NodeLabel>
  list< Node<NodeLabel>* > Graph<NodeLabel>::succ_of(t_nodelabel label)
  {
    Node<t_nodelabel>*	node = get_node(label);
    return node->get_successors();
  }

  template <class NodeLabel>
  bool	Graph<NodeLabel>::add_edge(t_node* node1, t_node* node2)
  {
    if (!contains_node(node1) || !contains_node(node2))
      return false;
    node1->add_succ(node2);
    node2->add_pred(node1);
    return true;
  }

  template <class NodeLabel>
  bool	Graph<NodeLabel>::add_edge(t_nodelabel label1,
				   t_nodelabel label2)
  {
    if (!contains_node(label1) || !contains_node(label2))
      return false;

    t_node* node1 = get_node(label1);
    t_node* node2 = get_node(label2);

    node1->add_succ(node2);
    node2->add_pred(node1);
    return true;
  }

  template <class NodeLabel>
  void	Graph<NodeLabel>::del_edge(t_node* node1,
				   t_node* node2)
  {
    if (!contains_node(node1) || !contains_node(node2))
      return ;
    node1->del_succ(node2);
    node2->del_pred(node1);
  }

  template <class NodeLabel>
  void	Graph<NodeLabel>::del_edge(t_nodelabel label1,
				   t_nodelabel label2)
  {
    if (!contains_node(label1) || !contains_node(label2))
      return ;

    t_node *node1 = get_node(label1);
    t_node *node2 = get_node(label2);

    node1->del_succ(node2);
    node2->del_pred(node1);
  }

  /** printer */
  template <class NodeLabel>
  ostream&	Graph<NodeLabel>::print(ostream& ostr) const
  {
    ostr << "Nodes : "<< endl;
    for (t_map_nodes_citer i = _nodes.begin();
	 i != _nodes.end(); i++)
      {
	ostr << "  " << i->first << endl ;
	t_node *node = i->second;
	ostr << "     preds : ";
        t_list_nodes preds = node->get_predecessors();
	for (t_list_nodes_citer k = preds.begin();
	     k != preds.end(); k++)
	  ostr << (*k)->get_label() << " ";
	ostr << endl;
	ostr << "     succs : ";
	t_list_nodes succs = node->get_successors();
	for (t_list_nodes_citer k = succs.begin();
	     k != succs.end(); k++)
	  ostr << (*k)->get_label() << " ";
	ostr << endl;
      }

    return ostr;
  }
}

#endif /* GRAPH_HXX */
