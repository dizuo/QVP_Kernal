#ifndef AGRAPHEXEC
# define AGRAPHEXEC

#include <list>
# include <iostream>
# include "../factory/flow.hh"
# include "../factory/process.hh"

using namespace std;

class Relation;
/**
 ** \brief This class is the interface between the execution model and
 **        the interface.
 */
class	AGraphExec
{
public:
  /** Default Ctor */
  AGraphExec() {}
  /** Default Dtor */
  virtual ~AGraphExec() {}

public:
  /*
  ** This method provde all the relations.
  */
	virtual std::list<Relation*> get_relations() const = 0;

  /*
  ** These methods create or destroy processes and flows
  ** but don't create relations between them.
  */
  /** add flow */
  virtual unsigned int 	add_flow(AFlow* flow) = 0;
  /** add process */
  virtual unsigned int	add_process(AProcess* process) = 0;

  /** RYF 2010/11/30*/
  /** add process and it id*/
  virtual void add_process(AProcess* process, unsigned int) = 0;
  /** delete flow */
  virtual bool		del_flow(unsigned int id_flow) = 0;
  /** delete process */
  virtual bool		del_process(unsigned int id_process) = 0;

  /*
  ** These methods create or destroy relations
  ** between processes and flows
  */
  /** Add connection between in buffer and flow */
  virtual bool		hang_flow_in(unsigned int id_process,
				     unsigned int id_flow,
				     unsigned int id_buffer) = 0;
  /** Add connection between out buffer and flow */
  virtual int		hang_flow_out(unsigned int id_process,
				     unsigned int id_flow,
				     unsigned int id_buffer) = 0;
  /** Delete connection between in buffer and flow */
  virtual bool		del_flow_in(unsigned int id_process,
				     unsigned int id_flow,
				     unsigned int id_buffer) = 0;
  /** Delete connection between out buffer and flow */
  virtual bool		del_flow_out(unsigned int id_process,
				     unsigned int id_flow,
				     unsigned int id_buffer,
				     int id_slot) = 0;

  /** Execute the created graph */
  virtual bool		exec() = 0;
  /** Just verify validity of graph */
  virtual bool		valid_graph() = 0;
  /** modify process's status and subtree status */
  virtual void		modify_proc(unsigned int id_proc) = 0;
  /** print graph */
  virtual ostream&	print(ostream& ostr) = 0;
  /** return the error message */
  virtual string	error() = 0;
};

#endif /* !AGRAPHEXEC */
