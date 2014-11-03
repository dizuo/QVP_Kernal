#ifndef RELATION_HH
# define RELATION_HH

#include "../factory/flow.hh"

/** \brief This class remember partial or complete
 ** relation between flows and processes. */
class Relation
{
  /** \name Ctor nad Dtor \{ */
public:
  /** Default Ctor*/
  Relation():
    _flow (0),
    _id_proc_in (0),
    _id_buff_in (0),
    _id_proc_out (0),
    _id_buff_out (0),
    _id_slot (0)
  {
  }

  /** Default Dtor*/
  Relation(AFlow* flow):
    _flow (flow),
    _id_proc_in (0),
    _id_buff_in (0),
    _id_proc_out (0),
    _id_buff_out (0),
    _id_slot (0)
  {
  }

  ~Relation(){}
  /** \} */

public:
  /** \name Modifiers \{ */
  /** set object flow */
  void	set_flow(AFlow* flow) {_flow = flow;}
  /** set id of input processor */
  void	set_id_proc_in(unsigned int id) {_id_proc_in = id;}
  /** set id of output processor */
  void	set_id_proc_out(unsigned int id) {_id_proc_out = id;}
  /** set id of processor's input buffer */
  void	set_id_buff_in(unsigned int id) {_id_buff_in = id;}
  /** set id of processor's output buffer */
  void	set_id_buff_out(unsigned int id) {_id_buff_out = id;}
  /** set id of slot */
  void	set_id_slot(int id) {_id_slot = id;}
  /** \} */

  /** \name Accessors \{ */
  /** get object flow*/
  AFlow*       	get_flow() {return _flow;}
  /** get id of input processor */
  unsigned int	get_id_proc_in() {return _id_proc_in;}
  /** get id of output processor */
  unsigned int	get_id_proc_out() {return _id_proc_out;}
  /** get id of processor's input buffer */
  unsigned int	get_id_buff_in() {return _id_buff_in;}
  /** get id of processor's output buffer */
  unsigned int	get_id_buff_out() {return _id_buff_out;}
  /** get id of slot */
  int		get_id_slot() {return _id_slot;}
  /** \} */

protected:
  /** \name Attibutes \{ */
  /** The flow which connect 2 processes */
  AFlow*       	_flow;
  /** ID process input */
  unsigned int	_id_proc_in;
  /** ID buffer input */
  unsigned int	_id_buff_in;
  /** ID process output */
  unsigned int	_id_proc_out;
  /** ID buffer output */
  unsigned int	_id_buff_out;
  /** ID slot */
  int		_id_slot;
  /** \} */
};

#endif /* RELATION_HH */
