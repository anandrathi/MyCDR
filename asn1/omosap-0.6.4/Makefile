omosap : omosap.o vdchunk-asn1.o 
	g++ -o omosap omosap.o vdchunk-asn1.o -lvdchunk-base

vdchunk-asn1.o : vdchunk-asn1.cpp vdchunk-asn1.h known_tags.h known_tags.cpp 
	g++ -c -pedantic vdchunk-asn1.cpp -I /usr/include/ -I /usr/local/include/ 

omosap.o : omosap.cpp 
	g++ -c -pedantic omosap.cpp -I /usr/include/ -I /usr/local/include/

clean : 
	rm -f *.o
	rm -f omosap
