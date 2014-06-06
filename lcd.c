#include <stdio.h>
#include "lcd.h"

void Lcd_Attente()
{
  printf("LCD: Attente()\n");
}

void Lcd_Affiche(int Ligne, int Colonne, char *Chaine)
{
  printf("LCD: Affiche(%i, %i, %s)\n", Ligne, Colonne, Chaine);
}

void Lcd_Init()
{
  printf("LCD: Init()\n");
}
