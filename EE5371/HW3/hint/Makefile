#         __   __
#         \/   \/ __ __   __ ________  TM
#         ||_  || \/ \ \  \/ |/~||~\|
#         ||-\-|| || ||\\ ||    ||
#         ||  ~|| || || \\||    ||
#         /\   /\ /\ /\  \ |    /\
#         ~~   ~~ ~~ ~~   \/    ~~
#
#  Creating HINT(tm)
#      1. Change ARCH to reflect your architecture. We currently support
#            unix      serial version only - general unix machine
#            NCUBE2S   requires mimd code
#            NCUBE2    requires mimd code
#            PARAGON   requires mimd code
#            SGI       shared memory parallel version - requires Power C
#            KSR       Kendall Square - requires shared memory version
#            MP1       MasPar 1 - requires SIMD version
#            MP2       MasPar 2 - requires SIMD version
#         note: If your architecture is not present, you will have to add a 
#               define and the architecture specific code within the HINT 
#               source code. See the README.porting and the comments within
#               the source code for help.
#      2. Set DATATYPE to the appropriate C data type to run your test on.
#         We have found that setting DATATYPE equal to DOUBLE is large enough
#         to sufficiently exercise most architectures. For integer testing, you
#         need to use at least a 32 bit integer. Supported data types are:
#            DOUBLE     double
#            FLOAT      float
#            LONGLONG   long long
#            LONG       long
#            INT        int
#      3. Set CC to the appropriate compiler.
#      4. Set the various FLAGS variables appropriately. 
#          The  "-D$(ARCH) -D$(DATATYPE)"  _MUST_ be present on both the 
#          KERN_CFLAGS and the DRIV_CFLAGS.
#       5. Save the Makefile and type 'make' to create the executables.
#          To run the benchmark type 'make run'.


ARCH       = unix
DATATYPE   = DOUBLE

DRIV_SRC   = hint.c
KERN_SRC   = hkernel.c
INCLUDES   = hint.h typedefs.h
TARGET     = hint
DATAFILE   = $(DATATYPE)

CC         = cc
KERN_CFLAGS= -c -O2 -D$(ARCH) -D$(DATATYPE)
DRIV_CFLAGS= -c -O2 -D$(ARCH) -D$(DATATYPE)
LFLAGS     = -lm

targets:    $(TARGET)


run:    $(TARGET) $(DATAFILE)


rerun:
		/bin/rm -f  $(DATAFILE)
		make run

clean:
		/bin/rm -f  $(DATAFILE) hint.o hkernel.o

clobber:
		/bin/rm -f  $(DATAFILE)
		/bin/rm -f  $(TARGET)


#
# This section of the Makefile is for compiling the binaries
#
hint: hint.o hkernel.o Makefile
		$(CC) hint.o hkernel.o -o hint $(LFLAGS)

hint.o: $(DRIV_SRC) $(INCLUDES) Makefile
		$(CC) $(DRIV_CFLAGS) -DIINT $(DRIV_SRC) -o hint.o

hkernel.o: $(KERN_SRC) $(INCLUDES) Makefile
		$(CC) $(KERN_CFLAGS) -DIINT $(KERN_SRC) -o hkernel.o

#
# This section has the data file dependancies
#

$(DATATYPE): hint
		./hint

