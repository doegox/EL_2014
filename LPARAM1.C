nbre_Liste_Int = 0;
k = 0;

if ((swRLg ) || (swCLG))
      {
	while(k <(gaiNbListScrut[giCurrentScr] - 1))
	     {
	       if(swRLg)
		 {
		   if(gshList->college == Choix_RLg) 
				    nbre_Liste_Int++; 
		 }
	       else
		 if(swCLG)
		  {
		    if(gshList->college == Choix_College) 
				      nbre_Liste_Int++; 
		  }
	       gshList++;
	       k++; 
	     }
	nbre_Liste_Int++; 
      }
else
      {
	while(k <(gaiNbListScrut[giCurrentScr] - 1))
	     {
	       if(gshList->college == Choix_College) 
			      nbre_Liste_Int++; 
	       gshList++;
	       k++; 
	     }
	nbre_Liste_Int++; 
       }

  giNbrLignes =  nbre_Liste_Int / 3;
  giNbrLignes +=((nbre_Liste_Int % 3) > 0) ? 1 : 0;
  ncolonne = 3;
  espl = 5;
/* << el2007 */
  espw = 2;
/* el2007 >> */
  lon = 210;


if (giNbrLignes <= 5)
	 {
/* << el2007 */
		 /*settextstyle( SMALL_FONT, HORIZ_DIR, 4);*/
		 settextstyle( SMALL_FONT, HORIZ_DIR, 6);
/* el2007 >> */
		 h = 60;
		 recl = 60;
	 }
if (giNbrLignes > 5 && giNbrLignes <= 6)
	 {
/* << el2007 */
		 /*settextstyle( SMALL_FONT, HORIZ_DIR, 4);*/
		 settextstyle( SMALL_FONT, HORIZ_DIR, 6);
/* el2007 >> */
		 h = 60;
		 recl = 56;
	 }
	
if (giNbrLignes > 6 && giNbrLignes <= 7)
	 {
/* << el2007 */
		 /*settextstyle( SMALL_FONT, HORIZ_DIR, 4);*/
		 settextstyle( SMALL_FONT, HORIZ_DIR, 6);
/* el2007 >> */
		h = 55;
		recl = 50;
	 }
	
if (giNbrLignes > 7 && giNbrLignes <= 8)
	 {
/* << el2007 */
		 /*settextstyle( SMALL_FONT, HORIZ_DIR, 4);*/
		 settextstyle( SMALL_FONT, HORIZ_DIR, 6);
/* el2007 >> */
		 h = 55;
                 recl = 42;
	 }

if (giNbrLignes > 8 && giNbrLignes <= 9)
         {
/* << el2007 */
		 /*settextstyle( SMALL_FONT, HORIZ_DIR, 4);*/
		 settextstyle( SMALL_FONT, HORIZ_DIR, 6);
/* el2007 >> */
                  h = 55;
                  recl = 36;
	}
if (giNbrLignes > 9 && giNbrLignes <= 10)
	 {
/* << el2007 */
		 /*settextstyle( SMALL_FONT, HORIZ_DIR, 4);*/
		 settextstyle( SMALL_FONT, HORIZ_DIR, 6);
/* el2007 >> */
		 h = 55;
                 recl = 32;
	 }

if (giNbrLignes > 10 && giNbrLignes <= 11)
         {
/* << el2007 */
		 /*settextstyle( SMALL_FONT, HORIZ_DIR, 4);*/
		 settextstyle( SMALL_FONT, HORIZ_DIR, 6);
/* el2007 >> */
                  h = 55;
                  recl = 30;
	}

if (giNbrLignes > 11 && giNbrLignes <= 12)
         {
/* << el2007 */
		 /*settextstyle( SMALL_FONT, HORIZ_DIR, 4);*/
		 settextstyle( SMALL_FONT, HORIZ_DIR, 6);
/* el2007 >> */
                  h = 55;
                 recl = 25;
	}

if (giNbrLignes > 12 && giNbrLignes <= 13)
         {
/* << el2007 */
		 /*settextstyle( SMALL_FONT, HORIZ_DIR, 4);*/
		 settextstyle( SMALL_FONT, HORIZ_DIR, 6);
/* el2007 >> */
                  h = 55;
                  recl = 25;
	}

if (giNbrLignes > 13 && giNbrLignes <= 14)
         {
/* << el2007 */
		 /*settextstyle( SMALL_FONT, HORIZ_DIR, 4);*/
		 settextstyle( SMALL_FONT, HORIZ_DIR, 6);
/* el2007 >> */
                  espl = 7;
                  h = 55;
                  recl = 25;
	}
