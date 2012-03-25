CC= g++
CFLAGS=-O0 -g -DLINUX -Wall
LFLAGS=-O0
OUTDIR=./
SRCDIR=../
OBJ=DStream.o DataPoint.o Grid.o Cluster.o CommonUtil.o StreamOperation.o main.o Dbscan.o ConstVar.o
multigrid:${OBJ}
					${CC} ${LFLAGS} -o ${OUTDIR}DStream ${OUTDIR}*.o
DStream.o:${SRCDIR}DStream.cpp ${SRCDIR}DStream.h
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
clean:
					rm -f ${OUTDIR}*.o ${OUTDIR}multigrid