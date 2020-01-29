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


  Term x = s->make_symbol("x", bvsort32);
  Term y = s->make_symbol("y", bvsort32);
  Term arr = s->make_symbol("arr", array32_32);

  cout << "Sorts:" << endl;
  cout << "\tbvsort32 : " << bvsort32 << endl;
  cout << "\tarray32_32 : " << array32_32 << endl;
  s->assert_formula(
      s->make_term(Not,
                   s->make_term(Implies,
                                s->make_term(Equal, x, y),
                                s->make_term(Equal,
                                             s->make_term(Select, arr, x),
                                             s->make_term(Select, arr, y)))));
  assert(!s->check_sat().is_sat());
  return 0;
}
