#ifndef GRAPH_HH
# define GRAPH_HH

#include <iostream>
#include <list>
#include <map>
#include <set>
#include "node.hh"

using namespace std;

namespace graph
{
  /** graph class */
  template <class NodeLabel>
  class Graph
  {
    /** \name Some Type defintion \{ */
  public:
    typedef NodeLabel		t_nodelabel;
    typedef Node<t_nodelabel>	t_node;

    typedef map< t_nodelabel, t_node* >			t_map_nodes;
    typedef typename t_map_nodes::iterator		t_map_nodes_iter;
    typedef typename t_map_nodes::const_iterator     	t_map_nodes_citer;

    typedef list< t_node* >				t_list_nodes;
    typedef typename t_list_nodes::iterator	       	t_list_nodes_iter;
    typedef typename t_list_nodes::const_iterator      	t_list_nodes_citer;
    /** \} */

    /** \name Manage Nodes \{ */
  public:
	Graph();
	~Graph();

    t_node*		add_node(t_nodelabel label);
    void		del_node(t_nodelabel label);
    bool		contains_node(t_nodelabel label) const;
    bool		contains_node(t_node *node) const;
    t_node*		get_node(t_nodelabel label);
    t_list_nodes	succ_of(t_node *node);
    t_list_nodes	succ_of(t_nodelabel label);
    /** \} */

    /** \name Manage Edges \{ */
  public:
    bool	add_edge(t_node *node1, t_node *node2);
    bool	add_edge(t_nodelabel label1, t_nodelabel label2);
    void	del_edge(t_node *node1, t_node *node2);
    void	del_edge(t_nodelabel label1, t_nodelabel label2);
    /** \} */

    /** \name \{ */
  public:
    ostream&	print(ostream& ostr) const;
    /** \} */

    /** \name Some algorithmes on graph \{ */
  public:
    t_list_nodes	get_roots();
    t_list_nodes	topological_sort();
    bool	       	one_component();
	bool			detect_cycle();
    /** \} */

    /** \name Attributes \{ */
  protected:
    t_map_nodes		_nodes;
    /** \} */
  };
}

#include "graph.hxx"
#include "graph_algorithms.hxx"

#endif /* GRAPH_HH */
