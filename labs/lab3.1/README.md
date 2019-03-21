# Lab 3.1 - File/Directory Monitor

This program logs all file creations, deletions, and renames under the directory named in its command-line argument. The program monitor events in all of the subdirectories under the specified directory. When a new subdirectory is added under the tree or a directory is deleted, the set of monitored subdirectories are going to be updated accordingly.

## Getting Started

First, get into this folder and type make

```

cd path/to/this/code
make

```

The Makefile will compile everything and will let the code be ready for use. 

To run this code type in your terminal 

```

./monitor dir1

```

where dir1 is any directory that you want to monitor

### Prerequisites

```

C compiler

```

## Brief usage example

We run our code and start monitoring a folder with

```

./monitor dir1

```

Open another terminal and start creating, deleting and renaming files inside dir1

```
touch exampleA
mkdir exampleB
mv exampleA exampleC
rm -rf exampleA

```

and check how the first terminal starts updating the events that you provoked 