build: quadtree.o
	gcc quadtree.c -lm -Wall -o quadtree

quadtree.o: quadtree.c
	gcc -c quadtree.c

run: quadtree
	./quadtree

clean:
	rm *.o quadtree
