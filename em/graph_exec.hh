#ifndef GRAPHEXEC_HH
# define GRAPHEXEC_HH

# include <map>
# include <utility>
# include "alib_graph.hh"
# include "default_lib_graph.hh"
# include "relation.hh"

# include "../factory/flow.hh"
# include "../factory/process.hh"
# include "../factory/status.hh"

using namespace std;

/** \brief Concrete execution model */
class	GraphExec : public AGraphExec
{
	/* max elements */
	static const unsigned int PROCS_MAX = INT_MAX;
	static const unsigned int FLOWS_MAX = INT_MAX;

	/** \name Ctor & Dtor.
	** \{ */
public:
	typedef map<unsigned int, AProcess* > proc_map_type;
	typedef proc_map_type::const_iterator proc_map_const_iter;
	typedef proc_map_type::iterator	proc_map_iter;

	typedef map<unsigned int, Relation* > flow_map_type;
	typedef flow_map_type::const_iterator flow_map_const_iter;
	typedef flow_map_type::iterator	flow_map_iter;

	/** \brief default Ctor */
	GraphExec():
	  _process_index (0),
		  _flow_index (0),
		  _error ("")
	  {
		  _lib_graph = new DefaultLibGraph();
	  }
	  /** \brief default Dtor */
	  ~GraphExec() 
	  {		  
		  //for ( flow_map_iter it = _flows.begin();
				//it != _flows.end();
				//it++ )
		  //{
			 // //------------------------------
			 // Relation *r = it->second;
			 // delete r->get_flow();
			 // delete r; r = 0;
			 // //------------------------------
		  //}
		  //
		  for ( proc_map_iter it = _processes.begin();
			   it != _processes.end();
			   it++ )
		  {
			  delete it->second;
			  it->second = 0;			 
		  }

		  _flows.clear();
		  _processes.clear();

			delete _lib_graph;
	  }
	  /** \} */

	  /** \name Creators and Destructors of process and flow
	  ** \{ */
public:
	/** \brief Interface to get all relations. useful for save scene.
	*/
	  std::list<Relation*> get_relations() const
	  {
		  std::list<Relation*> ret_list;

		flow_map_const_iter iter;
		for (iter = _flows.begin();
		   iter != _flows.end();
		   iter++)
		{
			ret_list.push_back(iter->second);		 
		}
		 return ret_list;
	  }

	/** \brief Add a new process and return his Id, 0 else. */
	unsigned int	add_process(AProcess* process)
	{
		_error = string("");
		unsigned int        temp = _process_index++;

		/* find valid Id */
		while ((_processes.find(_process_index) != _processes.end()) &&
			(temp != _process_index))
		{
			if (_process_index == (PROCS_MAX))
				_process_index = 1;
			else
				_process_index++;
		}

		/* All Id are distribued */
		if (temp == _process_index)
		{ _error = string("All ID are attribuated"); return 0;}
		_processes[_process_index] = process;
		_lib_graph->add_vertex(_process_index);
		return _process_index;
	}

	void add_process(AProcess* process, unsigned int proc_id)
	{
		_error = string("");
		unsigned int        temp = _process_index++;

		/* All Id are distribued */
		if (_processes.find(proc_id)!=_processes.end())
		{
			_error = string("This ID has been attribuated"); 
			return;
		}

		_processes[proc_id] = process;
		_lib_graph->add_vertex(proc_id);
	}

	/** \brief Add a new flow and return his Id, 0 else. */
	unsigned int 	add_flow(AFlow* flow)
	{
		_error = string("");
		unsigned int        temp = _flow_index++;

		/* find valid Id */
		while ((_flows.find(_flow_index) != _flows.end()) &&
			(temp != _flow_index))
		{
			if (_flow_index == (FLOWS_MAX))
				_flow_index = 1;
			else
				_flow_index++;
		}

		/* All Id are distribued */
		if (temp == _flow_index)
		{ _error = string("All ID are attribuated"); return 0;}
		_flows[_flow_index] = new Relation(flow);
		return _flow_index;
	}

