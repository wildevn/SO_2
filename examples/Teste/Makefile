all:	teste libt.a

libt.a:	
	cd ../../lib && make

teste: teste.c libt.a
	gcc -no-pie -I../../include teste.c -L../../lib -lt -lrt -o teste

clean:
	rm -f *.o *~ test core
