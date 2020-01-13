
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
#include <inttypes.h>

#include "yices2_factory.h"
#include "smt.h"
// after a full installation
// #include "smt-switch/boolector_factory.h"
// #include "smt-switch/smt.h"

using namespace smt;
using namespace std;

//Not really an arrays test, but just doing this for now. 
int main()
{

  SmtSolver s = Yices2SolverFactory::create();
  Sort intsort = s->make_sort(INT);
  Term x = s->make_symbol("x", intsort);
  Term y = s->make_symbol("y", intsort);
  Term f = s->make_term(And,
              s->make_term(Ge, x, s->make_term(55, intsort)),
              s->make_term(Le, y, s->make_term(35, intsort)),
              s->make_term(Equal,
                s->make_term(Plus, x, y),
                s->make_term(100, intsort)));

  s->assert_formula(f);
  Result r = s->check_sat();
  assert(r.is_sat());

  cout << "\t" << x << " = " << s->get_value(x) << endl;
  cout << "\t" << y << " = " << s->get_value(y) << endl;


  // context_t *ctx = yices_new_context(NULL);  // NULL means 'use default configuration'

  // switch (yices_check_context(ctx, NULL)) { // call check_context, NULL means 'use default heuristics'
  

  // I think yices ignores this option anyway. 
  // https://github.com/SRI-CSL/yices2/blob/622f0367ef6b0f4e7bfb380c856bac758f2acbe7/doc/manual/manual.tex
////  s->set_opt("produce-models", "true"); 

////  Sort bvsort8 = s->make_sort(BV, 8);

  // Sort array32_32 = s->make_sort(ARRAY, bvsort32, bvsort32);
////  Term x = s->make_symbol("x", bvsort8);
  // Term y = s->make_symbol("y", bvsort32);
  // Term arr = s->make_symbol("arr", array32_32);

 //// cout << "Sorts:" << endl;
 //// cout << "\tbvsort32 : " << bvsort32 << endl;


  /* simple test */
  // s->set_logic("QF_ABV");
  // s->set_opt("produce-models", "true");
  // Sort bvsort8 = s->make_sort(BV, 8);
  // Term x = s->make_symbol("x", bvsort8);
  // Term y = s->make_symbol("y", bvsort8);
  // Term xpy = s->make_term(BVAdd, x, y);
  // for (auto c : xpy)
  // {
  //   assert(c == x || c == y);
  // }
  /* end simple test */ 

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
