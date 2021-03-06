#-----------------------------------------------------------------------
# Makefile for building CLM on various platforms.
#
# This makefile assumes the existence the file:
#
# Filepath      The directories, listed one per line, that contain the source
#               code required to build CLM.  This list is used the set the
#               VPATH variable which is used by GNU make to search for
#               dependencies (after it looks in the directory from which
#               it was invoked).  This list of directories, prepended with ./,
#               is also used to construct the list of search directories 
#               used by the preprocessor (as specified by -I command-line options).
#
# The following macros may be set in the user's environment:
#
# ROOTDIR       Root directory for the CLM source distribution.  If ROOTDIR is not
#               set the makefile searches for it in a file called "Rootdir".
# EXENAME       Name to call the executable.  Default: clm
# MODEL_EXEDIR  Directory to build the executable in.  Default: ./
# INC_NETCDF    Directory location of the NetCDF include files.  Default: /usr/local/include
# LIB_NETCDF    Directory location of the NetCDF library.  Default: /usr/local/lib
# INC_MPI       Directory location of the MPI include files.  Default: /usr/local/include
# LIB_MPI       Directory location of the MPI library.  Default: /usr/local/lib
# ESMF_ROOT     Root directory of ESMF source tree.  Default: $(ROOTDIR)/models/utils/esmf
# ESMF_BLD      Upper level build directory for ESMF
#               The ESMF .o files are built in $(ESMF_BLD)/obj
#               The ESMF .m files are built in arch dependent subdirs of $(ESMF_BLD)/mod
#               The ESMF library is built in architecture and optimization
#               dependent subdirectories of $(ESMF_BLD)/lib
#               Default: ./esmf
# DEBUG         Set to TRUE to turn on compiler debugging options.  Default: FALSE 
# SPMD          Whether to build in SPMD mode or not. 	[values TRUE FALSE]
#               Default: read from ./misc.h
#               N.B. To ensure consistency between the Makefile and misc.h the SPMD
#                    macro should not be set.  This forces it to be read from misc.h.
# SMP           Set to TRUE to enable building in SMP mode (uses OpenMP).  
#               Currently implemented for IBM, SGI, linux-pgf90. (default is TRUE on IBM and 
#               linux-pgf90, and depends on SPMD setting on SGI).
# USER_FC       Allow user to override the default Fortran compiler specified in Makefile.
# USER_CC       Allow user to override the default C compiler specified in Makefile (linux only).
# USER_FFLAGS   Additional compiler flags that the user wishes to set.
# NO_SWITCH     On Compaq if the hardware switch is not available
#               set this env variable to "TRUE".
#
# Note: The ESMF library is included in the CLM distribution in 
#       $ROOTDIR/src/utils/esmf and is built using this makefile.  
#------------------------------------------------------------------------

# Set up special characters
null  :=
space := $(null) $(null)
comma := $(null),$(null)

# Determine distribution root directory
ifeq ($(ROOTDIR),$(null))
ROOTDIR := $(shell cat Rootdir)
endif

# Check for the NetCDF library and include directories 
ifeq ($(LIB_NETCDF),$(null))
LIB_NETCDF := /usr/local/lib
endif

ifeq ($(INC_NETCDF),$(null))
INC_NETCDF := /usr/local/include
endif

# Check for the MPI library and include directories 
ifeq ($(LIB_MPI),$(null))
LIB_MPI := /usr/local/lib
endif

ifeq ($(INC_MPI),$(null))
INC_MPI := /usr/local/include
endif

# Build the ESMF library
ifeq ($(ESMF_ROOT),$(null))
ESMF_ROOT := ${ROOTDIR}/src/utils/esmf
endif

ifeq ($(ESMF_BLD),$(null))
ESMF_BLD := $(shell (mkdir ./esmf 2>/dev/null;cd ./esmf;pwd))
endif

ESMF_BOPT := O
ifeq ($(DEBUG),TRUE)
  ESMF_BOPT := g
endif
ESMF_MOD := $(ESMF_BLD)/mod/mod$(ESMF_BOPT)
ESMF_LIB := $(ESMF_BLD)/lib/lib$(ESMF_BOPT)

# Check for directory in which to put executable
ifeq ($(MODEL_EXEDIR),$(null))
MODEL_EXEDIR := .
endif

# Check for name of executable
ifeq ($(EXENAME),$(null))
EXENAME := clm
endif

