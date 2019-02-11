The following files are C programs that works the same as the cat command, the difference between them is that one uses system calls and the other one uses stdio functions. 

## To run cat.c or cat2.c (the one with system calls and the one with stdio functions in respective order) follow these steps:
1. gcc cat.c

⋅⋅⋅Or gcc cat2.c depends on which one do you want to compile

2. ./a.out 

⋅⋅⋅Actually, the 2nd step works the same as cat, so consider ./a.out as cat and try it however you want.

if you are using linux, test their execution time differences with the time command, you'll notice that cat.c is faster.
