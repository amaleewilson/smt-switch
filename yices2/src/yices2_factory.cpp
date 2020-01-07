#include "yices2_factory.h"

#include "yices2_solver.h"

namespace smt {

/* Yices2SolverFactory implementation */
SmtSolver Yices2SolverFactory::create()
{
  return std::make_unique<Yices2Solver>();
}
/* end Yices2SolverFactory implementation */

}  // namespace smt