# Check if SPMD is defined in "misc.h"
# Ensure that it is defined and not just "undef SPMD" set in file
ifeq ($(SPMD),$(null))
  SPMDSET := $(shell grep SPMD misc.h)
  ifneq (,$(findstring define,$(SPMDSET)))
	SPMD := TRUE
  else
	SPMD := FALSE
  endif
endif

# Load dependency search path.
dirs := . $(shell cat Filepath)

# Set cpp search path, include netcdf
cpp_dirs := $(dirs) $(INC_NETCDF) $(INC_MPI)
cpp_path := $(foreach dir,$(cpp_dirs),-I$(dir)) # format for command line

# Expand any tildes in directory names. Change spaces to colons.
VPATH    := $(foreach dir,$(cpp_dirs),$(wildcard $(dir))) 
VPATH    := $(subst $(space),:,$(VPATH))               


#------------------------------------------------------------------------
# Primary target: build the model
#------------------------------------------------------------------------
all: $(MODEL_EXEDIR)/$(EXENAME)

# Get list of files and determine objects and dependency files
# Optionally build only a single dependency file for all .o files
#   using perl scripts mkSrcfiles and mkDepends

SOURCES   := $(shell cat Srcfiles)

Depends: Srcfiles Filepath
	$(ROOTDIR)/bld/offline/mkDepends Filepath Srcfiles > $@

Srcfiles: Filepath
	$(ROOTDIR)/bld/offline/mkSrcfiles > $@

OBJS   := $(addsuffix .o, $(basename $(SOURCES)))

CPPDEF := -DNO_SHR_VMATH -DHIDE_SHR_MSG

$(MODEL_EXEDIR)/$(EXENAME): $(OBJS)
	$(FC) -o $@ $(OBJS) -L$(LIB_NETCDF) -lnetcdf -L$(ESMF_LIB)/$(ESMF_ARCH) -lesmf $(LDFLAGS)

debug: $(OBJS)
	echo "FFLAGS: $(FFLAGS)"
	echo "LDFLAGS: $(LDFLAGS)"
	echo "OBJS: $(OBJS)"

#***********************************************************************
#********** Architecture-specific flags and rules***********************
#***********************************************************************

# Determine platform 
UNAMES := $(shell uname -s)
#UNAMES := SUPER-UX

#------------------------------------------------------------------------
# AIX
#------------------------------------------------------------------------

ifeq ($(UNAMES),AIX)

ifeq ($(SMP),$(null))
  SMP := TRUE
endif

#ADDRESS    := Q32
 ADDRESS    := Q64
ifeq ($(ADDRESS),Q32)
  QCMP      :=
  QLDR      := -bmaxdata:0x80000000 -bmaxstack:0x10000000
  ESMF_ARCH := rs6000_sp
endif
ifeq ($(ADDRESS),Q64)
  QCMP      := -q64
  QLDR      := -q64
  ESMF_ARCH := rs6000_64
endif

CPP        := /lib/cpp
CC         := xlc_r
CFLAGS     := $(cpp_path) -O2 -DAIX $(CPPDEF) $(QCMP)
FPPFLAGS   := -WF,-DAIX,$(subst $(space),$(comma),$(CPPDEF))

mod_path   := -I$(ESMF_MOD)/$(ESMF_ARCH) 
FFLAGS     := $(cpp_path) $(mod_path) $(FPPFLAGS) -qarch=auto -qrealsize=8 -qdpc=e \
		-qspillsize=2500 -g -qfullpath $(QCMP)
FREEFLAGS  := -qsuffix=f=f90:cpp=F90
FIXEDFLAGS := -qfixed=132

LDFLAGS    := $(QLDR)

# Use the thread-safe compilers even when using enabling openMP.
ifeq ($(SPMD),TRUE)
  FC := mpxlf90_r
else
  FC := xlf90_r
  FFLAGS += -WF,-DHIDE_MPI
endif

ifeq ($(SMP),TRUE)
  # THREADED_PTHREADS is used by the timing library
  CFLAGS += -DTHREADED_PTHREADS
endif

ifeq ($(DEBUG),TRUE)

  # Bounds checking is unreliable on the IBM.
  # Sometimes you can get it to go if you turn threading off (by deleting -qsmp=omp)
  # Only turn float-trapping on for debug mode as it's a 20% performance hit.
  FFLAGS += -qinitauto=FF911299 -qflttrap=ov:zero:inv:en -C
  ifeq ($(SMP),TRUE)
    FFLAGS += -qsmp=omp:noopt
    LDFLAGS += -qsmp=omp:noopt
  endif
