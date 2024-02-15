default:
	gcc project1.c -lm
test1:
	./a.out input.txt output.dat
test2:
	./a.out input2.txt output2.dat
test3:
	./a.out input3.txt output3.dat
test4:
	./a.out input4.txt output4.dat
clean:
	rm output.dat -f
	rm output2.dat -f
	rm output3.dat -f
	rm output4.dat -f
print1:
	xxd -b -c 4 output.dat
print2:
	xxd -b -c 4 output2.dat
