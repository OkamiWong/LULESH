NVCC		= nvcc
FLAGS		= -arch=sm_80
DFLAGS	= -lineinfo
RFLAGS 	= -O3 -DNDEBUG 

all: release 

debug: LINKFLAGS += 

release: 	FLAGS += $(RFLAGS)
debug: 		FLAGS += $(DFLAGS)

release: lulesh
debug: lulesh

lulesh: lulesh.o
	$(NVCC) $(LINKFLAGS) lulesh.o -o lulesh

lulesh.o: lulesh.cu util.h vector.h
	$(NVCC) $(FLAGS) lulesh.cu -I ./  $(INC_SILO) -c -o lulesh.o

clean: 
	rm -rf allocator.o lulesh-comms.o lulesh-comms-gpu.o lulesh.o lulesh xyz.asc regNumList.txt
