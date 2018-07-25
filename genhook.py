#! /usr/bin/env python

import sys

# this prog
prog = sys.argv[0]

if len(sys.argv) != 2:
    print "usage: {} <name>".format(prog)
    sys.exit(0)

name = sys.argv[1]

print "global _{}_stub".format(name)
print "global _{}_closure".format(name)
print "global _old_{}".format(name)
print ""
print "section .text"
print ""
print "extern save_regs"
print "extern restore_regs"
print "extern _hk_{}".format(name)
print ""
print "_{}_stub:".format(name)
print "  cmp rax, r15"
print "  jb _{}_stub.garbage".format(name)
print "  call save_regs"
print "  call _hk_{}".format(name)
print "  call restore_regs"
print "  push qword [rel _old_{}]".format(name)
print "  add qword [rsp], 0x09"
print "  ret"
print ".garbage:"
print "  mov rbx, [rel _{}_closure]".format(name)
print "  jmp qword [r13 - 0x08]"
print ""
print "section .bss"
print ""
print "_{}_closure: resq 1".format(name)
print "_old_{}: resq 1".format(name)
