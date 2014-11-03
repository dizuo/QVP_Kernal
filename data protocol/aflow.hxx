//	Decide the flow type.
//	@Note it depends on data pointer type which it holds.
template <class T>
bool AFlow::is() const
{
	return dynamic_cast<const Flow<T>* const>(this);
}

//	Drag the 'data pointer' from flow/pipe.
//	- Cast the common base class pointer to template derived class pointer.
//	- call the derived class member function get_value.
template <class T>
std::pair<bool, const T*> AFlow::get_value() const
{
	const Flow<T>* flow = dynamic_cast<const Flow<T>* >(this);
	if (! flow)
		return std::pair<bool, const T*>(false, 0);
	return std::pair<bool, const T*>(true, flow->get_value());
}

//	Store the 'data pointer' to flow/pipe.
//	- Cast the common base class pointer to template derived class pointer.
//	- call the derived class member function set_value.
template <class T>
bool AFlow::set_value(const T* ptr)
{
	Flow<T>* flow = dynamic_cast<Flow<T>*>(this);
	if (! flow)
		return false;

	flow->set_value(ptr);
	return true;
}