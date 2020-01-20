#ifndef SMT_YICES2_SORT_H
#define SMT_YICES2_SORT_H

#include "exceptions.h"
#include "sort.h"
#include "utils.h"

#include "yices.h"

namespace smt {

// forward declaration
class Yices2Solver;

class Yices2Sort : public AbsSort
{
 public:
  Yices2Sort(type_t y_type)
      : type(y_type), is_function(false) {};
  Yices2Sort(type_t y_type, type_t idx_t, type_t elem_t)
      : type(y_type), idx_type(idx_t), elem_type(elem_t), is_function(false) {};
  Yices2Sort(type_t y_type, bool is_fun)
      : type(y_type), is_function(is_fun) {};
  Yices2Sort(type_t y_type, bool is_fun, uint64_t sa)
      : type(y_type), is_function(is_fun), sort_arity(sa) {};
  ~Yices2Sort() = default;
  std::size_t hash() const override;
  uint64_t get_width() const override;
  Sort get_indexsort() const override;
  Sort get_elemsort() const override;
  SortVec get_domain_sorts() const override;
  Sort get_codomain_sort() const override;
  bool compare(const Sort s) const override;
  SortKind get_sort_kind() const override;
  type_t get_ytype() { return type; } ;

 protected:
  type_t type; 
  type_t idx_type;
  type_t elem_type;
  bool is_function;
  uint64_t sort_arity;

  friend class Yices2Solver;
};

}  // namespace smt

#endif
