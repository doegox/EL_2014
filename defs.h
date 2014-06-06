#define EL2014
/* uncomment the next line to compile a test version */
/* #define TST_EL2014 */

#ifdef EL2014
extern char my_msg[];
extern void my_Trace(void);
#endif

// Phil additions:
#define MAV_DIGIVOTE_1 1
#define MAV_DIGIVOTE_2 2
#define MAV_JITES_1 3
#define MAV_JITES_2 4


#ifdef TST_EL2014
#define  TST_PASSWORD  "0000000099"
#endif

#define BAL_LISTE_INCOMPLETE    0  /* 1=Yes  0=No                            */
#define LPMNGLIB                1  /* 1=Yes  0=No                            */
#define LPuserCURSORchange      0  /* 1=Yes  0=No                            */
#define MEMORY_CHECK            0  /* 1=Yes  0=No                            */
#define TERMINAL_CHECK          1  /* 1=Yes  0=No                            */
#define LOOPWRITE_CARTE         1  /* 1=Yes  0=No                            */
#define NOCARD_CHECK            1  /* 1=Yes  0=No                            */

/******************************* MACHVOTE ********************************/
#define  OFF '\xFC'
#define  RR  '\xFE'
#define  RG  '\xFF'
#define  GG  '\xFD'

#define _NOCARD 21845

#ifdef EL2014
#define N_SCR_MESSAGE 145
#else
#define N_SCR_MESSAGE 141
#endif
#define L_SCR_MESSAGE  65

#define  CURSOR_OFF 0
#define  CURSOR_ON  1
#define  CURSOR_XY  2

/* gcCardRW */
#define CARD_WRITE  0
#define CARD_READ   1

#define bool  int

/* vote */
#define _C_LISTE_MAX_BYTE     (2)

#define _C_JETON_MAX_BYTE     (5)
#define _C_TEST_MAX_BYTE      (1)
#define _C_CRC_MAX_BYTE       (4)
#define _C_PASSAGE_MAX_BYTE   (1)

#define  _C_DATA_PLACE        (0)
#define  _C_JETON_PLACE       (_C_DATA_PLACE + 0)
#define  _C_PASSAGE_PLACE     (_C_DATA_PLACE + 5)
#define  _C_CRC_PLACE         (_C_DATA_PLACE + 6)
#define  _C_TEST_PLACE        (_C_DATA_PLACE + 10)
#define  _C_VOTE_PLACE        (_C_DATA_PLACE + 11)

#ifdef EL2014
#define MAX_CARD_BYTES 52
#endif

#ifdef EL2014
#define NB_MAX_SCRUT     4
#else
#define NB_MAX_SCRUT     3
#endif
#define NB_MAX_LISTS    51
#define NB_MAX_CANDID  100

#ifdef EL2014
  #ifdef TST_EL2014
#define MAV_VERSION "T. 9.16"
  #else
#define MAV_VERSION "V. 9.16"
  #endif
#else
#define MAV_VERSION "V. 9.12"
#endif

#define PEN_OR_ALARMBOX_DISCONNECTED \
  if(iHwdDisconnected < 0 && \
     (gucMavType >= MAV_DIGIVOTE_1 && gucMavType <= MAV_DIGIVOTE_2)) \
  { \
    switch(iHwdDisconnected) \
    { \
      case -1: /* Light Pen has been disconnected */ \
        giCodeVoteEnd = 28; \
        ptfunct = Vote_End; \
        return(0); \
    } \
  }
