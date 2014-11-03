#ifndef DEFAULTLIBGRAPH
# define DEFAULTLIBGRAPH

#include <iostream>
#include <list>
#include "graph/graph.hh"

using namespace std;

/**
 ** This class is the interface between the execution model
 ** and the concrete graph for representing the relations between
 ** processes by flows.
 */
class DefaultLibGraph : public ALibGraph
{
  /** \name Ctor and Dtor \{  */
public:
  DefaultLibGraph() {_graph = new graph::Graph<unsigned int>();}
  ~DefaultLibGraph() 
  {
	  delete _graph; _graph = 0;
  }
  /** \} */

  /** \name Manage vertices \{ */
public:
  /** \brief Add vertex into the graph
   ** \param id : ID of vertex after addition into the graph
   */
  bool	       	add_vertex(unsigned int id)
  {
    _graph->add_node(id);
    return true;
  }

  /** \brief Delete vertex from the graph
   ** \param id : ID of vertex which must be deleted
   */
  bool	       	del_vertex(unsigned int id)
  {
    _graph->del_node(id);
    return true;
  }
  /** \} */




  /** \name Manage edges \{ */
public:
  /** \brief Add edge into the graph
   ** \param v1 : ID of source vertex
   ** \param v1 : ID of destination vertex
   */
  bool	       	add_edge(unsigned int v1, unsigned int v2)
  {
    _graph->add_edge(v1, v2);
    return true;
  }

  /** \brief Delete edge from the graph
   ** \param id : ID of edge which must be deleted
   */
  bool	       	del_edge(unsigned int)
  {
    /* Void impossible with our lib graph
    ** because it doesn't manage concrete
    ** edges
    */
    return false;
  }

  /** \brief Delete edge form the graph
   ** \param v1 : source node
   ** \param v2 : destination node
   */
  bool	       	del_edge(unsigned int v1, unsigned int v2)
  {
    _graph->del_edge(v1, v2);
    return true;
  }


public:
  /** \brief eval topological sort */
  list<unsigned int>	topological_sort() const
  {
    list< graph::Node<unsigned int>* >  res;
    res = _graph->topological_sort();

    list< unsigned int >        result;
    for (list< graph::Node<unsigned int>* >::const_iterator it = res.begin();
         it != res.end(); 
		 it++)
		result.push_back((*it)->get_label());
    return result;
  }

  /** \brief return successors of v */
  list<unsigned int>	succ_of(unsigned int v) const
  {
    list< graph::Node<unsigned int>* >  res;
    res = _graph->succ_of(v);

    list< unsigned int >        result;
    for (list< graph::Node<unsigned int>* >::const_iterator it = res.begin();
         it != res.end(); it++)
	 result.push_back((*it)->get_label());
    return result;
  }

  /** \brief return nodes sin predecessors */
  list<unsigned int>	get_roots() const
  {
    list< graph::Node<unsigned int>* >  res;
    res = _graph->get_roots();

    list< unsigned int >        result;
    for (list< graph::Node<unsigned int>* >::const_iterator it = res.begin();
         it != res.end(); it++)
		result.push_back((*it)->get_label());

    return result;
  }

  /** These methods can detect problem for the validity
   ** of an exection's graph. \{ */
public:
  /** Detect cycle into the graph */
  bool			detect_cycle() const
  {
    return _graph->detect_cycle();
  }

  /** Consider undirected graph and verify that
   ** there is only one component */
  bool			one_component() const
  {
    return _graph->one_component();
  }
  /** \} */

  /** printer */
  ostream&	print(ostream& ostr) const
  {
    return _graph->print(ostr);
  }

  /** \name Attributes\{ */
protected:
  /** graph of the used lib */
  graph::Graph<unsigned int>	*_graph;
  /** \} */
};

#endif /* !DEFAULTLIBGRAPH */





