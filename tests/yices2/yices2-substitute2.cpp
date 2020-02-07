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
  s->set_opt("produce-models", "true");
  Sort bvsort8 = s->make_sort(BV, 8);
  Term x = s->make_symbol("x", bvsort8);
  Term y = s->make_symbol("y", bvsort8);
  Term z = s->make_symbol("z", bvsort8);
  Term T = s->make_term(true);

  Term a = s->make_symbol("a", s->make_sort(INT));
  Term b = s->make_symbol("b", s->make_sort(INT));

  Term constraint = s->make_term(Equal, z, s->make_term(BVAdd, x, y));
  constraint = s->make_term(And, constraint, s->make_term(Lt, a, b));
  cout << constraint << endl;
  s->assert_formula(constraint);



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
        // assert(orig_set.find(t) != orig_set.end());
      }
    }
  }
  // assert(num_consts == orig_set.size());

  // cout << endl;

  // Term timed_constraint = s->substitute(
  //     constraint,
  //     UnorderedTermMap{
  //         { idx, idx0 }, { x, x0 }, { y, y0 }, { z, z0 }, { arr, arr0 } });

  // visited.clear();
  // to_visit.clear();
  // to_visit.push_back(timed_constraint);
  // num_consts = 0;
  // while (to_visit.size())
  // {
  //   t = to_visit.back();
  //   to_visit.pop_back();
  //   if (visited.find(t) == visited.end())
  //   {
  //     visited.insert(t);
  //     for (auto c : t)
  //     {
  //       to_visit.push_back(c);
  //     }

  //     if (t->is_symbolic_const())
  //     {
  //       num_consts++;
  //       cout << "checking " << t << endl;
  //       assert(timed_set.find(t) != timed_set.end());
  //     }
  //   }
  // }
  // assert(num_consts == timed_set.size());

  return 0;
}
