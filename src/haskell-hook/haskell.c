#include "haskell.h"

const char *const
closure_type_str(StgHalfWord type)
{
  switch (type) {
  case INVALID_OBJECT:
    return "INVALID_OBJECT";
  case CONSTR:
    return "CONSTR";
  case CONSTR_1_0:
    return "CONSTR_1_0";
  case CONSTR_0_1:
    return "CONSTR_0_1";
  case CONSTR_2_0:
    return "CONSTR_2_0";
  case CONSTR_1_1:
    return "CONSTR_1_1";
  case CONSTR_0_2:
    return "CONSTR_0_2";
  case CONSTR_NOCAF:
    return "CONSTR_NOCAF";
  case FUN:
    return "FUN";
  case FUN_1_0:
    return "FUN_1_0";
  case FUN_0_1:
    return "FUN_0_1";
  case FUN_2_0:
    return "FUN_2_0";
  case FUN_1_1:
    return "FUN_1_1";
  case FUN_0_2:
    return "FUN_0_2";
  case FUN_STATIC:
    return "FUN_STATIC";
  case THUNK:
    return "THUNK";
  case THUNK_1_0:
    return "THUNK_1_0";
  case THUNK_0_1:
    return "THUNK_0_1";
  case THUNK_2_0:
    return "THUNK_2_0";
  case THUNK_1_1:
    return "THUNK_1_1";
  case THUNK_0_2:
    return "THUNK_0_2";
  case THUNK_STATIC:
    return "THUNK_STATIC";
  case THUNK_SELECTOR:
    return "THUNK_SELECTOR";
  case BCO:
    return "BCO";
  case AP:
    return "AP";
  case PAP:
    return "PAP";
  case AP_STACK:
    return "AP_STACK";
  case IND:
    return "IND";
  case IND_STATIC:
    return "IND_STATIC";
  case RET_BCO:
    return "RET_BCO";
  case RET_SMALL:
    return "RET_SMALL";
  case RET_BIG:
    return "RET_BIG";
  case RET_FUN:
    return "RET_FUN";
  case UPDATE_FRAME:
    return "UPDATE_FRAME";
  case CATCH_FRAME:
    return "CATCH_FRAME";
  case UNDERFLOW_FRAME:
    return "UNDERFLOW_FRAME";
  case STOP_FRAME:
    return "STOP_FRAME";
  case BLOCKING_QUEUE:
    return "BLOCKING_QUEUE";
  case BLACKHOLE:
    return "BLACKHOLE";
  case MVAR_CLEAN:
    return "MVAR_CLEAN";
  case MVAR_DIRTY:
    return "MVAR_DIRTY";
  case TVAR:
    return "TVAR";
  case ARR_WORDS:
    return "ARR_WORDS";
  case MUT_ARR_PTRS_CLEAN:
    return "MUT_ARR_PTRS_CLEAN";
  case MUT_ARR_PTRS_DIRTY:
    return "MUT_ARR_PTRS_DIRTY";
  case MUT_ARR_PTRS_FROZEN_DIRTY:
    return "MUT_ARR_PTRS_FROZEN_DIRTY";
  case MUT_ARR_PTRS_FROZEN_CLEAN:
    return "MUT_ARR_PTRS_FROZEN_CLEAN";
  case MUT_VAR_CLEAN:
    return "MUT_VAR_CLEAN";
  case MUT_VAR_DIRTY:
    return "MUT_VAR_DIRTY";
  case WEAK:
    return "WEAK";
  case PRIM:
    return "PRIM";
  case MUT_PRIM:
    return "MUT_PRIM";
  case TSO:
    return "TSO";
  case STACK:
    return "STACK";
  case TREC_CHUNK:
    return "TREC_CHUNK";
  case ATOMICALLY_FRAME:
    return "ATOMICALLY_FRAME";
  case CATCH_RETRY_FRAME:
    return "CATCH_RETRY_FRAME";
  case CATCH_STM_FRAME:
    return "CATCH_STM_FRAME";
  case WHITEHOLE:
    return "WHITEHOLE";
  case SMALL_MUT_ARR_PTRS_CLEAN:
    return "SMALL_MUT_ARR_PTRS_CLEAN";
  case SMALL_MUT_ARR_PTRS_DIRTY:
    return "SMALL_MUT_ARR_PTRS_DIRTY";
  case SMALL_MUT_ARR_PTRS_FROZEN_DIRTY:
    return "SMALL_MUT_ARR_PTRS_FROZEN_DIRTY";
  case SMALL_MUT_ARR_PTRS_FROZEN_CLEAN:
    return "SMALL_MUT_ARR_PTRS_FROZEN_CLEAN";
  case COMPACT_NFDATA:
    return "COMPACT_NFDATA";
  default:
    return "<unknown closure type>";
  }
}

int
pointers_first(StgHalfWord type)
{
  switch (type) {
  case CONSTR_0_1:
    return 1;
  default:
    return 0;
  }
}

int
bitmap(StgHalfWord type)
{
  switch (type) {
  default:
    return 0;
  }
}
