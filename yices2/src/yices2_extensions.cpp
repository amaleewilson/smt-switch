#ifndef SMT_YICES2_EXTENSIONS_H
#define SMT_YICES2_EXTENSIONS_H

#include "yices.h"

using namespace std;


namespace smt {

// TODO
term_t ext_yices_make_select(term_t t1, term_t t2)
{

  term_t yices_select(uint32_t i, term_t t);

  // return yices_select(yices_get, t1);

  // msat_term negone = msat_make_number(e, "-1");
  // return msat_make_times(e, negone, t);
}


}  // namespace smt

#endif
