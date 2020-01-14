#include <sstream>

#include "yices2_sort.h"

#include "exceptions.h"

using namespace std;

namespace smt {

// Yices2Sort implementation

// TODO
std::size_t Yices2Sort::hash() const
{
    throw NotImplementedException("Todo item.");
  
}

uint64_t Yices2Sort::get_width() const
{
  size_t out_width;
  if (yices_type_is_bitvector(type))
  {
    return (unsigned int)yices_bvtype_size(type);
  }
  else
  {
    throw IncorrectUsageException("Can only get width from bit-vector sort");
  }
}

Sort Yices2Sort::get_indexsort() const
{
  if (yices_type_is_function(type))
  {
    return Sort(new Yices2Sort(idx_type));
  }
  else
  {
    throw IncorrectUsageException("Can only get index sort from array sort");
  }
}

Sort Yices2Sort::get_elemsort() const
{
  if (yices_type_is_function(type))
  {
    return Sort(new Yices2Sort(elem_type));
  }
  else
  {
    throw IncorrectUsageException("Can only get element sort from array sort");
  }
}

SortVec Yices2Sort::get_domain_sorts() const
{
  throw NotImplementedException("Todo item.");
}

Sort Yices2Sort::get_codomain_sort() const
{
  throw NotImplementedException("Todo item.");
}

bool Yices2Sort::compare(const Sort s) const
{
  throw NotImplementedException("Todo item.");
}

SortKind Yices2Sort::get_sort_kind() const
{
  if (yices_type_is_int(type))
  {
    return INT;
  }
  else if (yices_type_is_real(type))
  {
    return REAL;
  }
  else if (yices_type_is_bool(type))
  {
    return BOOL;
  }
  else if (yices_type_is_bitvector(type))
  {
    return BV;
  }
  else if (yices_type_is_function(type))
  {
    //test if ARRAY or actually function... 
    //just trying to get arrays to work for now.
    return ARRAY;
  }
  // TODO: ARRAY, FUNCTION
  //  In Yices, arrays are the same as functions, e.g.
  // an array indexed by integers is simply a function of domain int
  // else if (msat_is_array_type(env, type, nullptr, nullptr))
  // {
  //   return ARRAY;
  // }
  // else if (is_uf_type)
  // {
  //   return FUNCTION;
  // }
  else
  {
    throw NotImplementedException("Unknown Yices2 type.");
  }
}

}  // namespace smt
