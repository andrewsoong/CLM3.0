#-----------------------------------------------------------------------
# This Makefile is for building cprnc on AIX, Compaq, Linux (with pgf90 or 
# lf95 compiler), IRIX or SUN platforms. The code produces output nearly 
# identical to that of tools/cprnc. The latter code can be jettisoned when
# test-model goes away. Since test-model relies on cprnc, and demands certain
# quirky features of it (e.g. no .f90 files), the old cprnc code cannot be
# abandoned until test-model is also abandoned.
#
# These macros can be changed by setting environment variables:
#
# LIB_NETCDF --- Library directory location of netcdf. (defaults to /usr/local/lib)
# INC_NETCDF --- Include directory location of netcdf. (defaults to /usr/local/include)
#
#------------------------------------------------------------------------

# Set up special characters
null  :=

EXENAME = cprnc
RM = rm

# Check for the netcdf library and include directories 
ifeq ($(LIB_NETCDF),$(null))
LIB_NETCDF := /usr/local/lib
endif

ifeq ($(INC_NETCDF),$(null))
INC_NETCDF := /usr/local/include
endif

# Determine platform 
UNAMES := $(shell uname -s)

# Architecture-specific flags and rules
#------------------------------------------------------------------------
# SGI
#------------------------------------------------------------------------

ifeq ($(UNAMES),IRIX64)
FC      = f90
FFLAGS  = -64 -c -r8 -i4 -I$(INC_NETCDF) -O2
LDFLAGS = -64 -L$(LIB_NETCDF) -lnetcdf
endif

#------------------------------------------------------------------------
# AIX
#------------------------------------------------------------------------

ifeq ($(UNAMES),AIX)
FC      = xlf90
FFLAGS  = -c -I$(INC_NETCDF) -q64 -qsuffix=f=f90 -O2 -qmaxmem=-1
LDFLAGS = -L$(LIB_NETCDF) -q64 -lnetcdf
endif

#------------------------------------------------------------------------
# OSF1
#------------------------------------------------------------------------

ifeq ($(UNAMES),OSF1)
FC      = f90
FFLAGS  = -c -I$(INC_NETCDF)
LDFLAGS = -L$(LIB_NETCDF) -lnetcdf
endif

#-----------------------------------------------------------------------
# SUN
#-----------------------------------------------------------------------

ifeq ($(UNAMES),SunOS)
FC      = f90
FFLAGS  = -c -I$(INC_NETCDF) -stackvar
LDFLAGS = -L$(LIB_NETCDF) -L$(HOME)/lib -lnetcdf
endif

#------------------------------------------------------------------------
# Linux
#------------------------------------------------------------------------

ifeq ($(UNAMES),Linux)
  ifeq ($(USER_FC),$(null))
    FC := pgf90
  else
    FC := $(USER_FC)
  endif

  ifeq ($(FC),pgf90)
    ifeq ($(DEBUG),TRUE)
      FFLAGS = -c -I$(INC_NETCDF) -g -Ktrap=fp -Mbounds
    else
      FFLAGS = -c -I$(INC_NETCDF) -fast
    endif
    LDFLAGS = -L$(LIB_NETCDF) -lnetcdf
  endif

  ifeq ($(FC),lf95)
    ifeq ($(DEBUG),TRUE)
# currently lf95 debug will fail due to netcdf padding characters
      FFLAGS =  -c -I$(INC_NETCDF) -g --chk a,e,s,u
    else
      FFLAGS =  -c -I$(INC_NETCDF) -O
    endif
    LDFLAGS = -L$(LIB_NETCDF) -lnetcdf
  endif
endif

#------------------------------------------------------------------------
# Default rules and macros
#------------------------------------------------------------------------

OBJS := utils.o cprnc.o driver.o gatherstats.o netcdfids.o cntlvars.o \
        nfwrappers.o prec.o printers.o specialvalues.o types.o

.SUFFIXES:
.SUFFIXES: .f90 .o

.f90.o:
	$(FC) $(FFLAGS) $<

$(EXENAME): $(OBJS)
	$(FC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	$(RM) -f $(OBJS) *.mod $(EXEDIR)/$(EXENAME)

prec.o:
utils.o:
cprnc.o:         utils.o specialvalues.o netcdfids.o cntlvars.o nfwrappers.o driver.o utils.o
driver.o:        prec.o types.o cntlvars.o netcdfids.o specialvalues.o gatherstats.o nfwrappers.o \
                 printers.o utils.o
gatherstats.o:   prec.o types.o specialvalues.o cntlvars.o utils.o
netcdfids.o:
nfwrappers.o:    utils.o
printers.o:      prec.o types.o specialvalues.o nfwrappers.o netcdfids.o
cntlvars.o:    
specialvalues.o: prec.o
types.o:         prec.o
