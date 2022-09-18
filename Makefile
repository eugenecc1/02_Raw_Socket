output: my_ping
	gcc -o my_ping my_ping.c

clean:
	rm *.o output
