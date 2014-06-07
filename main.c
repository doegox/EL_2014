/*
#define EL2014
#define TST_EL2014
*/

/**************************************************************************
<MODULE NAME>
<FILENAME> main.c </FILENAME>
<HISTORY>
  <MODIFICATION>
    <DATE> </DATE>
    <AUTHOR> </AUTHOR>
    <DESCRIPTION>

    </DESCRIPTION>
  </MODIFICATION>
</HISTORY>
</MODULE NAME>
**************************************************************************/

/**************************************************************************
INCLUDE FILES
**************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libcrdrd.h"
#include "libalrbx.h"
#include "defs.h"

#ifdef EL2014
#include "variable.c"
#include "util_mav.c"
#endif

/**************************************************************************
FUNCTION PROTOTYPES
**************************************************************************/
extern int  Card_Eject(void);
extern int  Crc_Rijndael(void);
extern void Graph_Init(void);
extern void Hwd_Init(void);
extern void Lg_Init(void);
/* << 9.01b */
extern void Msg_Error_Exit(char *);
/* 9.01b >> */
extern void Msg_Init(void);
extern void Pwd(void);
extern void Screen_Bad_Crc(void);
extern void Screen_Wait(void);
extern void Scrut_Read(void);
extern void setLPcursor(bool);
extern unsigned char Test_AlarmBox(void);
extern int  Vote_Start(void);

/**************************************************************************
GLOBAL VARIABLES
**************************************************************************/
int giSWCard = 0;

/**************************************************************************
GLOBAL FUNCTIONS
**************************************************************************/
#ifdef EL2014
extern int  Vote_Start(void);
extern int  Scrut_Loop(void);
extern int  les_listes(void);
extern int  Vote_End(void);
extern int  Vote_Empty(void);
extern int  affichc(void);
extern int  Scrut_Result(void);

char *fxname(void *fx) {
  if (fx == 0) return "0";
  if (fx == Vote_Start) return "Vote_Start";
  if (fx == Scrut_Loop) return "Scrut_Loop";
  if (fx == les_listes) return "les_listes";
  if (fx == Vote_End) return "Vote_End";
  if (fx == Vote_Empty) return "Vote_Empty";
  if (fx == affichc) return "affichc";
  if (fx == Scrut_Result) return "Scrut_Result";
  return "(unknown)";
}
#endif

int (*ptfunct)(void);


/* << 9.01b
Modifications:
  1.main.c
    Si la fonction Tempo() renvoi un status d'erreur -> on quitte l'application
/* << 9.01c !!! non fourni dans la version 9.01b
  2.mod_list.c
    Mod_Scru.c
    Scrut_read()
    machvots.tbl, machvotl.tbl
    fopen doesn't check validity of returned handle (NULL)
    -> Nous ne testions pas si l'ouverture des fichiers s'était bien déroulée
 9.01c >>
  3.Mod_List.c
    List_Fill() (94, 1) source code . for long no reason
    static long depl = 0.;
    -> Mauvaise initialisation

  4.Mod_Vote.c
    Résolution problème lié aux lectures "défectueuses"
    (+ création nouvel écran quand carte bloquée suite à 3 mauvaises insertion...)
 9.01b >> */

/* << 9.01c
  1.mod_list.c
    Mod_Scru.c
    Scrut_read()
    machvots.tbl, machvotl.tbl
    fopen doesn't check validity of returned handle (NULL)
    -> Nous ne testions pas si l'ouvertures des fichiers s'était bien déroulée

  2.Tri du tableau (noms de fichiers) traité par Rijndael (signature)

  3.Gestion du code langue à 8 (Allemand <> Français) (Allemand -> "prioritaire")

  4.Dans la visualisation des listes et concernant le carré "Vote blanc":
    nous agrandissons la taille de la police de caractères
    Ceci est lié à la version Allemande: "Stimmenthaltung" -> 2 m consécutifs -> mauvais rendu
 9.01c >>

/* << 9.02
  1. Be.c -> le fichier fe_dsk crypté (fe_dsk.crp) par l'urne n'est pas pris en compte dans CalculMac()
 9.03 >> */

/* << 9.03
  1. Adaptations écrans (allemand)
  2. Suppression du module debug (mod_dbg.cpp) du projet (non utilisé...)
  3. En allemand, un temps d'attente plus conséquent est attribué à l'écran final de vote
     (fin vote + reprendre carte + invitation à visualiser le vote)
 9.03 >> */

/* << 9.04
  Adaptations écrans (allemand)
  Disquette de démarrage bureau de vote:
    gestion d'un 3eme fichier calibrage crayon optique (digivote 2c)
  9.04 >>
*/

/* << 9.05
  Changements fichier messages (allemand)
 9.05 >> */

#ifdef EL2014
#include <time.h>
char my_msg[512];
void my_Trace(void)
{
/*
  time_t t = time(0);
  struct tm *now = localtime(&t);
  FILE *fd = fopen("a:\\my_mav.txt","a");
  if (fd!=NULL) {
    fprintf(fd, "%02d-%02d-%02d %02d:%02d:%02d - %s\n", now->tm_mday, now->tm_mon+1, now->tm_year+1900, now-> tm_hour, now->tm_min, now->tm_sec, my_msg);
    fclose(fd);
  }
*/  
}
#endif

/**************************************************************************
<FUNCTION>
  <NAME> main </NAME>
</FUNCTION>
**************************************************************************/
void main(int argc, char *argv[])
{
  int iArg = 1, iStatus;

  /* Init Graphic */
  Graph_Init();

  /* Init Messages */
  Msg_Init();

  /* Read args */
  while(iArg < argc)
  {
#if (NOCARD_CHECK >= 1)
    /* Demo ? */
    if(!strcmp(argv[iArg], "-NOCARD"))
    {
      giSWCard = _NOCARD;
      ++iArg; continue;
  }
#endif
    /* LPCURSOR ? */
    if(!strcmp(argv[iArg], "-LPCURSOR"))
    {
      setLPcursor(1);
      ++iArg; continue;
    }
    ++iArg; /* parameter(s) skipped */
  }

  /* Init Hardware */
  Hwd_Init();

  /* Test Button/Leds */
  if(!Test_AlarmBox()) 
    
/* << 9.01 */
    Msg_Error_Exit("no Alarm box connected");
	
  /*{
    puts("no Alarm box connected");
    exit(0);
  }*/

  /* Init Language */
  Lg_Init();

  /* PWD */
  Pwd();

  /* Ask for Waiting */
  Screen_Wait();

  /* Rijndael */
  if(giSWCard != _NOCARD)
  {
#ifdef TST_EL2014
	iStatus = 1;
#else
    iStatus = Crc_Rijndael();
#endif	

    if(!iStatus)
      while(1)
        Screen_Bad_Crc();
  }

  /* Init Scrutin (from tbl) */
  Scrut_Read();

  /* Send tempo to reader (JITES) */
/* << 9.01b */
  /*iStatus = Tempo();
  if(iStatus > 0) printf("Tempo() returns %d\n", iStatus);*/
  if(Tempo() > 0)
    Msg_Error_Exit("Technical problem");
/* 9.01b >> */

  /* Still a card inside ? */
  Eject_Commande(0);

  /* Ready -> Ask for a card */
  ptfunct = Vote_Start;
  while(1) (ptfunct)();
}
