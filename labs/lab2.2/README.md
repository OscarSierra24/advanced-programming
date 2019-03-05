# Lab 2.2 - Logger Library

logger.c is a logger library that prints the following messages:

* INFO General information.
* WARN Warnings.
* ERROR Errors.
* PANIC Panics. Non recoverable issues with core dump.

To include this library you must include logger.h in your C code and then, when compiling, you must add logger.c

for example:

```
gcc -o tlogger logger.c testLogger.c -I.
```

Note: -I. makes gcc look for logger.h inside . folder, if you want to move it to another folder, -I. would change.
For example, if logger.h is located at tmp -Itmp would be the appropiate argument

## Getting Started

type: 

```
make
```

in your terminal.

### Prerequisites

```
a C compiler
```

## Testing

to test how each log message should look like, type ./tlogger in your terminal