	/** \brief Delete a process
	** \param id_process the Id the process which will be deleted
	*/
	bool	del_process(unsigned int id_process)
	{
		_error = string("");
		/* Test existence */
		if (_processes.find(id_process) == _processes.end())
			return false;

		/* delete this vertex and his relations in the concrete graph */
		_lib_graph->del_vertex(id_process);

		/* Delete all connection between process's buffer and
		** flow but not delete relation. */
		flow_map_iter	it;
		for (it = _flows.begin(); it != _flows.end(); it++)
		{
			Relation *r = it->second;
			if (r->get_id_proc_in() == id_process)
			{
				if (r->get_id_proc_out())
					_lib_graph->del_edge(r->get_id_proc_out(), id_process);
				r->set_id_proc_in(0);
				r->set_id_buff_in(0);
			}
			if (r->get_id_proc_out() == id_process)
			{
				if (r->get_id_proc_in())
					_lib_graph->del_edge(id_process, r->get_id_proc_in());
				r->set_id_proc_out(0);
				r->set_id_buff_out(0);
			}
		}
		
		//------------------------------
		proc_map_iter pos = _processes.find(id_process);
		delete pos->second;		pos->second = 0;
		_processes.erase(id_process);
		//------------------------------
		
		return true;
	}

	/** \brief Delete a flow
	** \param id_flow the Id the flow which will be deleted
	*/
	bool		del_flow(unsigned int id_flow)
	{
		_error = string("");
		/* Test existence */
		if (_flows.find(id_flow) == _flows.end())
		{_error = string("Bad flow ID"); return false;}

		/* Effect delete */
		Relation *r = _flows[id_flow];
		unsigned int out = r->get_id_proc_out();
		unsigned int in = r->get_id_proc_in();

		if (in && out)
			_lib_graph->del_edge(out, in);

		if (in)
			_processes[in]->unhook_flow_in(r->get_id_buff_in());

		if (out)
			_processes[out]->unhook_flow_out(r->get_id_buff_out(), r->get_id_slot());

		//------------------------------
		delete r->get_flow();
		delete r; r = 0;
		_flows.erase(id_flow);
		//------------------------------

		return true;
	}
	/** \} */


	/** \name Manager of relations between processes and flows
	** \{ */
public:
	/** \brief Connect a flow with an input buffer
	** input buffer can be connected at only one unique flow
	** doesn't effect connection if :
	**    - process, buffer or id doesn't exist
	**    - error type
	**    - the buffer of the process is already connected
	**    - the concerned flow's extremity is already connected
	**	 - create cycle
	*/
	bool		hang_flow_in(unsigned int id_process,
		unsigned int id_flow,
		unsigned int id_buffer)
	{
		_error = string("");
		/* Test existence */
		if (_processes.find(id_process) == _processes.end())
		{_error = string("Bad process ID"); return false;}
		if (_flows.find(id_flow) == _flows.end())
		{_error = string("Bad flow ID"); return false;}
		AProcess *proc = _processes[id_process];
		if (id_buffer >= proc->nb_inputs())
		{_error = string("Bad buffer ID"); return false;}

		/* type checking */
		Relation *r = _flows[id_flow];
		if (!proc->hang_flow_in(id_buffer, r->get_flow()))
		{_error = proc->error(); return false;}

		/* Test if connection is possible */
		if (r->get_id_proc_in() != 0) // there is already connection
		{
			proc->unhook_flow_in(id_buffer);
			_error = string("Can't connect at this buffer.");
			return false;
		}
		for (flow_map_const_iter i = _flows.begin();
			i != _flows.end(); i++)
		{
			Relation *r = i->second;
			// the input is already attribuated
			if ((r->get_id_proc_in() == id_process) &&
				(r->get_id_buff_in() == id_buffer))
			{
				proc->unhook_flow_in(id_buffer);
				_error = string("The buffer is already connected.");
				return false;
			}
		}
		if (r->get_id_proc_out())
		{
			_lib_graph->add_edge(r->get_id_proc_out(), id_process);
			if (_lib_graph->detect_cycle())
			{
				_lib_graph->del_edge(r->get_id_proc_out(), id_process);
				proc->unhook_flow_in(id_buffer);
				_error = string("The connection is impossible (create cycle).");
				return false;
			}
		}

		/* Effect connection */
		r->set_id_proc_in(id_process);
		r->set_id_buff_in(id_buffer);

		/* the process and the sub-tree will be executed next time */
		modify_proc(id_process);
		return true;
	}

