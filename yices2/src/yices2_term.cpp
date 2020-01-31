#include "gmp.h"
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
  term = it.term;
  pos = it.pos;
}

Yices2TermIter & Yices2TermIter::operator=(const Yices2TermIter & it)
{
  term = it.term;
  pos = it.pos;
  return *this;
}

void Yices2TermIter::operator++() { pos++; }

void print_all_answers(term_t t)
{
  term_constructor_t tc = yices_term_constructor(t);
  cout << " asking for term " << Term(new Yices2Term(t))->to_string() << endl;
  // cout << " term op " << Term(new Yices2Term(t))->get_op() << endl;
  // cout << "num_children?  " << yices_term_num_children(t) << endl; 
  // cout << "yices_term_is_function " << yices_term_is_function(t) << endl; 
  // cout << "yices_term_is_bvsum " << yices_term_is_bvsum(t) << endl; 
  // cout << "yices_term_is_product " << yices_term_is_product(t) << endl; 
  // cout << "yices_term_is_sum " << yices_term_is_sum(t) << endl; 
  // cout << "yices_term_is_composite " << yices_term_is_composite(t) << endl; 

  cout << "is YICES_SELECT_TERM ? " << (tc == YICES_SELECT_TERM) << endl;
  cout << "is YICES_BIT_TERM ? " << (tc == YICES_BIT_TERM) << endl;
  cout << "is YICES_BV_SUM ? " << (tc == YICES_BV_SUM) << endl;
  cout << "is YICES_ARITH_SUM ? " << (tc == YICES_ARITH_SUM) << endl;
  cout << "is YICES_POWER_PRODUCT ? " << (tc == YICES_POWER_PRODUCT) << endl;
  cout << "is YICES_BV_CONSTANT ? " << (tc == YICES_BV_CONSTANT) << endl;
  cout << "is YICES_ARITH_CONSTANT ? " << (tc == YICES_ARITH_CONSTANT) << endl;
  cout << "is YICES_UNINTERPRETED_TERM ? " << (tc == YICES_UNINTERPRETED_TERM) << endl;

  cout << " tc = " << tc << endl;

  cout << " errors? " << yices_error_string() << endl;
}

const Term Yices2TermIter::operator*()
{
  term_constructor_t tc = yices_term_constructor(term);

  if (yices_term_is_function(term))
  {
    cout << "function" << endl;
    if (!pos)
    {
      return Term(new Yices2Term(term));
    }

    else
    {
      uint32_t actual_idx = pos - 1;
      return Term(new Yices2Term(yices_term_child(term, actual_idx))); 
    }
  }
  else if (yices_term_is_bvsum(term)) 
  {
    cout << "bvsum" << endl;
    cout << ": " << yices_error_string() << endl;

    term_t component;
    uint64_t w = (Term(new Yices2Term(term))->get_sort()->get_width());
    int32_t val[w]; 
    yices_bvsum_component(term, pos, val, &component);
    cout << ": " << yices_error_string() << endl;

    return Term(new Yices2Term(yices_bvmul(yices_bvconst_from_array(w, val) , component)));
    // return Term(new Yices2Term(component));
  }
  else if (yices_term_is_product(term))
  {
    cout << "product" << endl;
    term_t component;
    uint32_t exp; // todo: make this equal to the size of the bv
    if (yices_term_num_children(term) == 1)
    {
      if (!pos)
      {
        yices_product_component(term, pos, &component, &exp);
        return Term(new Yices2Term(component));
      }
      else
      {
        print_all_answers(yices_int64(exp));
        yices_product_component(term, pos-1, &component, &exp);
        return Term(new Yices2Term(yices_int64(exp)));
      }
    }
    
    if (exp != 1)
    {
      return Term(new Yices2Term(yices_power(component, exp)));
    }
    else 
    {
      return Term(new Yices2Term(component));
    }
  }
  else if (yices_term_is_sum(term))
  {
    cout << "sum" << endl;
    cout << ": " << yices_error_string() << endl;

    term_t component;
    mpq_t coeff;
    mpq_init(coeff);

    // Special case for term components like (* -1 b)
    if (yices_term_num_children(term) == 1)
    {
      if (!pos)
      {
        yices_sum_component(term, pos, coeff, &component);
    cout << ": " << yices_error_string() << endl;

        return Term(new Yices2Term(yices_mpq(coeff)));
      }
      else
      {
        yices_sum_component(term, pos-1, coeff, &component);
    cout << ": " << yices_error_string() << endl;

        return Term(new Yices2Term(component));
      }
    }
    else 
    {
      yices_sum_component(term, pos, coeff, &component);

      return Term(new Yices2Term(yices_mul(component, yices_mpq(coeff))));
    }

  }
  else if (yices_term_is_composite(term))
  {

    uint32_t actual_idx = pos;
    return Term(new Yices2Term(yices_term_child(term, actual_idx)));
  }
  else
  {
    return Term(new Yices2Term(term)); 
  }

}

