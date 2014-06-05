/*
#define EL2014
*/

/* START CONFIDENTIAL */
/***********************************************/
#define  NMX_SCRUTIN_T         9

#define  IS_NONE               0
#define  IS_EUROPE             1
#define  IS_CHAMBRE            2
#define  IS_SENAT              3
#define  IS_REGION             4
#define  IS_PROVINCE           5
#define  IS_COMMUNE            6
#define  IS_CONSEIL_FLAMANT    7
#define  IS_DISTRICT           8
#ifdef EL2014
#define  IS_REGION_GERMANO     9
#else
#define  IS_CPAS               9
#endif


#define  VM_EUROPE             1
#define  VM_CHAMBRE            1
#define  VM_SENAT              1       /* votes multiples */
#define  VM_REGION             1
#define  VM_PROVINCE           1
#define  VM_COMMUNE            1
#define  VM_CONSEIL_FLAMANT    1
#define  VM_DISTRICT           1
#ifdef EL2014
#define  VM_REGION_GERMANO     1
#else
#define  VM_CPAS	       1
#endif

#ifdef EL2014
#define  NMXV_EUROPE          (1+19)
#define  NMXV_CHAMBRE         (1+28)
#else
#define  NMXV_EUROPE          (1+28)
#define  NMXV_CHAMBRE         (1+45)
#endif
#define  NMXV_SENAT           (1+39)     /* nb max candidat */
#define  NMXV_REGION          (1+88)
#define  NMXV_PROVINCE        (1+31)
#define  NMXV_COMMUNE         (1+55)
#define  NMXV_CONSEIL_FLAMANT (1+12)
#define  NMXV_DISTRICT	      (1+33)
#ifdef EL2014
#define  NMXV_REGION_GERMANO  (1+25)
#else
#define  NMXV_CPAS	      (1+11)
#endif


#ifdef EL2014
#define  NMXB_EUROPE          ( 5)
#define  NMXB_CHAMBRE         ( 8)
#else
#define  NMXB_EUROPE          ( 8)
#define  NMXB_CHAMBRE         (12)
#endif
#define  NMXB_SENAT           (10)     /* nb max bytes */
#define  NMXB_REGION          (23)
#define  NMXB_PROVINCE        ( 8)
#define  NMXB_COMMUNE         (14)
#define  NMXB_CONSEIL_FLAMANT ( 4)
#define  NMXB_DISTRICT        ( 9)
#ifdef EL2014
#define  NMXB_REGION_GERMANO  ( 7)
#else
#define  NMXB_CPAS            ( 3)
#endif

#ifndef _CFGSCRUT_
#define _CFGSCRUT_

typedef struct{
  int              swVM;
  int              nmxV;
  int              nmxB;
} s_CFGSCRUT;
#endif /* _CFGSCRUT_ */

/***********************************************/
#define  AGO_SWDEBUG 0
/***********************************************/
/* END CONFIDENTIAL */
