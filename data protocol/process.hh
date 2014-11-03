//	file:Provide the process factory.

#ifndef PROCESS_HH
#define PROCESS_HH

#include "aprocess.hh"
#include "inputs.hh"
#include "outputs.hh"

//	To have a working process, you only have to write these methods:
// 	- slot_name		, public
//	- slot_type		, public
//	- name			, public
//	- type_check_in , protected
//	- type_check_out, protected
//	- execution		, protected

//	To access datas produced by the process, 
//		use the AFlow::get_value or Flow::get_value methods to get 'data pointer', 
//		then drag the data from 'data pointer'.

//	To get inputs datas during the execution, 
//		use Inputs::get_value method to get 'data pointer', then drag the data from 'data pointer'.

//	To set outputs datas during the execution, 
//		use Outputs::set_value method to set the 'data pointer'.

template <unsigned M, unsigned N>
class Process : public AProcess
{
protected:
	//	Available for Process<M,N> derived classes.
	//	All input ports.
	Inputs<M>  _inputs; 
	//	All output ports.
	Outputs<N> _outputs; 

public:
	virtual const char* slot_name(bool isIn, int id) const = 0;

	virtual const char* slot_type(bool isIn, int id) const = 0;

	unsigned nb_inputs() const { return M; }

	unsigned nb_outputs() const { return N; }


	//	Default hanging of inputs.
	//	Default behavior: type checking and no multiple flows on same input port.
	virtual bool hang_flow_in(unsigned i, const AFlow* f)
	{
		// check the input port is empty or not
		if ( _inputs.is_hanged(i) ) {
			_error = "The input is already hanged to a flow.";
			return false;
		}
		
		// check the flow type
		if ( !type_check_in(i, f) ) {
			_error = "The flow type is wrong.";
			return false;
		}

		// do hang
		if (_inputs.hang(i, f)) { 
			return true; 
		}

		_error = "Impossible to hang to the input";
		return false;
	}

	//	Default unhooking.
	//	Default behavior: no verifications.
	virtual bool unhook_flow_in(unsigned i)
	{ 
		if (_inputs.unhook(i)) 
		{  return true; }
		_error = "Impossible to unhook from the input";
		return false;
	}

	//	Default hanging of outputs.
	//	@Note MULTIPLE flows can be hanged on same output port.
	virtual int hang_flow_out(unsigned i, AFlow* f)
	{
		// check flow type
		if ( !type_check_out(i, f) ) {
			_error = "The flow type is wrong.";
			return -1;
		}

		// do hang 
		int tmp = _outputs.hang(i, f);

		// check the hanged flow id
		if (tmp == -1) {
			_error = "Impossible to hang to the output";
			return tmp;
		}

		return tmp;
	}

	//	Default unhooking.
	//	Default behavior: no verifications
	virtual bool unhook_flow_out(unsigned i, unsigned j)
	{ 
		bool tmp = _outputs.unhook(i, j);

		//if (_outputs.nb_flows(i) == 0)
		//	reset();

		return tmp;
	}

	//	Reset the process.
	//	Request process::exec run
	virtual void reset()
	{
		_status.launched(false);
	}

protected:
	virtual bool type_check_in(unsigned ith, const AFlow* f) = 0;

	virtual bool type_check_out(unsigned ith, AFlow* f) = 0;

	virtual bool execution() = 0;

	//	case1 M>0, N==0 
	//		- for provider process 
	//		- at least one output port is hanged.
	//	case2 M==0, N>0
	//		- for consumer process
	//		- at least one input port is hanged.
	//	case3 M>0, N>0
	//		- for processor
	//		- at least one input port is hanged and at least one output port is hanged.
	virtual bool preconditions() const
	{
		bool iFlag(false);
		bool oFlag(false);

		for (unsigned i(0); i < M; i++) {
			if ( _inputs.is_hanged(i) )
				iFlag = true;
		}

		for (unsigned o(0); o < N; o++) {
			if (_outputs.is_hanged(o))
				oFlag = true;
		}
		
		if (0==N)				//case1
			return iFlag;
		else if (M>0 && N>0)	//case2
			return iFlag & oFlag;

		return oFlag;			//case3
	}
};

#endif //PROCESS_HH

