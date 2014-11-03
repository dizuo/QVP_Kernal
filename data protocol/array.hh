/** \file
 * Provides an almost-STL container for static arrays.
 */

#ifndef ARRAY_HH
#define ARRAY_HH

/** \addtogroup Array */
/** @{ */

/** An almost-STL container for arrays of constant size. */
template<unsigned N, typename T>
class array
{
private:
  T elems[N];	/**< fixed-size array of elements of type T. */

public:
  /* classical type definitions for STL container. */
  typedef T		value_type;	/**< value_type. */

  typedef T*		iterator;	/**< iterator. */
  typedef const T*	const_iterator;	/**< const-iterator. */

  typedef T&		reference;	/**< reference. */
  typedef const T&	const_reference;/**< const-reference. */

  /** iterator: begin, non-const. */
  iterator begin()
  { return elems; }

  /** iterator: begin, const. */
  const_iterator begin() const
  { return elems; }

  /** iterator: end, non-const. */
  iterator end()
  { return elems + N; }

  /** iterator: end, const. */
  const_iterator end() const
  { return elems + N; }

  /** operator[], non-const */
  reference operator[](unsigned i)
  { return elems[i]; }

  /** operator[], const */
  const_reference operator[](unsigned i) const
  { return elems[i]; }

  /** size (it is constant). */
  unsigned size() const
  { return N; }

  /** empty (can not be because of constant size). */
  bool empty() const
  { return false; }

  /** max_size (always N). */
  unsigned max_size() const
  { return N; }

  /** swap with another array. */
  void swap (array<N, T>& y)
  {
    for (unsigned i = 0; i < N; i++)
      {
	T tmp = this->elems[i];
	this->elems[i] = y.elems[i];
	y.elems[i] = tmp;
      }
  }

  /** direct access to data. */
  const T* data() const
  { return elems; }

  /** assignment with implicit type conversion. */
  template <typename T2>
  array<N, T>& operator=(const array<N, T2>& rhs)
  {
    for (unsigned i = 0; i < N; i++)
      this->elems[i] = rhs.elems[i];
    return *this;
  }

  /** assign one value to all elements. */
  void assign(const T& value)
  {
    for (unsigned i = 0; i < N; i++)
      this->elems[i] = value;
  }

  /** comparison function. */
  int compare(const array<N, T>& rhs) const
  {
    for (unsigned i = 0; i < N; i++)
      {
	if (this->elems[i] > rhs.elems[i])
	  return 1;
	if (this->elems[i] < rhs.elems[i])
	  return -1;
      }
    return 0;
  }
};

/** global swap(). */
template <unsigned N, typename T>
void swap(array<N, T>& x, array<N, T>& y)
{
  x.swap(y);
}

/** operator ==. */
template <unsigned N, typename T>
bool operator==(const array<N, T>& e1, const array<N, T>& e2)
{
  return e1.compare(e2) == 0;
}

/** operator !=. */
template <unsigned N, typename T>
bool operator!=(const array<N, T>& e1, const array<N, T>& e2)
{
  return e1.compare(e2) != 0;
}

/** operator <=. */
template <unsigned N, typename T>
bool operator <= (const array<N, T>& e1, const array<N, T>& e2)
{
  return e1.compare(e2) <= 0;
}

/** operator <. */
template <unsigned N, typename T>
bool operator< (const array<N, T>& e1, const array<N, T>& e2)
{
  return e1.compare(e2) < 0;
}

/** operator >=. */
template <unsigned N, typename T>
bool operator>= (const array<N, T>& e1, const array<N, T>& e2)
{
  return e1.compare(e2) >= 0;
}

/** operator >. */
template <unsigned N, typename T>
bool operator> (const array<N, T>& e1, const array<N, T>& e2)
{
  return e1.compare(e2) > 0;
}

#endif