else
  # Inline when not debugging
  FFLAGS += -O2 -qmaxmem=-1 -Q
  ifeq ($(SMP),TRUE)
    FFLAGS += -qsmp=omp
    LDFLAGS += -qsmp=omp -lmass
  endif
endif

.SUFFIXES:
.SUFFIXES: .F .F90 .c .o

.F.o:
	$(FC) -c $(FIXEDFLAGS) $(FFLAGS) $<

.F90.o:
	$(FC) -c $(FREEFLAGS) $(FFLAGS) $<

.c.o:
	cc -c  $(CFLAGS) $<

endif

#------------------------------------------------------------------------
# SGI
#------------------------------------------------------------------------

ifeq ($(UNAMES),IRIX64)

# default is either pure SPMD or pure SMP parallelism
ifeq ($(SMP),$(null))
  ifeq ($(SPMD),TRUE)
    SMP := FALSE
  else
    SMP := TRUE
  endif
endif

ESMF_ARCH := IRIX64
FC        := f90
CPP       := /lib/cpp
CPPFLAGS  := -P
CFLAGS    := $(cpp_path) -64 -DIRIX64 -O2 $(CPPDEF)
FFLAGS     = $(cpp_path) -64 -r8 -i4 -c -cpp -extend_source -DIRIX64 \
		$(CPPDEF) -I$(ESMF_MOD)/$(ESMF_ARCH)
# -mp is used at load time even when SMP=FALSE to satisfy external
# references (pthreads) in the timing library
LDFLAGS    = -64 -mp

ifeq ($(SPMD),TRUE)
  FFLAGS  += -I$(INC_MPI)
  LDFLAGS += -L$(LIB_MPI) -lmpi 
else
  FFLAGS  += -DHIDE_MPI
endif

ifeq ($(SMP),TRUE)
  # THREADED_OMP is used by the timing library
  CFLAGS += -DTHREADED_OMP
# Only enable OpenMP, not SGI specific parallelism
  FFLAGS  += -mp -MP:dsm=OFF -MP:old_mp=OFF
endif

# WARNING: -mp and -g together cause wrong answers
ifeq ($(DEBUG),TRUE)
  FFLAGS  += -g -DEBUG:trap_uninitialized=ON -C
else
  FFLAGS  += -O2
endif

.SUFFIXES:
.SUFFIXES: .F .F90 .c .o

.F.o:
	$(FC) $(FFLAGS) $<

.F90.o:
	$(FC) $(FFLAGS) $<
.c.o:
	cc -c  $(cpp_path) $(CFLAGS) $<

endif

#------------------------------------------------------------------------
# SUN
#------------------------------------------------------------------------

ifeq ($(UNAMES),SunOS)
ESMF_ARCH = solaris
FC	:= f90
FC77	:= f77
CPP     := /usr/ccs/lib/cpp
CFLAGS  := $(cpp_path) -DSUNOS $(CPPDEF)

# Don't use OpenMP on Solaris as it currently causes problems
FFLAGS  := $(cpp_path) -xs -stackvar -Qoption f90comp -r8const \
		-xtypemap=real:64,integer:32 -e -DSUNOS -DHIDE_MPI \
		-M$(ESMF_MOD)/$(ESMF_ARCH) $(CPPDEF)
LDFLAGS := -L/opt/SUNWspro/lib -lf77compat -openmp -fast
SPEC_FFLAGS := $(cpp_path) -r8 -i4 -c

ifeq ($(DEBUG),TRUE)
  FFLAGS += -g -dalign
  SPEC_FFLAGS += -g -dalign
else
  # Inline code when not debugging
  # Use symbol table (-g) as make execution stable and can find out where problems are.
  # Turn off aggressive optimization options (fsimple=1, fns=no) so the climate
  # will be similar to climate on the IBM
  FFLAGS += -inline=%auto -fast -g -fsimple=1 -fns=no -O4
endif

ifeq ($(SPMD),TRUE)
  FFLAGS  += -I$(INC_MPI)
  LDFLAGS += -L$(LIB_MPI) -lmpich -lnsl -lsocket
else
  FFLAGS  += -DHIDE_MPI
endif

.SUFFIXES:
.SUFFIXES: .F .F90 .c .o

