#include "cfgscrut.h"
#include "scrutin.h"

/**************************************************************************
EXTERNAL VARIABLES
**************************************************************************/
extern char   gszArrond[4];
extern int    giNbrScrutin;
extern s_CFGSCRUT gshCfgScrut[NMX_SCRUTIN_T+1];
extern s_Scrutin  gshScrutin[10];

/**************************************************************************
FUNCTION PROTOTYPES
**************************************************************************/

/**************************************************************************
LOCAL FUNCTIONS
**************************************************************************/
int existScrutin(int );

/**************************************************************************
GLOBAL VARIABLES
**************************************************************************/
extern int swVLR;


/**************************************************************************
<FUNCTION>
  <NAME> getVoteMaxBytes </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
int getVoteMaxBytes(int iScrutin, int corrected)
{
    int maxBytes = gshCfgScrut[gshScrutin[iScrutin].TYPE_SCRUTIN].nmxB;
    if (!corrected) return maxBytes;
    
    if(strcmp(gszArrond,"002") == 0 && gshScrutin[iScrutin].TYPE_SCRUTIN == IS_CHAMBRE) {
        /* if BXL, CK data is truncated by 1 */
        maxBytes = maxBytes - 1;        
    }
    if(strcmp(gszArrond,"002") != 0 && gshScrutin[iScrutin].TYPE_SCRUTIN == IS_REGION) {
        /* if not BXL, REG data is truncated by 1 */
        maxBytes = maxBytes - 1;        
    }
    if(gshScrutin[iScrutin].TYPE_SCRUTIN == IS_REGION &&
	  existScrutin(IS_CONSEIL_FLAMANT) == 1 &&
      swVLR == 1) {
        /* if BV, REG data is truncated by BV length */
        maxBytes = maxBytes - (2 + NMXB_CONSEIL_FLAMANT);        
    }
    if(gshScrutin[iScrutin].TYPE_SCRUTIN == IS_REGION &&
	   existScrutin(IS_REGION_GERMANO) == 1) {
        /* if DE, REG data is truncated by DE length */
        maxBytes = maxBytes - (2 + NMXB_REGION_GERMANO);        
    }
    return maxBytes;
}

/**************************************************************************
<FUNCTION>
  <NAME> getVoteMaxVotes </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
int getVoteMaxVotes(int iScrutin, int corrected)
{
    int maxVotes = gshCfgScrut[gshScrutin[iScrutin].TYPE_SCRUTIN].nmxV;
    if (!corrected) return maxVotes;
    
    if(strcmp(gszArrond,"002") == 0 && gshScrutin[iScrutin].TYPE_SCRUTIN == IS_CHAMBRE) {
        /* if BXL, CK data is truncated by 1 */
        maxVotes = maxVotes - (1 * 4);        
    }
    if(strcmp(gszArrond,"002") != 0 && gshScrutin[iScrutin].TYPE_SCRUTIN == IS_REGION) {
        /* if not BXL, REG data is truncated by 1 */
        maxVotes = maxVotes - (1 * 4);         
    }
    if(gshScrutin[iScrutin].TYPE_SCRUTIN == IS_REGION &&
	   existScrutin(IS_CONSEIL_FLAMANT) == 1 &&
       swVLR == 1) {
        /* if BV, REG data is truncated by BV length */
        maxVotes = maxVotes - ((2 + NMXB_CONSEIL_FLAMANT) * 4);        
    }
    if(gshScrutin[iScrutin].TYPE_SCRUTIN == IS_REGION &&
	   existScrutin(IS_REGION_GERMANO) == 1) {
        /* if DE, REG data is truncated by DE length */
        maxVotes = maxVotes - ((2 + NMXB_REGION_GERMANO) * 4);        
    }
    return maxVotes;
}

/**************************************************************************
<FUNCTION>
  <NAME> xxxVoteMark </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void addVoteMark(char *_szCarte, int pos)
{
    _szCarte[pos] = 'Z';
}

int isVoteMark(char *_szCarte)
{
    int i=0;
    int isZ = 0;
    
    for (i=_C_VOTE_PLACE; i < MAX_CARD_BYTES; i++) {
        if (_szCarte[i] == 'Z') {
            isZ = 1;
            break;
        }
    }
    return isZ;

    /*return (_szCarte[38] == 'Z');*/
}

/**************************************************************************
<FUNCTION>
  <NAME> existScrutin </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
int existScrutin(int iScrutin)
{
	int i, exist;
	
	exist = 0;
	for (i=0; i < giNbrScrutin; i++) {
	  if (gshScrutin[i].TYPE_SCRUTIN == iScrutin) exist = 1; 
    }	
	
	return exist;
}

