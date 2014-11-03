//	file:Provides the process interface

#ifndef APROCESS_HH
# define APROCESS_HH

#include <string>
#include <list>
#include "status.hh"
#include "aflow.hh"

//	This is an abstraction for a process.
//	Usage: Store processes without knowing the real functionnalities of it.
//	@see Process
class AProcess
{
protected:
	//	Record the lase error of the process.
	std::string		_error;

	//	Record last status of the process.
	Status			_status;

public:
	AProcess()
		: _error(""), _status()
	{}

	//	Virtual dtor.
	virtual ~AProcess() 
	{}

	//////////////////////////////////////////////////////////////////////////
	//	Common interface functions.
	//////////////////////////////////////////////////////////////////////////
	
	//	Gets the last error message.
	const std::string& error() const { return _error; }

	//	Accesses to the last status of the process.
	const Status& status() const { return _status; }

	//	Executes the process and returns its status. 
	//	Algo:
	//	- reset the status;
	//	- launch preconditions and stop if failure;
	//	- launch execution and stop if failure;
	//	After exec ends, return its status.
	const Status& exec()
	{
		_status.reset();
		_status.launched(true);

		if (preconditions())
			_status.precond(true);
		else
			return _status;

		if (execution())
			_status.exec(true);
		else
			return _status;

		return _status;
	}

	//////////////////////////////////////////////////////////////////////////
	//	Abstract virtual functions.
	//////////////////////////////////////////////////////////////////////////

	//	Get the process name
	virtual std::string name() const = 0;

	//	Gets the slot name by type(in/out) and id.
	//	@Note 
	//		isIn is true indicate the slot is input, otherwise is output.
	//		i_id:	[0, i_slot_number)
	//		o_id:	[0, o_slot_number)
	virtual const char* slot_name(bool isIn, int id) const = 0;

	//	Gets the slot type info by type and id.
	virtual const char* slot_type(bool isIn, int id) const = 0;

	//	Gets the number of inputs of the process
	virtual unsigned nb_inputs() const = 0;

	//	Gets the number of outputs of the process
	virtual unsigned nb_outputs() const = 0;

	//	Hangs a flow to the 'i'th input. 
	//	The process must be reset if the operation is successfull.
	virtual bool hang_flow_in(unsigned i, const AFlow* f) = 0;

	//	Unhooks a flow from the 'i'th input.
	//	The process must be reset if the operation is successfull.
	virtual bool unhook_flow_in(unsigned i) = 0;

	//	Hangs a flow to the 'i'th output.
	//	If needed the statuts must be changed.
	//	@return -1 if failed, the slot else.
	virtual int hang_flow_out(unsigned i, AFlow* f) = 0;

	//	Unhooks a flow from the 'i'th output, 'j'th slot.
	//	The process must be reset if the operation is successfull
	//	and if it was the last flow hanged to the 'i'th output.
	virtual bool unhook_flow_out(unsigned i, unsigned j) = 0;

	//	Reset the processus.
	virtual void reset() = 0;

	//	Transforms the process into a readable form.
	//	The informations that should appear are:
	//	- the name of the process;
	//	- the inputs and their types, and if they are empty;
	//	- the outputs and their types, and if they are empty.
	//	@note This is for debug only.
	//	virtual std::string to_string() const = 0;

protected:
	//	Type-checks between the flow 'f' and the 'ith' input.
	//	Should fill the _error attribute.
	virtual bool type_check_in(unsigned ith, const AFlow* f) = 0;

	//	Type-checks between the flow 'f' and the 'ith' output.
	//	Should fill the _error attribute. */
	virtual bool type_check_out(unsigned ith, AFlow* f) = 0;

	//	Checks the preconditions.
	virtual bool preconditions() const = 0;

	//	Executes the process functionnalities.
	virtual bool execution() = 0;

};

#endif //APROCESS_HH
