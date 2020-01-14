#include "yices2_solver.h"
#include "yices.h"
#include <inttypes.h>
#include "yices2_extensions.h"

using namespace std;

namespace smt
{

/* Yices2 Op mappings */
typedef term_t (*yices_un_fun)(term_t);
typedef term_t (*yices_bin_fun)(term_t, term_t);
typedef term_t (*yices_tern_fun)(term_t, term_t, term_t);
typedef term_t (*yices_variadic_fun)(uint32_t, term_t[]);

// TODO:
 //  /* Uninterpreted Functions */
 //  Apply,

 //  // Integers only
// yices_power(term_t t1, uint32_t d) 
 //  Pow,

 //  // Int/Real Conversion and Queries
 //  To_Real,

 //  /* Fixed Size BitVector Theory */
 //  Extract, ??
 //  BVComp, ?? 


 //  Zero_Extend,
 //  Sign_Extend,
 //  Repeat,
 //  Rotate_Left,
 //  Rotate_Right,
 //  // BitVector Conversion
 //  BV_To_Nat,
 //  Int_To_BV,

 //  /* Array Theory */
 //  Store,
 //  Const_Array,



const unordered_map<PrimOp, yices_un_fun> yices_unary_ops(
  {
    { Not, yices_not },
    { Negate, yices_neg },
    { Abs, yices_abs },
    { To_Int, yices_floor },
    { Is_Int, yices_is_int_atom },
    { BVNot, yices_bvnot },
    { BVNeg, yices_bvneg }
  });

const unordered_map<PrimOp, yices_bin_fun> yices_binary_ops(
    { 
      { And, yices_and2 },
      { Or, yices_or2 },
      { Xor, yices_xor2 },
      { Implies, yices_implies},
      { Iff, yices_iff},
      { Plus, yices_add },
      { Minus, yices_sub },
      { Mult, yices_mul },
      { Div, yices_division },
      { Lt, yices_arith_lt_atom },
      { Le, yices_arith_leq_atom },
      { Gt, yices_arith_gt_atom },
      { Ge, yices_arith_geq_atom },
      { Equal, yices_eq }, // yices_arith_eq_atom or yices_eq? 
      { Mod, yices_imod},
      { Concat, yices_bvconcat2},
      { BVAnd, yices_bvand2 },
      { BVOr, yices_bvor2 },
      { BVXor, yices_bvxor2 },
      { BVNand, yices_bvnand },
      { BVNor, yices_bvnor },
      { BVXnor, yices_bvxnor },
      { BVAdd, yices_bvadd },
      { BVSub, yices_bvsub },
      { BVMul, yices_bvmul },
      { BVUdiv, yices_bvdiv },
      { BVUrem, yices_bvrem },
      { BVSdiv, yices_bvsdiv },
      { BVSrem, yices_bvsrem },
      { BVSmod, yices_bvsmod },
      { BVShl, yices_bvshl },
      { BVAshr, yices_bvashr },
      { BVLshr, yices_bvlshr },
      { BVUlt, yices_bvlt_atom },
      { BVUle, yices_bvle_atom },
      { BVUgt, yices_bvgt_atom },
      { BVUge, yices_bvge_atom },
      { BVSle, yices_bvsle_atom },
      { BVSlt, yices_bvslt_atom },
      { BVSge, yices_bvsge_atom },
      { BVSgt, yices_bvsgt_atom },
      { Select, ext_yices_select }


    });

const unordered_map<PrimOp, yices_tern_fun> yices_ternary_ops(
    { { And, yices_and3 },
      { Or, yices_or3},
      { Xor, yices_xor3},
      { Ite, yices_ite },
      { BVAnd, yices_bvand3 },
      { BVOr, yices_bvor3 },
      { BVXor, yices_bvxor3 }
    });

const unordered_map<PrimOp, yices_variadic_fun> yices_variadic_ops(
    { { And, yices_and },
      { Or, yices_or},
      { Xor, yices_xor},
      { Distinct, yices_distinct}
      // { BVAnd, yices_bvand } needs const term. 
    });

// Need new list for variadic BV functions with const term_t[].



/* Yices2Solver implementation */

void Yices2Solver::set_opt(const std::string option, const std::string value)
{
  try
  {
    // I think yices ignores some options anyway. 
    // https://github.com/SRI-CSL/yices2/blob/622f0367ef6b0f4e7bfb380c856bac758f2acbe7/doc/manual/manual.tex
  }
  catch (std::exception & e)
  {
    throw InternalSolverException(e.what());
  }
}

void Yices2Solver::set_logic(const std::string logic) const
{
  // try
  // {
  //   solver.setLogic(logic);
  // }
  // catch (std::exception & e)
  // {
  //   throw InternalSolverException(e.what());
  // }
}

Term Yices2Solver::make_term(bool b) const
{
  // try
  // {
  //   Term c(new CVC4Term(solver.mkBoolean(b)));
  //   return c;
  // }
  // catch (std::exception & e)
  // {
  //   throw InternalSolverException(e.what());
  // }
  throw NotImplementedException(
      "Smt-switch does not have any sorts that take one sort parameter yet.");
}

Term Yices2Solver::make_term(int64_t i, const Sort & sort) const
{

    SortKind sk = sort->get_sort_kind();
    term_t y_term;
    if (sk == INT || sk == REAL)
    {
      y_term = yices_int64(i);
    }
    else if (sk == BV)
    {
      y_term = yices_bvconst_int64(sort->get_width(), i);
    }
    else 
    {
      //TODO
      throw NotImplementedException(
      "Smt-switch does not have any sorts that take one sort parameter yet.");
    }

  Term term(new Yices2Term(y_term));
  // symbol_names.insert(name);
  return term;

  //   ::CVC4::api::Term c;

  //   if ((sk == INT) || (sk == REAL))
  //   {
  //     c = solver.mkReal(i);
  //   }
  //   else if (sk == BV)
  //   {
  //     // CVC4 uses unsigned integers for mkBitVector
  //     // to avoid casting issues, always use a string in base 10
  //     std::string sval = std::to_string(i);
  //     c = solver.mkBitVector(sort->get_width(), sval, 10);
  //   }
  //   else
  //   {
  //     std::string msg = "Can't create constant with integer for sort ";
  //     msg += sort->to_string();
  //     throw IncorrectUsageException(msg.c_str());
  //   }

  //   Term res(new CVC4Term(c));
  //   return res;
  // }
  // catch (std::exception & e)
  // {
  //   // pretty safe to assume that an error is due to incorrect usage
  //   throw IncorrectUsageException(e.what());
  // // }
  // throw NotImplementedException(
  //     "Smt-switch does not have any sorts that take one sort parameter yet.");
}

Term Yices2Solver::make_term(std::string val,
                           const Sort & sort,
                           uint64_t base) const
{
  // try
  // {
  //   SortKind sk = sort->get_sort_kind();
  //   ::CVC4::api::Term c;

  //   if ((sk == INT) || (sk == REAL))
  //   {
  //     // TODO: Only do these checks in debug
  //     if (base != 10)
  //     {
  //       throw IncorrectUsageException("Can't use non-decimal base for reals and ints");
  //     }
  //     c = solver.mkReal(val);
  //   }
  //   else if (sk == BV)
  //   {
  //     c = solver.mkBitVector(sort->get_width(), val, base);
  //   }
  //   else
  //   {
  //     std::string msg = "Can't create constant with integer for sort ";
  //     msg += sort->to_string();
  //     throw IncorrectUsageException(msg.c_str());
  //   }

  //   Term res(new CVC4Term(c));
  //   return res;
  // }
  // catch (std::exception & e)
  // {
  //   // pretty safe to assume that an error is due to incorrect usage
  //   throw IncorrectUsageException(e.what());
  // }
  throw NotImplementedException(
      "Smt-switch does not have any sorts that take one sort parameter yet.");
}

Term Yices2Solver::make_term(const Term & val, const Sort & sort) const
{
  throw NotImplementedException("Constant arrays not yet implemented.");
}

void Yices2Solver::assert_formula(const Term& t) const
{
  shared_ptr<Yices2Term> yterm = static_pointer_cast<Yices2Term>(t);
  if (yices_assert_formula(ctx, yterm->term))
  {
    string msg("Cannot assert term: ");
    msg += t->to_string();
    throw IncorrectUsageException(msg);
  }
}

Result Yices2Solver::check_sat()
{

  auto res = yices_check_context(ctx, NULL);
  if (res == STATUS_SAT)
  {
    return Result(SAT);
  }
  else if (res == STATUS_UNSAT)
  {
    return Result(UNSAT);
  }
  else
  {
    return Result(UNKNOWN);
  }

}

Result Yices2Solver::check_sat_assuming(const TermVec & assumptions)
{

  // expecting (possibly negated) boolean literals
  for (auto a : assumptions)
  {
    if (!a->is_symbolic_const() || a->get_sort()->get_sort_kind() != BOOL)
    {
      if (a->get_op() == Not && (*a->begin())->is_symbolic_const())
      {
        continue;
      }
      else
      {
        throw IncorrectUsageException(
            "Expecting boolean indicator literals but got: " + a->to_string());
      }
    }
  }
  
  printf("check sat assuming\n");

  vector<term_t> y_assumps;
  y_assumps.reserve(assumptions.size());

  shared_ptr<Yices2Term> ya;
  for (auto a : assumptions)
  {
    ya = static_pointer_cast<Yices2Term>(a);
    y_assumps.push_back(ya->term);
  }

  auto res =
      yices_check_context_with_assumptions(ctx, NULL, y_assumps.size(), &y_assumps[0]);

  if (res == STATUS_SAT)
  {
    return Result(SAT);
  }
  else if (res == STATUS_UNSAT)
  {
    return Result(UNSAT);
  }
  else
  {
    return Result(UNKNOWN);
  }
}

void Yices2Solver::push(uint64_t num)
{
  // try
  // {
  //   solver.push(num);
  // }
  // catch (std::exception & e)
  // {
  //   throw InternalSolverException(e.what());
  // }
}

void Yices2Solver::pop(uint64_t num)
{
  // try
  // {
  //   solver.pop(num);
  // }
  // catch (std::exception & e)
  // {
  //   throw InternalSolverException(e.what());
  // }
}

Term Yices2Solver::get_value(Term & t) const
{
  shared_ptr<Yices2Term> yterm = static_pointer_cast<Yices2Term>(t);
  model_t* model = yices_get_model(ctx, true);

  // note: Function types are not supported for get_val_as_term
  term_t yices_val = yices_get_value_as_term(model, yterm->term);

  return Term(new Yices2Term(yices_val));

  //todo: error checking. 

}

Sort Yices2Solver::make_sort(const std::string name, uint64_t arity) const
{
  // try
  // {
  //   Sort s(new CVC4Sort(solver.declareSort(name, arity)));
  //   return s;
  // }
  // catch (std::exception & e)
  // {
  //   throw InternalSolverException(e.what());
  // }
  throw NotImplementedException(
      "Smt-switch does not have any sorts that take one sort parameter yet.");
}

Sort Yices2Solver::make_sort(SortKind sk) const
{
  try
  {
    if (sk == BOOL)
    {
      Sort s(new Yices2Sort(yices_bool_type()));
      return s;
    }
    else if (sk == INT)
    {
      Sort s(new Yices2Sort(yices_int_type()));
      return s;
    }
    else if (sk == REAL)
    {
      Sort s(new Yices2Sort(yices_real_type()));
      return s;
    }
    else
    {
      std::string msg("Can't create sort with sort constructor ");
      msg += to_string(sk);
      msg += " and no arguments";
      throw IncorrectUsageException(msg.c_str());
    }
  }
  catch (std::exception & e)
  {
    throw InternalSolverException(e.what());
  }
}

// WiP
Sort Yices2Solver::make_sort(SortKind sk, uint64_t size) const
{
  try
  {
    if (sk == BV)
    {
      Sort s(new Yices2Sort(yices_bv_type(size)));
      return s;
    }
    else
    {
      std::string msg("Can't create sort with sort constructor ");
      msg += to_string(sk);
      msg += " and an integer argument";
      throw IncorrectUsageException(msg.c_str());
    }
  }
  catch (std::exception & e)
  {
    throw InternalSolverException(e.what());
  }
}

Sort Yices2Solver::make_sort(SortKind sk, const Sort & sort1) const
{
  throw NotImplementedException(
      "Smt-switch does not have any sorts that take one sort parameter yet.");
}

Sort Yices2Solver::make_sort(SortKind sk,
                           const Sort & sort1,
                           const Sort & sort2) const
{

  if (sk == ARRAY)
  {
    std::shared_ptr<Yices2Sort> yidxsort =
        std::static_pointer_cast<Yices2Sort>(sort1);
    std::shared_ptr<Yices2Sort> yelemsort =
        std::static_pointer_cast<Yices2Sort>(sort2);
    Sort s(new Yices2Sort(yices_function_type1(yidxsort->type, yelemsort->type), yidxsort->type, yelemsort->type));
    return s;
  }
  else
  {
    std::string msg("Can't create sort with sort constructor ");
    msg += to_string(sk);
    msg += " and two Sort arguments";
    throw IncorrectUsageException(msg.c_str());
  }
}

Sort Yices2Solver::make_sort(SortKind sk,
                           const Sort & sort1,
                           const Sort & sort2,
                           const Sort & sort3) const
{
  throw NotImplementedException(
      "Smt-switch does not have any sorts that take three sort parameters "
      "yet.");
}

Sort Yices2Solver::make_sort(SortKind sk, const SortVec & sorts) const
{
  // try
  // {
  //   if (sk == FUNCTION)
  //   {
  //     if (sorts.size() < 2)
  //     {
  //       throw IncorrectUsageException(
  //           "Function sort must have >=2 sort arguments.");
  //     }

  //     // arity is one less, because last sort is return sort
  //     uint32_t arity = sorts.size() - 1;

  //     std::vector<::CVC4::api::Sort> csorts;
  //     csorts.reserve(arity);
  //     ::CVC4::api::Sort csort;
  //     for (uint32_t i = 0; i < arity; i++)
  //     {
  //       csort = std::static_pointer_cast<CVC4Sort>(sorts[i])->sort;
  //       csorts.push_back(csort);
  //     }

  //     csort = std::static_pointer_cast<CVC4Sort>(sorts.back())->sort;
  //     ::CVC4::api::Sort cfunsort = solver.mkFunctionSort(csorts, csort);
  //     Sort funsort(new CVC4Sort(cfunsort));
  //     return funsort;
  //   }
  //   else if (sorts.size() == 1)
  //   {
  //     return make_sort(sk, sorts[0]);
  //   }
  //   else if (sorts.size() == 2)
  //   {
  //     return make_sort(sk, sorts[0], sorts[1]);
  //   }
  //   else if (sorts.size() == 3)
  //   {
  //     return make_sort(sk, sorts[0], sorts[1], sorts[2]);
  //   }
  //   else
  //   {
  //     std::string msg("Can't create sort from sort constructor ");
  //     msg += to_string(sk);
  //     msg += " with a vector of sorts";
  //     throw IncorrectUsageException(msg.c_str());
  //   }
  // }
  // catch (std::exception & e)
  // {
  //   throw InternalSolverException(e.what());
  // }
  throw NotImplementedException(
      "Smt-switch does not have any sorts that take one sort parameter yet.");
}

Term Yices2Solver::make_symbol(const std::string name, const Sort & sort)
{
  // check that name is available
  // avoids memory leak when boolector aborts
  // if (symbol_names.find(name) != symbol_names.end())
  // {
  //   throw IncorrectUsageException("symbol " + name + " has already been used.");
  // }

  // std::shared_ptr<BoolectorSortBase> bs =
  //     std::static_pointer_cast<BoolectorSortBase>(sort);


  // SortKind sk = sort->get_sort_kind();
  // term_t y_term; 
  shared_ptr<Yices2Sort> ysort = static_pointer_cast<Yices2Sort>(sort);
  term_t y_term = yices_new_uninterpreted_term(ysort->type);
  yices_set_term_name(y_term, name.c_str());


  //   }
  //   else if (sk == INT)
  //   {
  //     Sort s(new Yices2Sort(yices_int_type()));
  //     return s;
  //   }
  //   else if (sk == REAL)
  //   {
  //     Sort s(new Yices2Sort(yices_real_type()));
  //     return s;
  //   }

  // if (sk == INT){
  //   y_term = yices_new_uninterpreted_term(YICES_BV_ARRAY);
  // }


  // // yices 2 term_t thing
  // term_t y_term = yices_new_uninterpreted_term(YICES_BV_ARRAY);

  // BoolectorNode * n;
  // if (sk == ARRAY)
  // {
  //   n = boolector_array(btor, bs->sort, name.c_str());
  //   // TODO: get rid of this
  //   //       only needed now because array models are partial
  //   //       we want to represent it as a sequence of stores
  //   //       ideally we could get this as a sequence of stores on a const array
  //   //       from boolector directly
  //   uint64_t node_id = (uint64_t)n;
  //   std::string base_name = name + "_base";
  //   BoolectorNode * base_node = boolector_array(btor, bs->sort, base_name.c_str());
  //   if (array_bases.find(node_id) != array_bases.end())
  //   {
  //     throw InternalSolverException("Error in array model preparation");
  //   }
  //   array_bases[node_id] = base_node;
  // }
  // else if (sk == FUNCTION)
  // {
  //   n = boolector_uf(btor, bs->sort, name.c_str());
  // }
  // else
  // {
  //   n = boolector_var(btor, bs->sort, name.c_str());
  // }

  // note: giving the symbol a null Op
  // Yices2Term term(y_term);

  Term term(new Yices2Term(y_term));
  // symbol_names.insert(name);
  return term;
  // throw NotImplementedException(
  //     "Smt-switch does not have any sorts that take one sort parameter yet.");
}

Term Yices2Solver::make_term(Op op, const Term & t) const
{
  shared_ptr<Yices2Term> yterm0 = static_pointer_cast<Yices2Term>(t);
  term_t res;
  if (!op.num_idx)
  {
    if (yices_unary_ops.find(op.prim_op) != yices_unary_ops.end())
    {
      res = yices_unary_ops.at(op.prim_op)(yterm0->term);
    }
    else
    {
      string msg("Can't apply ");
      msg += op.to_string();
      msg += " to the term or not supported by Yices2 backend yet.";
      throw IncorrectUsageException(msg);
    }
  }
  else
  {
    string msg = op.to_string();
    msg += " not supported for one term argument";
    throw IncorrectUsageException(msg);
  }
    return Term(new Yices2Term(res));
}

Term Yices2Solver::make_term(Op op, const Term & t0, const Term & t1) const
{
  shared_ptr<Yices2Term> yterm0 = static_pointer_cast<Yices2Term>(t0);
  shared_ptr<Yices2Term> yterm1 = static_pointer_cast<Yices2Term>(t1);
  term_t res;
  if (!op.num_idx)
  {
    if (yices_binary_ops.find(op.prim_op) != yices_binary_ops.end())
    {
      res = yices_binary_ops.at(op.prim_op)(yterm0->term, yterm1->term);
    }
    else if (yices_variadic_ops.find(op.prim_op) != yices_variadic_ops.end())
    {
      term_t terms[2] = {yterm0->term, yterm1->term};
      res = yices_variadic_ops.at(op.prim_op)(2, terms);
    }
    else
    {
      string msg("Can't apply ");
      msg += op.to_string();
      msg += " to two terms, or not supported by Yices2 backend yet.";
      throw IncorrectUsageException(msg);
    }
  }
  else
  {
    string msg = op.to_string();
    msg += " not supported for two term arguments";
    throw IncorrectUsageException(msg);
  }
    return Term(new Yices2Term(res));
}

Term Yices2Solver::make_term(Op op,
                           const Term & t0,
                           const Term & t1,
                           const Term & t2) const
{
  shared_ptr<Yices2Term> yterm0 = static_pointer_cast<Yices2Term>(t0);
  shared_ptr<Yices2Term> yterm1 = static_pointer_cast<Yices2Term>(t1);
  shared_ptr<Yices2Term> yterm2 = static_pointer_cast<Yices2Term>(t2);
  term_t res;
  if (!op.num_idx)
  {
    if (yices_ternary_ops.find(op.prim_op) != yices_ternary_ops.end())
    {
      res = yices_ternary_ops.at(op.prim_op)(yterm0->term, yterm1->term, yterm2->term);
    }
    else
    {
      string msg("Can't apply ");
      msg += op.to_string();
      msg += " to two terms, or not supported by Yices2 backend yet.";
      throw IncorrectUsageException(msg);
    }
  }
  else
  {
    string msg = op.to_string();
    msg += " not supported for two term arguments";
    throw IncorrectUsageException(msg);
  }
    return Term(new Yices2Term(res));
}

Term Yices2Solver::make_term(Op op, const TermVec & terms) const
{
  // try
  // {
  //   std::vector<::CVC4::api::Term> cterms;
  //   cterms.reserve(terms.size());
  //   std::shared_ptr<CVC4Term> cterm;
  //   for (auto t : terms)
  //   {
  //     cterm = std::static_pointer_cast<CVC4Term>(t);
  //     cterms.push_back(cterm->term);
  //   }
  //   if (op.num_idx == 0)
  //   {
  //     Term result(
  //         new CVC4Term(solver.mkTerm(primop2kind.at(op.prim_op), cterms)));
  //     return result;
  //   }
  //   else
  //   {
  //     ::CVC4::api::OpTerm ot = make_op_term(op);
  //     Term result(
  //         new CVC4Term(solver.mkTerm(primop2kind.at(op.prim_op), ot, cterms)));
  //     return result;
  //   }
  // }
  // catch (std::exception & e)
  // {
  //   throw InternalSolverException(e.what());
  // }
  throw NotImplementedException(
      "Smt-switch does not have any sorts that take one sort parameter yet.");
}

void Yices2Solver::reset()
{
  // try
  // {
  //   solver.reset();
  // }
  // catch (std::exception & e)
  // {
  //   throw InternalSolverException(e.what());
  // }
}

void Yices2Solver::reset_assertions()
{
  // try
  // {
  //   solver.resetAssertions();
  // }
  // catch (std::exception & e)
  // {
  //   throw InternalSolverException(e.what());
  // }
}

void Yices2Solver::dump_smt2(FILE * file) const
{
  throw NotImplementedException("Not yet implemented dumping smt2");
}

/**
   Helper function for creating an OpTerm from an Op
   Preconditions: op must be indexed, i.e. op.num_idx > 0
*/
// ::CVC4::api::OpTerm Yices2Solver::make_op_term(Op op) const
// {
//   if (op.num_idx == 1)
//   {
//     return solver.mkOpTerm(primop2optermcon.at(op.prim_op), op.idx0);
//   }
//   else if (op.num_idx == 2)
//   {
//     return solver.mkOpTerm(primop2optermcon.at(op.prim_op), op.idx0, op.idx1);
//   }
//   else
//   {
//     throw NotImplementedException(
//         "CVC4 does not have any indexed "
//         "operators with more than two indices");
//   }
// }

/* end Yices2Solver implementation */

}
