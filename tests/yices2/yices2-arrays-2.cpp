#include <iostream>
#include <memory>
#include <vector>
#include "assert.h"

#include "yices2_factory.h"
#include "yices2_term.h"
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

  Sort test_sort = s->make_sort("test", 0);
  // Cannot print this, unknown type for smt-switch.
  // cout << "\ttest_sort uninterpreted : " << test_sort << endl;

  // Should fail, does. 
  // Sort test_sort2 = s->make_sort("test", 1);

  test_sort = s->make_sort(INT);
  cout << "\ttest_sort int : " << test_sort << endl;
  test_sort = s->make_sort(REAL);
  cout << "\ttest_sort real : " << test_sort << endl;
  test_sort = s->make_sort(BOOL);
  cout << "\ttest_sort bool : " << test_sort << endl;



  Sort bvsort32 = s->make_sort(BV, 32);
  Sort array32_32 = s->make_sort(ARRAY, bvsort32, bvsort32);

  test_sort = array32_32->get_indexsort();
  cout << "\ttest_sort index : " << test_sort << endl;
  test_sort = array32_32->get_elemsort();
  cout << "\ttest_sort elem : " << test_sort << endl;

  SortVec sv {bvsort32, bvsort32};
  // sv.push_back(array32_32);
  // sv.push_back(bvsort32);

  test_sort = s->make_sort(FUNCTION, sv);
  cout << "\ttest_sort function : " << test_sort << endl;

  Sort intsort = s->make_sort(INT);
  Term x = s->make_symbol("x", intsort);
  Term y = s->make_symbol("y", intsort);

  Term z = s->make_term(100, intsort);
  cout << "\t" << z << endl;

  Term a = s->make_term(Plus, x, y);
  cout << "\t" << a << endl;
  Term b = s->make_term(Equal, a, z);
  cout << "\t" << b << endl;

  Term c = s->make_term(55, intsort);
  cout << "\t" << c << endl;
  Term d = s->make_term(35, intsort);
  cout << "\t" << d << endl;

  Term e = s->make_term(Ge, x, c);
  cout << "\t" << e << endl;
  Term g = s->make_term(Le, y, d);
  cout << "\t" << g << endl;

  Term f = s->make_term(And, e, g, b);
  cout << "\t" << f->to_string() << endl;
  cout << "f hash " << f->hash() << endl;


  s->assert_formula(f);
  Result r = s->check_sat();
  assert(r.is_sat());

  cout << "\t" << x << " = " << s->get_value(x) << endl;
  cout << "\t" << y << " = " << s->get_value(y) << endl;

  shared_ptr<Yices2Term> yterm0 = static_pointer_cast<Yices2Term>(f);
  Term nu_term = Term(new Yices2Term(yterm0->hash()));
  cout << "\t" << nu_term << endl;
  cout << "nu_term hash " << nu_term->hash() << endl;


   for (auto c : f)
  {
    cout << "op: " << f->get_op() << " str: " << f->to_string() << endl;

    for (auto t : c)
    {
      cout << t->to_string() << endl;
    }
  }
  // (a & b) = (!a | !b) 



  // Sort int_sort = s->make_sort(INT);

  // Term q = s->make_symbol("q", int_sort);
  // Term q2 = s->make_symbol("q2", int_sort);


  // Term r = s->make_term(Gt, q, s->make_term(100, int_sort));
  // cout << "\t" << r << endl;


  // Term t = s->make_term(Plus, q, q2);
  // cout << "\t" << t << endl;

  // Term u = s->make_term(Ge, t, s->make_term(100, int_sort));
  // cout << "\t" << u << endl;

  // // Implies gets rewritten as an or statment.. 
  // Term f = s->make_term(Implies, r, u);
  // cout << "\t" << f << endl;

  // // And is also rewritten, but it is printed differently. 
  // Term f2 = s->make_term(And, s->make_term(Ge, q2, s->make_term(50, int_sort)), f );
  // cout << "\t" << f2 << endl;

  // // s->assert_formula(r);

  // // assert(s->check_sat().is_sat());
  // s->assert_formula(f2);
  // Result r2 = s->check_sat();
  // assert(r2.is_sat());
  // cout << "\t" << q << " = " << s->get_value(q) << endl;
  // cout << "\t" << q2 << " = " << s->get_value(q2) << endl;



  // Term x = s->make_symbol("x", bvsort32);
  // Term y = s->make_symbol("y", bvsort32);
  // Term arr = s->make_symbol("arr", array32_32);

  // cout << "Sorts:" << endl;
  // cout << "\tbvsort32 : " << bvsort32 << endl;
  // cout << "\tarray32_32 : " << array32_32 << endl;
  // s->assert_formula(
  //     s->make_term(Not,
  //                  s->make_term(Implies,
  //                               s->make_term(Equal, x, y),
  //                               s->make_term(Equal,
  //                                            s->make_term(Select, arr, x),
  //                                            s->make_term(Select, arr, y)))));
  // assert(!s->check_sat().is_sat());
  return 0;
}
