#ifndef SMT_YICES2_EXTENSIONS_H
#define SMT_YICES2_EXTENSIONS_H

#include "yices.h"

namespace smt {

term_t ext_yices_select(term_t arr, term_t idx);

}  // namespace smt

#endif
