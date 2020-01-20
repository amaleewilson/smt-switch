#ifndef SMT_YICES2_TERM_H
#define SMT_YICES2_TERM_H

#include <vector>

#include "term.h"
#include "utils.h"

#include "yices2_sort.h"

namespace smt {

// forward declaration
class Yices2Solver;

// helpers
// Op lookup_op(Btor * btor, Yices2Node * n);

class Yices2TermIter : public TermIterBase
{
 public:
  Yices2TermIter(term_t t, uint32_t p) : term(t), pos(p) {};
  Yices2TermIter(const Yices2TermIter & it)
  {
    term = it.term;
    pos = it.pos;
  };
  ~Yices2TermIter(){};
  Yices2TermIter & operator=(const Yices2TermIter & it);
  void operator++() override;
  const Term operator*() override;
  bool operator==(const Yices2TermIter & it);
  bool operator!=(const Yices2TermIter & it);

 protected:
  bool equal(const TermIterBase & other) const override;

 private:
  term_t term;
  uint32_t pos;
};

class Yices2Term : public AbsTerm
{
 public:
  Yices2Term(term_t t) : term(t) {};
  Yices2Term(term_t t, uint32_t ar) : term(t), term_arity(ar) {};
  ~Yices2Term() {};
  std::size_t hash() const override;
  bool compare(const Term & absterm) const override;
  Op get_op() const override;
  Sort get_sort() const override;
  bool is_symbolic_const() const override;
  bool is_value() const override;
  virtual std::string to_string() const override;
  uint64_t to_int() const override;
  /** Iterators for traversing the children
   */
  TermIter begin() override;
  TermIter end() override;

 protected:
  term_t term;
  // Btor * btor;
  // the actual API level node that is used
  // Yices2Node * node;
  // the real address of the boolector node
  // allows us to look up:
  //   kind: for retrieving operator
  //   e:    for getting children
  // BtorNode * bn;
  // true iff the node is negated
  bool negated;
  // true iff the node is a symbolic constant
  bool is_sym;
  // for iterating args nodes
  // BtorArgsIterator ait;
  // for storing nodes before iterating
  // std::vector<BtorNode *> children;
  bool is_function;
  uint32_t term_arity; 

  // helpers
  bool is_const_array() const;

  friend class Yices2Solver;
  friend class Yices2TermIter;
};

}  // namespace smt

#endif