.F90.o:
	$(FC) -c $(FFLAGS) $<

endif

#------------------------------------------------------------------------
# Linux
#------------------------------------------------------------------------

ifeq ($(UNAMES),Linux)

ESMF_ARCH = linux

# If the compiler is not specified, choose PGI pgf90.

ifeq ($(USER_FC),$(null))
  FC := pgf90
else
  FC := $(USER_FC)
endif

# If USER_CC is set, use it. 
# Otherwise use pgcc if pgf90 is the Fortran compiler, and cc if not.

ifeq ($(USER_CC),$(null))
  ifeq ($(FC),pgf90)
    CC := pgcc
  else
    CC := cc
  endif
else
  CC := $(USER_CC)
endif

# Figure out the ESMF architecture.

ifeq ($(FC),pgf90)
  ifeq ($(CC),pgcc)
    ESMF_ARCH = linux_pgi
  else
    ESMF_ARCH = linux_gnupgf90
  endif
endif

ifeq ($(SMP),TRUE)
  # THREADED_OMP and THREADED_PTHREADS are used by the timing library
  ifeq ($(CC),pgcc)
    CPPDEF += -DTHREADED_OMP
  else
    CPPDEF += -DTHREADED_PTHREADS
  endif
endif

CFLAGS = $(cpp_path) -DLINUX $(CPPDEF)
ifeq ($(CC),pgcc)
  CFLAGS += -fast
else
  CFLAGS += -DUSE_GCC
endif

#-------------------
# pgf90
#-------------------

ifeq ($(FC),pgf90)

  ifeq ($(SMP),$(null))
    SMP := FALSE
  endif
  FFLAGS    := $(cpp_path) -r8 -i4 $(CPPDEF) -Mrecursive -Mdalign -Mextend -DLINUX \
	       -DPGF90 -I$(ESMF_MOD)/$(ESMF_ARCH) -byteswapio
  FREEFLAGS := -Mfree
  LDFLAGS   :=
  ifeq ($(DEBUG),TRUE)
   #Newer pgf90 version have problems with bounds checking and -g for the clm code
   #For now turning this off in debug mode
   #FFLAGS      += -g -Ktrap=fp -Mbounds
    FFLAGS      +=    -Ktrap=fp
    SPEC_FFLAGS := $(FFLAGS) 
  else
    SPEC_FFLAGS := $(FFLAGS) 
    FFLAGS += -fast
  endif
  ifeq ($(SMP),TRUE)
    FFLAGS  += -mp
    LDFLAGS += -mp
  endif

endif

#-------------------
# lf95
#-------------------

ifeq ($(FC),lf95)

  # Note that as of lf95 version 6.1 threading does NOT work because of
  # ridiculously small per thread stacksize limits.
  #  -CcdRR8 is an undocumented flag which promotes only vars declared "real", not "real*8"
  # --trace produces a call traceback on abort
  # --trap causes code to stop on divide by zero or overflow exceptions
  # --pca prevents overwriting constant arguments
  # --staticlink enables transportability of the executable
  # --chk for basic compiler checking (a,e,s,u,x)
  # --chkglobal for global checking

  ESMF_ARCH   := linux_lf95
  FFLAGS      := $(cpp_path) -CcdRR8 $(CPPDEF) --trace --trap --wide -DLINUX -I$(ESMF_MOD)/$(ESMF_ARCH)
  SPEC_FFLAGS := $(FFLAGS)
  LDFLAGS     := --staticlink
  ifeq ($(DEBUG),TRUE)
    FFLAGS += -g --chk a,e,s,u --pca
  else
    FFLAGS += -O
  endif

endif

#-------------------
# FFC f90
#-------------------

ifeq ($(FC),f90)

  FFLAGS  = -CcdRR8 -Cpp -X9 -fw -Am -Wa,-W $(cpp_path) $(CPPDEF) -DLINUX \
                 -I$(ESMF_MOD)/$(ESMF_ARCH) -byteswapio
  LDFLAGS = 
  ifeq ($(DEBUG),TRUE)
    FFLAGS += -g -H aseu
  else
    FFLAGS += -Kfast,eval,fastlib,auto 
  endif

endif

#-------------------
# Flags common to all compilers
#-------------------

ifeq ($(SPMD),TRUE)
  FFLAGS  += -I$(INC_MPI) 
  LDFLAGS += -L$(LIB_MPI) -lmpich
else
  FFLAGS  += -DHIDE_MPI 
