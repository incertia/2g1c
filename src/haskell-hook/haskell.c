#include "haskell.h"

/* stolen from rts/ClosureFlags.c */
StgWord16 closure_flags[] = {

/* ToDo: some of these flags seem to be duplicated.
 *       - NS is the same as HNF, and the negation of THU
 * (however, we set NS for indirections, which is probably the
 *  right thing to do, since we never get indirections pointing
 *  to thunks.)
 */

/*                            0    1    2    4   5    6   7     8 */
/*                          HNF  BTM   NS  THU MUT  UPT SRT   IND */

 [INVALID_OBJECT]       =  ( 0                                    ),
 [CONSTR]               =  (_HNF|     _NS                         ),
 [CONSTR_1_0]           =  (_HNF|     _NS                         ),
 [CONSTR_0_1]           =  (_HNF|     _NS                         ),
 [CONSTR_2_0]           =  (_HNF|     _NS                         ),
 [CONSTR_1_1]           =  (_HNF|     _NS                         ),
 [CONSTR_0_2]           =  (_HNF|     _NS                         ),
 [CONSTR_NOCAF]         =  (_HNF|     _NS                         ),
 [FUN]                  =  (_HNF|     _NS|             _SRT       ),
 [FUN_1_0]              =  (_HNF|     _NS|             _SRT       ),
 [FUN_0_1]              =  (_HNF|     _NS|             _SRT       ),
 [FUN_2_0]              =  (_HNF|     _NS|             _SRT       ),
 [FUN_1_1]              =  (_HNF|     _NS|             _SRT       ),
 [FUN_0_2]              =  (_HNF|     _NS|             _SRT       ),
 [FUN_STATIC]           =  (_HNF|     _NS|             _SRT       ),
 [THUNK]                =  (              _THU|        _SRT       ),
 [THUNK_1_0]            =  (              _THU|        _SRT       ),
 [THUNK_0_1]            =  (              _THU|        _SRT       ),
 [THUNK_2_0]            =  (              _THU|        _SRT       ),
 [THUNK_1_1]            =  (              _THU|        _SRT       ),
 [THUNK_0_2]            =  (              _THU|        _SRT       ),
 [THUNK_STATIC]         =  (              _THU|        _SRT       ),
 [THUNK_SELECTOR]       =  (              _THU|        _SRT       ),
 [BCO]                  =  (_HNF|     _NS                         ),
 [AP]                   =  (              _THU                    ),
 [PAP]                  =  (_HNF|     _NS                         ),
 [AP_STACK]             =  (              _THU                    ),
 [IND]                  =  (          _NS|                   _IND ),
 [IND_STATIC]           =  (          _NS|                   _IND ),
 [RET_BCO]              =  ( 0                                    ),
 [RET_SMALL]            =  (     _BTM|                  _SRT      ),
 [RET_BIG]              =  (                            _SRT      ),
 [RET_FUN]              =  ( 0                                    ),
 [UPDATE_FRAME]         =  (     _BTM                             ),
 [CATCH_FRAME]          =  (     _BTM                             ),
 [UNDERFLOW_FRAME]      =  (     _BTM                             ),
 [STOP_FRAME]           =  (     _BTM                             ),
 [BLACKHOLE]            =  (          _NS|         _UPT           ),
 [BLOCKING_QUEUE]       =  (          _NS|    _MUT|_UPT           ),
 [MVAR_CLEAN]           =  (_HNF|     _NS|    _MUT|_UPT           ),
 [MVAR_DIRTY]           =  (_HNF|     _NS|    _MUT|_UPT           ),
 [TVAR]                 =  (_HNF|     _NS|    _MUT|_UPT           ),
 [ARR_WORDS]            =  (_HNF|     _NS|         _UPT           ),
 [MUT_ARR_PTRS_CLEAN]   =  (_HNF|     _NS|    _MUT|_UPT           ),
 [MUT_ARR_PTRS_DIRTY]   =  (_HNF|     _NS|    _MUT|_UPT           ),
 [MUT_ARR_PTRS_FROZEN_DIRTY] =  (_HNF|     _NS|    _MUT|_UPT           ),
 [MUT_ARR_PTRS_FROZEN_CLEAN] =  (_HNF|     _NS|         _UPT           ),
 [MUT_VAR_CLEAN]        =  (_HNF|     _NS|    _MUT|_UPT           ),
 [MUT_VAR_DIRTY]        =  (_HNF|     _NS|    _MUT|_UPT           ),
 [WEAK]                 =  (_HNF|     _NS|         _UPT           ),
 [PRIM]                 =  (_HNF|     _NS|         _UPT           ),
 [MUT_PRIM]             =  (_HNF|     _NS|    _MUT|_UPT           ),
 [TSO]                  =  (_HNF|     _NS|    _MUT|_UPT           ),
 [STACK]                =  (_HNF|     _NS|    _MUT|_UPT           ),
 [TREC_CHUNK]           =  (          _NS|    _MUT|_UPT           ),
 [ATOMICALLY_FRAME]     =  (     _BTM                             ),
 [CATCH_RETRY_FRAME]    =  (     _BTM                             ),
 [CATCH_STM_FRAME]      =  (     _BTM                             ),
 [WHITEHOLE]            =  ( 0                                    ),
 [SMALL_MUT_ARR_PTRS_CLEAN] =  (_HNF|     _NS|     _MUT|_UPT           ),
 [SMALL_MUT_ARR_PTRS_DIRTY] =  (_HNF|     _NS|     _MUT|_UPT           ),
 [SMALL_MUT_ARR_PTRS_FROZEN_DIRTY] =  (_HNF|     _NS|     _MUT|_UPT           ),
 [SMALL_MUT_ARR_PTRS_FROZEN_CLEAN] =  (_HNF|     _NS|          _UPT           ),
 [COMPACT_NFDATA]       =  (_HNF|     _NS                              ),
};

#if N_CLOSURE_TYPES != 64
#error closure types changed: update haskell.c!
#endif

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
  return !bitmap(type);
}

int
bitmap(StgHalfWord type)
{
  return closure_flags[type] & _BTM;
}
