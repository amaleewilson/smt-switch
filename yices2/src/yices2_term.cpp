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
  // cout << "Yices2TermIter::Yices2TermIter(const Yices2TermIter & it)" << endl;
  term = it.term;
  pos = it.pos;
}

Yices2TermIter & Yices2TermIter::operator=(const Yices2TermIter & it)
{
  // cout << "" << endl;

  term = it.term;
  pos = it.pos;
  return *this;
}

void Yices2TermIter::operator++() { 
  // cout << "++" << endl;

  pos++; }

const Term Yices2TermIter::operator*()
{
  term_constructor_t tc = yices_term_constructor(term);

  if (! yices_term_is_composite (term) && !yices_term_is_function(term)){
    // cout << " is bv sum? " << (tc == YICES_BV_SUM) << endl;
    cout << " not composite : " << (Term(new Yices2Term(term)))->to_string() << endl;
    // cout << " yices_term_is_tuple : " << yices_term_is_tuple(term) << endl;
    // cout << " yices_term_is_product(term) : " << yices_term_is_product(term) << endl;
    // cout << " yices_term_bitsize : " << yices_term_bitsize(term) << endl;
    // cout << "  yices_term_is_ground : " <<  yices_term_is_ground(term) << endl;
    // cout << " yices_term_is_scalar : " << yices_term_is_scalar(term) << endl;
    // cout << " yices_term_is_bitvector : " << yices_term_is_bitvector(term) << endl;
    // cout << " yices_term_is_arithmetic : " << yices_term_is_arithmetic(term) << endl;
    // cout << " type : " << yices_type_of_term(term) << endl;
    // cout << "num children " << yices_term_num_children(term) << endl;
    // cout << " child 0 " << yices_term_child(term, 0) << endl;
    // cout << " child 1 " << yices_term_child(term, 1) << endl;
    // cout << " child 2 " << yices_term_child(term, 2) << endl;
    // return Term(new Yices2Term((term)));
  }

  if (yices_term_is_function(term))
  {
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
    // cout << "term is bvsum " << endl; 
    term_t component;
    int32_t i = pos;
    int32_t val[100]; // todo: make this equal to the size of the bv
    yices_bvsum_component(term, i, val, &component);
    return Term(new Yices2Term(component));
  }
  else if (yices_term_is_product(term))
  {
    cout << "term is product " << endl;
    term_t component;
    int32_t i = pos;
    uint32_t exp; // todo: make this equal to the size of the bv
    yices_product_component(term, i, &component, &exp);
    return Term(new Yices2Term(component));
  }
  else if (yices_term_is_sum(term))
  {
    cout << "term is sum, pos = " << pos << endl;
    term_t component;
    int32_t i = pos;
    mpq_t coeff;
    mpq_init(coeff);
    // = {1};

    // cout << "child count: " << yices_term_num_children(term) << endl;
    // cout << "yices_term_is_arithmetic: " << yices_term_is_arithmetic(term) << endl;

    // term_t test_child = yices_term_child(term, pos);

    yices_sum_component(term, i, coeff, &component);
    // cout << "component called = " << endl;
    // cout << "component after call = " << component << endl;

    // cout << "coeff " << coeff << endl;
    gmp_printf ("%s is an mpz %Qd\n", "here", coeff);
    cout << "component " << (Term(new Yices2Term(component)))->to_string() << endl;

    if (mpq_cmp_si(coeff,1,1) != 0)
    {
      cout << "MPQ not ONE" << endl;
      Term to_ret = Term(new Yices2Term(yices_mul(component, yices_mpq(coeff))));
      shared_ptr<Yices2Term> ret2 = std::static_pointer_cast<Yices2Term>(to_ret);
      cout << ret2->term << endl;
      cout << ret2->to_string() << endl;
      cout << "is prod? " << yices_term_is_product(ret2->term) << endl;
      cout << "type ? " << yices_term_is_product(ret2->term) << endl;
      cout << " yices_term_is_scalar : " << yices_term_is_scalar(ret2->term) << endl;
      cout << " yices_term_is_bitvector : " << yices_term_is_bitvector(ret2->term) << endl;
      cout << " yices_term_is_arithmetic : " << yices_term_is_arithmetic(ret2->term) << endl;
      cout << " yices_term_is_composite : " << yices_term_is_composite(ret2->term) << endl;
      cout << " type : " << yices_type_of_term(ret2->term) << endl;
      return to_ret;
    }
    else
    {
      cout << "MPQ eq ONE" << endl;
      return Term(new Yices2Term(component));
    }

  }
  else
  {
    cout << " falling through... " << tc << " is it yices_arith_leq_atom? " << " pos " << pos << endl;
    cout << "get_op " << (Term(new Yices2Term(term)))->get_op() << endl;
    if (tc == 28 )
    {
      Term qq = Term(new Yices2Term(yices_term_child(term, pos)));
      cout << " 28 : " << qq->to_string() << endl;
    }
    uint32_t actual_idx = pos;
    return Term(new Yices2Term(yices_term_child(term, actual_idx))); 
  }

  // // cout << "*" << endl;
  // if (!pos && /*msat_term_is_uf(env, term)*/ yices_term_is_function(term))
  // {
    
  // }
  // else if (!pos && yices_term_is_bvsum(term)) 
  // {

  // }
  // else 
  // {// uint32_t yices_val_function_arity(model_t *mdl, const yval_t *v)
  //   // might need to handle children for ITE for example??
  //   uint32_t actual_idx = pos;
  //   if (yices_term_is_function(term))
  //   {
  //     actual_idx--;
  //   }
  //   // unsure if this is right
  //   if ( yices_term_is_function(term)){
  //     // cout << " function " << endl;
  //   }
  //   if (yices_term_child(term, actual_idx) == NULL_TERM){
  //     // cout << "null term!!!" << endl;
  //   }
  //   return Term(new Yices2Term(yices_term_child(term, actual_idx))); 
  // }
  // // throw NotImplementedException(
  // //     "Yices2Term function not implemented yet."); 
}

