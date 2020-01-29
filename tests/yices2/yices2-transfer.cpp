#include <iostream>
#include <memory>
#include <vector>
#include "assert.h"


#include "gmp.h"
#include "yices.h"
#include "yices2_term.h"
#include "yices2_sort.h"


#include "yices2_factory.h"
#include "smt.h"
// after a full installation
// #include "smt-switch/msat_factory.h"
// #include "smt-switch/smt.h"

using namespace smt;
using namespace std;

int main()
{
  SmtSolver s = Yices2SolverFactory::create();
  s->set_opt("produce-models", "true");
  Sort bvsort8 = s->make_sort(BV, 8);
  Term x = s->make_symbol("x", bvsort8);
  Term y = s->make_symbol("y", bvsort8);
  Term z = s->make_symbol("z", bvsort8);
  Term T = s->make_term(true);

  Term a = s->make_symbol("a", s->make_sort(INT));
  Term b = s->make_symbol("b", s->make_sort(INT));

  Term constraint = s->make_term(Equal, z, s->make_term(BVAdd, x, y));
  // cout << "constraint " << constraint << endl;
  constraint = s->make_term(And, constraint, s->make_term(Lt, a, b));

  cout << " is not? " << (yices_term_constructor(constraint->hash()) == YICES_NOT_TERM) << endl;

type_t int_type = yices_int_type();
term_t x2 = yices_new_uninterpreted_term(int_type);
term_t y2 = yices_new_uninterpreted_term(int_type);
  term_t f = yices_and3(yices_arith_geq0_atom(x2),
                      yices_arith_geq0_atom(y2),
                      yices_arith_eq_atom(yices_add(x2, y2),
                                          yices_int32(100)));
  cout << "f to string" << yices_term_to_string(f, 120, 1, 0) << endl;

  cout << " is f not? " << (yices_term_constructor(f) == YICES_NOT_TERM) << endl;
  cout << " is f not? " << (yices_term_constructor(yices_term_child(f, 0)) == YICES_OR_TERM) << endl;



  cout << "constraint " << constraint << endl;
  s->assert_formula(constraint);

  SmtSolver s2 = Yices2SolverFactory::create();
  s2->set_opt("produce-models", "true");
  s2->set_opt("incremental", "true");

  TermTranslator tt(s2);

  Term constraint2 = tt.transfer_term(constraint);
  Term T2 = tt.transfer_term(T);
  // // ensure it can handle transfering again (even though it already built the
  // // node)
  constraint2 = tt.transfer_term(constraint);
  s2->assert_formula(constraint2);

  cout << "term from solver 1: " << constraint << endl;
  cout << "term from solver 2: " << constraint2 << endl;

  assert(s->check_sat().is_sat());
  assert(s2->check_sat().is_sat());

  return 0;
}
