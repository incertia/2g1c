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

## profiting
1. launch your haskell application (`$ stack exec stack-calc`)
2. inject your haskell application (`$ sudo ./injector <pid>
   libhaskell-hook.dylib`)
3. profit

## goals

1. provide a better interface for closures
2. extract the numbers that are added togehter in `base GHC.Num +`
