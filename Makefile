cc=g++
objects=source.o Matrix.o

ex.o: $(objects)
	   $(cc) $(objects) -o ex.out -lpthread

source.o: source.cpp             
	   $(cc) -c source.cpp    

Matrix.o: Matrix.cpp             
	   $(cc) -c Matrix.cpp    

clean:
	rm -f *.o
	rm -f *.out
     