endif

ifeq ($(SMP),TRUE)
  # THREADED_OMP and THREADED_PTHREADS are used by the timing library
  ifeq ($(CC),pgcc)
    CFLAGS += -DTHREADED_OMP
  else
    CFLAGS += -DTHREADED_PTHREADS
  endif
endif

.SUFFIXES:
.SUFFIXES: .F .F90 .c .o

.F90.o:
	$(FC) -c $(FREEFLAGS) $(FFLAGS) $<
.F.o:
	$(FC) -c $(FFLAGS) $<
.c.o:
	$(CC) -c $(CFLAGS) $<

endif

#------------------------------------------------------------------------
# OSF1
#------------------------------------------------------------------------

ifeq ($(UNAMES),OSF1)

ESMF_ARCH     := alpha
CFLAGS        := $(cpp_path) -DOSF1 -O2 -omp $(CPPDEF)
FC            := f90
FFLAGS        := $(cpp_path) $(CPPDEF) -r8 -i4 -c -omp -automatic -fpe3 \
			-I$(ESMF_MOD)/$(ESMF_ARCH) -check omp_bindings
FFLAGS_DOTF90 := -DOSF1 -free -fpe3
FFLAGS_DOTF   := -extend_source -omp -automatic
LDFLAGS       := -omp 

ifeq ($(DEBUG),TRUE)
  FFLAGS += -g3 -C
else
# Inline when not debugging
  FFLAGS += -O3 -inline speed
endif

ifeq ($(NO_SWITCH),$(null))
  NO_SWITCH := FALSE
endif
ifeq ($(SPMD),TRUE)
  FFLAGS  += -I$(INC_MPI) 
  LDFLAGS += -lmpi
else
  FFLAGS  += -DHIDE_MPI
endif

ifeq ($(SMP),TRUE)
  # THREADED_OMP is used by the timing library
  CFLAGS += -DTHREADED_OMP
endif

ifneq ($(NO_SWITCH),TRUE)
  LDFLAGS += -lelan
endif

.SUFFIXES:
.SUFFIXES: .F .F90 .c .o

.F.o:
	$(FC) $(FFLAGS) $(FFLAGS_DOTF) $<

.F90.o:
	$(FC) $(FFLAGS) $(FFLAGS_DOTF90) $<

.c.o:
	cc -c  $(CFLAGS) $<

endif

#------------------------------------------------------------------------
# Cray X1
#------------------------------------------------------------------------

ifeq ($(UNAMES),UNICOS/mp)

ESMF_ARCH     := cray_x1
COPT          := -O2
FOPT          := -Ovector3,stream3,scalar3
ifeq ($(SMP),TRUE)
   FOPT	      += -x csd -Otask1
   # THREADED_OMP is used by the timing library
   CPPDEF     += -DTHREADED_OMP
else
   FOPT	      += -x omp -Otask0
endif
CFLAGS        := $(cpp_path) $(CPPDEF) -DUNICOSMP -DCPP_VECTOR -h gen_private_callee
FC            := ftn
FFLAGS        := $(cpp_path) $(CPPDEF) -em -dp -s real64 -F \
			-rm -ra -Omsgs,negmsgs -Ogen_private_callee \
			-DUNICOSMP -DCPP_VECTOR -dy -p$(ESMF_MOD)/$(ESMF_ARCH)
FREEFLAGS     := -f free
FIXEDFLAGS    := -f fixed -N 132
LDFLAGS       :=

.SUFFIXES:
.SUFFIXES: .F .F90 .c .o

.F.o:
	$(FC) -c $(FIXEDFLAGS) $(FFLAGS) $(FOPT) $<

.F90.o:
	$(FC) -c $(FREEFLAGS) $(FFLAGS) $(FOPT) $<

.c.o:
	cc -c  $(CFLAGS) $(COPT) $<

FrictionVelocityMod.o : FrictionVelocityMod.F90 clmtype.o shr_const_mod.o shr_kind_mod.o clm_varcon.o preproc.h misc.h
	$(FC) -c $(FREEFLAGS) $(FFLAGS) $(FOPT) -Omodinline $<

QSatMod.o : QSatMod.F90 shr_const_mod.o shr_kind_mod.o preproc.h misc.h
	$(FC) -c $(FREEFLAGS) $(FFLAGS) $(FOPT) -Omodinline $<

