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

// TODO: ask Makai about this test. :) 

int main()
{
  SmtSolver s = Yices2SolverFactory::create();
  s->set_logic("QF_ABV");
  s->set_opt("produce-models", "true");
  Sort bvsort4 = s->make_sort(BV, 4);
  Sort bvsort8 = s->make_sort(BV, 8);
  Sort array4_8 = s->make_sort(ARRAY, bvsort4, bvsort8);

  Term idx = s->make_symbol("idx", bvsort4);
    // cout << "idx " << idx->hash() << endl;

  Term x = s->make_symbol("x", bvsort8);
    // cout << "x " << x->hash() << endl;

  Term y = s->make_symbol("y", bvsort8);
    // cout << "y " << y->hash() << endl;

  Term z = s->make_symbol("z", bvsort8);
    // cout << "z " << z->hash() << endl;

  Term arr = s->make_symbol("arr", array4_8);
    // cout << "arr " << arr->hash() << endl;


  UnorderedTermSet orig_set = { idx, x, y, z, arr };

  Term idx0 = s->make_symbol("idx0", bvsort4);
  Term x0 = s->make_symbol("x0", bvsort8);
  Term y0 = s->make_symbol("y0", bvsort8);
  Term z0 = s->make_symbol("z0", bvsort8);
  Term arr0 = s->make_symbol("arr0", array4_8);

  UnorderedTermSet timed_set = { idx0, x0, y0, z0, arr0 };

  Term a = s->make_term(Select, arr, idx);
    // cout << "a " << a->hash() << endl;

  Term b = s->make_term(BVMul, y, z);
    // cout << "b " << b->hash() << endl;
  Term d = s->make_term(BVAdd, x, b);
    // cout << "d " << d->hash() << endl;

  Term constraint =
      s->make_term(Equal,
                   a,
                   d);
    // cout << "constraint " << constraint->hash() << endl;


  UnorderedTermSet visited;
  TermVec to_visit({ constraint });
  Term t;
  int num_consts = 0;
  while (to_visit.size())
  {
    t = to_visit.back();
    // cout << "t " << t << endl;
    to_visit.pop_back();
    if (visited.find(t) == visited.end())
    {
      // cout << " visited. find == end" << endl;
      visited.insert(t);
      for (auto c : t)
      {
        // cout << " c " << c << endl;
        to_visit.push_back(c);
      }

      if (t->is_symbolic_const() || t->is_value())
      {
        num_consts++;
        cout << "checking " << t << endl;
        assert(orig_set.find(t) != orig_set.end());
      }
    }
  }
  assert(num_consts == orig_set.size());

  cout << endl;

  Term timed_constraint = s->substitute(
      constraint,
      UnorderedTermMap{
          { idx, idx0 }, { x, x0 }, { y, y0 }, { z, z0 }, { arr, arr0 } });

  visited.clear();
  to_visit.clear();
  to_visit.push_back(timed_constraint);
  num_consts = 0;
  while (to_visit.size())
  {
    t = to_visit.back();
    to_visit.pop_back();
    if (visited.find(t) == visited.end())
    {
      visited.insert(t);
      for (auto c : t)
      {
        to_visit.push_back(c);
      }

      if (t->is_symbolic_const())
      {
        num_consts++;
        cout << "checking " << t << endl;
        assert(timed_set.find(t) != timed_set.end());
      }
    }
  }
  assert(num_consts == timed_set.size());

  return 0;
}
