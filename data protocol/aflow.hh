#ifndef AFLOW_HH
#define AFLOW_HH

#include <utility>	//std::pair

//////////////////////////////////////////////////////////////////////////
//	This is an abstraction for a flow.
//	Usage: Transport the data pointer without knowing its type.
//	@see Flow 
//////////////////////////////////////////////////////////////////////////
class AFlow
{
public:
	//////////////////////////////////////////////////////////////////////////
	//	ctor & dtor
	//////////////////////////////////////////////////////////////////////////
	AFlow() {}
	virtual ~AFlow() {}

	//	To know the type of the 'data pointer' really transported by the flow. 
	//	@warning DO NOT WORK with inheritance! (eg: C inherits from A)
	//	@code
	//		class A {};
	//		class C : public A{};
	//		AFlow *f = new Flow<C>(); 
	//		f.is<A>();	// false
	//	The end code will return false because Flow<C> does not inherit from Flow<A>
	template <class T>
	bool is() const;

	//	To access to the 'data pointer'.
	//	@return
	//		<false, X>	-> the is<T>() failed;
	//		<true, X>	-> X is the 'data pointer'. 
	template <class T>
	std::pair<bool, const T*> get_value() const;

	//	To modify the 'data pointer'.
	//	@return  
	//		false	-> the is<T>() failed ('data pointer' have not been modified);
	//		true	-> the 'data pointer' have been modified.
	template <class T>
	bool set_value(const T* ptr);

	//	To clean 'data pointer' inside the flow.
	virtual void clean() = 0;

	//	Does the 'data pointer' in the flow is null or not?
	virtual bool is_available() const = 0;
};


//	Implementation is in aflow.hxx which is included in flow.hh

#endif //AFLOW_HH
