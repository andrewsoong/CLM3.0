!===============================================================================
! CVS: $Id: shr_kind_mod.F90,v 1.2.6.1 2004/01/07 20:50:02 mvertens Exp $
! CVS: $Source: /fs/cgd/csm/models/CVS.REPOS/shared/csm_share/shr/shr_kind_mod.F90,v $
! CVS: $Name: clm3_deva_21 $
!===============================================================================

MODULE shr_kind_mod

   !----------------------------------------------------------------------------
   ! precision/kind constants add data public
   !----------------------------------------------------------------------------
   public
   integer,parameter :: SHR_KIND_R16= selected_real_kind(24) ! 16 byte real
   integer,parameter :: SHR_KIND_R8 = selected_real_kind(12) ! 8 byte real
   integer,parameter :: SHR_KIND_R4 = selected_real_kind( 6) ! 4 byte real
   integer,parameter :: SHR_KIND_RN = kind(1.0)              ! native real
   integer,parameter :: SHR_KIND_I8 = selected_int_kind (13) ! 8 byte integer
   integer,parameter :: SHR_KIND_I4 = selected_int_kind ( 6) ! 4 byte integer
   integer,parameter :: SHR_KIND_IN = kind(1)                ! native integer
   integer,parameter :: SHR_KIND_CL = 256                    ! long char
   integer,parameter :: SHR_KIND_CS = 80                     ! short char

END MODULE shr_kind_mod
