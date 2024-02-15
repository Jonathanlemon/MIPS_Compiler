For testing the compiler:

"make" runs the makefile to compile the program
"make clean" cleans the workspace of the previous output files

"make test1" tests the program with input.txt, and generates output to output.dat
"make print1" prints the hexdump of output.dat

"make test2" tests the program with input2.txt, and generates output to output2.dat
"make print2" prints the hexdump of output2.dat

"make test3" tests the program with input.txt, and generates output to output3.dat
This test intentionally fails, to show the compiler can see an invalid register

"make test4" tests the program with input4.txt, and generates output to output4.dat
This test intentionally fails, to show the compiler can see an invalid instruction