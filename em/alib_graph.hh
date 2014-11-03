#ifndef ALIBGRAPH
# define ALIBGRAPH

#include <iostream>
#include <list>

using namespace std;

/**
 ** This class is the interface between the execution model
 ** and the concrete graph for representing the relations between
 ** processes by flows.
 */
class ALibGraph
{
public:
  /** Default Ctor */
  ALibGraph() {};
  /** Dtor */
  virtual ~ALibGraph() {};

public:
  /**
   ** \brief Methods for creating concrete graph using ID 
   ** of processes and flows for correspondance 
   ** between process/flows and concrete nodes/edges.
   ** \{ 
   */
  /** add edge */
  virtual bool	       	add_edge(unsigned int v1, unsigned int v2) = 0;
  /** add vertex */
  virtual bool	       	add_vertex(unsigned int) = 0;
  /** add edge */
  virtual bool	       	del_edge(unsigned int edge) = 0;
  /** delete edge */
  virtual bool	       	del_edge(unsigned int v1, unsigned int v2) = 0;
  /** delete vertex */
  virtual bool	       	del_vertex(unsigned int v) = 0;
  /** \} */

  /** Method for the exectution */
  virtual list<unsigned int>	succ_of(unsigned int v) const = 0;
  /** get process id sin predecessors */
  virtual list<unsigned int>	get_roots() const = 0;
  /** topological sort */
  virtual list<unsigned int>	topological_sort() const = 0;

  /** \brief These methods can detect problems for the validity 
   ** of an exection's graph. \{ */
  /** detect cycle in graph */
  virtual bool			detect_cycle() const = 0;
  /** test if there is one component in undirected graph */
  virtual bool			one_component() const = 0;
  /** print graph information */
  virtual ostream&	print(ostream& ostr) const = 0;
  /** \} */
};

#endif /* !ALIBGRAPH */
