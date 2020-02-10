#include <iostream>
#include <memory>
#include <vector>
#include "assert.h"

#include "gmp.h"
#include "yices.h"

#include "smt.h"
#include "yices2_factory.h"
// after a full installation
// #include "smt-switch/boolector_factory.h"
// #include "smt-switch/smt.h"

// remove these
#include "yices2_term.h"

using namespace smt;
using namespace std;

int main()
{
  SmtSolver s = Yices2SolverFactory::create();
  s->set_logic("QF_ABV");
  s->set_opt("produce-models", "true");
  Sort bvsort8 = s->make_sort(BV, 8);
  Term x = s->make_symbol("x", bvsort8);

  shared_ptr<Yices2Term> yterm;

  // try
  // {
  //   Term x = s->make_symbol("x", bvsort8);
  //   // assert(false);
  // }
  // catch (IncorrectUsageException & e)
  // {
  //   cout << "caught error with message: " << e.what() << endl;
  // }

  Term y = s->make_symbol("y", bvsort8);
  Term z = s->make_symbol("z", bvsort8);
  Term _true = s->make_term(true);

  Term t =
      s->make_term(Iff, s->make_term(Equal, x, y), s->make_term(Equal, x, z));
  shared_ptr<Yices2Term> yt = std::static_pointer_cast<Yices2Term>(t);
  cout << "t " << t << " term constructor "
       << yices_term_constructor(yt->hash()) << " op = " << t->get_op() << endl;

  t = s->make_term(
      Implies, s->make_term(Equal, x, y), s->make_term(Equal, x, z));
  yt = std::static_pointer_cast<Yices2Term>(t);
  cout << "t " << t << " term constructor "
       << yices_term_constructor(yt->hash()) << " op = " << t->get_op() << endl;

  t = s->make_term(Iff, s->make_term(Equal, x, y), s->make_term(Equal, x, z));
  yt = std::static_pointer_cast<Yices2Term>(t);
  cout << "t " << t << " term constructor "
       << yices_term_constructor(yt->hash()) << " op = " << t->get_op() << endl;

  Sort int_sort = s->make_sort(INT);

  t = s->make_term(Distinct, x, y);
  yt = std::static_pointer_cast<Yices2Term>(t);
  cout << "t " << t << " term constructor "
       << yices_term_constructor(yt->hash()) << " op = " << t->get_op() << endl;

  t = s->make_term(BVNot, x);
  yt = std::static_pointer_cast<Yices2Term>(t);
  cout << "t " << t << " term constructor "
       << yices_term_constructor(yt->hash()) << " op = " << t->get_op() << endl;

  for (auto c : t)
  {
    cout << "c " << c << " term constructor "
         << yices_term_constructor(c->hash()) << " op = " << c->get_op()
         << " yices_term_is_bitvector " << yices_term_is_bitvector(c->hash())
         << endl;
    for (auto d : c)
    {
      cout << "d " << d << " term constructor "
           << yices_term_constructor(d->hash()) << " op = " << d->get_op()
           << " yices_term_is_bitvector " << yices_term_is_bitvector(d->hash())
           << endl;
    }
  }

  Term bvextr = Term(new Yices2Term(yices_bvextract(x->hash(), 6, 6)));
  // (bool-to-bv (bit x 6))
  cout << "bvextr " << bvextr << endl;

  t = s->make_term(BVAnd, x, s->make_term(BVNot, y));
  yt = std::static_pointer_cast<Yices2Term>(t);
  Term q = *yt->begin();
  cout << "t " << t << " term constructor " 
    <<  yices_term_constructor(yt->hash())
   << " op = " << t->get_op() <<  " yices_term_is_bitvector " 
   << yices_term_is_bitvector (q->hash()) <<  endl;






  t = s->make_term(BVOr, x, y);
  yt = std::static_pointer_cast<Yices2Term>(t);
  cout << "t " << t << " term constructor " <<  yices_term_constructor(yt->hash()) << " op = " << t->get_op() << endl;
  t = s->make_term(BVXor, x, y);
  yt = std::static_pointer_cast<Yices2Term>(t);
  cout << "t " << t << " term constructor " <<  yices_term_constructor(yt->hash()) << " op = " << t->get_op() << endl;
  t = s->make_term(BVNand, x, y);
  yt = std::static_pointer_cast<Yices2Term>(t);
  cout << "t " << t << " term constructor " <<  yices_term_constructor(yt->hash()) << " op = " << t->get_op() << endl;
  t = s->make_term(BVNor, x, y);
  yt = std::static_pointer_cast<Yices2Term>(t);
  cout << "t " << t << " term constructor " <<  yices_term_constructor(yt->hash()) << " op = " << t->get_op() << endl;
  t = s->make_term(BVXnor, x, y);
  yt = std::static_pointer_cast<Yices2Term>(t);
  cout << "t " << t << " term constructor " <<  yices_term_constructor(yt->hash()) << " op = " << t->get_op() << endl;
  // t = s->make_term(BVComp, x, y);
  // yt = std::static_pointer_cast<Yices2Term>(t);
  // cout << "t " << t << " term constructor " <<  yices_term_constructor(yt->hash()) << " op = " << t->get_op() << endl;

















  assert(x != y);
  Term x_copy = x;
  assert(x == x_copy);

  // check sorts
  Sort xsort = x->get_sort();
  Sort ysort = y->get_sort();
  assert(xsort == ysort);

  Sort arr_sort = s->make_sort(ARRAY, s->make_sort(BV, 4), bvsort8);
  assert(xsort != arr_sort);
  assert(xsort != arr_sort->get_indexsort());
  assert(xsort == arr_sort->get_elemsort());

  Term xpy = s->make_term(BVAdd, x, y);
  Term z_eq_xpy = s->make_term(Equal, z, xpy);

  assert(x->is_symbolic_const());
  assert(!xpy->is_symbolic_const());

  Op ext30 = Op(Extract, 3, 0);
  Term x_lower = s->make_term(ext30, x);
  Term x_ext = s->make_term(Op(Zero_Extend, 4), x_lower);

  Sort funsort =
      s->make_sort(FUNCTION, SortVec{ x_lower->get_sort(), x->get_sort() });
  Term uf = s->make_symbol("f", funsort);
  cout << "uf: " << uf << " sort : " << uf->get_sort()  << " op " << uf->get_op() << endl;
  Term uf_app = s->make_term(Apply, uf, x_lower);
  assert(uf_app->get_op() == Apply);
  assert(*uf_app->begin() == uf);
  
  // TODO: this assertion fails... 
  // assert((*uf_app->begin())->get_sort()->get_sort_kind() == FUNCTION);
  for (auto c : uf_app)
  {
    cout << "c sort  = " << c->get_sort()->to_string() << endl;

  }
  cout << "(*uf_app->begin())->get_sort() = " << (*uf_app->begin())->get_sort()->to_string() << endl;
  cout << "funsort " << funsort->to_string() << endl;

  assert(uf->get_sort() == funsort);
  assert(uf->get_sort() != uf_app->get_sort());

  s->assert_formula(z_eq_xpy);

  s->assert_formula(s->make_term(BVUlt, x, s->make_term(4, bvsort8)));
  s->assert_formula(s->make_term(BVUlt, y, s->make_term(4, bvsort8)));
  s->assert_formula(s->make_term(BVUgt, z, s->make_term("5", bvsort8)));
  // This is actually a redundant assertion -- just testing
  s->assert_formula(s->make_term(Equal, x_ext, x));
  s->assert_formula(s->make_term(Distinct, x, z));
  s->assert_formula(s->make_term(BVUle, uf_app, s->make_term(3, bvsort8)));
  s->assert_formula(
      s->make_term(BVUge, uf_app, s->make_term("00000011", bvsort8, 2)));

  Result r = s->check_sat();
  assert(r.is_sat());

  Term xc = s->get_value(x);
  Term yc = s->get_value(y);
  Term zc = s->get_value(z);
  Term x_extc = s->get_value(x_ext);
  Term x_lowerc = s->get_value(x_lower);
  Term uf_appc = s->get_value(uf_app);

  cout << "Got the following values:" << endl;
  cout << "\t" << x << " " << xc << endl;
  cout << "\t" << y << " " << yc->to_int() << endl;
  cout << "\t" << z << " " << zc->to_int() << endl;
  cout << "\t" << x_lower << " " << x_lowerc->to_int() << endl;
  cout << "\t" << x_ext << " " << x_extc->to_int() << endl;
  cout << "\t" << uf_app << " " << uf_appc->to_int() << endl;
  return 0;
}
