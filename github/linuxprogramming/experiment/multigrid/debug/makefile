CC= g++
CFLAGS=-O0 -g -DLINUX
LFLAGS=-O0
CUDAFLAGS = -O0 -g -DLINUX -arch compute_11 -code sm_11  # -deviceemu 
OUTDIR=./
SRCDIR=../
EMULIBS= -lcufftemu -lcublasemu -lcutil -L/usr/local/cuda/lib64 -L. -L/usr/local/cuda/sdk/C/common/lib/linux -L/usr/local/cuda/sdk/C/lib
LIBS= -lcudart -lcufft -lcutil -lcublas -L/usr/local/cuda/lib64 -L. -L/usr/local/cuda/sdk/C/common/lib64/linux -L/usr/local/cuda/sdk/C/lib
INCLS=. -I/usr/local/include 
CINCLS=. -I/usr/local/cuda/include -I/usr/local/cuda/sdk/C/common/inc
OBJ=DStream.o DataPoint.o Grid.o Cluster.o CommonUtil.o StreamOperation.o main.o Dbscan.o ConstVar.o cudawarp.o
multigrid:${OBJ}
					${CC} ${LFLAGS} -o ${OUTDIR}multigrid ${OUTDIR}*.o ${LIBS}
DStream.o:${SRCDIR}DStream.cpp ${SRCDIR}DStream.h ${SRCDIR}cudaRelated.h 
					${CC} ${CFLAGS} -c ${SRCDIR}DStream.cpp -o ${OUTDIR}DStream.o
DataPoint.o:${SRCDIR}DataPoint.cpp ${SRCDIR}DataPoint.h
					${CC} ${CFLAGS} -c ${SRCDIR}DataPoint.cpp	-o ${OUTDIR}DataPoint.o
Grid.o:${SRCDIR}Grid.cpp ${SRCDIR}Grid.h
					${CC} ${CFLAGS} -c ${SRCDIR}Grid.cpp	-o ${OUTDIR}Grid.o
Cluster.o:${SRCDIR}Cluster.cpp ${SRCDIR}Cluster.h
					${CC} ${CFLAGS} -c ${SRCDIR}Cluster.cpp	-o ${OUTDIR}Cluster.o
CommonUtil.o:${SRCDIR}CommonUtil.cpp ${SRCDIR}CommonUtil.h
					${CC} ${CFLAGS} -c ${SRCDIR}CommonUtil.cpp	-o ${OUTDIR}CommonUtil.o
StreamOperation.o:${SRCDIR}StreamOperation.cpp ${SRCDIR}StreamOperation.h
					${CC} ${CFLAGS} -c ${SRCDIR}StreamOperation.cpp 	-o ${OUTDIR}StreamOperation.o
main.o:${SRCDIR}main.cpp 
					${CC} ${CFLAGS} -c ${SRCDIR}main.cpp -o ${OUTDIR}main.o
Dbscan.o:${SRCDIR}Dbscan.cpp ${SRCDIR}Dbscan.h
				${CC} ${CFLAGS} -c ${SRCDIR}Dbscan.cpp -o ${OUTDIR}Dbscan.o
ConstVar.o:${SRCDIR}ConstVar.cpp  ${SRCDIR}ConstVar.h
					${CC} ${CFLAGS} -c ${SRCDIR}ConstVar.cpp		-o ${OUTDIR}ConstVar.o
cudaRelated.o:${SRCDIR}cudaRelated.cpp  ${SRCDIR}cudaRelated.h
				${CC} ${CFLAGS} -c ${SRCDIR}cudaRelated.cpp		-o ${OUTDIR}cudaRelated.o
cudawarp.o:${SRCDIR}cudawarp.cu ${SRCDIR}cudaKernel.cu
				nvcc ${CUDAFLAGS} -c ${SRCDIR}cudawarp.cu -I${CINCLS} -o cudawarp.o
clean:
					rm -f ${OUTDIR}*.o ${OUTDIR}multigrid