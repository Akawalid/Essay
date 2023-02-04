#include<stdlib.h>
#include<stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "TP2_CHABANE_TEBBAL_G01.c"


int main(){
    srand(time(NULL));
    FILE *F;

    //Gnerer le tableaux des etablissement
    F = fopen("Etablissement_Univ.txt", "r");
    for(int i = 0; i <112; ++i)
        fgets(tab_Etablissement[i], sizeof(tab_Etablissement[i]), F);

    fclose(F);

    //Generation du tableaux des Wilaya
    F = fopen("Wilayas.txt", "r");
    for(int i = 0; i < 58; i++)
    {
         fgets(tab_wilaya[i], sizeof(tab_wilaya[i]), F);
         tab_wilaya[i][14]= '\0';
    }
    fclose(F);

    //Récupérer la date courrante
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    currentDt.jj =  tm.tm_mday;
    currentDt.mm = tm.tm_mon + 1;
    currentDt.yyyy = tm.tm_year + 1900;

    f.F = NULL;
    Bitmaps = (bool**)malloc(124 * sizeof(bool*));

    for (int index=0;index<114 + NbSpecialites; ++index)
    {
        Bitmaps[index] = (bool*)malloc(900000 * sizeof(bool));
    }
////////////////////////////////////////////////
    getchar();
    FisrtScreen();
    MenuPrimaire();
    FermerBitMap();
    Fermer();
    FermerInd();
    for (int index=0; index<114 + NbSpecialites; ++index)
        free(Bitmaps[index]);

    free(Bitmaps);
    return 0;
}