bool Yices2TermIter::operator==(const Yices2TermIter & it)
{
  // cout << "==" << endl;
  return ((term == it.term) && (pos == it.pos));
}

bool Yices2TermIter::operator!=(const Yices2TermIter & it)
{
  // cout << "!=" << endl;
  return ((term != it.term) || (pos != it.pos));
}

bool Yices2TermIter::equal(const TermIterBase & other) const
{
  // cout << "equal" << endl;
  const Yices2TermIter & cti = static_cast<const Yices2TermIter &>(other);
  return ((term == cti.term) && (pos == cti.pos));
}

// end Yices2TermIter implementation

// Yices2Term implementation

size_t Yices2Term::hash() const
{
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
  
  switch (tc) 
  {
    case YICES_NOT_TERM: 
      return Op(Not);
      break;
    case YICES_BOOL_CONSTANT:
      return Op();
      break;
    case YICES_ARITH_CONSTANT:
      return Op();
      break;
    case YICES_BV_CONSTANT:
      return Op();
      break;
    case YICES_SCALAR_CONSTANT:
      return Op();
      break;
    case YICES_VARIABLE:
      return Op();
      break;
    case YICES_UNINTERPRETED_TERM:
      return Op();
      break;
  // composite terms
    case YICES_ITE_TERM:
      return Op(Ite);
      break;
  // YICES_APP_TERM,
  // YICES_UPDATE_TERM,
  // YICES_TUPLE_TERM,
  case YICES_EQ_TERM:
      return Op(Equal);
      break;
  // YICES_DISTINCT_TERM,
  // YICES_FORALL_TERM,
  // YICES_LAMBDA_TERM,
  // YICES_OR_TERM,
  // YICES_XOR_TERM,
  // YICES_BV_ARRAY,
  // YICES_BV_DIV,
  // YICES_BV_REM,
  // YICES_BV_SDIV,
  // YICES_BV_SREM,
  // YICES_BV_SMOD,
  // YICES_BV_SHL,
  // YICES_BV_LSHR,
  // YICES_BV_ASHR,
  // YICES_BV_GE_ATOM,
  // YICES_BV_SGE_ATOM,
  // YICES_ARITH_GE_ATOM,
  // YICES_ARITH_ROOT_ATOM,
  // YICES_ABS,
  // YICES_CEIL,
  // YICES_FLOOR,
  // YICES_RDIV,
  // YICES_IDIV,
  // YICES_IMOD,
  case YICES_IS_INT_ATOM:
    // How to discriminate amongst the atomics? 
    std::cout << " !!! IS INT ATOM " << std::endl;
  // YICES_DIVIDES_ATOM,
  // // projections
  // YICES_SELECT_TERM,
  // YICES_BIT_TERM,
  // // sums
  case YICES_BV_SUM:
      return Op(BVAdd);
      break;
  case YICES_ARITH_SUM:
      return Op(Plus);
      break;
  // // products
  // YICES_POWER_PRODUCT
    default:
      return Op();
      break;
  }
}

