include ../Makefile.inc

GEN_EXE = simple_pipe

EXE= ${GEN_EXE} 

all : ${EXE}

allgen : ${GEN_EXE}

clean :
	rm -f $(EXE) $(ERRLOG) $(IO)

showall :
	 @echo ${EXE}

${EXE} : ${SYSPRG_LIB}