	/** \brief Connect a flow with an output buffer
	** an out buffer can be connected at many flows
	** doesn't effect connection if :
	**    - process, buffer or id doesn't exist
	**    - error type
	**    - the concerned flow's extremity is already connected
	**	 - create cycle
	** return -1 fi no ok
	*/
	int		hang_flow_out(unsigned int id_process,
		unsigned int id_flow,
		unsigned int id_buffer)
	{
		_error = string("");
		/* Test existence */
		if (_processes.find(id_process) == _processes.end())
		{_error = "Bad process ID"; return false;}
		if (_flows.find(id_flow) == _flows.end())
		{_error = "Bad flow ID"; return false;}
		AProcess *proc = _processes[id_process];
		if (id_buffer >= proc->nb_outputs())
		{_error = "Bad buffer ID"; return false;}

		/* Verif type */
		Relation* r = _flows[id_flow];

		int	id_slot = proc->hang_flow_out(id_buffer, r->get_flow());
		if (id_slot == -1)
			return -1;

		/* Test if connection is possible */
		if (r->get_id_proc_out())
		{
			proc->unhook_flow_out(id_buffer, id_slot);
			return -1;
		}
		if (r->get_id_proc_in())
		{
			_lib_graph->add_edge(id_process, r->get_id_proc_in());
			if (_lib_graph->detect_cycle())
			{
				_lib_graph->del_edge(id_process, r->get_id_proc_in());
				proc->unhook_flow_out(id_buffer, id_slot);
				_error = string("The connection is impossible (create cycle).");
				return -1;
			}
		}

		/* Effect connection */
		r->set_id_proc_out(id_process);
		r->set_id_buff_out(id_buffer);
		r->set_id_slot(id_slot);
		/* the process at enter flow's extremity and
		the sub-tree will be executed next time */
		modify_proc(id_process);
		return id_slot;
	}

	/** \brief  Disconnect a flow with an input buffer */
	bool		del_flow_in(unsigned int id_process,
		unsigned int id_flow,
		unsigned int id_buffer)
	{
		_error = string("");
		/* Test existence */
		if (_processes.find(id_process) == _processes.end())
		{_error = "Bad process ID"; return false;}
		if (_flows.find(id_flow) == _flows.end())
		{_error = "Bad flow ID"; return false;}
		AProcess *proc = _processes[id_process];
		if (id_buffer >= proc->nb_inputs())
		{_error = "Bad buffer ID"; return false;}

		Relation *r = _flows[id_flow];
		/* Delete edge from the lib graph if there is an
		** edge with this relation */
		if ((r->get_id_proc_in() != 0) && (r->get_id_proc_out() != 0))
			_lib_graph->del_edge(id_flow);

		/* Effect disconnection */
		r->set_id_proc_in(0);
		r->set_id_buff_in(0);
		proc->unhook_flow_in(id_buffer);
		return true;
	}

	/** \brief Disconnect a flow with an output buffer */
	bool		del_flow_out(unsigned int id_process,
		unsigned int id_flow,
		unsigned int id_buffer,
		int id_slot)
	{
		_error = string("");
		/* Test existence */
		if (_processes.find(id_process) == _processes.end())
		{_error = "Bad process ID"; return false;}
		if (_flows.find(id_flow) == _flows.end())
		{_error = "Bad flow ID"; return false;}
		AProcess *proc = _processes[id_process];
		if (id_buffer >= proc->nb_outputs())
		{_error = "Bad buffer ID"; return false;}

		Relation *r = _flows[id_flow];
		/* Delete edge from the lib graph if there is an
		** edge with this relation */
		if ((r->get_id_proc_in() != 0) && (r->get_id_proc_out() != 0))
			_lib_graph->del_edge(id_flow);

		/* Effect disconnection */
		r->set_id_proc_out(0);
		r->set_id_buff_out(0);
		r->set_id_slot(0);
		proc->unhook_flow_out(id_buffer, id_slot);
		return true;
	}
	/** /} */

public:
	/** \brief Execute processes */
	bool	exec();

	/** \brief Test intigrity of execution graph */
	bool	valid_graph();

	/** \brief clear processor status and processor's subtree status */
	void	modify_proc(unsigned int id_process);

	/** \brief print exec graph informations */
	ostream&	print(ostream& ostr);

	/** \brief return the error message */
	string	error();

	/** \name Attributes \{ */
protected:
	/** Permit to create relation beteween processes and flows */
	ALibGraph*		       	_lib_graph;
	/** Permit to coresponding Id with process */
	proc_map_type	_processes;
	/** Permit to coresponding Id with flow */
	flow_map_type	_flows;
	/** index for finding ungiven Id */
	unsigned int		       	_process_index;
	/** index for finding ungiven Id */
	unsigned int		       	_flow_index;
	/** the error message */
	string			_error;

	/* AGraphExec::exec AGraphExec::modify_proc */
	std::map<int, bool>		modify_helper;	
	/** \} */
};

#include "graph_exec.hxx"

#endif /* !GRAPHEXEC_HH */
