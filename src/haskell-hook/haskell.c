#include "haskell.h"

#include <stdio.h>

const StgWord *stg_arg_stuff;

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

const char *const
fun_type_str(StgWord type)
{
  switch (type) {
  case ARG_GEN:
    return "ARG_GEN";
  case ARG_GEN_BIG:
    return "ARG_GEN_BIG";
  case ARG_BCO:
    return "ARG_BCO";
  case ARG_NONE:
    return "ARG_NONE";
  case ARG_N:
    return "ARG_N";
  case ARG_P:
    return "ARG_P";
  case ARG_F:
    return "ARG_F";
  case ARG_D:
    return "ARG_D";
  case ARG_L:
    return "ARG_L";
  case ARG_V16:
    return "ARG_V16";
  case ARG_V32:
    return "ARG_V32";
  case ARG_V64:
    return "ARG_V64";
  case ARG_NN:
    return "ARG_NN";
  case ARG_NP:
    return "ARG_NP";
  case ARG_PN:
    return "ARG_PN";
  case ARG_PP:
    return "ARG_PP";
  case ARG_NNN:
    return "ARG_NNN";
  case ARG_NNP:
    return "ARG_NNP";
  case ARG_NPN:
    return "ARG_NPN";
  case ARG_NPP:
    return "ARG_NPP";
  case ARG_PNN:
    return "ARG_PNN";
  case ARG_PNP:
    return "ARG_PNP";
  case ARG_PPN:
    return "ARG_PPN";
  case ARG_PPP:
    return "ARG_PPP";
  case ARG_PPPP:
    return "ARG_PPPP";
  case ARG_PPPPP:
    return "ARG_PPPPP";
  case ARG_PPPPPP:
    return "ARG_PPPPPP";
  case ARG_PPPPPPP:
    return "ARG_PPPPPPP";
  case ARG_PPPPPPPP:
    return "ARG_PPPPPPPP";
  default:
    return "???";
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

void printregs(stg_regset_t *regs)
{
  printf("BaseReg=%p\n", (void *)regs->basereg);
  printf("Sp=%p\n", regs->sp);
  printf("SpLim=%p\n", regs->splim);
  printf("Hp=%p\n", regs->hp);
  printf("HpLim=%p\n", regs->basereg->rHpLim);
  printf("R1=%p\n", regs->r1.a);
  printf("R2=%p\n", regs->r2.a);
  printf("R3=%p\n", regs->r3.a);
  printf("R4=%p\n", regs->r4.a);
  printf("R5=%p\n", regs->r5.a);
  printf("R6=%p\n", regs->r6.a);
  printf("MachSp=%p\n", regs->machsp);
}

void
print_closure(StgPtr closure,
              const char *name,
              int indent,
              stg_regset_t *regs)
{
  uint64_t tag = (uint64_t)closure & 0x7;
  StgClosure *c = (StgClosure *)((uint64_t)closure & ~0x7);
  StgPtr _c = (StgPtr)c;
  const char *where;
  const StgInfoTable *i;
  char fmt[4096];
  StgHalfWord ctype;

  if (!closure) {
    snprintf(fmt, 4096, "%%%ds%s: null pointer\n", indent, name);
    fprintf(stderr, fmt, "");
    return;
  }

  if (_c >= regs->sp) {
    where = "STACK";
  } else if (_c <= regs->hp &&
             _c >= regs->basereg->rCurrentNursery->start) {
    where = "HEAP";
  } else {
    where = "???";
  }

  i = c->header.info;
  snprintf(fmt, 4096, "%%%ds%s: %s closure at %p...\n", indent, name, where,
           closure);
  printf(fmt, "");
  snprintf(fmt, 4096, "%%%ds%s: tag=%llx\n", indent, name, tag);
  printf(fmt, "");
  snprintf(fmt, 4096, "%%%ds%s: real addr=%p\n", indent, name, c);
  printf(fmt, "");
  snprintf(fmt, 4096, "%%%ds%s: info ptr=%p\n", indent, name, i);
  printf(fmt, "");
  i--;
  snprintf(fmt, 4096, "%%%ds%s: info tbl=%p\n", indent, name, i);
  printf(fmt, "");

  ctype = i->type;

  snprintf(fmt, 4096, "%%%ds%s: closure type: %s (%d)\n", indent, name,
           closure_type_str(ctype), ctype);
  printf(fmt, "");

  if (ctype >= N_CLOSURE_TYPES) {
    snprintf(fmt, 4096, "%%%ds%s: not a valid closure\n", indent, name);
    fprintf(stderr, fmt, "");
    return;
  }

  if (pointers_first(ctype)) {
    int j;
    int n;
    snprintf(fmt, 4096, "%%%ds%s: closure payload pointers: %u\n", indent, name,
             i->layout.payload.ptrs);
    printf(fmt, "");
    snprintf(fmt, 4096, "%%%ds%s: closure payload non-pointers: %u\n", indent,
             name, i->layout.payload.nptrs);
    printf(fmt, "");

    n = i->layout.payload.ptrs + i->layout.payload.nptrs;

    for (j = 0; j < n; j++) {
      snprintf(fmt, 4096, "%%%ds%s: payload[%d]=%p\n", indent, name, j,
               c->payload[j]);
      printf(fmt, "");
      // if (j < i->layout.payload.ptrs) {
      //   print_closure((StgPtr)c->payload[j], "PAYLOADPTR", indent + 2, regs);
      // }
    }
  } else if (bitmap(ctype)) {
    snprintf(fmt, 4096, "%%%ds%s: closure layout: 0x%llx\n", indent, name,
             i->layout.bitmap);
    printf(fmt, "");
  } else {
    snprintf(fmt, 4096, "%%%ds%s: print_closure: unknown layout field format: "
             "%d\n", indent, name, ctype);
    fprintf(stderr, fmt, "");
  }
  fflush(stdout);
}

void
print_function(StgPtr closure,
               const char *name,
               int indent,
               stg_regset_t *regs)
{
  StgClosure *c = (StgClosure *)((uint64_t)closure & ~0x7);
  const StgFunInfoTable *i;
  char fmt[4096];

  print_closure(closure, name, indent, regs);

  i = (StgFunInfoTable *)c->header.info;
  i--;

  snprintf(fmt, 4096, "%%%ds%s: function info table at %p...\n", indent,
           name, i);
  printf(fmt, "");
  snprintf(fmt, 4096, "%%%ds%s: slow_apply_offset=0x%x\n", indent, name,
           i->f.slow_apply_offset);
  printf(fmt, "");
  snprintf(fmt, 4096, "%%%ds%s: fun_type: %s (%u)\n", indent, name,
           fun_type_str(i->f.fun_type), i->f.fun_type);
  printf(fmt, "");
  snprintf(fmt, 4096, "%%%ds%s: arity=%u\n", indent, name, i->f.arity);
  printf(fmt, "");
  snprintf(fmt, 4096, "%%%ds%s: fun_bitmap=", indent, name);
  printf(fmt, "");
  switch (i->f.fun_type) {
  case ARG_GEN:
    print_small_bitmap(i->f.b.bitmap);
  case ARG_GEN_BIG:
    print_large_bitmap(GET_FUN_LARGE_BITMAP(i));
    break;
  default:
    print_small_bitmap(stg_arg_stuff[i->f.fun_type]);
    break;
  }
  printf("\n");
  fflush(stdout);
}

void
print_small_bitmap(StgWord bitmap)
{
  StgWord bits = BITMAP_BITS(bitmap);
  StgWord size = BITMAP_SIZE(bitmap);

  for (StgWord i = 0; i < size; i++, bits >>= 1) {
    if (bits & 1) {
      printf("W");
    } else {
      printf("P");
    }
  }
}

void
print_large_bitmap(StgLargeBitmap *bitmap)
{
  StgWord size = bitmap->size;
  StgWord bits;

  for (StgWord i = 0, bmp = 0; i < size; i++) {
    if (i % (8 * sizeof(StgWord)) == 0) {
      bits = bitmap->bitmap[bmp++];
    }
    if (bits & 1) {
      printf("W");
    } else {
      printf("P");
    }
    bits >>= 1;
  }
}
