/*
** $Id: f_wrappers.c,v 1.2.10.3 2004/01/06 16:12:51 forrest Exp $
** 
** Fortran wrappers for timing library routines
*/

#include <cfort.h>
#include <gpt.h>

#if ( defined FORTRANCAPS )

#define t_initializef T_INITIALIZEF
#define t_prf T_PRF
#define t_resetf T_RESETF
#define t_stampf T_STAMPF
#define t_startf T_STARTF
#define t_stopf T_STOPF
#define t_setoptionf T_SETOPTIONF

#elif ( defined FORTRANUNDERSCORE )

#define t_initializef t_initializef_
#define t_prf t_prf_
#define t_resetf t_resetf_
#define t_stampf t_stampf_
#define t_startf t_startf_
#define t_stopf t_stopf_
#define t_setoptionf t_setoptionf_

#elif ( defined FORTRANDOUBLEUNDERSCORE )

#define t_initializef t_initializef__
#define t_prf t_prf__
#define t_resetf t_resetf__
#define t_stampf t_stampf__
#define t_startf t_startf__
#define t_stopf t_stopf__
#define t_setoptionf t_setoptionf__

#endif

int t_startf (char *, int);
int t_stopf (char *, int);

int t_initializef ()
{
  return t_initialize ();
}

int t_prf (int *procid)
{
  return t_pr (*procid);
}

void t_resetf ()
{
  t_reset();
  return;
}

int t_setoptionf (int *option, int *val)
{
  return t_setoption ( (OptionName) *option, (Boolean) *val);
}

int t_stampf (double *wall, double *usr, double *sys)
{
  return t_stamp (wall, usr, sys);
}

int t_startf (char *name, int nc1)
{
  char cname[MAX_CHARS+1];
  int numchars;

  numchars = MIN (nc1, MAX_CHARS);
  strncpy (cname, name, numchars);
  cname[numchars] = '\0';
  return t_start (cname);
}

int t_stopf (char *name, int nc1)
{
  char cname[MAX_CHARS+1];
  int numchars;

  numchars = MIN (nc1, MAX_CHARS);
  strncpy (cname, name, numchars);
  cname[numchars] = '\0';
  return t_stop (cname);
}
