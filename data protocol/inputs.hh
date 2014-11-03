//	file:Provide the inputs factory.

#ifndef INPUTS_HH
#define INPUTS_HH

//	prevent development errors
#include <cassert>  

//	Since the std C++ lib does not implements static arrays, use our own...
#include "array.hh" 

//	The inputs are const Aflow*
#include "aflow.hh"		

//////////////////////////////////////////////////////////////////////////
//	Handles the inputs of a process.
//	The only possible operations:
//	- hanging a flow.
//	- unhooking a flow.
//	- knowing if a input port is empty
//	- knowing if a input flow is available, SUPPOSE the this port has hung a flow.
//	- retrieving of the 'data pointer'.
//////////////////////////////////////////////////////////////////////////
//	Inputs<M> stands for process input ports.
//	@Param M is number of input ports.
//////////////////////////////////////////////////////////////////////////
template <unsigned M>
class Inputs
{
private:
	array<M, const AFlow*> _inputs;

public:
	Inputs()
	{
		for (unsigned i = 0; i < M; i++)
			_inputs[i] = 0;
	}

	~Inputs()
	{
		for (unsigned i = 0; i < M; i++)
			_inputs[i] = 0;
	}


public:
	//	Each port only can hang one flow.
	//	Hangs the flow 'f' to the 'i'th input and returns true.
	bool hang(unsigned i, const AFlow* f)
	{ 
		assert(i < M); 
		
		_inputs[i] = f; 
		return true;
	}

	//	Unhooks the 'i'th input and returns true.
	bool unhook(unsigned i)
	{ 
		assert(i < M); assert(_inputs[i]); 
		
		_inputs[i] = 0; 
		return true;
	}

	//	Is the 'i'th input hanged to a flow ?
	//  if 'i'th port is empty ?
	bool is_hanged(unsigned i) const
	{
		assert(i < M);
		return _inputs[i];
	}

	//	Is the 'i'th input flow available ?
	//	@Note suppose 'i'th input has been hanged to a flow. 
	bool is_available(unsigned i) const
	{ 
		assert(i < M);
		return _inputs[i]->is_available();
	}

	//	Gets the 'i'th input flow value.
	//	Called by Process<M,N>::execution function.
	template <class T>
	std::pair<bool, const T*> get_value(unsigned i) const
	{ 
		assert(i < M);
		return _inputs[i]->get_value<T>();
	} 
};


//	Specialization when the process has no input.
//	All the methods are calls to assert(0).
template <>
class Inputs<0>
{
public:
	bool hang(unsigned, const AFlow*)
	{ 
		assert(0);
		return false; //to avoid stupid compiler warnings
	}

	bool unhook(unsigned)
	{ 
		assert(0);
		return false; //to avoid stupid compiler warnings
	}

	bool is_hanged(unsigned) const
	{
		assert(0);
		return false; //to avoid stupid compiler warnings
	}

	bool is_available(unsigned) const
	{
		assert(0);
		return true; //to avoid stupid compiler warnings
	}

	template <class T>
	std::pair<bool, const T*> get_value(unsigned) const
	{ 
		assert(0);
		//to avoid stupid compiler warnings
		return std::pair<bool, const T*>(false, 0);
	} 
};

#endif //INPUTS_HH
