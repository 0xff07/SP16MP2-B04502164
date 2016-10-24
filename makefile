main : 
	gcc tree_walk.c -g -o tree_walk
clean : 
	rm -f *.o
	rm -f *.exec
	rm -f -r *.dSYM
	rm -f tree_walk