bool Yices2TermIter::operator==(const Yices2TermIter & it)
{
  return ((term == it.term) && (pos == it.pos));
}

bool Yices2TermIter::operator!=(const Yices2TermIter & it)
{
  return ((term != it.term) || (pos != it.pos));
}

bool Yices2TermIter::equal(const TermIterBase & other) const
{
  const Yices2TermIter & cti = static_cast<const Yices2TermIter &>(other);
  return ((term == cti.term) && (pos == cti.pos));
}

// end Yices2TermIter implementation

// Yices2Term implementation

size_t Yices2Term::hash() const
{
  // term_t is a unique id for the term.
  return term;
}

bool Yices2Term::compare(const Term & absterm) const
{
  shared_ptr<Yices2Term> yterm = std::static_pointer_cast<Yices2Term>(absterm);
  return term == yterm->term;
}

Op Yices2Term::get_op() const
{
  term_constructor_t tc = yices_term_constructor(term);
  std::string sres;
  switch (tc) 
  {
    // atomic terms
    case YICES_BOOL_CONSTANT:
    case YICES_ARITH_CONSTANT:
    case YICES_BV_CONSTANT:
    case YICES_SCALAR_CONSTANT:
    case YICES_VARIABLE:
    case YICES_UNINTERPRETED_TERM:
      return Op();
    // composite terms
    case YICES_ITE_TERM:
      return Op(Ite);
    case YICES_APP_TERM:
      return Op(Apply);
    case YICES_UPDATE_TERM:
      return Op();
    case YICES_TUPLE_TERM:
      return Op();
    case YICES_EQ_TERM:
      return Op(Equal);
    case YICES_DISTINCT_TERM:
      return Op(Distinct);
    case YICES_FORALL_TERM:
      return Op();
    case YICES_LAMBDA_TERM:
      return Op();
    case YICES_NOT_TERM: 
      return Op(Not);
    case YICES_OR_TERM:
      return Op(Or);
    case YICES_XOR_TERM:
      return Op(Xor);
    case YICES_BV_ARRAY:
      return Op();
    case YICES_BV_DIV:
      return Op(BVUdiv);
    case YICES_BV_REM:
      return Op(BVUrem);
    case YICES_BV_SDIV:
      return Op(BVSdiv);
    case YICES_BV_SREM:
      return Op(BVSrem);
    case YICES_BV_SMOD:
      return Op(BVSmod);
    case YICES_BV_SHL:
      return Op(BVShl);
    case YICES_BV_LSHR:
      return Op(BVLshr);
    case YICES_BV_ASHR:
      return Op(BVAshr);
    case YICES_BV_GE_ATOM:
      return Op(BVUge);
    case YICES_BV_SGE_ATOM:
      return Op(BVSge);
    case YICES_ARITH_GE_ATOM:
      return Op(Ge);
    case YICES_ABS:
      return Op(Abs);
    case YICES_ARITH_ROOT_ATOM:
      return Op();
    case YICES_CEIL:
      return Op();
    case YICES_FLOOR:
      return Op();
    case YICES_RDIV:
      return Op(Div);
    case YICES_IDIV:
      return Op(Div);
    case YICES_IMOD:
      return Op(Mod);
    case YICES_IS_INT_ATOM:
      return Op();
    case YICES_DIVIDES_ATOM:
      return Op();
    // projections
    case YICES_SELECT_TERM:
      return Op(Select);
    case YICES_BIT_TERM:
      return Op();
    // // sums
    case YICES_BV_SUM:
      return Op(BVAdd);
    case YICES_ARITH_SUM:
      /* Arithmetic sums are represented as polynomials, 
       * and something like (+ a (-b)) is actually 
       * (+ a (* -1 b)), but the individual component
       * (* -1 b) is still of type YICES_ARITH_SUM. To transfer this
       * term, you need to construct the multiply. 
       */
      sres = this->to_string();
      sres = sres.substr(sres.find("(")+1, sres.length());
      sres = sres.substr(0, sres.find(" "));
      if (yices_term_num_children(term) == 1 && sres == "*")
      {
        return Op(Mult);
      }
      return Op(Plus);
    // products
    case YICES_POWER_PRODUCT:
      sres = this->to_string();
      sres = sres.substr(sres.find("(")+1, sres.length());
      sres = sres.substr(0, sres.find(" "));
      if (sres == "bv-mul")
      {
        return Op(BVMul);
      }
      if (sres == "*")
      {
        return Op(Mult);
      }
      return Op(Pow);
    default:
      return Op();
  }
}

