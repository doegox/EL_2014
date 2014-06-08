/**************************************************************************
<MODULE NAME>
<FILENAME> mod_wait.c </FILENAME>
<HISTORY>
  <MODIFICATION>
    <DATE>  </DATE>
    <AUTHOR>  </AUTHOR>
    <DESCRIPTION>
    Waiting loops
    </DESCRIPTION>
  </MODIFICATION>
</HISTORY>
</MODULE NAME>
**************************************************************************/



/**************************************************************************
EXTERNAL VARIABLES
**************************************************************************/
extern int giKlavier;

/**************************************************************************
FUNCTION PROTOTYPES
**************************************************************************/
extern int Pen_Capture(int *, int *);


/**************************************************************************
<FUNCTION>
  <NAME> Wait_Loop </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  Loop waiting for X secs
  </DESCRIPTION>
  <PARAMETERS>
    <PARAMETER>
      <NAME> _iSecs </NAME>
      <TYPE> int </TYPE>
      <IO> IN </ IO >
      <DESCRIPTION>
      </DESCRIPTION>
    </PARAMETER>
  </PARAMETERS>
</FUNCTION>
**************************************************************************/
#pragma argsused
void Wait_Loop(int _iSecs)
{
  sleep(_iSecs);
}

/**************************************************************************
<FUNCTION>
  <NAME> Wait_Loop_Int </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  Loop (like Wait_Loop) but can be interrupted (button or light pen)
  </DESCRIPTION>
  <PARAMETERS>
    <PARAMETER>
      <NAME> _iSecs </NAME>
      <TYPE> int </TYPE>
      <IO> IN </ IO >
      <DESCRIPTION>
      </DESCRIPTION>
    </PARAMETER>
  </PARAMETERS>
</FUNCTION>
**************************************************************************/
#pragma argsused
void Wait_Loop_Int(int _iSecs)
{
  int i, iPenX, iPenY;
  for(i=0; (i<_iSecs) && (!Pen_Capture(&iPenX, &iPenY)) && giKlavier; i++) {
    sleep(1);
  }
}
