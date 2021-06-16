all: fs

mkfs:
	gcc fs.c -o fs -g
clean:
	rm -rf *.o fs *.img