Sort Yices2Term::get_sort() const
{

  return Sort(new Yices2Sort(yices_type_of_term(term)));
  // Reconstruct functions? 


  // TODO: need to support functions/arrays. 

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
  // throw NotImplementedException(
  //     "Yices2Term function not implemented yet."); 
}

bool Yices2Term::is_symbolic_const() const
{
  term_constructor_t tc = yices_term_constructor(term);
  return ((tc == YICES_UNINTERPRETED_TERM && 
          yices_term_num_children(term) == 0)) ;
}

bool Yices2Term::is_value() const
{
  term_constructor_t tc = yices_term_constructor(term);

  return (tc == YICES_BOOL_CONSTANT || 
          tc == YICES_ARITH_CONSTANT ||
          tc == YICES_BV_CONSTANT ||
          tc == YICES_SCALAR_CONSTANT);
}

string Yices2Term::to_string() const
{
  // shared_ptr<Yices2Term> yterm0 = static_pointer_cast<Yices2Term>(f);

  // shared_ptr<Yices2Term> yterm0 = static_pointer_cast<Yices2Term>(*this);
  Term nu_term = Term(new Yices2Term(this->hash()));


  std::string sres = yices_term_to_string(nu_term->hash(), 120, 1, 0);
  return sres;
  // TODO: error handling? 

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
  std::string val = yices_term_to_string(term, 120, 1, 0);


  // process bit-vector format
  if (yices_term_is_bitvector(term))
  {
    if (val.find("0b") == std::string::npos)
    {
      std::string msg = val;
      msg += " is not a constant term, can't convert to int.";
      throw IncorrectUsageException(msg.c_str());
    }
    try
    {
      return std::stoi(val.substr(val.find("b")+1, val.length()), 0, 2);
    }
    catch (std::exception const & e)
    {
      std::string msg("Term ");
      msg += val;
      msg += " does not contain an integer representable by a machine int.";
      throw IncorrectUsageException(msg.c_str());
    }
  }

  try
  {
    return std::stoi(val);
  }
  catch (std::exception const & e)
  {
    std::string msg("Term ");
    msg += val;
    msg += " does not contain an integer representable by a machine int.";
    throw IncorrectUsageException(msg.c_str());
  }

}

TermIter Yices2Term::begin() { 
  return TermIter(new Yices2TermIter(term, 0)); 
}

TermIter Yices2Term::end()
{

  if (yices_term_num_children(term) < 0)
  {
    // cout << term << endl;
    // cout << yices_error_string() << endl;
  }

  if (this->get_op() == Mult && yices_term_num_children(term) == 1)
  {
    return TermIter(new Yices2TermIter(term, yices_term_num_children(term) + 1));
  }
  if (this->get_op() == Pow && yices_term_num_children(term) == 1)
  {
    return TermIter(new Yices2TermIter(term, yices_term_num_children(term) + 1));
  }

  return TermIter(new Yices2TermIter(term, yices_term_num_children(term)));

}

// end Yices2Term implementation

}  // namespace smt
