#ifdef ESMC_RCS_HEADER
"$Id: ESMF_conf.h,v 1.1.10.2 2003/03/12 19:44:18 mvertens Exp $"
"Defines the configuration for this machine"
#endif

#if !defined(INCLUDED_CONF_H)
#define INCLUDED_CONF_H

#define PARCH_linux_gnupgf90
#define ESMF_ARCH_NAME "linux_gnupgf90"

#define ESMC_HAVE_LIMITS_H
#define ESMC_HAVE_PWD_H 
#define ESMC_HAVE_MALLOC_H 
#define ESMC_HAVE_STRING_H 
#define ESMC_HAVE_GETDOMAINNAME
#define ESMC_HAVE_DRAND48 
#define ESMC_HAVE_UNAME 
#define ESMC_HAVE_UNISTD_H 
#define ESMC_HAVE_SYS_TIME_H 
#define ESMC_HAVE_STDLIB_H

#define ESMC_HAVE_FORTRAN_UNDERSCORE 
#define ESMC_HAVE_FORTRAN_UNDERSCORE_UNDERSCORE

#define ESMC_POINTER_SIZE 4
#undef ESMC_HAVE_OMP_THREADS 

#undef ESMC_HAVE_MPI

#define ESMC_HAVE_READLINK
#define ESMC_HAVE_MEMMOVE
#define ESMC_HAVE_TEMPLATED_COMPLEX

#define ESMC_HAVE_DOUBLE_ALIGN_MALLOC
#define ESMC_HAVE_MEMALIGN
#define ESMC_HAVE_SYS_RESOURCE_H
#define ESMC_SIZEOF_VOIDP 4
#define ESMC_SIZEOF_INT 4
#define ESMC_SIZEOF_DOUBLE 8

#if defined(fixedsobug)
#define ESMC_USE_DYNAMIC_LIBRARIES 1
#define ESMC_HAVE_RTLD_GLOBAL 1
#endif

#define ESMC_HAVE_SYS_UTSNAME_H
#endif
