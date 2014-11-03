#ifndef NODE_HH
# define NODE_HH

#include <list>

using namespace std;

namespace graph
{
  /** \brief Describe graph's node */
  template <class NodeLabel>
  class Node
  {
    /** \name Ctor and Dtor\{ */
  public:
    Node()
    {
      _successors.clear();
      _predecessors.clear();
    }

    Node(NodeLabel label):
      _label (label)
    {
      _successors.clear();
      _predecessors.clear();
    }

    ~Node() {}
    /** \} */
    
    /** \name Some type definition \{ */
  public:
    typedef NodeLabel		t_nodelabel;
    typedef Node<t_nodelabel>	t_node;

    typedef list< t_node* >				t_list_nodes;
    typedef typename t_list_nodes::iterator		t_list_nodes_iter;
    typedef typename t_list_nodes::const_iterator	t_list_nodes_citer;
    /** \} */

    /** \name operations on label \{ */
  public:
    void	set_label(t_nodelabel label) {_label = label;}
    t_nodelabel	get_label() {return _label;}
    /** \} */
    
    /** \name operations on successors and predecessors \{ */
  public:
    /* Accessors */
    t_list_nodes	get_successors() {return _successors;}
    t_list_nodes	get_predecessors() {return _predecessors;}

    /* modifiers */
    void	set_successors(t_list_nodes successors) 
    {_successors = successors;}
    void	set_predecessors(t_list_nodes predecessors) 
    {_predecessors = predecessors;}

    /** Add a successor into the list, accept repeated successor */
    void	add_succ(t_node *node) {_successors.push_back(node);}

    /** Add a predecessor into the list, accept repeated predecessor */
    void	add_pred(t_node *node) {_predecessors.push_back(node);}

    /** Delete the first element which is equal with node */
    void	del_succ(t_node *node)
    {
      for (t_list_nodes_iter i = _successors.begin();
           i != _successors.end(); i++)
        if (node == (*i))
          {
            _successors.erase(i);
            return ;
          }
    }
    
    /** Delete all element equal with node */
    void        del_succ_all(t_node *node) {_successors.remove(node);}

    /** Delete the first element which is equal with node */
    void	del_pred(t_node *node) 
    {
      for (t_list_nodes_iter i = _predecessors.begin(); 
	   i != _predecessors.end(); i++)
	if (node == (*i))
	  {
	    _predecessors.erase(i);
	    return ;
	  }
    }

    /** Delete all element equal with node */
    void        del_pred_all(t_node *node) {_predecessors.remove(node);}

    /** Test if node is in predecessor list */
    bool	contains_pred(t_node *node)
    {
      for (t_list_nodes_iter i = _predecessors.begin(); 
	   i != _predecessors.end(); i++)
	if (node == (*i))
	  return true;
      return false;
    }

    /** Test if node is in successor list */
    bool	contains_succ(t_node *node)
    {
      for (t_list_nodes_iter i = _successors.begin(); 
	   i != _successors.end(); i++)
	if (node == (*i))
	  return true;
      return false;
    }
    
    /** \} */

    /** \name \{ */
  private:
    /** the node's label */
    t_nodelabel	       	_label;
    /** list of successors */
    t_list_nodes	_successors;
    /** list of predecessors */
    t_list_nodes	_predecessors;
    /** \} */
  };
}

#endif /* NODE_HH */