Sort Yices2Term::get_sort() const
{

  return Sort(new Yices2Sort(yices_type_of_term(term)));



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
  // yices_type_num_children, yices_term_num_children

  // Seems possibly relevant... 
  // uint32_t yices_val_function_arity(model_t *mdl, const yval_t *v)

  term_constructor_t tc = yices_term_constructor(term);

  // TODO: This probably is not quite right. 
  // Maybe use yices_type_of_term? 
  // BtorTerm has is_sym, maybe that's a better idea. 
  // an uninterpreted term can be a function, so this is v problematic. 
  /// TODO: question: what about constant arrays???????
  return (tc == YICES_UNINTERPRETED_TERM && yices_term_num_children(term) == 0) ||
          tc == YICES_BV_CONSTANT; //|| yices_term_is_atomic(term);


  // throw NotImplementedException(
  //     "Yices2Term function not implemented yet."); 
}

bool Yices2Term::is_value() const
{
  term_constructor_t tc = yices_term_constructor(term);

  // TODO: Also this could be wrong... 
  return (tc == YICES_BOOL_CONSTANT || 
      tc == YICES_ARITH_CONSTANT ||
      // tc == YICES_BV_CONSTANT ||
      tc == YICES_SCALAR_CONSTANT);
  // value if it has no children and a built-in interpretation
  // return (msat_term_is_number(env, term) || msat_term_is_true(env, term)
  //         || msat_term_is_false(env, term) ||
  //         // constant arrays are considered values in smt-switch
  //         msat_term_is_array_const(env, term));
  // throw NotImplementedException(
  //     "Yices2Term function not implemented yet."); 
}

string Yices2Term::to_string() const
{

  std::string sres = yices_term_to_string(term, 120, 1, 0);
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

  bool is_bv = yices_term_is_bitvector(term);

  // process bit-vector format
  if (is_bv)
  {
    if (val.find("0b") == std::string::npos)
    {
      std::string msg = val;
      msg += " is not a constant term, can't convert to int.";
      throw IncorrectUsageException(msg.c_str());
    }
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

TermIter Yices2Term::begin() { 
  // cout << "new iter" << endl;
  return TermIter(new Yices2TermIter(term, 0)); 
  // throw NotImplementedException(
  //     "Yices2Term function not implemented yet."); 
}

TermIter Yices2Term::end()
{
  // uint32_t arity = 
  // uint32_t arity = msat_term_arity(term);
  // if (msat_term_is_uf(env, term))
  // {
  //   // consider the function itself a child
  //   arity++;
  // }
  // cout << yices_term_num_children(term) << endl;
    // cout << term << endl;

  if (yices_term_num_children(term) < 0)
  {
    // cout << term << endl;
    // cout << yices_error_string() << endl;
  }
  return TermIter(new Yices2TermIter(term, yices_term_num_children(term)));
  // throw NotImplementedException(
  //     "Yices2Term function not implemented yet."); 
}

// end Yices2Term implementation

}  // namespace smt
