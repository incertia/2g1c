#ifndef __HASKELL_H__
#define __HASKELL_H__

#include <stddef.h>

/* benefits of building a local GHC: we don't have to copy paste any structs and
 * we can just directly include everything */
#define SIZEOF_VOID_P 8
/* make this void bc idk where the definition is */
typedef void CostCentreStack;
#include <Stg.h>
#include <stg/Types.h>
#include <stg/Regs.h>
#include <rts/Constants.h>
#include <rts/Types.h>
#include <rts/storage/Block.h>
#include <rts/storage/Closures.h>
#include <rts/storage/ClosureTypes.h>
#include <rts/storage/InfoTables.h>
// #include <rts/storage/ClosureMacros.h>

typedef struct stg_regset_t {
  StgRegTable *basereg;
  StgPtr sp;
  StgPtr hp;
  StgUnion r1;
  StgUnion r2;
  StgUnion r3;
  StgUnion r4;
  StgUnion r5;
  StgUnion r6;
  StgPtr splim;
  StgPtr machsp;
} stg_regset_t;

extern void getregs(stg_regset_t *);

const char *const closure_type_str(StgHalfWord type);
int pointers_first(StgHalfWord type);
int bitmap(StgHalfWord type);

#endif
