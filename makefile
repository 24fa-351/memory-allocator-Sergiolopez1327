CC = gcc
CFLAGS = -lpthread

# Targets for building the main server
memtest: memtest.c malloc.c
	$(CC) -o memtest memtest.c malloc.c $(CFLAGS)

memtest.o: memtest.c malloc.h
	$(CC) -c memtest.c

malloc.o: malloc.c malloc.h
	$(CC) -c malloc.c

# Run two tests and save results to results.txt
test: memtest
	# Run with default test string
	@echo "Memory Allocation Test" > results.txt
	@echo "Command-line input: \"./memtest.c\"" >> results.txt
	@echo "" >> results.txt
	@./memtest >> results.txt 2>&1
	@echo "" >> results.txt
	@echo "--------------------------------------------------" >> results.txt
	@echo "" >> results.txt

	# Run with custom test string
	@echo "Memory Allocation Test" >> results.txt
	@echo "Command-line input: \"./memtest.c Custom test string for allocation testing!\"" >> results.txt
	@echo "" >> results.txt
	@./memtest "Custom test string for allocation testing!" >> results.txt 2>&1

# Clean up compiled files
clean:
	rm -f memtest memtest.o malloc.o results.txt

