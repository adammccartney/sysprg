include ../Makefile.inc

GEN_EXE = simple_pipe pipe_sync pipe_ls_wc popen_glob fifo_seqnum_client \
		  fifo_seqnum_server

EXE= ${GEN_EXE} 

all : ${EXE}

allgen : ${GEN_EXE}

clean :
	rm -f $(EXE) $(ERRLOG) $(IO)

showall :
	 @echo ${EXE}

${EXE} : ${SYSPRG_LIB}
