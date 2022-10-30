all: dh_assign_1.o rsa_assign_1.o dh_assign_1 rsa_assign_1

dh_assign_1: dh_assign_1.o 
	gcc dh_assign_1.o -o dh_assign_1 -lgmp

rsa_assign_1: rsa_assign_1.o 
	gcc rsa_assign_1.o -o rsa_assign_1 -lgmp

dh_assign_1.o: dh_assign_1.c
	gcc -c dh_assign_1.c -g3

rsa_assign_1.o: rsa_assign_1.c
	gcc -c rsa_assign_1.c -g3

clean:
	rm -rvf *.o *.key dh_assign_1 rsa_assign_1 $(filter-out plaintext.txt,$(wildcard *.txt))