#ifndef GRAPHEXEC_HXX
# define GRAPHEXEC_HXX

# include <iostream>
# include <cassert>
# include <list>
# include <queue>
# include <set>

#include <QDebug>
#include <ctime>

# include "../factory/process.hh"
# include "../factory/status.hh"

using namespace std;

/** \brief execute the graph */
bool		GraphExec::exec()
{
	_error = string("");

	bool	flag = true;

#ifdef _DEBUG
	const clock_t begin = clock();
#endif

	list<unsigned int> res = _lib_graph->topological_sort();
	qDebug() << "Topo Sort : " << clock() - begin; 

	// initialize state_table
	for(list<unsigned int>::const_iterator i = res.begin(); i != res.end(); i++)
		modify_helper[ *i ] = false;

	for(list<unsigned int>::const_iterator i = res.begin(); i != res.end(); i++)
	{
		unsigned int	cur_id = *i;
		Status sta = _processes[cur_id]->status();
		
		if (sta.perfect())		// perfect state
			continue;		
		else					// initial state: exec==false, launched==false
		{	
			if (!sta.launched() && !modify_helper[cur_id])		//update state
				modify_proc(cur_id);							// update all children.

			// just exec
			#ifdef _DEBUG
			clock_t sec1 = clock();
			#endif
			Status t = _processes[cur_id]->exec();
			qDebug() << _processes[cur_id]->name().c_str() << " Exec: " << clock() - sec1; 

			if (!t.exec() && (_error == ""))
			{
				_error = "Process " + string(_processes[cur_id]->name()) + " : "
					+ string(_processes[cur_id]->error());
				flag = false;
			}
		}
	}

	return flag;
}

/** \brief verify the validity of the graph
** for being ready for execution
*/
bool	GraphExec::valid_graph()
{
	_error = string("");

	if ( _processes.size()<2 || _flows.size()<1 )
		return false;

	/*Test if all all node's slots are hanged flows*/
	for (map<unsigned int, Relation *>::const_iterator i = _flows.begin();
		i != _flows.end(); i++)
	{
		Relation* r = i->second;
		if ((r->get_id_proc_in() == 0) || (r->get_id_proc_out() == 0))
		{
			_error = string("Some flows aren't connected.");
			return false;
		}
	}

	/*Is there cycle in graph*/
	if (_lib_graph->detect_cycle())
	{
		_error = string("Cycle detected.");
		return false;
	}
	
	/*Is there a component in graph*/
	/**
	 ** 图中有多个连通分量也是可以搞定的。先简单情况
	 **/
	if (!_lib_graph->one_component())
	{
		_error = string("All elements (process/flows) aren't in the same component.");
		return false;
	}
	return true;
}

/** \brief clear status of modified process and
** all subtree of this process
*/
void	GraphExec::modify_proc(unsigned int id_process)
{
	queue<unsigned int>	Q;
	set<unsigned int>	S; S.clear();

	Q.push(id_process);
	S.insert(id_process);

	/* clear subtree status */
	while (!Q.empty())
	{
		unsigned int id_cur_proc = Q.front(); Q.pop();

		/* clear status of current process*/
		AProcess *proc = _processes[id_cur_proc];
		proc->reset();
		modify_helper[id_cur_proc] = true;

		/* search successors uncleared */
		list<unsigned int> l = _lib_graph->succ_of(id_cur_proc);
		for (list<unsigned int>::const_iterator it = l.begin();
			it != l.end(); it++)
			if (S.find(*it) == S.end())
			{
				Q.push(*it);
				S.insert(*it);
			}
	}
}

/** return the error message */
string	GraphExec::error()
{
	return _error;
}

/** print graph information */
ostream&	GraphExec::print(ostream& ostr)
{
	ostr << endl << "***> PROCESSES" << endl;
	map<unsigned int, AProcess *>::iterator	it;
	for (it = _processes.begin(); it != _processes.end(); it++)
		ostr << "   process : " << it->first << endl;
	ostr << endl;
	ostr << "***> FLOWS" << endl;
	map<unsigned int, Relation *>::iterator	jt;
	for (jt = _flows.begin(); jt != _flows.end(); jt++)
		ostr << "   flow : " << jt->first << " with : "
		<< jt->second->get_id_proc_out() << "("
		<< jt->second->get_id_buff_out()
		<< ") -> "
		<< jt->second->get_id_proc_in() << "("
		<< jt->second->get_id_buff_in() << ")"
		<< endl;
	ostr << endl << endl;
	return _lib_graph->print(ostr);
}

#endif /* GRAPHEXEC_HXX*/



