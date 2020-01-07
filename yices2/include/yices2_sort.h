#ifndef SMT_YICES2_SORT_H
#define SMT_YICES2_SORT_H

#include "exceptions.h"
#include "sort.h"
#include "utils.h"

#include "yices.h"

namespace smt {

// forward declaration
class Yices2Solver;

class Yices2SortBase : public AbsSort
{
 public:
  Yices2SortBase(SortKind sk)
      : sk(sk) {};
  // virtual ~Yices2SortBase();
  std::size_t hash() const override;
  uint64_t get_width() const override;
  Sort get_indexsort() const override;
  Sort get_elemsort() const override;
  SortVec get_domain_sorts() const override;
  Sort get_codomain_sort() const override;
  bool compare(const Sort s) const override;
  SortKind get_sort_kind() const override { return sk; };

 protected:
  SortKind sk;

  friend class Yices2Solver;
};

/** The boolector C API doesn't support querying sorts for width, etc...
    (in boolector asking for the width is done on a node, i.e. Term, rather than
   a sort) Thus, we need to track some extra information for implementing
   AbsSort. To make this simpler, we have unique classes for each sort.
 */

class Yices2BVSort : public Yices2SortBase
{
 public:
  Yices2BVSort( uint64_t w)
      : Yices2SortBase(BV), width(w){};
  uint64_t get_width() const override { return width; };

 protected:
  // bit-vector width
  // boolector Note: we need to store this in addition to the boolectorSort
  //       because in boolector the width is retrieved from a node not a sort
  unsigned width;

  friend class Yices2Solver;
};

// class Yices2ArraySort : public Yices2SortBase
// {
//  public:
//   Yices2ArraySort(Btor * b, Yices2Sort s, Sort is, Sort es)
//     : Yices2SortBase(ARRAY, b, s), indexsort(is), elemsort(es) {};
//   Sort get_indexsort() const override { return indexsort; };
//   Sort get_elemsort() const override { return elemsort; };

//  protected:
//   Sort indexsort;
//   Sort elemsort;

//   friend class Yices2Solver;
// };

// class Yices2UFSort : public Yices2SortBase
// {
//  public:
//   Yices2UFSort(Btor * b, Yices2Sort s, SortVec sorts, Sort sort)
//       : Yices2SortBase(FUNCTION, b, s),
//         domain_sorts(sorts),
//         codomain_sort(sort){};
//   SortVec get_domain_sorts() const override { return domain_sorts; };
//   Sort get_codomain_sort() const override { return codomain_sort; };

//  protected:
//   SortVec domain_sorts;
//   Sort codomain_sort;

//   friend class Yices2Solver;
// };
}  // namespace smt

#endif
