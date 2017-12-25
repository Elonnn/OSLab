objects = main.o call.o function.o verify.o 
result:$(objects)
	cc -o result -Wall $(objects)
main.o:header.h hello.h 
call.o:header.h
function.o:hello.h
verify.o:header.h

.PHONY:clean
clean:
	cat main.c
	rm result $(objects) 

	
