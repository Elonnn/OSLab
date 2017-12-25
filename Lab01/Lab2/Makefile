all: floor1 floor2 floor3 elevator elevator_monitor run

floor1: floor1.c
	gcc -o floor1 floor1.c -pthread
floor2: floor2.c
	gcc -o floor2 floor2.c -pthread
floor3: floor3.c
	gcc -o floor3 floor3.c -pthread
elevator : elevator.c
	gcc -o elevator elevator.c -pthread
elevator_monitor: elevator_monitor.c
	gcc -o elevator_monitor elevator_monitor.c -pthread
run : run.c
	gcc -o run run.c

clean:
	rm floor1 floor2 floor3 elevator elevator_monitor run
