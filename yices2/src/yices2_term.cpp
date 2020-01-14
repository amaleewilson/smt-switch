#include "yices.h"
#include "yices2_term.h"
#include "yices2_sort.h"

#include "exceptions.h"
#include "ops.h"

#include <unordered_map>

using namespace std;

namespace smt {


// Yices2TermIter implementation

Yices2TermIter::Yices2TermIter(const Yices2TermIter & it)
{
  // term = it.term;
  // pos = it.pos;
  throw NotImplementedException(
      "Yices2Term function not implemented yet."); 
}

Yices2TermIter & Yices2TermIter::operator=(const Yices2TermIter & it)
{
  // term = it.term;
  // pos = it.pos;
  // return *this;
  throw NotImplementedException(
      "Yices2Term function not implemented yet."); 
}

void Yices2TermIter::operator++() { pos++; }

const Term Yices2TermIter::operator*()
{
  // if (!pos && msat_term_is_uf(env, term))
  // {
  //   return Term(new Yices2Term(env, msat_term_get_decl(term)));
  // }
  // else
  // {
  //   uint32_t actual_idx = pos;
  //   if (msat_term_is_uf(env, term))
  //   {
  //     actual_idx--;
  //   }
  //   return Term(new Yices2Term(env, msat_term_get_arg(term, actual_idx)));
  // }
  throw NotImplementedException(
      "Yices2Term function not implemented yet."); 
}

bool Yices2TermIter::operator==(const Yices2TermIter & it)
{
  // return ((msat_term_id(term) == msat_term_id(it.term)) && (pos == it.pos));
  throw NotImplementedException(
      "Yices2Term function not implemented yet."); 
}

bool Yices2TermIter::operator!=(const Yices2TermIter & it)
{
  // return ((msat_term_id(term) != msat_term_id(it.term)) || (pos != it.pos));
  throw NotImplementedException(
      "Yices2Term function not implemented yet."); 
}

bool Yices2TermIter::equal(const TermIterBase & other) const
{
  // const Yices2TermIter & cti = static_cast<const Yices2TermIter &>(other);
  // return ((msat_term_id(term) == msat_term_id(cti.term)) && (pos == cti.pos));
  throw NotImplementedException(
      "Yices2Term function not implemented yet."); 
}

// end Yices2TermIter implementation

// Yices2Term implementation

size_t Yices2Term::hash() const
{
  // if (!is_uf)
  // {
  //   return msat_term_id(term);
  // }
  // else
  // {
  //   return msat_decl_id(decl);
  // }
  throw NotImplementedException(
      "Yices2Term function not implemented yet."); 
}

bool Yices2Term::compare(const Term & absterm) const
{
  // shared_ptr<Yices2Term> mterm = std::static_pointer_cast<Yices2Term>(absterm);
  // if (is_uf ^ mterm->is_uf)
  // {
  //   // can't be equal if one is a uf and the other is not
  //   return false;
  // }
  // else if (!is_uf)
  // {
  //   return (msat_term_id(term) == msat_term_id(mterm->term));
  // }
  // else
  // {
  //   return (msat_decl_id(decl) == msat_decl_id(mterm->decl));
  // }
  throw NotImplementedException(
      "Yices2Term function not implemented yet."); 
}

Op Yices2Term::get_op() const
{
  throw NotImplementedException(
      "Yices2Term function not implemented yet."); 
}

Sort Yices2Term::get_sort() const
{
  // if (!is_uf)
  // {
  //   return Sort(new MsatSort(env, msat_term_get_type(term)));
  // }
  // else
  // {
  //   // need to reconstruct the function type
  //   vector<msat_type> param_types;
  //   size_t arity = msat_decl_get_arity(decl);
  //   param_types.reserve(arity);
  //   for (size_t i = 0; i < arity; i++)
  //   {
  //     param_types.push_back(msat_decl_get_arg_type(decl, i));
  //   }

  //   if (!param_types.size())
  //   {
  //     throw InternalSolverException("Expecting non-zero arity for UF.");
  //   }

  //   msat_type funtype = msat_get_function_type(env,
  //                                              &param_types[0],
  //                                              param_types.size(),
  //                                              msat_decl_get_return_type(decl));

  //   return Sort(new MsatSort(env, funtype, decl));
  // }
  throw NotImplementedException(
      "Yices2Term function not implemented yet."); 
}

bool Yices2Term::is_symbolic_const() const
{
  // a symbolic constant is a term with no children and no built-in
  // interpretation
  // return (
  //     (msat_term_arity(term) == 0)
  //     && (msat_decl_get_tag(env, msat_term_get_decl(term)) == MSAT_TAG_UNKNOWN)
  //     && !msat_term_is_number(env, term));

  // good idea??? 
  // return yices_type_is_bool()
  ////// WiP monday: 
  // return yices_type_is_bool(this->getsort->type) || yices_type_is_int .. etc

  // throw NotImplementedException(
  //     "Yices2Term function not implemented yet."); 
}

bool Yices2Term::is_value() const
{
  // value if it has no children and a built-in interpretation
  // return (msat_term_is_number(env, term) || msat_term_is_true(env, term)
  //         || msat_term_is_false(env, term) ||
  //         // constant arrays are considered values in smt-switch
  //         msat_term_is_array_const(env, term));
  throw NotImplementedException(
      "Yices2Term function not implemented yet."); 
}

string Yices2Term::to_string() const
{

  std::string sres = yices_term_to_string(term, 120, 1, 0);
  return sres;

  // if (is_uf)
  // {
  //   if (MSAT_ERROR_DECL(decl))
  //   {
  //     throw SmtException("Can't get representation for MathSAT error decl!");
  //   }
  //   return msat_decl_repr(decl);
  // }
  // else
  // {
  //   if (MSAT_ERROR_TERM(term))
  //   {
  //     throw SmtException("Can't get representation for MathSAT error term!");
  //   }
  //   char * s = msat_to_smtlib2_term(env, term);
  //   string res = s;
  //   msat_free(s);
  //   return res;
  // }
}

uint64_t Yices2Term::to_int() const
{
  // char * s = msat_to_smtlib2_term(env, term);
  // std::string val = s;
  // msat_free(s);
  // bool is_bv = msat_is_bv_type(env, msat_term_get_type(term), nullptr);

  // // process smt-lib bit-vector format
  // if (is_bv)
  // {
  //   if (val.find("(_ bv") == std::string::npos)
  //   {
  //     std::string msg = val;
  //     msg += " is not a constant term, can't convert to int.";
  //     throw IncorrectUsageException(msg.c_str());
  //   }
  //   val = val.substr(5, val.length());
  //   val = val.substr(0, val.find(" "));
  // }

  // try
  // {
  //   return std::stoi(val);
  // }
  // catch (std::exception const & e)
  // {
  //   std::string msg("Term ");
  //   msg += val;
  //   msg += " does not contain an integer representable by a machine int.";
  //   throw IncorrectUsageException(msg.c_str());
  // }
  throw NotImplementedException(
      "Yices2Term function not implemented yet."); 
}

TermIter Yices2Term::begin() { 
  // return TermIter(new Yices2TermIter(env, term, 0)); 
  throw NotImplementedException(
      "Yices2Term function not implemented yet."); 
}

TermIter Yices2Term::end()
{
  // uint32_t arity = msat_term_arity(term);
  // if (msat_term_is_uf(env, term))
  // {
  //   // consider the function itself a child
  //   arity++;
  // }
  // return TermIter(new Yices2TermIter(env, term, arity));
  throw NotImplementedException(
      "Yices2Term function not implemented yet."); 
}

// end Yices2Term implementation

}  // namespace smt
