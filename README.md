# non-consensual haskell jit

inject a live haskell application and do magic with C

you can probably make this a non-consensual jit bit compiling haskell and
linking it into your c executable and then performing some hooks

## dependencies/inspirations

1. [mach_inject](https://github.com/rentzsch/mach_inject)
2. [osxinj](https://github.com/scen/osxinj)

## requirements

1. `macOS` - the injector code is macOS only via mach_inject but if you port the
   injector code to your platform then everything *should* work
2. `x86_64` - the code is simply not written to handle normal `x86`
3. some version of [ghc](https://www.haskell.org/ghc/)
4. [stack](https://www.stackage.org/)
5. a lot of free time

## buliding

1. build your application (`$ stack build` inside `./stack-calc`)
2. checkout a relevant copy of [ghc](http://git.haskell.org/ghc.git) into
   `./src/ghc/`
3. [start building ghc](https://ghc.haskell.org/trac/ghc/wiki/Building)

   in particular, you want to make sure `SIZEOF_VOID_P == 8` and
   `TABLES_NEXT_TO_CODE` is a thing
4. wait 5 years for ghc to finish building
5. use CMake to generate build files of your own choice
6. `$ make`
   * this may require `stack ghc -- -fobject-code unsafe_zp.hs`

## profiting
1. launch your haskell application (`$ stack exec stack-calc`)
2. inject your haskell application (`$ sudo ./injector <pid>
   libhaskell-hook.dylib`)
3. profit

## comments
1. laziness makes it very hard to actually pull out values
2. `base_GHCziNum_zdfNumInteger_closure` is nowhere to be found when we are in
   `base_GHCziNum_zp_info`?
3. gluing `safe_weird_zp` to `NumInteger_closure` causes it to return the
   constant `2199023255808 = 0x20000000100` which is very weird considering it
   expects a `Num` instance (`arity=3`) as well and it doesn't crash
4. gluing `safe_weird_zp'` to `NumInteger_closure` does what we expect
5. gluing the other functions `unsafe_zp'` and `unsafe_zp''` and `foreign_zp`
   cause the runtime to crash, probably because of `unsafePerformIO`

   i postulate that `unsafePerformIO` can be made to work, but i do not know
   how yet
6. the rts has static closures for the integers `[-16,16]`

       (lldb) x/132x 0x10022e588
       0x10022e588: 0x001bd660 0x00000001 0xfffffff0 0xffffffff
       0x10022e598: 0x001bd660 0x00000001 0xfffffff1 0xffffffff
       0x10022e5a8: 0x001bd660 0x00000001 0xfffffff2 0xffffffff
       0x10022e5b8: 0x001bd660 0x00000001 0xfffffff3 0xffffffff
       0x10022e5c8: 0x001bd660 0x00000001 0xfffffff4 0xffffffff
       0x10022e5d8: 0x001bd660 0x00000001 0xfffffff5 0xffffffff
       0x10022e5e8: 0x001bd660 0x00000001 0xfffffff6 0xffffffff
       0x10022e5f8: 0x001bd660 0x00000001 0xfffffff7 0xffffffff
       0x10022e608: 0x001bd660 0x00000001 0xfffffff8 0xffffffff
       0x10022e618: 0x001bd660 0x00000001 0xfffffff9 0xffffffff
       0x10022e628: 0x001bd660 0x00000001 0xfffffffa 0xffffffff
       0x10022e638: 0x001bd660 0x00000001 0xfffffffb 0xffffffff
       0x10022e648: 0x001bd660 0x00000001 0xfffffffc 0xffffffff
       0x10022e658: 0x001bd660 0x00000001 0xfffffffd 0xffffffff
       0x10022e668: 0x001bd660 0x00000001 0xfffffffe 0xffffffff
       0x10022e678: 0x001bd660 0x00000001 0xffffffff 0xffffffff
       0x10022e688: 0x001bd660 0x00000001 0x00000000 0x00000000
       0x10022e698: 0x001bd660 0x00000001 0x00000001 0x00000000
       0x10022e6a8: 0x001bd660 0x00000001 0x00000002 0x00000000
       0x10022e6b8: 0x001bd660 0x00000001 0x00000003 0x00000000
       0x10022e6c8: 0x001bd660 0x00000001 0x00000004 0x00000000
       0x10022e6d8: 0x001bd660 0x00000001 0x00000005 0x00000000
       0x10022e6e8: 0x001bd660 0x00000001 0x00000006 0x00000000
       0x10022e6f8: 0x001bd660 0x00000001 0x00000007 0x00000000
       0x10022e708: 0x001bd660 0x00000001 0x00000008 0x00000000
       0x10022e718: 0x001bd660 0x00000001 0x00000009 0x00000000
       0x10022e728: 0x001bd660 0x00000001 0x0000000a 0x00000000
       0x10022e738: 0x001bd660 0x00000001 0x0000000b 0x00000000
       0x10022e748: 0x001bd660 0x00000001 0x0000000c 0x00000000
       0x10022e758: 0x001bd660 0x00000001 0x0000000d 0x00000000
       0x10022e768: 0x001bd660 0x00000001 0x0000000e 0x00000000
       0x10022e778: 0x001bd660 0x00000001 0x0000000f 0x00000000
       0x10022e788: 0x001bd660 0x00000001 0x00000010 0x00000000
       (lldb) disas -s 0x1001bd660 -c 1
       stack-calc`ghczmprim_GHCziTypes_Izh_con_info:
           0x1001bd660 <+0>: inc    rbx

   these numbers are `stg_ap_p_fast`'d when you do something small like `4 2 +`
7. the application **should** be single threaded, yet sometimes in `lldb` i will get haskell output in between hooked output

       ...
       R5: HEAP closure at 0x420007d380...
       R5: tag=0
       R5: real addr=0x420007d380
       R5: info ptr=0x100013548
       R5: info tbl=0x100013538
       R5: closure type: THUNK_1_0 (16)
       R5: closure payload pointers: 1
       R5: closure payload non-pointers: 0
       R5: payload[0]=0x420007d310
       R6: null pointer
       7
       *Sp: STACK closure at 0x42001e7e70...
       *Sp: tag=0
       *Sp: real addr=0x42001e7e70
       *Sp: info ptr=0x1001de1d8
       *Sp: info tbl=0x1001de1c8
       *Sp: closure type: UPDATE_FRAME (33)
       *Sp: closure layout: 0x1
       ...

## goals

1. provide a better interface for closures
2. extract the numbers that are added togehter in `base GHC.Num +`