shr_orb_mod.o : shr_orb_mod.F90 shr_const_mod.o shr_sys_mod.o shr_kind_mod.o
	$(FC) -c $(FREEFLAGS) $(FFLAGS) $(FOPT) -Omodinline $<

endif

#------------------------------------------------------------------------
# NEC SX6
#------------------------------------------------------------------------

ifeq ($(UNAMES),SUPER-UX)

ESMF_ARCH     := SX6
PROF          :=
PROF          := -ftrace
COPT          :=
FOPT          := -Cvsafe -Wf,-L,fmtlist,map,transform \
		-Wf,-pvctl,loopcnt=100000,-A,idbl4,-ptr,byte
FOPT          := -pi -Cvopt -Wf,-L,fmtlist,map,transform \
		-Wf,-pvctl,fullmsg,noassume,loopcnt=1000000,noloopchg,-A,idbl4,-ptr,byte
CC            := sxc++
CFLAGS        := $(cpp_path) $(CPPDEF) -D_SX  -DNEC_SX -DCPP_VECTOR \
		$(PROF)
FC            := sxf90 -Yf,/SX/opt/sxf90/rev285/lib
FFLAGS        := $(cpp_path) $(CPPDEF)  -DHIDE_MPI -D_SX -DNEC_SX -DCPP_VECTOR \
		-I$(ESMF_MOD)/$(ESMF_ARCH) \
		$(PROF) $(FOPT)
FREEFLAGS     :=
FIXEDFLAGS    :=
LDFLAGS       := $(PROF)

ifeq ($(SMP),TRUE)
  CPPDEF += -DTHREADED_OMP
endif

.SUFFIXES:
.SUFFIXES: .F .F90 .c .o

.F.o:
	$(FC) -c $(FIXEDFLAGS) $(FFLAGS) $<

.F90.o:
	$(FC) -c $(FREEFLAGS) $(FFLAGS) $<

.c.o:
	$(CC) -c  $(CFLAGS) $<

QSatMod.f90:	QSatMod.F90
	 cpp $(cpp_path) $(CPPDEFS) $< > QSatMod.f90

CanopyFluxesMod.o:	CanopyFluxesMod.F90 QSatMod.f90  FrictionVelocityMod.f90
	$(FC) -c $(FREEFLAGS) $(FFLAGS) $< -pi exp=qsat,moninobukini expin=QSatMod.f90,FrictionVelocityMod.f90

Biogeophysics1Mod.o:	Biogeophysics1Mod.F90 QSatMod.f90
	$(FC) -c $(FREEFLAGS) $(FFLAGS) $< -pi exp=qsat expin=QSatMod.f90

FrictionVelocityMod.f90	: FrictionVelocityMod.F90
	 cpp $(cpp_path) $(CPPDEFS) $< > FrictionVelocityMod.f90

BareGroundFluxesMod.o	: BareGroundFluxesMod.F90 FrictionVelocityMod.f90
	$(FC) -c $(FREEFLAGS) $(FFLAGS) $< -pi exp=moninobukini expin=FrictionVelocityMod.f90

BiogeophysicsLakeMod.o	: BiogeophysicsLakeMod.F90 FrictionVelocityMod.f90
	$(FC) -c $(FREEFLAGS) $(FFLAGS) $< -pi exp=moninobukini expin=FrictionVelocityMod.f90

endif

#------------------------------------------------------------------------
# Targets/rules that depend on architecture specific variables.
#------------------------------------------------------------------------

# The library is not made to be built in parallel, so we must null passed options and
# specify only one job is to run.
$(ESMF_LIB)/$(ESMF_ARCH)/libesmf.a:
	cd $(ESMF_ROOT); \
           $(MAKE) -j 1 BOPT=$(ESMF_BOPT) ESMF_BUILD=$(ESMF_BLD) ESMF_DIR=$(ESMF_ROOT) ESMF_ARCH=$(ESMF_ARCH);

time_manager.o : $(ESMF_LIB)/$(ESMF_ARCH)/libesmf.a

RM := rm
# Add user defined compiler flags if set, and replace FC if USER option set.
FFLAGS  += $(USER_FFLAGS)
ifneq ($(USER_FC),$(null))
FC := $(USER_FC)
endif

clean:
	$(RM) -rf esmf
	$(RM) -f Depends Srcfiles *.o *.mod *.stb *.f90 $(MODEL_EXEDIR)/$(EXENAME)

realclean: clean

include Depends

