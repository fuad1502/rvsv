# int fibonacci(int n) {
#	int t0 = 0;
#	int t1 = 1;
# 	for (int i = 0; i < n; i++) {
#		int temp = t0 + t1;
#		t0 = t1;
#		t1 = t;
#	}
#	return t0;
# }
fibonacci:
	addi t0, zero, 0 # t0
	addi t1, zero, 1 # t1
	addi t2, zero, 0 # i
	loop:
	bge t2, a0, end
	add t3, t0, t1 # temp
	addi t0, t1, 0
	addi t1, t3, 0
	addi t2, t2, 1
	jal t4, loop
	end:
	addi a0, t0, 0
