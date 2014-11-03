// file:Provide the status tool.

#ifndef STATUS_HH
# define STATUS_HH

#include <string>

//	The 4 informations about the status of a process. 

//	@Note As soon there is one false, the followings are meaningless. 
//	- if the precond is false, then exec is not significant; 
//	- if launched is false then the others are all meaningless.

//	short circuit 

//	@see AProcess, Process
class Status
{
private:
	//	true if Process::exec() has been called.
	bool _launched;

	//	true if Process::preconditions() has succeed.
	bool _precond;

	//	true if Process::execution() has succeed.
	bool _exec;

public:
	Status() :
	  _launched(false), _precond(false), _exec(false)
	  { }

	  //	Inits the attributes
	  Status(bool l, bool pre, bool e, bool post) :
	  _launched(l), _precond(pre), _exec(e)
	  {}

	  // The copy ctor and operator= given by the compiler are ok.

	  // Handy SHORTCUT to test all the components of a status.
	  bool perfect() const
	  { return _launched && _precond && _exec; }

	  // Retrieves launched attribute.
	  bool launched() const
	  { return _launched; }

	  // Retrieves precond attribute.
	  bool precond() const
	  { return _precond; }

	  // Retrieves exec attribute.
	  bool exec() const
	  { return _exec; }

	  // Sets all the boolean to false.
	  void reset()
	  {
		  _launched = false;
		  _precond = false;
		  _exec = false;
	  }

	  // Sets launched to 'b'.
	  void launched(bool b)
	  { _launched = b; }

	  // Sets precond to 'b'.
	  void precond(bool b)
	  { _precond = b; }

	  // Sets exec to 'b'.
	  void exec(bool b)
	  { _exec = b; }


	  // Debug infos
	  std::string to_string() const
	  {
		  const std::string l("launched:");
		  const std::string pre("precond:");
		  const std::string ex("exec:");

		  const std::string yes("yes");
		  const std::string no("no");
		  const std::string verif("verified");
		  const std::string not_verif("not verified");
		  const std::string nl("\n");
		  std::string res("");
		  res += l + (_launched ? yes : no) + nl;
		  res += pre + (_precond ? verif : not_verif) + nl;
		  res += ex + (_exec ? yes : no) + nl;
		  //  res += post + (_postcond ? verif : not_verif) + nl;
		  return res;
	  }
};

// 7/22/2010 RYF
// remove the post-condition variable

#endif //STATUS_HH
