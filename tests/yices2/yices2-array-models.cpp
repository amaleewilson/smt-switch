#include <iostream>
#include <memory>
#include <vector>
#include "assert.h"

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
  Sort bvsort32 = s->make_sort(BV, 32);
  Sort array32_32 = s->make_sort(ARRAY, bvsort32, bvsort32);
  Term x0 = s->make_symbol("x0", bvsort32);
  Term x1 = s->make_symbol("x1", bvsort32);
  Term y = s->make_symbol("y", bvsort32);
  Term arr = s->make_symbol("arr", array32_32);
  cout << "arr: " << arr << " sort : " << arr->get_sort() << endl;

  Term qq = s->make_term(Select, arr, x0);
  Term q2 = s->make_term(Select, arr, x1);


  Term constraint = s->make_term(Equal, qq, x1);
  constraint = s->make_term(
      And, constraint, s->make_term(Equal, q2, y));
  constraint = s->make_term(And, constraint, s->make_term(Distinct, x1, y));
  s->assert_formula(constraint);
  Result r = s->check_sat();

  assert(r.is_sat());


  // Cannot do get val on array because it is a function
  Term qq_val = s->get_value(qq);
  cout << qq_val << endl;
  Term q2_val = s->get_value(q2);
  cout << q2_val << " select op " << q2->get_op()<< endl;
  // Term arr_val = s->get_value(arr);
  // cout << arr_val << endl;

  return 0;
}
