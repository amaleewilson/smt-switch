#include "assert.h"

#include "api/cvc4cpp.h"

#include "exceptions.h"

#include "cvc4_sort.h"
#include "cvc4_term.h"

namespace smt {

// the kinds CVC4 needs to build an OpTerm for an indexed op
const std::unordered_map<::CVC4::api::Kind, size_t> kind2numindices(
    { { ::CVC4::api::BITVECTOR_EXTRACT, 2 },
      { ::CVC4::api::BITVECTOR_ZERO_EXTEND, 1 },
      { ::CVC4::api::BITVECTOR_SIGN_EXTEND, 1 },
      { ::CVC4::api::BITVECTOR_REPEAT, 1 },
      { ::CVC4::api::BITVECTOR_ROTATE_LEFT, 1 },
      { ::CVC4::api::BITVECTOR_ROTATE_RIGHT, 1 },
      { ::CVC4::api::INT_TO_BITVECTOR, 1 } });

const std::unordered_map<::CVC4::api::Kind, PrimOp> kind2primop(
    { { ::CVC4::api::AND, And },
      { ::CVC4::api::OR, Or },
      { ::CVC4::api::XOR, Xor },
      { ::CVC4::api::NOT, Not },
      { ::CVC4::api::IMPLIES, Implies },
      { ::CVC4::api::ITE, Ite },
      { ::CVC4::api::EQUAL, Iff },
      { ::CVC4::api::EQUAL, Equal },
      { ::CVC4::api::DISTINCT, Distinct },
      /* Uninterpreted Functions */
      { ::CVC4::api::APPLY_UF, Apply },
      /* Arithmetic Theories */
      { ::CVC4::api::PLUS, Plus },
      { ::CVC4::api::MINUS, Minus },
      { ::CVC4::api::UMINUS, Negate },
      { ::CVC4::api::MULT, Mult },
      { ::CVC4::api::DIVISION, Div },
      { ::CVC4::api::LT, Lt },
      { ::CVC4::api::LEQ, Le },
      { ::CVC4::api::GT, Gt },
      { ::CVC4::api::GEQ, Ge },
      { ::CVC4::api::INTS_MODULUS, Mod },
      { ::CVC4::api::ABS, Abs },
      { ::CVC4::api::POW, Pow },
      { ::CVC4::api::TO_REAL, To_Real },
      { ::CVC4::api::TO_INTEGER, To_Int },
      { ::CVC4::api::IS_INTEGER, Is_Int },
      /* Fixed Size BitVector Theory */
      { ::CVC4::api::BITVECTOR_CONCAT, Concat },
      // Indexed Op
      { ::CVC4::api::BITVECTOR_EXTRACT, Extract },
      { ::CVC4::api::BITVECTOR_NOT, BVNot },
      { ::CVC4::api::BITVECTOR_NEG, BVNeg },
      { ::CVC4::api::BITVECTOR_AND, BVAnd },
      { ::CVC4::api::BITVECTOR_OR, BVOr },
      { ::CVC4::api::BITVECTOR_XOR, BVXor },
      { ::CVC4::api::BITVECTOR_NAND, BVNand },
      { ::CVC4::api::BITVECTOR_NOR, BVNor },
      { ::CVC4::api::BITVECTOR_XNOR, BVXnor },
      { ::CVC4::api::BITVECTOR_COMP, BVComp },
      { ::CVC4::api::BITVECTOR_PLUS, BVAdd },
      { ::CVC4::api::BITVECTOR_SUB, BVSub },
      { ::CVC4::api::BITVECTOR_MULT, BVMul },
      { ::CVC4::api::BITVECTOR_UDIV, BVUdiv },
      { ::CVC4::api::BITVECTOR_SDIV, BVSdiv },
      { ::CVC4::api::BITVECTOR_UREM, BVUrem },
      { ::CVC4::api::BITVECTOR_SREM, BVSrem },
      { ::CVC4::api::BITVECTOR_SMOD, BVSmod },
      { ::CVC4::api::BITVECTOR_SHL, BVShl },
      { ::CVC4::api::BITVECTOR_ASHR, BVAshr },
      { ::CVC4::api::BITVECTOR_LSHR, BVLshr },
      { ::CVC4::api::BITVECTOR_ULT, BVUlt },
      { ::CVC4::api::BITVECTOR_ULE, BVUle },
      { ::CVC4::api::BITVECTOR_UGT, BVUgt },
      { ::CVC4::api::BITVECTOR_UGE, BVUge },
      { ::CVC4::api::BITVECTOR_SLT, BVSlt },
      { ::CVC4::api::BITVECTOR_SLE, BVSle },
      { ::CVC4::api::BITVECTOR_SGT, BVSgt },
      { ::CVC4::api::BITVECTOR_SGE, BVSge },
      // Indexed Op
      { ::CVC4::api::BITVECTOR_ZERO_EXTEND, Zero_Extend },
      // Indexed Op
      { ::CVC4::api::BITVECTOR_SIGN_EXTEND, Sign_Extend },
      // Indexed Op
      { ::CVC4::api::BITVECTOR_REPEAT, Repeat },
      // Indexed Op
      { ::CVC4::api::BITVECTOR_ROTATE_LEFT, Rotate_Left },
      // Indexed Op
      { ::CVC4::api::BITVECTOR_ROTATE_RIGHT, Rotate_Right },
      { ::CVC4::api::SELECT, Select },
      { ::CVC4::api::STORE, Store } });

// struct for hashing
CVC4::api::TermHashFunction termhash;

/* CVC4TermIter implementation */
CVC4TermIter & CVC4TermIter::operator=(const CVC4TermIter & it)
{
  term_it = it.term_it;
  return *this;
}

void CVC4TermIter::operator++() { term_it++; }

const Term CVC4TermIter::operator*()
{
  Term t(new CVC4Term(*term_it));
  return t;
}

TermIterBase * CVC4TermIter::clone() const { return new CVC4TermIter(term_it); }

bool CVC4TermIter::operator==(const CVC4TermIter & it)
{
  return term_it == it.term_it;
}

bool CVC4TermIter::operator!=(const CVC4TermIter & it)
{
  return term_it != term_it;
}

bool CVC4TermIter::equal(const TermIterBase & other) const
{
  const CVC4TermIter & cti = static_cast<const CVC4TermIter &>(other);
  return term_it == cti.term_it;
}

/* end CVC4TermIter implementation */

/* CVC4Term implementation */

std::size_t CVC4Term::hash() const
{
  return termhash(term);
}

bool CVC4Term::compare(const Term & absterm) const
{
  std::shared_ptr<CVC4Term> other =
    std::static_pointer_cast<CVC4Term>(absterm);
  return term == other->term;
}

Op CVC4Term::get_op() const
{
  if (!term.hasOp())
  {
    // return a null op
    return Op();
  }

  CVC4::api::Op cvc4_op = term.getOp();
  CVC4::api::Kind cvc4_kind = cvc4_op.getKind();

  // special cases
  if (cvc4_kind == CVC4::api::Kind::STORE_ALL)
  {
    // constant array is a value in smt-switch
    return Op();
  }

  // implementation checking
  if (kind2primop.find(cvc4_kind) == kind2primop.end())
  {
    throw NotImplementedException("get_op not implemented for CVC4 Kind "
                                  + CVC4::api::kindToString(cvc4_kind));
  }
  PrimOp po = kind2primop.at(cvc4_kind);

  // create an smt-switch Op and return it
  if (cvc4_op.isIndexed())
  {
    if (kind2numindices.find(cvc4_kind) == kind2numindices.end())
    {
      throw NotImplementedException("get_op not implemented for CVC4 Kind "
                                    + CVC4::api::kindToString(cvc4_kind));
    }
    size_t num_indices = kind2numindices.at(cvc4_kind);
    if (num_indices == 1)
    {
      uint32_t idx0 = cvc4_op.getIndices<uint32_t>();
      return Op(po, idx0);
    }
    else
    {
      assert(num_indices == 2);
      std::pair<uint32_t, uint32_t> indices =
          cvc4_op.getIndices<std::pair<uint32_t, uint32_t>>();
      return Op(po, indices.first, indices.second);
    }
  }
  else
  {
    return Op(po);
  }
}

Sort CVC4Term::get_sort() const
{
  Sort s(new CVC4Sort(term.getSort()));
  return s;
}

bool CVC4Term::is_symbolic_const() const
{
  return (term.getKind() == ::CVC4::api::CONSTANT);
}

bool CVC4Term::is_value() const
{
  // checking all possible const types for future-proofing
  // not all these sorts are even supported at this time
  ::CVC4::api::Kind k = term.getKind();
  return ((k == ::CVC4::api::CONST_BOOLEAN)
          || (k == ::CVC4::api::CONST_BITVECTOR)
          || (k == ::CVC4::api::CONST_RATIONAL)
          || (k == ::CVC4::api::CONST_FLOATINGPOINT)
          || (k == ::CVC4::api::CONST_ROUNDINGMODE)
          || (k == ::CVC4::api::CONST_STRING) || (k == ::CVC4::api::STORE_ALL));
}

std::string CVC4Term::to_string() const { return term.toString(); }

uint64_t CVC4Term::to_int() const
{
  std::string val = term.toString();
  ::CVC4::api::Sort sort = term.getSort();

  // process smt-lib bit-vector format
  if (sort.isBitVector())
  {
    if (val.find("(_ bv") == std::string::npos)
    {
      std::string msg = val;
      msg += " is not a constant term, can't convert to int.";
      throw IncorrectUsageException(msg.c_str());
    }
    val = val.substr(5, val.length());
    val = val.substr(0, val.find(" "));
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

/** Iterators for traversing the children
 */
TermIter CVC4Term::begin()
{
  return TermIter(new CVC4TermIter(term.begin()));
}

TermIter CVC4Term::end()
{
  return TermIter(new CVC4TermIter(term.end()));
}

/* end CVC4Term implementation */

}
