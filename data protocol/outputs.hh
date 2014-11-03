/// \file
// Provide the outputs factory. */

#ifndef OUTPUTS_HH
#define OUTPUTS_HH

//	prevent development errors
#include <cassert>  

//	The outputs are not unique and must be identifiable
#include <map>      

//	Since the std C++ lib does not implements static arrays, use our own...
#include "array.hh" 

//	The outputs are AFlow*
#include "aflow.hh" 

//	Handles the outputs of a process.
//	The only possible operations:
//	- hanging a flow.
//	- unhooking a flow.
//	- having the number of flows in a output port.
//	- knowing if a output port is empty.
//	- knowing if a output port is available, SUPPOSE this port has hung a flow.
//	- cleaning and setting of the 'data pointer'.

//	Outputs<M> stands for process output ports.
//	@param N is number of output port.
template <unsigned N>
class Outputs
{
private:
	typedef std::pair<unsigned, AFlow*> Entry;

	typedef std::map<unsigned, AFlow*>	FlowMap;
	
	//	All output ports.
	//	@Note ONE output port can correspond with AT LEAST ONE input ports(in other process).
	//	so each port has a flow map to record this relationship.
	array<N, FlowMap> _outputs;

public:
	Outputs()
	{
		//	a map is automatically init to 0.
	}

	~Outputs()
	{
		for (unsigned i = 0; i < N; i++)
			_outputs[i].clear();
	}

public:
	//	Hangs the flow 'f' to the 'ith' output and returns the id of the flow.
	//	i.e: if 2 flows are hanged to the 1st output, then they have different ids.
	int hang(unsigned i, AFlow* f)
	{
		assert(i < N);
		unsigned j;
		for (j = 0; ; j++)
		{
			std::pair<FlowMap::iterator, bool> tmp;
			tmp = _outputs[i].insert(Entry(j, f));
			if (tmp.second)
				break ;
		}
		return j;
	}

	//	Unhooks the 'j'th flow of the 'i'th output and returns true.
	//	@Note i indicates output port.
	//	j indicates flows whose share the same output port.
	bool unhook(unsigned i, unsigned j)
	{ 
		assert(i < N); assert(_outputs[i].find(j) != _outputs[i].end());

		_outputs[i][j]->clean();		
		_outputs[i].erase(j);
		return true;
	}

	//	Is the 'i'th output hanged to at least one flow ?
	//  if 'i'th port is empty ?
	bool is_hanged(unsigned i) const
	{
		assert(i < N);
		return !_outputs[i].empty();
	}

	//	Returns the number of flows on the 'i'th output.
	unsigned nb_flows(unsigned i) const
	{ return _outputs[i].size(); }

	//	Is the 'i'th 'j'th flow available?
	bool is_available(unsigned i, unsigned j) const
	{ 
		assert(i < N);
		assert(j < _outputs[i].size());
		
		return _outputs[i][j]->is_available();
	}

	//	Cleans all the outputs flows but does not unhook them 
	void clean()
	{
		for (unsigned i = 0; i < N; i++)
			for (FlowMap::iterator it = _outputs[i].begin(); it != _outputs[i].end(); it++)
				 it->second->clean();
	}

	//	Sets the 'i'th all output flow value.
	//	Called by Process<M,N>::execution function.
	template <class T>
	bool set_value(unsigned i, T* ptr)
	{ 
		assert(i < N); 
		
		//assert(! _outputs[i].empty()); 
		
		bool success = true;

		//	_output[i]'s all flows share the same data ptr.
		for ( FlowMap::iterator it = _outputs[i].begin();
			  it != _outputs[i].end(); 
			  it++ )
		{
			if ( !it->second->set_value<T>(ptr) )
				success = false;
		}
		return success;
	} 

};



/// Specialization when the process has no output 
// All the methods are calls to assert(0). */
template <>
class Outputs<0>
{
public:
	bool hang(unsigned, const AFlow*)
	{ 
		assert(0);
		return false; //to avoid stupid compiler warnings
	}

	bool unhook(unsigned, unsigned)
	{ 
		assert(0);
		return false; //to avoid stupid compiler warnings
	}

	bool is_hanged(unsigned) const
	{
		assert(0);
		return true;
	}

	unsigned nb_flows(unsigned) const
	{
		assert(0);
		return 0; //to avoid stupid compiler warnings
	}

	bool is_available(unsigned) const
	{
		assert(0);
		return true; //to avoid stupid compiler warnings
	}

	void clean()
	{
		assert(0);
	}

	template <class T>
	bool set_value(unsigned, const T&) const
	{ 
		assert(0);
		return false; //to avoid stupid compiler warnings
	} 
};

#endif //OUTPUTS_HH
