# 2 haskells 1 hook - the tale of a non-consensual haskell jit

inject a live haskell application and do magic with both C and haskell

## the goal

your application:

    main :: IO ()
    main = print $ 2 + 3

an aggressive bystander:

    new_plus :: Integer -> Integer -> Integer
    new_plus a b = unsafePerformIO $ do
      let c = a + b
      putStrLn $ show a ++ " + " ++ show b ++ " = " ++ show c
      return c

regular output:

    5

output on steroids:

    2 + 3 = 5
    5

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
4. gluing `safe_weird_zp'` to `NumInteger_closure` does what we expect.
5. gluing the other functions `unsafe_zp'` and `unsafe_zp''` and `foreign_zp`
   cause the runtime to crash due to multiple statically linked runtimes.

   i postulate that `unsafePerformIO` can be made to work, but i do not know
   how yet.
6. the application **should** be single threaded, yet sometimes in `lldb` i
   will get haskell output in between hooked output

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
7. with respect to `unsafePerformIO`, the issue is almost certainly the fact
   that there are two statically linked runtimes competing with each other here.
   in particular, within `maybePerformBlockedException`, there are several
   checks against `END_TSO_QUEUE` and `END_BLOCKED_EXCEPTIONS_QUEUE`, both of
   which are defined to be `stg_END_TSO_QUEUE_closure`, but because there are
   two and everything is compiled with `rip` relative addressing, dynamic
   patching is needed to ensure that the runtimes agree with each other. this
   is currently done within `main.c` of `haskell-hook`, but the goal is to put
   the logic elsewhere once we get `unsafePerformIO` working. when these values
   are made to agree, the runtime will end up dying in
   [`evacuate`](https://github.com/ghc/ghc/blob/ghc-8.4/rts/sm/Evac.c#L518) by
   hitting the `default` case, probably because the `ARR_WORDS` it finds is
   allocated in the injected runtime. `HEAP_ALLOCED_GC` is defined as a macro
   when `USE_LARGE_ADDRESS_SPACE` is set so we cannot hook it to do what we
   expect. if we can get heap allocations to agree between the two runtimes
   then i suspect that we can finally get `unsafePerformIO` to work.

## goals

1. provide a better interface for closures
2. extract the numbers that are added togehter in `base GHC.Num +`
