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
  Sort bvsort9 = s->make_sort(BV, 9);
  Term x = s->make_symbol("x", bvsort9);
  Term y = s->make_symbol("y", bvsort9);
  Term onebit = s->make_symbol("onebit", s->make_sort(BV, 1));

  Term unnecessary_rotation = s->make_term(Op(Rotate_Right, 1), onebit);

  Op ext74 = Op(Extract, 7, 4);
  Term x_upper = s->make_term(ext74, x);

  Op op = x_upper->get_op();
  // TODO: this is failing with a null op.. 
  cout << "Op: " << op << endl;

  Term y_ror = s->make_term(Op(Rotate_Right, 2), y);
  Term y_rol = s->make_term(Op(Rotate_Left, 2), y);

  /// TODO: these assertions are failing.. 
  cout << "y_ror->to_string() " << y_ror->to_string() << endl;
  cout << "y_ror->to_string() " << y_ror->get_op() << endl;
  cout << "y_rol->to_string() " << y_rol->to_string() << endl;
  cout << "y_rol->to_string() " << y_rol->get_op() << endl;

  for (auto c : y_ror)
  {
    cout << "c op:  " << c->get_op() << endl;
  }
  
  // assert(y_ror->to_string() == "((_ rotate_right 2) y)");

  // assert(y_rol->to_string() == "((_ rotate_left 2) y)");

  s->assert_formula(s->make_term(Equal, y_ror, y_rol));
  s->assert_formula(s->make_term(Distinct, y, s->make_term(0, bvsort9)));
  s->assert_formula(s->make_term(
      Equal, x, s->make_term(Op(Repeat, 9), unnecessary_rotation)));

  Result r = s->check_sat();
  assert(r.is_sat());

  Term xc = s->get_value(x);
  Term x_upperc = s->get_value(x_upper);
  Term yc = s->get_value(y);

  cout << "Results:" << endl;
  cout << x << ": " << xc->to_int() << endl;
  cout << x_upper << ": " << x_upperc->to_int() << endl;
  cout << y << ": " << yc->to_int() << endl;
  return 0;
}
