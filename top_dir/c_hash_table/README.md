c_hash_table project

### Project information ###
This is a project used to implement a Hash table using C. 
The submodules folder conatins the clone of git repo from where the project was learnt from and implemented.

The main_code directory contains the code to implement the functioanalities. There is an input trace_file.txt which contains a lot of input to run and the input value to be stored across the key/ dummy value in case of delete operation (col1, col2, col3 :: Operation, key, Value/dummy value).

The corresponding output for the testcases ran can be found in the results.txt file.

Additionally, a debugging compiler macro (DEBUG_PRINTS) is also added which the makeilfe uses during build.

### How to run ###
1) Move to directory top_dir\c_hash_table\main_code\src
2) Use the appropriate make command:
    make        --> This would build the code and generate the object files and executable. (same as "make all" and "make final")
    make clean  --> Delete object files and executable
    make debug  --> Would compile with the DEBUG_PRINTS macro defined