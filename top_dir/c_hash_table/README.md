Goal:
This is a learning project to understand Hash Tables concept by designing a hash table from scratch.

Outline:
I have written necessary functions that perform useful operations on hash tables. To verify the functionality of each of these, I created a tracefile.txt and upon running the program a results.txt file is created which outputs PASS/FAIL output for each operation mentioned in the tracefile.

I have also created a Makefile  (with GNU Make 4.4.1) for easier compilation and execution.
Makefile options: 
make   --> for general compilation which creates executable hash_bin.exe
make run  --> compiles and runs the executable hash_bin.exe
make debug  --> compiles with DEBUG_PRINTS macro enabled. This prints extra info useful for debugging upon execution
make clean  --> cleans the project by deleting executables and .o files

References:
I have taken help of "https://github.com/codecrafters-io/build-your-own-x" hash table project as guideline to achieve this. I have added the git repo as a submodule to this project