#define VERSION "14.01"
#define CDT_NUM_WIDTH   28 /* 0=NONE  !0=Numeo de candidat visible et = 'largeur du champs numeo' */

#ifdef EL2014
#define MINI_PWD "6987"
#define MINI_POS "2368"
#else
/* << AP 16/04/2010 
__#define MINI_PWD "2701"
__#define MINI_POS "1467"
*/

#define MINI_PWD "2703"
#define MINI_POS "1368"

/* >> AP 16/04/2010 */
#endif

//int swDeselect = 0;

/* el2007 >> */
/******************************* MACHVOTE ********************************/
//#define  OFF '\xFC'
//#define  RR  '\xFE'
//#define  RG  '\xFF'
//#define  GG  '\xFD'

//#define  false     0
//#define  true      1
//#define  time_out  2

//#define _ZEIT_BUFFER_MAX_BYTE   86
//#define _EJECT_FRONT   "f"
//#define _EJECT_REAR    "e"
//#define _READ          "r"
//#define _WRITE         "w"
//#define _CR            "\x0D"
//#define _STATUS        "s"
//#define _NOCARTE 21845


//extern char Minicodage[5];

//int swNOCarte = 0; /* _NOCARTE=WITHOUT Carte  else=WITH Carte */
//int swLPcursor = 0; /* 0=WITHOUT lpCursor  1=WITH lpCursor */
extern int swRLg; /* 0=NONE  1=REGIME LINGUISTIQUE */
extern int swCLG; /* 0=NONE  1=COLLEGE */
extern int swVLR; /* 0=NONE  1=CONSEIL FLAMAND */
extern char  RLg; /* REGIME LINGUISTIQUE */
extern int iRuptureColonne;

extern int ccol;
extern int crl;
extern int nbr_sup;
extern int lon, espw, espl, recl;

extern char Entete_Col[80+1];
extern char Entete_RLg[80+1];
//extern char LANGUE;
extern int CHOIX;
//int ncolonne;
extern int affTL_code, affColMax;
//int i,j,x,y,z,valid,msg_error,nbc[4],type_erreur,trouve,c;
extern int x_dep,x_ent_scr,x_numl,x_noml,nbc_col;
extern int h,lon,espl,espw,recl,nb,nbr_max,nbr_liste_ligne,nombre_col;

//280613 extern int eject, swpas, swnoir, swconf,klavier;
extern int Hcadre_candid ,Interl_candid,Lcadre_candid;
extern int LAsaisie_candid, HAsaisie_candid;
extern int Interc_candid,nbccol;
//int scr = 0,list = 0,candid = 0;
//extern int posl[51][4],posc[90][4];
extern int poslBL2[1][4];
//int cx,cy,rc,k,kp,w,temps = 1;
//int debut;
//char memos[5];
//char memol[5][51];

/*char memoc[4][51][100];*/
//char te[5][51];
//char ts[5][51];
//int dejasel = 0, typesel = 0;
//int testcarte , passe;
//int C_VOTE_MAX_BYTE ;
//int  GLOBAL_NATIO,GLOBAL_EURO,GLOBAL_STR;
extern int  ClOui;
//unsigned char carte[75];

//#if (LOOPWRITE_CARTE >= 1)
//#  define LCARTEBAK  75
//   unsigned char carteBAK[LCARTEBAK];
//   int           nPassWriteCarte = 0;
//#endif

//char Lect_Buffer[75];
//char Str_Commande[75];
//unsigned long temptimes;
//int nbr_lignes,nbre_Liste_Int,Cl;  /* Affichage / ligne */

/****************************AFFICHE CANDIDATS **************************/
 extern int status,Diam;
// long deplacement_file_cand;
/*************************** GARNIR ************************************/
//int nbrscrutin,code_scr;
//int nbrlist[5];        /* nombre de liste / scrutin */
//int effectif,suppleant;
extern int Debut_Affichage;
//int visu;
//int Visu_tempo = 10;
extern int Condense;

//char jeton[16];
/* << el2007 */
//char idCreation[12];
/* el2007 >> */
// char Arrond[4];
//char jetonmini[6];
//char code_langue;

//char Crc_Calcul[5],Crc_Recup[5];

extern char Entete_Col[80+1];
extern char Entete_RLg[80+1];

//#define N_SCR_MESSAGE 140
//#define L_SCR_MESSAGE  65

//char *message_1[N_SCR_MESSAGE];
//char *message_2[N_SCR_MESSAGE];
//char **Message_point;

extern char Choix_College;
extern char Choix_RLg;
//int  nListeF = 0, nListeN = 0;
//FILE *fps,*fpl,*fpc;
//FILE *fpm;

//char (huge *memoc)[51][100];

/***************************************************************/
//int Bilingue();
//int  recom();
//int boucle();
//int les_listes();
//int affichc();
//int selectc();
//int result();
//int fin();
//int choix_langue();
//int Confirm_langue();
//int message_error();
//int Ecrire();
//int (*ptfunct)();

/* START CONFIDENTIAL */

//struct
//{
//  unsigned ch0 : 1;
//} ch[91];
//char codage[] = "0123456789ABCDEF";

/* END CONFIDENTIAL */

//#if (LPMNGLIB >= 1)
//extern void lpCursorOn();
//extern void lpCursorOff();
//extern void lpPosxy();
//#endif

/***************************************************************************/
//unsigned long cipherDataLen,plainDataLen;
//char floppy[60];
//char source[60];
//char wrkspc[60];
//int ret,count;             /* holds the     return code of important method calls. */
//unsigned long matches;        /* counts the number of equal bytes in the plaintext and the decrypted ciphertext. */

//char iv[100];
//char derivedKey[100];
//char derivedKeyMini[100];
//char decryptedMacKeyMini[100];
//char decryptedEncKeyMini[100];
//char CMinipassword[12];
//char fullPasswordMini[12];

//char decryptedMacKey[100];
//char decryptedEncKey[100];
//char *decrypted = NULL;
//char hashResult[100];
//char macResult[100];
//char *authenticatedEncryptedPlaintext;
//char dataFromFloppy[50];
//char mode[2],niveau[2];
//struct
//{
//    char nom[14];
//}stru_fichier[150];
/***************************************************************************/
//char ElecDate[12];
//int NonBoucle = 0;
