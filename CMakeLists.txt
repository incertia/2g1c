cmake_minimum_required(VERSION 2.8)

project(haskell_injector)

enable_language(ASM_NASM)
set(CAN_USE_ASSEMBLER TRUE)

set(CMAKE_C_FLAGS "-pthread -fmessage-length=0 -fdiagnostics-show-note-include-stack -fmacro-backtrace-limit=0 -std=gnu99 -O0 -Wall -DDEBUG=1")

set(CMAKE_C_LINK_FLAGS "-pthread")
set(GHC_HOME "$ENV{HOME}/.stack/programs/x86_64-osx/ghc-8.4.3/lib/ghc-8.4.3")

#set(CMAKE_SHARED_LINKER_FLAGS "-read_only_relocs suppress")

set_source_files_properties(src/injector/mach_inject.c PROPERTIES LANGUAGE C)
include_directories(injector src/haskell-hook/libudis86 src/haskell-hook)

# Oddly, you can't have a folder with the same name as the output file. Hence,
# 'injector_app'
add_executable(injector src/injector/mach_inject.c src/injector/injector.c src/injector/main.c)

add_library(bootstrap SHARED src/bootstrap/main.c)

add_library(haskell-hook SHARED src/haskell-hook/libudis86/input.c
  src/haskell-hook/libudis86/syn.c src/haskell-hook/libudis86/udis86.c
  src/haskell-hook/libudis86/syn-att.c src/haskell-hook/libudis86/decode.c
  src/haskell-hook/libudis86/itab.c src/haskell-hook/main.c
  src/haskell-hook/hook.c src/haskell-hook/zp.c src/haskell-hook/haskell.c
  src/haskell-hook/zp.asm src/haskell-hook/stgreg.asm
  src/haskell-hook/unsafe_zp.o src/haskell-hook/foreign_c_add.c)

target_link_libraries(haskell-hook
  ${GHC_HOME}/rts/libHSrts.a
  ${GHC_HOME}/rts/libCffi.a
  ${GHC_HOME}/base-4.11.1.0/libHSbase-4.11.1.0.a
  ${GHC_HOME}/ghc-prim-0.5.2.0/libHSghc-prim-0.5.2.0.a
  ${GHC_HOME}/integer-gmp-1.0.2.0/libHSinteger-gmp-1.0.2.0.a
  iconv
  )

include_directories(haskell-hook ./src/ghc/includes)
target_compile_definitions(haskell-hook PRIVATE
  NO_GLOBAL_REG_DECLS
  HAVE_STRING_H)
