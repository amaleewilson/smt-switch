
// #include <stdio.h>
#include <yices.h>

// int main(void) {
//    printf("Testing Yices %s (%s, %s)\n", yices_version,
//            yices_build_arch, yices_build_mode);
//    return 0;
// }

#include <iostream>
#include <memory>
#include <vector>
#include "assert.h"

#include "yices2_factory.h"
#include "smt.h"
// after a full installation
// #include "smt-switch/boolector_factory.h"
// #include "smt-switch/smt.h"

using namespace smt;
using namespace std;

int main()
{

  // yices_init();
  // term_t x = yices_new_uninterpreted_term(YICES_BV_ARRAY);
  // yices_set_term_name(x, "x");
  // term_t y = yices_new_uninterpreted_term(YICES_BV_ARRAY);
  // yices_set_term_name(y, "y");

  SmtSolver s = Yices2SolverFactory::create();

  // I think yices ignores this option anyway. 
  // https://github.com/SRI-CSL/yices2/blob/622f0367ef6b0f4e7bfb380c856bac758f2acbe7/doc/manual/manual.tex
  s->set_opt("produce-models", "true"); 

  Sort bvsort32 = s->make_sort(BV, 32);
  
  // Sort array32_32 = s->make_sort(ARRAY, bvsort32, bvsort32);
  Term x = s->make_symbol("x", bvsort32);
  // Term y = s->make_symbol("y", bvsort32);
  // Term arr = s->make_symbol("arr", array32_32);

  cout << "Sorts:" << endl;
  cout << "\tbvsort32 : " << bvsort32 << endl;
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