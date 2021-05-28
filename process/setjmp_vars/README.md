# Processes

Small program to show the evils(?!) of gotos in systems programming. Based on
the useful writing of Michael Kerrisk: The Linux Programming Interface chpt6

The code segment in `setjmp_vars.c` is lifted from the book and deals with the
setjmp() and longjmp() system calls. 

The point here is to emphasize how compiler optimizations handle tricks with
gotos. The longjmp allows us to jump out 
