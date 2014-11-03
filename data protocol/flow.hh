#ifndef FLOW_HH
#define FLOW_HH

#include "aflow.hh" //Flow<T> inherits from it

//	Flow is a POINTER carrier or pipe, it can carry any type 'data pointer'.
//	Flow  can IFF carry 'data pointer'.  // if and only if

//	@Sample usage:
//		AFlow* int_f = new Flow<int>;		//create a flow.
//		int *pi = new int(5);
//		if ( int_f->set_value<int>(pi) )	//setup 'data pointer' to int_flow/pipe
//		{
//			// drag data pointer from int_flow/pipe
//			std::pair<bool, int*> res = int_f->get_value<int>();
//			if (res.first && res.second)
//				cout << *res.second;	// print 5
//		}
template <class T>
class Flow : public AFlow
{
public:
	typedef T* ptr_type;

	Flow()
		: _ptr(0)
	{}

	//	Dtor donnot take care of value destruction. 
	//	flow is only a pipe for data communication.
	//		only set _ptr 0
	~Flow() {  _ptr = 0; }

	//	To access the 'data pointer', in only-read-mode
	const T* get_value() const { return _ptr; }

	//	To change the 'data pointer' _ptr.
	void set_value(const T* ptr) { _ptr = const_cast<T*>(ptr); }

	//////////////////////////////////////////////////////////////////////////
	//	Re-implement virtual functions.
	//////////////////////////////////////////////////////////////////////////

	//	If _ptr has hold the data address.
	bool is_available() const { return (_ptr); }
	
	//	Clear the 'data pointer' _ptr.
	void clean() { _ptr = 0; };

private:

	//	'data pointer'
	ptr_type	_ptr;
};

#include "aflow.hxx"	//the AFlow functions implementation

#endif //FLOW_HH
