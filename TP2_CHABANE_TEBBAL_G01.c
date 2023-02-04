#include "TP2_CHABANE_TEBBAL_G01.h"
//+++++D�claration des structures des fichier qu'on va utiliser (Fichier enseignant, BitMap, Index)+++++
fileInd fInd;
file f;
FILE *Fbitmap;
//Tableau pour compter le cout du programme � chaque execution(LireDire, EcrireDir)
/*
    CounLE[0] : Nombres des lectures effectuer sur le fichier d'enseignants.
    CounLE[1] : Nombres des ecritures effectuer sur le fichier d'enseignants.
    CounLE[2] : Nombres des lectures effectuer sur le fichier d'index.
    CounLE[3] : Nombres des ecritures effectuer sur le fichier d'index.
    CounLE[4] : Nombres des lectures effectuer sur le fichier BitMap.
    CounLE[5] : Nombres des ecritures effectuer sur le fichier BitMap.
*/
int oualid = 4;
int CountLE[6] = {0, 0, 0, 0, 0, 0};

//Chaines de caract�res d�clar�es globalement pour contenire les noms des fichiers
char NameBitmap[cchaine] = "FBitMap.bin";//BitMap
char Namefich[30] = "test.bin";//Informations
char NameInd[30] = "i.bin";//Index

//Variable utilis�e dans les operations de lectures (� partire de la console)
int choice;
int Ind = 0;
int n = 168;//Affichage
//Pointeur vers les chaines BitMaps des etablissements, regions et les specialit�s
bool** Bitmaps;
//Tableaux utilis�s pour l'affichage des informations relatifs au enseignants et qui sont stock�s en m�moire secondaire sous format "Entier"
//Tableau du groupe sanguin
char Groupe_Sanguin[8][4] = {"O+ ", "A+ ", "B+ ", "O- ", "A- ", "AB+", "B- ", "AB-"};
//Sexe
char tabgender[2][10] = {"Homme", "Femme"};
//Grade
char Grade[6][29] = {"Professeur(e)               ", "Maitre de conferances Rang A", "Maitre de conferances Rang B", "Maitre assistant(e) Rang A  ", "Maitre assistant(e) Rang B  ", "Assistant(e)                "};
//Derdip
char Der_Dip[6][27] = {"Doctorant d'etat         ", "Doctorant en sciences    ", "Doctorant D-LMD          ", "PhD                      ", "Habillation Universitaire", "Magistere                "};
//Sp�cialit�s
char specialites[10][50] = {"Informatique", "Architecture", "Medcine     ", "Automatique ", "Mathematique", "Physique    ", "Commerce    ", "Geologie    ", "Chimie      ","Geophysique "};
//Tableaux des �tablissements et de wilayas qu'on remplira � partire de fichier des �tablissements et de wilayas (resp) "Etablissements_Univ.txt", "Wilayas.txt"
char tab_Etablissement [111][150];
char tab_wilaya[58][30];

//Chaines des noms des fichiers (Eneignants Index, Bitmap) qui seront g�ner�s par la fonction du codage et de d�codage
//nom de fichier d'enseignant cod�
char fichEcode[cchaine] = "(null)";
//nom de fichier d'enseignant d�cod�
char fichEres[cchaine] = "(null)";
//nom de fichier d'index cod�
char fichIcode[cchaine] = "(null)";
//nom de fichier d'index d�cod�
char fichIres[cchaine] = "(null)";
//nom de fichier Bitmap cod�
char fichBcode[cchaine] = "(null)";
//nom de fichier Bitmap d�cod�
char fichBres[cchaine] = "(null)";
//La cl� du codage et de d�codage.
char key[cchaine];


int OuvrireInd( char Name[ cchaine ], char mode){
    //Ouverture d'index
    int x, y, z;
    char tab[2][70] = {"> Continuer.", "> Quitter."};
    adrouvInd:
    textcolor(12);printf("\n\n\t\t\t> ");textcolor(11);printf("Veuillez vous introduire le nom du fichier Index a ouvrire (Index.bin): ");textcolor(12);
    scanf("%s", Name);
    if(mode == 'A')
    {
        fInd.F= fopen( Name, "rb+");
        //Chargement de l'entete dans une structure lors de l'ouverture du fichier.
        if(fInd.F == NULL)
        {
                //Fichier non existant
               textcolor(12);
               printf("\n\n\t\t\t\tFichier introuvable!");textcolor(11);printf("  Voulez-vous ressayer de nouveau\n");
               textcolor(12);printf("\n\t\t\t\t");x = wherex();y = wherey();printf(">");textcolor(11);printf("Continuer.");
               textcolor(12);printf("\n\t\t\t\t>");textcolor(11);printf("Quitter.");
               z = pointer(x - 1,y , 2, tab, 12, 11, 12, 10);//Listes des choix ( Affichage )
               gotoxy(x, y + 3);
               //Si il y aura une autre �criture dans la console , elle n'ecrasera pas le menu de d�cision (continuer, quitter)
               if(z == 0)
                  goto adrouvInd;//Essayer de nouveau un autre nom.
                else
                    goto nextouvInd;//Quitter l'operation de la cr�ation.
        }
        fread(&fInd.Entete, sizeof(fInd.Entete), 1, fInd.F);
    }
    else
    {
        fInd.F = fopen( Name, "wb+");
        //Initialisation de la stucture d'entete lors de l'ouverture en mode nouveau
        fInd.Entete.tab[0] = 0; fInd.Entete.tab[1] = 0;
    }
    textcolor(12);printf("\n\n\t\t\t\t> ");textcolor(11);printf("Ouverture du fichier terminee avec succes.");
    getch();
    nextouvInd:
    return 1;
}

int FermerInd(){
    //Positionement du flux dans le d�but du fichier pour sauvgarder l'entete
    rewind(fInd.F);
    //Sauvgarde de l'entete dans la MS.
    fwrite(&fInd.Entete, sizeof(fInd.Entete), 1, fInd.F);
    //Fermeture de fichier.
    fclose(fInd.F);
}

int EcrireDirInd(FILE *F, int i, BlockIndex buff){
    //Ecriture du buffeur dans le block num�ro i en memoire secondaire
    //On va utiliser le d�but du fichier comme un entete, donc les op�ration sur les blocks s'�ffectuent au dessous de la zone allou�e pour l'entete
    fseek(F, (i-1)*sizeof(BlockIndex) + sizeof(IEntete),SEEK_SET);//Positionner le pointeur dans la position qui convient
    fwrite(&buff, sizeof(BlockIndex), 1, F);//Ecriture dans le block num�ro i en m�moire secondaire
    return 1; // Bien �ffectu�e
}

int LireDirInd(FILE *F, int i, BlockIndex *buff){
    //Lecture Index
    fseek(F, (i-1)*sizeof(BlockIndex) + sizeof(IEntete),SEEK_SET);//Positionner le pointeur dans la position qui convient
    fread(buff, sizeof(BlockIndex), 1, F);//Lecture dans le buffeur de l'information qui se trouve dans le block num�ro i en m�moire secondaire
    return 1;//Bien effectu�e
}

int OuvrireBitMap(char Name[cchaine], char mode)
{//Ouverture du fichier BitMap
    int x, y, z;
    char tab[2][70] = {"> Continuer.", "> Quitter."};
    adrouvbt:
    textcolor(12);printf("\n\n\t\t\t> ");textcolor(11);printf("Veuillez vous introduire le nom du fichier Bitmap a ouvrire (FBitMap.bin) : ");textcolor(12);
    scanf("%s", Name);
    if(mode == 'A')
    {
        //Chargements des chaines bmp sauvgard�es dans la MS en m�moire centrale pour acc�lerer la manipulation.
        //Fbitmap est le pointeur globale du fichier Bitmap.
        Fbitmap = fopen(Name, "rb+");
        if(Fbitmap == NULL)
        {
                         //Fichier non existant
               textcolor(12);
               printf("\n\n\t\t\t\tFichier introuvable!");textcolor(11);printf("  Voulez-vous ressayer de nouveau\n");
               textcolor(12);printf("\n\t\t\t\t");x = wherex();y = wherey();printf(">");textcolor(11);printf("Continuer.");
               textcolor(12);printf("\n\t\t\t\t>");textcolor(11);printf("Quitter.");
               z = pointer(x - 1,y , 2, tab, 12, 11, 12, 10);//Listes des choix ( Affichage )
               gotoxy(x, y + 3);
               //Si il y aura une autre �criture dans la console , elle n'ecrasera pas le menu de d�cision (continuer, quitter)
               if(z == 0)
                  goto adrouvbt;//Essayer de nouveau un autre nom.
                else
                    goto nextouvbt;//Quitter l'operation de la cr�ation.
        }
        else
            for(int i = 0; i < 114 + NbSpecialites; ++i )
                fread(Bitmaps[i], (f.Entete.tab[4] - 1)*b, 1, Fbitmap);
    }
    else
        Fbitmap = fopen(Name, "wb+");

    textcolor(12);printf("\n\n\t\t\t\t> ");textcolor(11);printf("Ouverture du fichier terminee avec succes.");
    getch();
    nextouvbt:
    return 1;
}

void FermerBitMap()
{
    //Fermeture de fichier bitmap
    //Positionement au d�but du fichier Bmp pour sauvgarder les chaines.
     fseek(Fbitmap, 0, SEEK_SET);
     for(int i = 0; i < 114 + NbSpecialites ; ++i)//114 correspond au 111 �tablissements + 3 regions( Est, Ouest, Centre)
        fwrite(Bitmaps[i], (f.Entete.tab[4] - 1)*b, 1, Fbitmap);//Ecriture de chaque chaine BitMap dans sa position(0-110;etablissements;111-121;Specialit�s;121...Region

    fclose(Fbitmap);
}

int Ouvrire(char Name[ cchaine ], char mode){
    //Ouverture du fichier enseignants
    clrscr();
    if( mode == 'A')
    {
         textcolor(15);
         printf("\n________________________________________________________________________________________________________________________________________________________________________");
         textcolor(9);
         printf("\n                                 ____                       _                        _          __ _      _     _           ");
         printf("\n                                / __ \\                     | |                      | |        / _(_)    | |   (_)          ");
         printf("\n                               | |  | |_   ___   _____ _ __| |_ _   _ _ __ ___    __| |_   _  | |_ _  ___| |__  _  ___ _ __ ");
         printf("\n                               | |  | | | | \\ \\ / / _ \\ '__| __| | | | '__/ _ \\  / _` | | | | |  _| |/ __| '_ \\| |/ _ \\ '__|");
         printf("\n                               | |__| | |_| |\\ V /  __/ |  | |_| |_| | | |  __/ | (_| | |_| | | | | | (__| | | | |  __/ |   ");
         printf("\n                                \\____/ \\__,_| \\_/ \\___|_|   \\__|\\__,_|_|  \\___|  \\__,_|\\__,_| |_| |_|\\___|_| |_|_|\\___|_|   ");
         textcolor(15);
         printf("\n________________________________________________________________________________________________________________________________________________________________________");
         lines(n);
         char tab[2][70] = {"> Continuer.", "> Quitter."};
         int z, x, y;//Variables d'affichage.
         adrouv:
         textcolor(12);printf("\n\n\t\t\t> ");textcolor(11);printf("Veuillez vous introduire le nom du fichier a ouvrire (Principal.bin) : ");textcolor(12);
         scanf("%s", Name);
         f.F =  fopen( Name, "rb+");
         if(f.F == NULL)
         {
             //Fichier non existant
               textcolor(12);
               printf("\n\n\t\t\t\tFichier introuvable!");textcolor(11);printf("  Voulez-vous ressayer de nouveau\n");
               textcolor(12);printf("\n\t\t\t\t");x = wherex();y = wherey();printf(">");textcolor(11);printf("Continuer.");
               textcolor(12);printf("\n\t\t\t\t>");textcolor(11);printf("Quitter.");
               z = pointer(x - 1,y , 2, tab, 12, 11, 12, 10);//Listes des choix ( Affichage )
               gotoxy(x, y + 3);
               //Si il y aura une autre �criture dans la console , elle n'ecrasera pas le menu de d�cision (continuer, quitter)
               if(z == 0)
                  goto adrouv;//Essayer de nouveau un autre nom.
                else
                    goto nextouv;//Quitter l'operation de la cr�ation.
        }
        else{
                //Chargement de l'entete du fichier principale dans une stucture globale "f.Entete"
               fread(&(f.Entete), sizeof(f.Entete), 1, f.F);
               textcolor(12);printf("\n\n\t\t\t\t> ");textcolor(11);printf("Ouverture du fichier terminee avec succes.");
        }
    }
    else
    {
        textcolor(15);
        printf("\n________________________________________________________________________________________________________________________________________________________________________");
        textcolor(9);
        printf("\n                                        _____                _   _                   _          __ _      _     _           ");
        printf("\n                                       / ____|              | | (_)                 | |        / _(_)    | |   (_)          ");
        printf("\n                                      | |     _ __ ___  __ _| |_ _  ___  _ __     __| |_   _  | |_ _  ___| |__  _  ___ _ __ ");
        printf("\n                                      | |    | '__/ _ \\/ _` | __| |/ _ \\| '_ \\   / _` | | | | |  _| |/ __| '_ \\| |/ _ \\ '__|");
        printf("\n                                      | |____| | |  __/ (_| | |_| | (_) | | | | | (_| | |_| | | | | | (__| | | | |  __/ |   ");
        printf("\n                                       \\_____|_|  \\___|\\__,_|\\__|_|\\___/|_| |_|  \\__,_|\\__,_| |_| |_|\\___|_| |_|_|\\___|_|   ");
        textcolor(15);
        printf("\n\n________________________________________________________________________________________________________________________________________________________________________");
        lines(n);
        textcolor(12);printf("\n\n\t\t\t> ");textcolor(11);printf("Veuillez vous introduire le nom du fichier a creer (Principal.bin): ");textcolor(12);

        scanf("%s", Name);
        f.F =  fopen( Name, "wb+");
        if(f.F != NULL)
        {
             textcolor(12);printf("\n\n\t\t\t\t> ");textcolor(11);printf("Creation du fichier terminee avec succes.");
             f.Entete.tab[0 ] = 0; f.Entete.tab[1 ] = 0; f.Entete.tab[2 ] = 0;f.Entete.tab[ 4 ] = 1;f.Entete.tab[ 5 ] = 0;
        }
        else{textcolor(12);printf("\n\n\t\t\t> Erreur lors de la creation!");}

    }
    getch();
    nextouv:
    textcolor(15);
    return 1;
}

void Fermer(){
    //Positioner au d�but du fichier des enseignant pour sauvgarder l'entete
    rewind(f.F);
    //Sauvgarde de l'entete
    fwrite(&(f.Entete), sizeof(f.Entete), 1, f.F);
    //Fermeture du fichier
    fclose(f.F);
}

int EcrireDir(FILE *F, int i, Block buff){
    //Ecriture du buffeur dans le block num�ro i en memoire secondaire
    //On utilisera le d�but du fichier comme une entete, donc les op�ration sur les blocks s'�ffectuent au dessous de la zone allou�e pour l'entete
    fseek(F, (i-1)*sizeof(buff) + sizeof(SEntete),SEEK_SET);//Positionner le flux � la fin de l'entete
    fwrite(&buff, sizeof(buff), 1, F);//Ecriture dans le block num�ro i en m�moire secondaire
    return 1; // Bien �ffectu�e
}

int LireDir(FILE *F, int i, Block *buff){
    //Lecture d'un block � partire le fichier enseignant
    fseek(F, (i-1)*sizeof(*buff) + sizeof(SEntete),SEEK_SET);//Positionner le pointeur dans la position qui convient
    fread(buff, sizeof(*buff), 1, F);//Lecture dans le buffeur de l'information qui se trouve dans le block num�ro i en m�moire secondaire
    return 1;//Bien effectu�e
}

int Alloc_Block(){
    //Allouer un block vide pour le fichier enseignants
    int bl;//retouner le premier maillon dans la tete des blocks vide si elle est pas vide , sinon le plus grand block
    Block buff;
    if (f.Entete.tab[2] == 0)
    {
        //Si la tete de la liste des blocks vides est vide, retourner le plus grand block non allou� qui est le  plus grand num�ro du block utilis� + 1
        bl =  f.Entete.tab[4];//plus grand
        f.Entete.tab[4]++;//Mise � jour du plus grand block libre.
    }
    else
    {
        //Si la liste des blocks vides est pas vides alors retourner sa tete,
        bl = f.Entete.tab[2];
        LireDir(f.F, f.Entete.tab[2], &buff);
        CountLE[0]++;//Incrimenter le nombre des lectures fichier enseignant
        f.Entete.tab[2] = buff.lien;//Mise � jour de la tete des blocks vides au suivant.
        if(f.Entete.tab[2] == 0)//Si la liste sera vide alors mise � jour sa queue � 0 "On travaille avec 0 comme indicateur de fin de liste.
            f.Entete.tab[3] = 0;
    }
    return bl;
}

int RechInd(fileInd fInd, int matricule, int *trouv, int *i, int *j){
    // Recherche dichotomique du matricule dans l'index
    int bs,bi,inf,sup;
    bool stop;
    BlockIndex buff;
    bs = fInd.Entete.tab[0];
    bi = 1;
    *trouv = 0;
    stop = false;
    *j = 0;
    while (bi<=bs && (!(*trouv)) && (!stop)){
        //Recherche Intra-Blocks
        *i = (bi+bs)/2 ;
        LireDirInd(fInd.F, *i, &buff);
        CountLE[2]++;
        //printf("\MInd : %d\n i : %d", buff.tab[0].matricule, *i);
        if ((matricule >= buff.tab[0].matricule) && (matricule <= buff.tab[buff.Nb - 1].matricule)){
            inf = 0; sup = buff.Nb - 1;

            while ((inf <= sup) && (!(*trouv))&& (!stop)){
                    //Recherche Inter-Blocks
                *j = (inf+sup)/2 ;
                if (matricule == buff.tab[*j].matricule)
                    if(buff.tab[*j].eff == 0)//Si le block trouv� est pas supprimer logiquement, affecter � trouv 1
                        *trouv = 1;
                    else
                        stop = true;//Le matricule qu'on cherche n'existe pas, car il doit etre dans ce block qui est pas le cas.

                else {
                    // La recherche dans l'autre sous-tableau
                    if (matricule < buff.tab[*j].matricule) {sup = (*j)-1 ;}
                    else {inf = (*j) + 1 ;}
                }
            }
            if (inf>sup){ *j = inf ;}
            stop = true;
        }//La recherche dans l'autre sous fichier.
        else if (matricule < buff.tab[0].matricule) {bs = (*i)-1 ;}
        else bi = (*i)+1 ;

    }
    if (bi>bs) {
            //Si le matricule n'existe pas , retourner la position <i, j> ou il doit exister.
            *i = bi ;
            *j = 0;
    }

};

int insertInd(fileInd *fInd, SEnsInd e, bool condition) {
    /*Insertion dans un fichier TOF, condition pour indiquer si il faut faire la recherche
    pour savoire si il existe, sinon si on a des informations sur son existance on l'insert directement*/
    int trouv;
    bool continu;
    int i,j,k;
    BlockIndex buffInd;
    SEnsInd x;
    if(condition == true)//On sais rien sur l'existance du matricule donc il faut le rechercher.
        RechInd(*fInd, e.matricule, &trouv, &i, &j);
    else//On sais que le matricule n'existe pas donc on l'insert directement pour diminuer le nombre des lectures et ecritures index
        trouv = 0;

    if (!trouv){
        //On insert que si le matricule n'existe pas
        continu = true;
        while (continu && i<=fInd->Entete.tab[0]){
            //Si on est pas dans la fin du fichier index, ou on n'a opas encore inserer, on boucle
            LireDirInd(fInd->F, i, &buffInd);
            CountLE[2]++;
            //On sauvgarde le dernier element du block en cas de d�bordement intra blocks on le perdera pas, (insertion � d�calages)
            x = buffInd.tab[buffInd.Nb - 1];
            k = buffInd.Nb - 1;
            while(k>j){
                //Effectuer une insertion � d�calage
                buffInd.tab[k] = buffInd.tab[k-1];
                k = k-1;
            }
            //Insertion de la donn�e
            buffInd.tab[j] = e;
        if (buffInd.Nb < MaxInd){
            //Si les d�calage ne causent pas  un d�bordement itra-blocks, on sauvgarde le buffeur dans la m�moire secondaire
            buffInd.Nb++ ;
            buffInd.tab[buffInd.Nb - 1] = x ;
            CountLE[3]++;
            EcrireDirInd(fInd->F, i, buffInd);
            continu = false;
        }
        else {
            //Si ily aura un d�bordement on insert la derniere donn�e r�cup�r� dans le prochain block
            CountLE[3]++;
            EcrireDirInd(fInd->F,i,buffInd);
            //Aller au prochain block
            i++;
            //Positionement de l'indice de beffeur au d�but
            j = 0;
            e = x;
        }


      }
      if (i > fInd->Entete.tab[0]){
        //Si la donn�es � inserer est la plus grande , et le dernier block est remplis, on alloue un autre block
        //Nouveau Block
        buffInd.tab[0] = e;
        buffInd.Nb = 1;
        EcrireDirInd(fInd->F,i,buffInd);
        CountLE[3]++;
        //Mise � jour de nombre des blocks
        fInd->Entete.tab[0] = i;

      }
     //Incrimenter le nombre d'enregistrements inser�s dans le fichier index
      fInd->Entete.tab[1]++;
    }
    return !trouv;
}

int supInd( int i ,int j){
    //Suppression logique dans l'index
    BlockIndex buffer;
    LireDirInd(fInd.F,i,&buffer);
    //Incrimentation de nombre des lecturesIndex
    CountLE[2]++;
    //On mise � jour le champ effac� � vrai
    buffer.tab[j].eff = 1;
    //On sauvgarde dans l'index
    EcrireDirInd(fInd.F ,i,buffer);
    //On incrimente le nombre des �critures index
    CountLE[3]++;
    //On d�criment le nombre d'enregistrements
    fInd.Entete.tab[1]--;
}

int pointer(int x, int y, int Nb, char tabch[4][70], int color1,  int color2, int color3, int color4)
{
    //Fonction d'affichage des listes  de d�cisions
     int z = 0, y1 = y;
     int key;
     do
     {
         gotoxy(x, y);
         textcolor(color4);
         printf(">");
         textcolor(color1);
         printf("   %s", tabch[z]);
         key = getch();
         gotoxy(x, y);
         printf(" ");
         textcolor(color3);
         printf("%c", tabch[z][0]);textcolor(color2);printf("%s   ",tabch[z] + 1);
         if(key == 72){
            y--;
            z--;
            if(y < y1){
                y = y1 + Nb - 1;
                z = Nb - 1;
            }
         }
         else if(key == 80){
            y++;
            z++;
            if(y >= y1 + Nb){
                y = y1;
                z = 0;
            }
         }
     }
     while(key != 27 && key != 13);
     if(key == 13)
        return z;
    else
        return -1;
}

void lines(int fin)
{
    for(int i =0; i< fin; i++)
        printf("%c", -80);

}
int AnBissextile(int year){
  // ann�e bissextile
   if (year % 400 == 0)
     return 1;
   // non bissextile si divisble par 100 et non 400
   else if (year % 100 == 0)
      return 0;
   // Ann�e bissextile si non divisible par 100 mais divisible par 4

   else if (year % 4 == 0)
      return 1;
   // non bissextile
   else
      return 0;
}

SEns Generer(){
    SEns str;
    str.Matricule = rand()%(999999 - 112111) + 112111 ;//rand()%10;

    int taille = rand()%27 + 4;
    int c ;//pour contenire le code ascii du i-�me caracter du nom/prenom gener�

    for(int i = 0; i < taille ; i++)
    {
        c = rand()%26 + (rand()%2)*32 + 65;//generer un nombre entre 1 et 26 puis le ranger dans la plage des code ascii, traiter les majuscule et miniscule par rand()%2 * 32
        str.Nom_Prenom[i] = (char) c;//type caster  l'entier en caracter et l'affecter dans la case i de np
    }

    for(int i = taille; i< 30; i++)
        str.Nom_Prenom[i] = ' ';//Completer par des espaces vides pour des raisons de justification du texte

    str.Nom_Prenom[30] = '\0';//fin de chaine de caracter
    c = rand()%(taille - 2) + 1;//l'emplacement de s�parateur /du nom/pr�nom gener� al�atoirement vers la fin
    str.Nom_Prenom[c] = '.';//S�parer le nom et le pr�nom

    str.Date_naissance.yyyy = rand()%(1998 - 1953 + 1) + 1953;
    str.Date_naissance.mm = rand()%12 + 1;
    //V�rification si le jour g�n�r� est valide
    if(str.Date_naissance.mm == 2)
        str.Date_naissance.jj = rand()%(28 + AnBissextile(str.Date_naissance.yyyy));
    else if(((str.Date_naissance.mm > 5)&&(str.Date_naissance.mm % 2 == 0))||((str.Date_naissance.mm < 6)&&(str.Date_naissance.mm % 2 == 1)))
        str.Date_naissance.jj = rand()%30 + 1;
    else if(((str.Date_naissance.mm > 5)&&(str.Date_naissance.mm % 2 == 1))||((str.Date_naissance.mm < 6)&&(str.Date_naissance.mm % 2 == 0)))
        str.Date_naissance.jj = rand()%31 + 1;

    //La wilaya de naissance
    str.Wilaya_naissance = rand()%58;
    //Groupe sanguin
    str.Groupe_sanguin =rand()%8;

    //Gender
    str.sexe = rand()%2;
    //Date de recrutement
    str.Date_recrutement.yyyy = rand()%(2022 - 1985 + 1) + 1985;
    while(str.Date_recrutement.yyyy - str.Date_naissance.yyyy < 20)
        str.Date_recrutement.yyyy++;
    str.Date_recrutement.mm = rand()%12 + 1;
    //gendate.jj = rand()%32 + 1;
    if(str.Date_recrutement.mm == 2)
        str.Date_recrutement.jj = rand()%(28 + AnBissextile(str.Date_recrutement.yyyy)) ;
    else if(((str.Date_recrutement.mm > 5)&&(str.Date_recrutement.mm % 2 == 0))||((str.Date_recrutement.mm < 6)&&(str.Date_recrutement.mm % 2 == 1)))
        str.Date_recrutement.jj = rand()%30 + 1;
    else if(((str.Date_recrutement.mm > 5)&&(str.Date_recrutement.mm % 2 == 1))||((str.Date_recrutement.mm < 6)&&(str.Date_recrutement.mm % 2 == 0)))
        str.Date_recrutement.jj = rand()%31 + 1;
    //Grade

    str.Grade = rand()%6;
    //Dernier diplome
    str.Dernier_diplome = rand()%6;
    //Etablissement_Universitaire
    str.Etablissement_universitaire = rand()%111;
    //10 specialit�s
    str.Specialite = rand()%10;
    return str;
}

int region(int i)
{
    //Retourner le code r�gion(0 : est, 1: centre, 2: ouest � partire du code d'�tablissement donn� � l'entr�
    if(i < 41)//Est
        return ( 0);

    else if(i < 82 && i >= 41)//centre
        return (1);

    else //ouest
        return (2);
}
int u = 111111;
void DispEtablissements()
{
    //Fonction d'affichage des �tablissements
     textcolor(15);
     printf("\n________________________________________________________________________________________________________________________________________________________________________\n");
     textcolor(9);
     printf("\n                                           ______ _        _     _ _                                   _         ");textcolor(10);printf("_    _ ");
     textcolor(9); printf("\n                                          |  ____| |      | |   | (_)                                 | |       ");textcolor(10);printf("| |  | |");
     textcolor(9);printf("\n                                          | |__  | |_ __ _| |__ | |_ ___ ___  ___ _ __ ___   ___ _ __ | |_ ___  ");textcolor(10);printf("| |  | |_ ____   __");
     textcolor(9); printf("\n                                          |  __| | __/ _` | '_ \\| | / __/ __|/ _ \\ '_ ` _ \\ / _ \\ '_ \\| __/ __|");textcolor(10);printf(" | |  | | '_ \\ \\ / /");
     textcolor(9);printf("\n                                          | |____| || (_| | |_) | | \\__ \\__ \\  __/ | | | | |  __/ | | | |_\\__ \\");textcolor(10);printf(" | |__| | | | \\ V /");
     textcolor(9);printf("\n                                          |______|\\__\\__,_|_.__/|_|_|___/___/\\___|_| |_| |_|\\___|_| |_|\\__|___/");textcolor(10);printf(" \\ ____/|_| |_|\\_/");
     textcolor(9);textcolor(15);
     printf("\n\n________________________________________________________________________________________________________________________________________________________________________\n\n");
     lines(n);
     int x = wherex() - 100, y1 = wherey(), y = y1;
     for(int i = 0; i< 111; ++i)
     {
            if(i % 56 == 0)
            {x = x + 90;y = y1;}
            gotoxy(x, y);
            if(tab_Etablissement[i][0] == 'E')//Afficher les �coles avec un libell� rouge
                textcolor(12);
            else if(tab_Etablissement[i][0] == 'U')//Afficher les universit�s avec un libell� blanc
                textcolor(15);
            else
                textcolor(9);//Afficher les centres universitaires avec un libell� blue

            lines(1);
            textcolor(10);
            printf("%.3d :", i);
            textcolor(15);
            printf("%s", tab_Etablissement[i]);
            if(i == 39 || i == 81)
            {
                y++;
                gotoxy(x, y);
                printf("+-------------------------------------+\n");
            }
            y++;
    }
}

int Chargement_Init(){
    clrscr();
    //Fonction du chargement initial.
     printf("\n________________________________________________________________________________________________________________________________________________________________________\n");
     textcolor(9);
     printf("\n                                            _____ _                                               _     _       _ _   _       _ ");
     printf("\n                                           / ____| |                                             | |   (_)     (_) | (_)     | |");
     printf("\n                                          | |    | |__   __ _ _ __ __ _  ___ _ __ ___   ___ _ __ | |_   _ _ __  _| |_ _  __ _| |");
     printf("\n                                          | |    | '_ \\ / _` | '__/ _` |/ _ \\ '_ ` _ \\ / _ \\ '_ \\| __| | | '_ \\| | __| |/ _` | |");
     printf("\n                                          | |____| | | | (_| | | | (_| |  __/ | | | | |  __/ | | | |_  | | | | | | |_| | (_| | |");
     printf("\n                                           \\_____|_| |_|\\__,_|_|  \\__, |\\___|_| |_| |_|\\___|_| |_|\\__| |_|_| |_|_|\\__|_|\\__,_|_|");
     printf("\n                                                                    __/ |                                                        ");
     printf("\n                                                                   |___/                                                         ");
     textcolor(15);
     printf("\n\n________________________________________________________________________________________________________________________________________________________________________");
     lines(n);
     textcolor(12);
     printf("\n\n\n\n                         >");textcolor(11);
     printf(" Veuillez introduire le nombre d'enregistrements a inserer : ");textcolor(12);
     int NombreEnrg;
     //R�cuperer le nombre d'enregistrements � inserer
     scanf(" %d",&NombreEnrg);
     textcolor(11);
     printf("\n\n                                        LOADING... ");textcolor(9);printf("|");
     gotoxy(wherex() + 62, wherey());
     printf("|");
     gotoxy(wherex() - 61, wherey());
     textcolor(14);
     //Allouer un block pour le d�part
     int j = 0, i = Alloc_Block() , trouve, a, z;
     Block buff;
     SEnsInd str;
     //initialiser le Block suivant � 0 (null)
     buff.lien = 0;
     //Initialisation de chaque bit de BitMap � faux
     for(int s1 = 0; s1 < NbSpecialites + 114; ++s1)
        for(int s = 0; s < NombreEnrg/b + 1; s++)
            Bitmaps[s1][s] = false;

    for (int k = 0; k < NombreEnrg; k++)
    {//Insertion enregistrement par enregistrement dans le beffeur puis le charger dans m�moire secondaire
        if(j < b)
        {
            //Si il y a de l'espace dans le block, on incrimente le Nb puis on insert, avec l'insertion dans l'index si le matricule n'existe pas
            //Dans le fochier, l'�puration est faite avec le chargement initial
            buff.tab[j] = Generer();
            //Affectation dans les champs d'enregistrement � inserer dasn l'index (matricule, position, �ffac�)
            str.Block = i;
            str.Enrg = j;
            str.matricule = buff.tab[j].Matricule;
            str.eff = 0;
            //On insert avec le test d'existance du matricule
            if(!insertInd(&fInd, str, true))
                j--;//Si le matricule existe on fixe l'indice � la meme case j du beffeur de fichier enseignant

            else
            {
                //Si l'insertion est �ffectu�, on mise � jour dans le bitmap de chaque �tablissement, specialit� et r�gion.
                for(int n1 = 0; n1 < 111; n1++)
                {
                    //Bitmaps des �tablissements
                    if(n1 == buff.tab[j].Etablissement_universitaire)
                    {

                        Bitmaps[n1][(i-1)*b + j] = true;

                    }
                    else{

                        Bitmaps[n1][(i-1)*b + j] = false;

                    }
                }
                for(int n1 = 111; n1 < 111 + NbSpecialites; n1++)
                {
                    //Bitmap des specialit�s
                    if(n1 - 111 == buff.tab[j].Specialite)
                        Bitmaps[n1][(i-1)*b + j] = true;
                    else
                        Bitmaps[n1][(i-1)*b + j] = false;
                }
                for(int n1 = 111 + NbSpecialites; n1 < 114 + NbSpecialites ; n1++)
                {
                    //Bitmaps des r�gions
                    if(n1 - 111 - NbSpecialites == region(buff.tab[j].Etablissement_universitaire))
                        Bitmaps[n1][(i-1)*b + j] = true;
                    else
                        Bitmaps[n1][(i-1)*b + j] = false;
                }
            }
            ++j;
        }
        else
        {
            //Si le block est plein, on le sauvgarde puis on insert l'nregistrement num�ro k, dans la premiere case du prochain block
            buff.Nb = b;//Block plein
            EcrireDir(f.F, i, buff);
            CountLE[1]++;
            //On block nouveau comme la tete de la liste et on point vers l'ancien block remplis
            buff.lien = i;
            //Allocation de la nouvelle tete ( nouveau block )
            i = Alloc_Block();
            //On genere l'enregistrementb num�ro k
            buff.tab[0] = Generer();
            //On affect au matricule la variable globale u, qu'on est sur qu'elle n'existe pas dans l'index,
            // pour eviter le cas ou les enregistrements � patrire de k + p eme jusq'au dernier enregistrement
            // genere un matricule qui existe et le dernier block soit rempli, alors on aura un block en plus qu'on l'utilisera pas
            buff.tab[0].Matricule = u;
            ++u;//On incrimente u pour le prochain matricule
            //On remplie la structure de l'index
            str.Block = i;
            str.Enrg = 0;
            str.eff = 0;
            str.matricule = buff.tab[0].Matricule;
            //On pr�pare la position j pour le prochain enregistrement
            j = 1;
            insertInd(&fInd, str, true);
            //On mise � jour dans les chaines bitmaps
            for(int n1 = 0; n1 < 111; n1++)
            {
                if(n1 == buff.tab[0].Etablissement_universitaire)
                    Bitmaps[n1][(i-1)*b + 0] = true;
                else
                    Bitmaps[n1][(i-1)*b + 0] = false;
            }
            for(int n1 = 111; n1 < 111 + NbSpecialites; n1++)
            {
                if(n1 - 111 == buff.tab[0].Specialite)
                    Bitmaps[n1][(i-1)*b + 0] = true;
                else
                    Bitmaps[n1][(i-1)*b + 0] = false;
            }
            for(int n1 = 111 + NbSpecialites; n1 < 114 + NbSpecialites ; n1++)
            {
                if(n1 - 111 - NbSpecialites == region(buff.tab[0].Etablissement_universitaire))
                    Bitmaps[n1][(i-1)*b + 0] = true;
                else
                    Bitmaps[n1][(i-1)*b + 0] = false;
            }
        }
        //Affichage (Loading...)
        if(NombreEnrg > 10000)
        {
                if(k % (NombreEnrg/15 + 1) == 0)
                    {
                        textbackground(0);printf("  ");textbackground(15);printf("  ");
                    }
        }
    }
    //Le chargement  du dernier block dans la m�moire secondaire
    buff.Nb = j;
    EcrireDir(f.F, i, buff);
    CountLE[1]++;
    f.Entete.tab[0] = i;//tete
    f.Entete.tab[1] = 1;//queue
    f.Entete.tab[5] = (i - 1)*b + j;
    //Affichage
    if(NombreEnrg  <= 10000)
        for(int y = 0; y < 15; y++)
        {
            textbackground(0);printf("  ");textbackground(15);printf("  ");
            for(int o = 0; o < 60000000; o++){}
        }
    textbackground(0);textcolor(12);printf("\n\n\n                                              >");textcolor(11);printf(" Chargement termine!");
    getch();
    textcolor(15);
    return 1;
}

int Codef(FILE **in, char key[cchaine], char nout[cchaine]){
     textcolor(15);
     clrscr();
     printf("\n________________________________________________________________________________________________________________________________________________________________________");
     textcolor(9);
     printf("\n                                                     _____          _                  ");
     printf("\n                                                    / ____|        | |                 ");
     printf("\n                                                   | |     ___   __| | __ _  __ _  ___ ");
     printf("\n                                                   | |    / _ \\ / _` |/ _` |/ _` |/ _ \\");
     printf("\n                                                   | |___| (_) | (_| | (_| | (_| |  __/");
     printf("\n                                                    \\_____\\___/ \\__,_|\\__,_|\\__, |\\___|");
     printf("\n                                                                            __/ |             ");
     printf("\n                                                                           |___/              ");
     textcolor(15);
     printf("\n\n________________________________________________________________________________________________________________________________________________________________________");
     lines(n);
    //Proc�dure pour crypter un fichier binaire.
    unsigned char buff;//On utilise le non sign� parce que on ne va pas effectuer des operations de soustraction.
    int len, cpt = 0;//cpt pour codder chaqque caracter avec ce qu'il convient dans la cl�.
    FILE *out;
    textcolor(12);printf("\n\t\t> ");textcolor(11);printf("Veuillez vous saisir la cle du cryptage : ");textcolor(12);
    scanf("%s", key);
    len = strlen(key);
    textcolor(12);printf("\n\n\t\t> ");textcolor(11);printf("Veuillez vous introduire le nom du fichier crypte : ");textcolor(12);
    scanf("%s", nout);//Le nom de fichier crypter � g�n�rer.
    out = fopen(nout,"wb+");//ouverture du fichier crypt� en mode nouveau, binaire.
    fseek(*in, 0, SEEK_SET);

    while(fread(&buff, sizeof(buff), 1, *in))
    {
        buff= buff + key[cpt];//Crypter
        fwrite(&buff, sizeof(buff), 1, out);//Ecrire dans B
        cpt = (cpt + 1) % len;//Aller au prochaine caracter de la cl�.
    }
    fclose(out);
    textcolor(12);printf("\n\n\n\n\t\t\t\t> ");textcolor(11);printf("Codage termine.");textcolor(12);
    getch();
    return 1;
}

int Decodef(char file[cchaine], char key[cchaine], char nout[cchaine]){
 clrscr();
 textcolor(15);
 printf("\n________________________________________________________________________________________________________________________________________________________________________");
 textcolor(9);
 printf("\n                                                   _____                     _                  ");
 printf("\n                                                  |  __ \\                   | |                 ");
 printf("\n                                                  | |  | | ___  ___ ___   __| | __ _  __ _  ___ ");
 printf("\n                                                  | |  | |/ _ \\/ __/ _ \\ / _` |/ _` |/ _` |/ _ \\");
 printf("\n                                                  | |__| |  __/ (_| (_) | (_| | (_| | (_| |  __/");
 printf("\n                                                  |_____/ \\___|\\___\\___/ \\__,_|\\__,_|\\__, |\\___|");
 printf("\n                                                                                      __/ |     ");
 printf("\n                                                                                     |___/   ");
 textcolor(15);
printf("\n\n________________________________________________________________________________________________________________________________________________________________________");
lines(n);
    //Proc�dure pour d�crypter un fichier binaire.
    signed char buff;//On utilise le signed char parcequ'on va effectuer des operations de soustraction
    FILE *out, *in;
    in = fopen(file, "rb");
    int len, cpt = 0;
    textcolor(12);printf("\n\t\t> ");textcolor(11);printf("Veuillez vous saisir la cle du cryptage : ");textcolor(12);
    scanf("%s", key);
    len = strlen(key);
    textcolor(12);printf("\n\n\t\t> ");textcolor(11);printf("Veuillez vous introduire le nom du fichier resultat : ");textcolor(12);
    scanf("%s", nout);
    out = fopen(nout,"wb+");
    if(!out)
        return 0;

    fseek(in, 0, SEEK_SET);
    while(fread(&buff, sizeof(buff), 1, in))
    {
        buff= buff - key[cpt];
        fwrite(&buff, sizeof(buff), 1, out);
        cpt = (cpt + 1) % len;
    }
    fclose(out);//fermer le fichier r�sultant
    fclose(in);
    textcolor(12);printf("\n\n\n\n\t\t\t\t> ");textcolor(11);printf("Decodage termine.");textcolor(12);
    getch();
    return 1;
}

int SupEngMat(Block *buff, int i, int j ){
    //On utilise 3 buffeurs, le premier pour le block source , l'autre pour le block destination, et le troisieme pour la mise � jour des liens dans l'entete
    Block buff1, buff2, buff3;
    //On peut utiliser moins de variables, la raison d'utilisation de plusieurs variable
    //est pour faciliter la r�alisation de la suppression
    /*
        s2 : Region de l'�tablissement l'enregistrement rempla�ant
        s1 : Region de l'�tablissement l'enregistrement remplac�
        sp1: Specialit� de l'erregistrement remplac�
        sp2: Specialit� de l'erregistrement rempla�ant.
        i1 : Cordonn�es de l'enregistrement remplac�
        i2 : Cordonn�es de l'enregistrement rempla�ant
        j1 : Cordonn�es de l'enregistrement remplac�
        j2 : Cordonn�es de l'enregistrement rempla�ant
        se1: Etablissement universitaire de l'enregistrement remplac�
        se2: Etablissement universitaire de l'enregistrement rempla�ant
    */
    int trouv, a, c, s1, s2, sp1, sp2, i1, i2, j1, j2, se1, se2;
    BlockIndex buffInd;
    //Lecture
    LireDir(f.F, i, &buff1);
    CountLE[0]++;
    //Si l'enregistrement est supprim� sans remplacement par un autre, on est pas besoin de modifier 2 chaines bitmaps , une seule suffit
    sp1 = -1;
    if( i == f.Entete.tab[0])
    {
        //Si la tete est le block de l'enregistrement � supprim�.

        //Ajustement des indices des bitmaps de l'enregistrement rempla�ant
        sp2 = buff1.tab[buff1.Nb - 1].Specialite + 1;
        i2 = i;
        j2 = buff1.Nb - 1;
        s2 = region(buff1.tab[buff1.Nb - 1].Etablissement_universitaire);
        se2 = buff1.tab[buff1.Nb - 1].Etablissement_universitaire;

        //Si la tete contient un seul enregistrement donc le fichier sera vide
        if(buff1.Nb == 1)
        {
            if(f.Entete.tab[3] != 0)
            {
                //Si la queue des listes vides n'est pas vide, on mise � jour les liens
                LireDir(f.F, f.Entete.tab[3], &buff3);
                CountLE[0]++;
                buff3.lien = f.Entete.tab[0];
                EcrireDir(f.F, f.Entete.tab[3], buff3);
                CountLE[1]++;
            }
            else//Sinon on mise � jour directement la tete et la queue de liste des blocks vides dans l'entete ( vide avant la mise � jour)
                 f.Entete.tab[2] = f.Entete.tab[0];
            f.Entete.tab[3] = f.Entete.tab[0];

            //on Avance l'ntete au suivant "0" dans ce cas
            f.Entete.tab[0] = buff1.lien;
            buff->Nb = 0;//Block vide
            if(f.Entete.tab[0] == 0)
                f.Entete.tab[1] = 0;//Fichier vide
            if(f.Entete.tab[0] == f.Entete.tab[4] - 1)
                f.Entete.tab[4]--;//Si la tete est le block le avant le plus grand block qui est souvant le cas, alors on d�criment le plus grand block pour
                //ne pas perdre d'�space
        }
        else if(buff1.Nb > 1)
        {
            //Sinon il y a au moins 2 enregistrements , il suffit de remplacer par le dernier.
            if(j != buff1.Nb - 1){
                    //Si l'enregistrement � supprimer est le dernier, il suffit just de d�crimenter Nb, et mise � jour 3 chaines de ButMap uniquement
                sp1 = buff1.tab[j].Specialite + 1;
                j1 = j;
                i1 = i;
                s1 = region(buff1.tab[j].Etablissement_universitaire);
                se1 = buff1.tab[j].Etablissement_universitaire;
            }
            buff1.Nb--;
            //Remplacer par le dernier
            buff1.tab[j] = buff1.tab[buff1.Nb];
            //Mise �  jour dans l'index apres la suppresion de chaque block
            RechInd(fInd, buff1.tab[buff1.Nb].Matricule, &trouv, &a, &c);
            if(trouv){
                LireDirInd(fInd.F, a, &buffInd);
                CountLE[2]++;
                buffInd.tab[c].Enrg = j;
                EcrireDirInd(fInd.F, a, buffInd);
                CountLE[3]++;
            }
            //On mise � jour le buffeur courrant pour le retourner, c'est pour des raisons d'optimisation( diminuer le nombre des acc�s m�moire )
            *buff = buff1;
             EcrireDir(f.F, i, buff1);
             CountLE[1]++;
        }
    }
    else
    {
        //On est pas dans la tete, on lit le buffeur d'enregistrement � supprimer dans buff1, et buff2 contient la tete
        LireDir(f.F, f.Entete.tab[0], &buff2);
        CountLE[0]++;
        sp1 = buff1.tab[j].Specialite + 1;
        j1 = j;
        i1 = i;
        s1 = region(buff1.tab[j].Etablissement_universitaire);
        se1 = buff1.tab[j].Etablissement_universitaire;
        //Le remplacement
        buff1.tab[j] = buff2.tab[buff2.Nb - 1];
        sp2 = buff2.tab[buff2.Nb - 1].Specialite + 1;
        i2 = f.Entete.tab[0];
        j2 = buff2.Nb - 1;
        s2 = region(buff2.tab[buff2.Nb - 1].Etablissement_universitaire);
        se2 = buff2.tab[buff2.Nb - 1].Etablissement_universitaire;
        //La mise � jour dans l'index
        RechInd(fInd, buff2.tab[buff2.Nb - 1].Matricule, &trouv, &a, &c);
        if(trouv){
            LireDirInd(fInd.F, a, &buffInd);
            CountLE[2]++;
            buffInd.tab[c].Enrg = j;
            buffInd.tab[c].Block = i;
            EcrireDirInd(fInd.F, a, buffInd);
            CountLE[3]++;
        }
        //Mise � jour du buffeur d'entr�
        *buff = buff1;
        EcrireDir(f.F, i, buff1);
        CountLE[1]++;
        //Si la tete contient un seul enregistrement, donc il faut la modifier
        if(buff2.Nb == 1)
        {
            if(f.Entete.tab[3] != 0)
            {
                //Si la liste des blocks vides n'est pas vide , on mise � jour les liens
                LireDir(f.F, f.Entete.tab[3], &buff3);
                CountLE[0]++;
                buff3.lien = f.Entete.tab[0];
                EcrireDir(f.F, f.Entete.tab[3], buff3);
                CountLE[1]++;
            }
            else
                f.Entete.tab[2] = f.Entete.tab[0];

            f.Entete.tab[3] = f.Entete.tab[0];
            f.Entete.tab[0] = buff2.lien;
            if(f.Entete.tab[0] == 0)
                f.Entete.tab[1] = 0;//Fichier vide
            if(f.Entete.tab[0] == f.Entete.tab[4] - 1)
                f.Entete.tab[4]--;
        }
        else if(buff2.Nb > 1)
        {
            buff2.Nb--;
            EcrireDir(f.F, f.Entete.tab[0], buff2);
            CountLE[1]++;
        }
    }
    //Les mise � jours dans les chaines BitMaps

    //L'enregistrement supprim� r�element dans la tete
    Bitmaps[se2][(i2 - 1)*b + j2] = false;//Etablissement
    Bitmaps[sp2 + 110][(i2 - 1)*b + j2] = false;//pecialit�
    Bitmaps[s2 + 111 + NbSpecialites][(i2 - 1)*b + j2] = false;//Region
    if(sp1 !=  -1)
    {
        //Les nouvelles valeurs de l'enregistrement de destination, l'eregistrement � remplacer
        Bitmaps[se1][(i1 - 1)*b + j1] = false;
        Bitmaps[s1 + 111 + NbSpecialites][(i1 - 1)*b + j1] = false;
        Bitmaps[sp1 + 110][(i1 - 1)*b + j1] = false;
        Bitmaps[se1][(i - 1)*b + j] = true;
        Bitmaps[sp2 + 110][(i - 1)*b + j] = true;
        Bitmaps[s2 + 111 + NbSpecialites][(i - 1)*b + j] = true;
    }
    f.Entete.tab[5]--;
    return 1;
}

int InsErgMat( SEns e){
    //Insertion dans le fichier enseignats
        int i, j = 0, s;
        int trouve;
        Block buff1;
        SEnsInd SInd;
        //Recherhe dans l'index si le matricule � supprimer existe
        RechInd(fInd, e.Matricule, &trouve, &i, &j);
        if( !trouve )
        {
            //Si il n'existe, et le fichier n'est pas vide
            if(f.Entete.tab[0] !=   0)
                LireDir(f.F, f.Entete.tab[0], &buff1);//Lecture de la tete pour inserer l'enregistrement
            else
            {
                //On alloue une tete pour inserer l'nregistrement et faire les mise � jour dans l'entete, et eventuellement dans le block
                f.Entete.tab[0] = Alloc_Block();
                buff1.Nb = 0;
                buff1.lien = 0;
            }
            if(buff1.Nb < b)
            {
                //Si il y a de l'espace dans le block pour inserer l'enregistrement, on incriment Nb et On insert
                SInd.Enrg = buff1.Nb;
                SInd.Block = f.Entete.tab[0];
                buff1.tab[buff1.Nb] = e;
                buff1.Nb++;//Mise � jour de nombre des enregistrements dans le block
                EcrireDir(f.F, f.Entete.tab[0], buff1);
                CountLE[1]++;
            }
            else
            {
                //Si le block est rempli ou le fichier est vide pr�alablement
                buff1.tab[0] = e;//On insert l'nregitrement dans la premi�re case
                buff1.Nb = 1;//
                buff1.lien = f.Entete.tab[0];
                i = Alloc_Block();//On alloue le block pour sauvgarder le buffeur
                SInd.Enrg = 0;
                SInd.Block =  i;
                f.Entete.tab[0] = i;//Mis e� jour de la tete de la liste au nouveau block
                EcrireDir(f.F, i, buff1);
                CountLE[1]++;
            }
            SInd.eff =  0;
            SInd.matricule =  e.Matricule;
            //Insertion sans testes d'existance
            insertInd(&fInd, SInd, true);
            //Mise � jour de nombre des enregistrements inser�s
            f.Entete.tab[5]++;
            //Mise � jour des Bitmaps
            Bitmaps[e.Specialite + 111][(SInd.Block - 1)*b + SInd.Enrg] = true;
            Bitmaps[region(e.Etablissement_universitaire) + 111 + NbSpecialites][(SInd.Block - 1)*b + SInd.Enrg] = true;
            Bitmaps[e.Etablissement_universitaire][(SInd.Block - 1)*b + SInd.Enrg] = true;

        }
        return 1;
}

void Affichage_Index(fileInd f)
{
     clrscr();
     textcolor(15);
     printf("\n________________________________________________________________________________________________________________________________________________________________________");
     textcolor(9);
     printf("\n                                                                    _____           _           ");
     printf("\n                                                                   |_   _|         | |          ");
     printf("\n                                                                     | |  _ __   __| | _____  __");
     printf("\n                                                                     | | | '_ \\ / _` |/ _ \\ \\/ /");
     printf("\n                                                                    _| |_| | | | (_| |  __/>  < ");
     printf("\n                                                                   |_____|_| |_|\\__,_|\\___/_/\\_\\");
     textcolor(15);
     printf("\n\n________________________________________________________________________________________________________________________________________________________________________");
    lines(n);
    int count = 1, cd = 1;
    int x, y, z;
    BlockIndex  buff;
    char tab[3][70] = {"> Continuer par morceaux.", "> Continuer jusqu'a la fin.", "> Quitter l'affichage."};
    for(int i = 1; i <= f.Entete.tab[0]; ++i)
    {
        textcolor(12);
        printf("\n\n                         +");textcolor(11);printf("------------------------------------------------------------------------------------------------------------");textcolor(12);printf("+");
        textcolor(11);printf("\n                         |");textcolor(12);printf("                                              -");textcolor(11);printf("Block");textcolor(12);printf(" %d-                                                     ", i);textcolor(11);printf("|");

        textcolor(12);printf("\n                         +");textcolor(11);printf("------------------------------------------------------------------------------------------------------------");textcolor(12);printf("+");
         textcolor(12);
         printf("\n                         +");textcolor(11);printf("---------");textcolor(12);printf("+");textcolor(11);printf("------------------------");textcolor(12);
         printf("+");textcolor(11);printf("---------------------");textcolor(12);printf("+");textcolor(11);printf("---------------------------------------------------");
         textcolor(12);printf("+");
         textcolor(11);printf("\n                         |");textcolor(12);printf(" Num Eng ");textcolor(11);printf("|");textcolor(12);printf("       Matricule        ");
         textcolor(11);printf("|");textcolor(12);printf("        .eff         ");
         textcolor(11);printf("|");textcolor(12);printf("                      Cordonnees                   ");textcolor(11);printf("|");textcolor(12);
         printf("\n                         +");textcolor(11);printf("---------");textcolor(12);printf("+");textcolor(11);printf("------------------------");textcolor(12);
         printf("+");textcolor(11);printf("---------------------");textcolor(12);printf("+");textcolor(11);printf("---------------------------------------------------");
         textcolor(12);printf("+");
        LireDirInd(f.F, i, &buff);
        CountLE[2]++;
        for( int j = 0; j < buff.Nb; ++j){
                printf("\n                         ");textcolor(11);printf("|");textcolor(14);printf("  %.4d   ", j);textcolor(11);printf("|");textcolor(14);
                printf("         %.6d         ", buff.tab[j].matricule);textcolor(11);printf("|");
                if(buff.tab[j].eff)textcolor(12);else textcolor(14); printf("          %.1d          ", buff.tab[j].eff);textcolor(11);printf("|");textcolor(11);
                printf("                      <");textcolor(12);printf("%.2.d", buff.tab[j].Block);textcolor(11);printf(",");textcolor(12);printf(" %.4d", buff.tab[j].Enrg);textcolor(11);
                printf(">");textcolor(11);printf("                   |");
                if(cd){
                    if(count % 60 == 0){
                       count = 0;
                       textcolor(12);printf("\n\n\t\t\t\t");x = wherex();y = wherey();printf("> ");textcolor(11);printf("Continuer par morceaux.");
                       textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Continuer jusqu'a la fin.");
                       textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("> Quitter l'affichage.");
                       z = pointer(x - 1,y , 3, tab, 12, 11, 12, 10);
                       gotoxy(x, y + 4);
                       if(z == 1)cd = 0;
                       else if (z == 2) goto nextInd;
                    }
                    count++;
                }
        }
         textcolor(12);
         printf("\n                         +");textcolor(11);printf("---------");textcolor(12);printf("+");textcolor(11);printf("------------------------");textcolor(12);
         printf("+");textcolor(11);printf("---------------------");textcolor(12);printf("+");textcolor(11);printf("---------------------------------------------------");
         textcolor(12);printf("+\n");textcolor(15);

    }

    textcolor(12);printf("\n\n\t\t\t> ");textcolor(11);printf("Affichage termine");
    getch();
    nextInd:
    textcolor(15);
}

int SupEns()
{
     textcolor(15);
     clrscr();
     printf("\n________________________________________________________________________________________________________________________________________________________________________");
     textcolor(9);
     printf("\n                                     _____                                     _               __  __       _   ");
     printf("\n                                    / ____|                                   (_)             |  \\/  |     | |  ");
     printf("\n                                   | (___  _   _ _ __  _ __  _ __ ___  ___ ___ _  ___  _ __   | \\  / | __ _| |_ ");
     printf("\n                                    \\___ \\| | | | '_ \\| '_ \\| '__/ _ \\/ __/ __| |/ _ \\| '_ \\  | |\\/| |/ _` | __|");
     printf("\n                                    ____) | |_| | |_) | |_) | | |  __/\\__ \\__ \\ | (_) | | | | | |  | | (_| | |_ ");
     printf("\n                                   |_____/ \\__,_| .__/| .__/|_|  \\___||___/___/_|\\___/|_| |_| |_|  |_|\\__,_|\\__|");
     printf("\n                                                | |   | |                                                       ");
     printf("\n                                                |_|   |_|                                                       ");
     textcolor(15);
     printf("\n\n________________________________________________________________________________________________________________________________________________________________________");
   lines(n);
    int trouv, i, j, matricule;
    BlockIndex buff;
    Block buf;
    //Si le ficher n'est pas vide
    if(fInd.Entete.tab[1] > 0){
        adrsup:
        textcolor(12);printf("\n\n\t\t> ");textcolor(11);
        printf("Quelle matricule souhaitez vous supprimer : ");
        textcolor(12);scanf(" %d", &matricule);
        //On test si le matricule r�cuper� existe
        RechInd(fInd, matricule, &trouv, &i, &j);
        if(trouv)
        {
            //Lecture dans l'index pour r�cuperer les cordonn�es de l'enregistrement dans le fichier des enseignants
            LireDirInd(fInd.F, i, &buff);
            CountLE[2]++;
            //Supprimer l'enregistrement
            SupEngMat(&buf, buff.tab[j].Block, buff.tab[j].Enrg);
            //Supprimer dans l'index
            supInd( i, j);
            textcolor(12);printf("\n\t\t\t> ");textcolor(11);printf("Suppression terminee");
            //Bloquer l'�cran
            getch();
        }
        else{
            //Matricle n'existe pas dans l'index
            //... Suits des d��cisions � faire
            textcolor(12);
            printf("\n\t\t\tMatricule inexistant!!");
            if(i > fInd.Entete.tab[0])
            {
               LireDirInd(fInd.F, i - 1, &buff);
               CountLE[2]++;
               j = buff.Nb - 1;
            }
            else{
                LireDirInd(fInd.F, i, &buff);CountLE[2]++;}

            while(buff.tab[j].eff == 1 && j >= 0)
                j--;
            if(buff.tab[j].eff == 0){
                textcolor(11);
                printf("\n\t\tLe plus proche matricule trouve est : ");textcolor(12);printf("%d", buff.tab[j].matricule);
            }
            char tab[2][70] = {"> Continuer.", "> Quitter."};
            int z, x, y;
           textcolor(12);printf("\n\n\t\t\t");x = wherex();y = wherey();printf(">");textcolor(11);printf("Continuer.");
           textcolor(12);printf("\n\t\t\t>");textcolor(11);printf("Quitter.");
           z = pointer(x - 1,y , 2, tab, 12, 11, 12, 10);
           gotoxy(x, y + 3);
           if(z == 0)
            goto adrsup;

        }
    }
    else
    {
        textcolor(12);printf("\n\t\tFichier vide!");
    }

    return 1;
}

void Temp()
{
    //Affichage de la chaine BitMap
    textcolor(15);
    clrscr();
    printf("\n________________________________________________________________________________________________________________________________________________________________________");
    textcolor(9);
    printf("\n                                                                       ____  _ _   __  __                 ");
    printf("\n                                                                      |  _ \\(_) | |  \\/  |                ");
    printf("\n                                                                      | |_) |_| |_|\\ \\  / | __ _ _ __  ___ ");
    printf("\n                                                                      |  _ <| | __| |\\/| |/ _` | '_ \\/ __|");
    printf("\n                                                                      | |_) | | |_| |  | | (_| | |_) \\__ \\");
    printf("\n                                                                      |____/|_|\\__|_|  |_|\\__,_| .__/|___/");
    printf("\n                                                                                                 | |        ");
    printf("\n                                                                                                 |_|        ");
    textcolor(15);
    printf("\n\n________________________________________________________________________________________________________________________________________________________________________");
    lines(n);
    //Affichage
    Block buff;
    char tab2[10][70] = {"> Informatique.", "> Architecture.", "> Medcine.", "> Automatique.", "> Mathematiques.", "> Physique.", "> Commerce.", "> Geologie.", "> Chimie.", "> Geophysique."};
    int spec, x, y, z, x1, y1;
    char tabch[3][70] = {"> BitMaps des etablissements.", "> BitMaps des specialites.", "> BitMaps des regions."};
    char tabtemp[3][70] = {"> Est.", "> Centre.", "> Ouest."};
    textcolor(12);printf("\n\n\t\t\t> ");textcolor(11);printf("Quelle BitMap souhaitez vous l'afficher?");
    textcolor(12);printf("\n\t\t\t\t");x = wherex(); y = wherey();printf("> ");textcolor(11);printf(" BitMaps des etablissements.");
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf(" BitMaps des specialites.");
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf(" BitMaps des regions.");
    z = pointer(x - 1, y, 3, tabch, 12, 11, 12, 9);
    gotoxy(x, y + 3);
    switch(z)
    {
    case 0:
        //Affichage des �tablissements nationals avec leurs codes.
        DispEtablissements();
        adrBitmp0:
        textcolor(12);printf("\n\n\t\t\t> ");textcolor(11);printf("Veuillez vous saisir le code de l'etablissement qui correspond au BitMap a afficher : ");
        textcolor(12);scanf("%d", &spec);
        if(spec < 0 || spec > 111)
        {
            //Controle de saisie
            textcolor(12);
            printf("\n\t\t\t\tCode invalide!");
            goto adrBitmp0;
        }
        break;
    case 1:
            //Affichage
            clrscr();
            printf("\n________________________________________________________________________________________________________________________________________________________________________");
            textcolor(9);
            printf("\n                                                                       ____  _ _   __  __                 ");
            printf("\n                                                                      |  _ \\(_) | |  \\/  |                ");
            printf("\n                                                                      | |_) |_| |_|\\ \\  / | __ _ _ __  ___ ");
            printf("\n                                                                      |  _ <| | __| |\\/| |/ _` | '_ \\/ __|");
            printf("\n                                                                      | |_) | | |_| |  | | (_| | |_) \\__ \\");
            printf("\n                                                                      |____/|_|\\__|_|  |_|\\__,_| .__/|___/");
            printf("\n                                                                                                 | |        ");
            printf("\n                                                                                                 |_|        ");
            textcolor(15);
            printf("\n\n________________________________________________________________________________________________________________________________________________________________________");
            lines(n);
            textcolor(12);printf("\n\n\t\t\t> ");textcolor(11);printf("Specialite : ");
            textcolor(12);printf("\n\t\t\t\t");x = wherex(); x1 = x; y = wherey();y1 = y;printf("> ");textcolor(11);printf("Informatique.");
            textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Architecture.");
            textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Medcine.");
            textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Automatique.");
            textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Mathematiques.");
            textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Physique.");
            textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Commerce.");
            textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Geologie.");
            textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Chimie.");
            textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Geophysique.");
            spec = pointer(x - 1, y, 10, tab2, 12, 11, 12, 10);
            spec = spec + 111;
            gotoxy(x1, y1 + 11);
        break;
    case 2:
        //Affichage
        clrscr();
        printf("\n________________________________________________________________________________________________________________________________________________________________________");
        textcolor(9);
        printf("\n                                                                       ____  _ _   __  __                 ");
        printf("\n                                                                      |  _ \\(_) | |  \\/  |                ");
        printf("\n                                                                      | |_) |_| |_|\\ \\  / | __ _ _ __  ___ ");
        printf("\n                                                                      |  _ <| | __| |\\/| |/ _` | '_ \\/ __|");
        printf("\n                                                                      | |_) | | |_| |  | | (_| | |_) \\__ \\");
        printf("\n                                                                      |____/|_|\\__|_|  |_|\\__,_| .__/|___/");
        printf("\n                                                                                                 | |        ");
        printf("\n                                                                                                 |_|        ");
        textcolor(15);
        printf("\n\n________________________________________________________________________________________________________________________________________________________________________");
        lines(n);
        textcolor(12);printf("\n\n\t\t> ");textcolor(11);printf("Quelle region souhaitez vous afficher : ");
        textcolor(12);printf("\n\n\t\t\t\t");x = wherex(); y = wherey();printf("> ");textcolor(11);printf(" Est.");
        textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf(" Centre.");
        textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf(" Ouest.");
        spec = pointer(x - 1, y, 3, tabtemp, 12, 11, 12, 10);
        gotoxy(x, y + 4);
        spec = spec + 111 + NbSpecialites;
        break;
    default:
        break;
    }
    textcolor(11);
    printf("\n       ");textcolor(12);printf("+");textcolor(11);
    for(int o = 0; o < 150; o++)
        printf("-");

    textcolor(12);printf("+");textcolor(11);printf("\n          ");
    for(int i = 0; i < (f.Entete.tab[4] - 1)*b; ++i)
    {
        x = wherex();
        if(x > 10 && x < 157)
            if(Bitmaps[spec][i] == true){textcolor(12);
                printf("1");textcolor(11);}
            else
                printf("0");
        else{
            printf("\n          ");
            i--;
        }
    }
    printf("\n");
    textcolor(15);
    textcolor(12);printf("\n\n\t\t\t> ");textcolor(11);printf("Fin d'affichage.");
    getch();

}

int SupSpec()
{
     clrscr();
     textcolor(15);
     printf("\n________________________________________________________________________________________________________________________________________________________________________");
     textcolor(9);
     printf("\n                                       _____                                     _                _____                 ");
     printf("\n                                      / ____|                                   (_)              / ____|                ");
     printf("\n                                     | (___  _   _ _ __  _ __  _ __ ___  ___ ___ _  ___  _ __   | (___  _ __   ___  ___ ");
     printf("\n                                      \\___ \\| | | |  _ \\|  _ \\|  __/ _ \\/ __/ __| |/ _ \\| '_ \\   \\___ \\|  _ \\ / _ \\/ __|");
     printf("\n                                      ____) | |_| | |_) | |_) | | |  __/\\__ \\__ \\ | (_) | | | |  ____) | |_) |  __/ (__ ");
     printf("\n                                     |_____/ \\__,_|  __/|  __/|_|  \\___||___/___/_|\\___/|_| |_| |_____/|  __/ \\___|\\___|");
     printf("\n                                                  | |   | |                                            | |              ");
     printf("\n                                                  |_|   |_|                                            |_|              ");
     textcolor(15);
     printf("\n\n________________________________________________________________________________________________________________________________________________________________________");
     lines(n);
     Block buf;
     int trouv, i, j, i1, j1, k =0, spec,x,y;

     char tab2[10][70] = {"> Informatique.", "> Architecture.", "> Medcine.", "> Automatique.", "> Mathematiques.", "> Physique.", "> Commerce.", "> Geologie.", "> Chimie.", "> Geophysique."};
     printf("> \n\n\t\t\t");textcolor(12);textcolor(11);printf("Quelle specialite souhaitez vous supprimer : ");
     textcolor(12);printf("\n\n\t\t\t> ");textcolor(11);printf("Specialites : ");
     textcolor(12);printf("\n\t\t\t\t");x = wherex(); y = wherey();printf("> ");textcolor(11);printf("Informatique.");
     textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Architecture.");
     textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Medcine.");
     textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Automatique.");
     textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Mathematiques.");
     textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Physique.");
     textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Commerce.");
     textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Geologie.");
     textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Chimie.");
     textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Geophysique.");
     spec = pointer(x - 1, y, 10, tab2, 12, 11, 12, 10);
     gotoxy(x, y + 11);

     while( k < (f.Entete.tab[4] - 1)*b)
     {
         //Parcouire la chaine, si on trouve une cellule � vrai alors on extait les cordonn�es qui correspond k eme poid de la chaine
        if(Bitmaps[111 + spec][k] == true)
        {
             i = k / b + 1;//Les cordonn�es du block
             j = k % b; //Les cordonn�es de l'nregistrement
             //Lecture du ieme block ou il y a l'enrgistrement � supprimer
            LireDir(f.F, i, &buf);
            CountLE[0]++;
            if(!(j >= buf.Nb && i == f.Entete.tab[0]))
            {
                //On test si la cellule du bitmap est valide, puis on supprime l'nregistrement
                //r�cup�rer les cordon�es dans l'index
                RechInd(fInd, buf.tab[j].Matricule, &trouv, &i1, &j1);
                //Suppression dans l'index
                supInd(i1, j1);
                //Suppression dans le ficher
                SupEngMat(&buf, i, j);
                //Qu'on supprime un enregistrement il faut parcourire du nouveau la chaine, parce que la suppression
                //Modifie les chaine Bitmap
                k = -1;
            }
        }
        //Passer � la cellule suivante
        k++;

    }
     textcolor(12);printf("\n\n\t\t\t> ");textcolor(11);printf("Suppression terminee.");
     getch();
    return 1;
}

int AffichageParRegion(){
    //Affichage par regions
        clrscr();
        textcolor(15);
        printf("\n________________________________________________________________________________________________________________________________________________________________________");
        textcolor(9);
        printf("\n                                                           __  __ _      _                        _____           ");
        printf("\n                                                    /\\    / _|/ _(_)    | |                      |  __ \\          ");
        printf("\n                                                   /  \\  | |_| |_ _  ___| |__   __ _  __ _  ___  | |__) |___  __ _");
        printf("\n                                                  / /\\ \\ |  _|  _| |/ __| '_ \\ / _` |/ _` |/ _ \\ |  _  // _ \\/ _` |");
        printf("\n                                                 / ____ \\| | | | | | (__| | | | (_| | (_| |  __/ | | \\ \\  __/ (_| |");
        printf("\n                                                /_/    \\_\\_| |_| |_|\\___|_| |_|\\__,_|\\__, |\\___| |_|  \\_\\___|\\__, |");
        printf("\n                                                                                      __/ |                   __/ |");
        printf("\n                                                                                     |___/                   |___/ ");
        textcolor(15);
        printf("\n\n________________________________________________________________________________________________________________________________________________________________________");
        lines(n);
        Block buffer;
        char tab[3][70] = {"> Continuer par morceaux.", "> Continuer jusqu'a la fin.", "> Quitter l'affichage."};
        int trouv, i, j, i1, j1, reg, x, y, z, cd = 1, ct = 1, text, t = 14;
        char tabtemp[3][70] = {"> Est.", "> Centre.", "> Ouest."};
        textcolor(12);printf("\n\n\t\t> ");textcolor(11);printf("Quelle region souhaitez vous afficher : ");
        textcolor(12);printf("\n\n\t\t\t\t");x = wherex(); y = wherey();printf("> ");textcolor(11);printf(" Est.");
        textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf(" Centre.");
        textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf(" Ouest.");
        reg = pointer(x - 1, y, 3, tabtemp, 12, 11, 12, 10);
        gotoxy(x, y + 4);
        clrscr();
        textcolor(12);printf("+");textcolor(11);printf("----");textcolor(12);printf("+");textcolor(11);printf("--------");textcolor(12);
        printf("+");textcolor(11);printf("----------");textcolor(12);printf("+");textcolor(11);printf("----------");textcolor(12);
        printf("+");textcolor(11);printf("-----");textcolor(11);printf("+-----");textcolor(12);printf("+");textcolor(11);
        printf("------------------------------");textcolor(12);printf("+");textcolor(11);printf("--------------");textcolor(12);
        printf("+");textcolor(11);printf("----------------------------");textcolor(12);printf("+");textcolor(11);
        printf("------------");textcolor(12);printf("+");textcolor(11);printf("-------------------------");textcolor(12);
        printf("+");textcolor(11);printf("----");textcolor(12);printf("+");
        textcolor(11);printf("|");textcolor(12);printf(" Nu ");textcolor(11);printf("|");textcolor(12);printf("  Matr  ");textcolor(11);printf("|");textcolor(12);
        printf(" Date Rec ");textcolor(11);printf("|");textcolor(12);printf(" Date Nai ");textcolor(11);
        printf("|");textcolor(12);printf("Sexe ");textcolor(11);printf("|");textcolor(12);printf("Sang ");
        textcolor(11);printf("|");textcolor(12);printf("        Nom & Prenom          ");textcolor(11);
        printf("|");textcolor(12);printf("  Wilaya Nais ");textcolor(11);printf("|");textcolor(12);
        printf("           Grade            ");textcolor(11);printf("|");textcolor(12);printf(" Specialite ");
        textcolor(11);printf("|");textcolor(12);printf("     Dernier Diplome     ");textcolor(11);printf("|");textcolor(12);
        printf("Etab");textcolor(11);printf("|");
        textcolor(12);printf("+");textcolor(11);printf("----");textcolor(12);printf("+");textcolor(11);printf("--------");textcolor(12);
        printf("+");textcolor(11);printf("----------");textcolor(12);printf("+");textcolor(11);printf("----------");textcolor(12);
        printf("+");textcolor(11);printf("-----");textcolor(11);printf("+-----");textcolor(12);printf("+");textcolor(11);
        printf("------------------------------");textcolor(12);printf("+");textcolor(11);printf("--------------");textcolor(12);
        printf("+");textcolor(11);printf("----------------------------");textcolor(12);printf("+");textcolor(11);
        printf("------------");textcolor(12);printf("+");textcolor(11);printf("-------------------------");textcolor(12);
        printf("+");textcolor(11);printf("----");textcolor(12);printf("+");
        for(int k = 0; k < f.Entete.tab[5]; ++k)
            if(Bitmaps[reg + 111 + NbSpecialites][k] == true)
            {
                //Parcourire la chaine Bitmap qui corresond � la region donn�e, puis afficher l'nregistrement rematif � la cellule �
                //Chaque fois on trouv vrai
                 i = k / b + 1;//Cordonn�es du block
                 j = k % b;//Cordonn�es de l'enregistrement
                LireDir(f.F, i, &buffer);
                CountLE[0]++;
                //Variable utilis� que dans l'affichage du Nom et Pr�nom avec des d�ffirants couleurs
                text = 0;
                textcolor(11);printf("|");textcolor(t);printf("%.4d", j);textcolor(11);printf("|");textcolor(11);
                printf(" %.6d ", buffer.tab[j].Matricule);textcolor(11);printf("|");textcolor(t);printf("%.2d", buffer.tab[j].Date_recrutement.jj);textcolor(12);
                printf(".");textcolor(t);printf("%.2d", buffer.tab[j].Date_recrutement.mm);textcolor(12);printf(".");textcolor(t);
                printf("%.4d", buffer.tab[j].Date_recrutement.yyyy);textcolor(11);printf("|");textcolor(t);printf("%.2d", buffer.tab[j].Date_naissance.jj);textcolor(12);
                printf(".");textcolor(t);printf("%.2d", buffer.tab[j].Date_naissance.mm);textcolor(12);printf(".");textcolor(t);
                printf("%.4d", buffer.tab[j].Date_naissance.yyyy);textcolor(11);printf("|");textcolor(t);printf("%.5s", tabgender[buffer.tab[j].sexe]);textcolor(11);
                printf("|");textcolor(t);printf(" %.3s ", Groupe_Sanguin[buffer.tab[j].Groupe_sanguin]);textcolor(11);printf("|");textcolor(11);
                while(buffer.tab[j].Nom_Prenom[text] != '.'){printf("%c", buffer.tab[j].Nom_Prenom[text]);++text;}textcolor(12);printf("%c", buffer.tab[j].Nom_Prenom[text]);

                textcolor(t);printf("%s", &buffer.tab[j].Nom_Prenom[text] + 1);textcolor(11);printf("|");textcolor(t);printf("%s", tab_wilaya[buffer.tab[j].Wilaya_naissance]);textcolor(11);printf("|");
                textcolor(t);printf("%s", Grade[buffer.tab[j].Grade]);textcolor(11);printf("|");textcolor(t);printf("%s", specialites[buffer.tab[j].Specialite]);
                textcolor(11);printf("|");textcolor(t);printf("%s" , Der_Dip[buffer.tab[j].Dernier_diplome]);textcolor(11);printf("|");textcolor(t);
                printf("%.3d", buffer.tab[j].Etablissement_universitaire);textcolor(11);printf(" |");
                if(cd){
                    //cd utiliser pour indiquer si on continue l'affichage par morceaux , ou jusqu'a la fin directement
                    if(ct % 50 == 0){
                        ct = 1;
                        textcolor(12);printf("+");textcolor(11);printf("----");textcolor(12);printf("+");textcolor(11);printf("--------");textcolor(12);
                        printf("+");textcolor(11);printf("----------");textcolor(12);printf("+");textcolor(11);printf("----------");textcolor(12);
                        printf("+");textcolor(11);printf("-----");textcolor(12);printf("+");textcolor(11);printf("-----");textcolor(12);printf("+");textcolor(11);
                        printf("------------------------------");textcolor(12);printf("+");textcolor(11);printf("--------------");textcolor(12);
                        printf("+");textcolor(11);printf("----------------------------");textcolor(12);printf("+");textcolor(11);
                        printf("------------");textcolor(12);printf("+");textcolor(11);printf("-------------------------");textcolor(12);
                        printf("+");textcolor(11);printf("----");textcolor(12);printf("+");

                       textcolor(12);printf("\n\n\t\t\t\t");x = wherex();y = wherey();printf("> ");textcolor(11);printf("Continuer par morceaux.");
                       textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Continuer jusqu'a la fin.");
                       textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("> Quitter l'affichage.");
                       z = pointer(x - 1,y , 3, tab, 12, 11, 12, 10);
                       gotoxy(1, y + 4);
                        textcolor(12);printf("+");textcolor(11);printf("----");textcolor(12);printf("+");textcolor(11);printf("--------");textcolor(12);
                        printf("+");textcolor(11);printf("----------");textcolor(12);printf("+");textcolor(11);printf("----------");textcolor(12);
                        printf("+");textcolor(11);printf("-----");textcolor(11);printf("+-----");textcolor(12);printf("+");textcolor(11);
                        printf("------------------------------");textcolor(12);printf("+");textcolor(11);printf("--------------");textcolor(12);
                        printf("+");textcolor(11);printf("----------------------------");textcolor(12);printf("+");textcolor(11);
                        printf("------------");textcolor(12);printf("+");textcolor(11);printf("-------------------------");textcolor(12);
                        printf("+");textcolor(11);printf("----");textcolor(12);printf("+");
                       if(z == 1)cd = 0;
                       else if (z == 2) goto nextAffReg;
                    }
                    ct++;
                }
            }
        textcolor(12);printf("+");textcolor(11);printf("----");textcolor(12);printf("+");textcolor(11);printf("--------");textcolor(12);
        printf("+");textcolor(11);printf("----------");textcolor(12);printf("+");textcolor(11);printf("----------");textcolor(12);
        printf("+");textcolor(11);printf("-----");textcolor(11);printf("+-----");textcolor(12);printf("+");textcolor(11);
        printf("------------------------------");textcolor(12);printf("+");textcolor(11);printf("--------------");textcolor(12);
        printf("+");textcolor(11);printf("----------------------------");textcolor(12);printf("+");textcolor(11);
        printf("------------");textcolor(12);printf("+");textcolor(11);printf("-------------------------");textcolor(12);
        printf("+");textcolor(11);printf("----");textcolor(12);printf("+");
        textcolor(12);printf("\n\n\n\t\t> ");textcolor(11);printf("Affichage termine.");
        getch();
        nextAffReg:
        textbackground(0);
}

int ConsultationEtabAnc()
{
    clrscr();
    textcolor(15);
    printf("\n________________________________________________________________________________________________________________________________________________________________________");
    textcolor(9);
    printf("\n              _____                      _ _        _   _                                       _        _            _                      ");
    printf("\n             / ____|                    | | |      | | (_)                                     | |      | |          | |                     ");
    printf("\n            | |     ___  _ __  ___ _   _| | |_ __ _| |_ _  ___  _ __    _ __   __ _ _ __    ___| |_ __ _| |__     ___| |_    __ _ _ __   ___ ");
    printf("\n            | |    / _ \\| '_ \\/ __| | | | | __/ _` | __| |/ _ \\| '_ \\  | '_ \\ / _` | '__|  / _ \\ __/ _` | '_ \\   / _ \\ __|  / _` | '_ \\ / __|");
    printf("\n            | |___| (_) | | | \\__ \\ |_| | | || (_| | |_| | (_) | | | | | |_) | (_| | |    |  __/ || (_| | |_) | |  __/ |_  | (_| | | | | (__ ");
    printf("\n             \\_____\\___/|_| |_|___/\\__,_|_|\\__\\__,_|\\__|_|\\___/|_| |_| | .__/ \\__,_|_|     \\___|\\__\\__,_|_.__/   \\___|\\__|  \\__,_|_| |_|\\___|");
    printf("\n                                                                     | |                                                                   ");
    printf("\n                                                                     |_|                                                                   ");
    textcolor(15);
    printf("\n\n________________________________________________________________________________________________________________________________________________________________________");
    lines(n);
    textcolor(12);
    Block buffer;
    //Pour l'affichage
    int i, j, Anc1, Anc2, etab, x, y, z, cd = 1, ct = 1, text, t = 14;
    char tab[3][70] = {"> Continuer par morceaux.", "> Continuer jusqu'a la fin.", "> Quitter l'affichage."};
    //Affichage des �tablissements nationals
    DispEtablissements();
    adrCons1:
    //R�cup�ration du matricule et les anciennet�s avec les controlles de saisie.
    textcolor(12);printf("\n\n\t\t> ");textcolor(11);printf("Veuillez vous saisir le code de l'etablissement : ");textcolor(12);
    scanf(" %d", &etab);
    if(etab > 111 || etab < 0)
    {
        textcolor(12);
        printf("\n\n\t\t\tCode invalide!");textcolor(11);printf(" Les codes valides sont entre");textcolor(12);printf(" 0 ");textcolor(11);printf("et");textcolor(12);printf(" 111.\n\n");
        goto adrCons1;
    }
    adrCons2 :
    textcolor(12);printf("\n\t\t\t> ");textcolor(11);printf("La premiere anciennete : ");textcolor(12);
    scanf("%d", &Anc1);
    if(Anc1 < 0)
    {
        textcolor(12);
        printf("\t\t\t\tNombre invalide! l'anciennete est positive!");
        goto adrCons2;
    }
    adrCons3 :
    textcolor(12);printf("\n\t\t\t> ");textcolor(11);printf("La deuxieme anciennete");textcolor(12);printf(" (");textcolor(11);printf("> Anc1");textcolor(12);printf(") ");textcolor(11);printf(": ");textcolor(12);
    scanf("%d", &Anc2);
    if(Anc2 < Anc1)
    {
        textcolor(12);
        printf("\t\t\t\tLa deuxieme anciennete est superieure a la premiere anciennete");
        goto adrCons3 ;
    }
    clrscr();
    textcolor(12);printf("+");textcolor(11);printf("----");textcolor(12);printf("+");textcolor(11);printf("--------");textcolor(12);
    printf("+");textcolor(11);printf("----------");textcolor(12);printf("+");textcolor(11);printf("----------");textcolor(12);
    printf("+");textcolor(11);printf("-----");textcolor(11);printf("+-----");textcolor(12);printf("+");textcolor(11);
    printf("------------------------------");textcolor(12);printf("+");textcolor(11);printf("--------------");textcolor(12);
    printf("+");textcolor(11);printf("----------------------------");textcolor(12);printf("+");textcolor(11);
    printf("------------");textcolor(12);printf("+");textcolor(11);printf("-------------------------");textcolor(12);
    printf("+");textcolor(11);printf("----");textcolor(12);printf("+");
    textcolor(11);printf("|");textcolor(12);printf(" Nu ");textcolor(11);printf("|");textcolor(12);printf("  Matr  ");textcolor(11);printf("|");textcolor(12);
    printf(" Date Rec ");textcolor(11);printf("|");textcolor(12);printf(" Date Nai ");textcolor(11);
    printf("|");textcolor(12);printf("Sexe ");textcolor(11);printf("|");textcolor(12);printf("Sang ");
    textcolor(11);printf("|");textcolor(12);printf("        Nom & Prenom          ");textcolor(11);
    printf("|");textcolor(12);printf("  Wilaya Nais ");textcolor(11);printf("|");textcolor(12);
    printf("           Grade            ");textcolor(11);printf("|");textcolor(12);printf(" Specialite ");
    textcolor(11);printf("|");textcolor(12);printf("     Dernier Diplome     ");textcolor(11);printf("|");textcolor(12);
    printf("Etab");textcolor(11);printf("|");
    textcolor(12);printf("+");textcolor(11);printf("----");textcolor(12);printf("+");textcolor(11);printf("--------");textcolor(12);
    printf("+");textcolor(11);printf("----------");textcolor(12);printf("+");textcolor(11);printf("----------");textcolor(12);
    printf("+");textcolor(11);printf("-----");textcolor(11);printf("+-----");textcolor(12);printf("+");textcolor(11);
    printf("------------------------------");textcolor(12);printf("+");textcolor(11);printf("--------------");textcolor(12);
    printf("+");textcolor(11);printf("----------------------------");textcolor(12);printf("+");textcolor(11);
    printf("------------");textcolor(12);printf("+");textcolor(11);printf("-------------------------");textcolor(12);
    printf("+");textcolor(11);printf("----");textcolor(12);printf("+");
    for(int k = 0; k < (f.Entete.tab[4] - 1)*b; k++)
    {
        //Parcourire la chaine Bitmap correspondant � l'�tablissements et puis affichier les r�sultats
        i = k/b + 1;
        j = k%b;
        LireDir(f.F, i, &buffer);
        CountLE[0]++;
        if(!(i == f.Entete.tab[0] && j >= buffer.Nb))
            if(Bitmaps[etab][k] == true && (currentDt.yyyy - buffer.tab[j].Date_recrutement.yyyy) > Anc1 && (currentDt.yyyy - buffer.tab[j].Date_recrutement.yyyy) < Anc2)
            {
                text = 0;
                textcolor(11);printf("|");textcolor(t);printf("%.4d", j);textcolor(11);printf("|");textcolor(11);
                printf(" %.6d ", buffer.tab[j].Matricule);textcolor(11);printf("|");textcolor(t);printf("%.2d", buffer.tab[j].Date_recrutement.jj);textcolor(12);
                printf(".");textcolor(t);printf("%.2d", buffer.tab[j].Date_recrutement.mm);textcolor(12);printf(".");textcolor(t);
                printf("%.4d", buffer.tab[j].Date_recrutement.yyyy);textcolor(11);printf("|");textcolor(t);printf("%.2d", buffer.tab[j].Date_naissance.jj);textcolor(12);
                printf(".");textcolor(t);printf("%.2d", buffer.tab[j].Date_naissance.mm);textcolor(12);printf(".");textcolor(t);
                printf("%.4d", buffer.tab[j].Date_naissance.yyyy);textcolor(11);printf("|");textcolor(t);printf("%.5s", tabgender[buffer.tab[j].sexe]);textcolor(11);
                printf("|");textcolor(t);printf(" %.3s ", Groupe_Sanguin[buffer.tab[j].Groupe_sanguin]);textcolor(11);printf("|");textcolor(11);
                while(buffer.tab[j].Nom_Prenom[text] != '.'){printf("%c", buffer.tab[j].Nom_Prenom[text]);++text;}textcolor(12);printf("%c", buffer.tab[j].Nom_Prenom[text]);

                textcolor(t);printf("%s", &buffer.tab[j].Nom_Prenom[text] + 1);textcolor(11);printf("|");textcolor(t);printf("%s", tab_wilaya[buffer.tab[j].Wilaya_naissance]);textcolor(11);printf("|");
                textcolor(t);printf("%s", Grade[buffer.tab[j].Grade]);textcolor(11);printf("|");textcolor(t);printf("%s", specialites[buffer.tab[j].Specialite]);
                textcolor(11);printf("|");textcolor(t);printf("%s" , Der_Dip[buffer.tab[j].Dernier_diplome]);textcolor(11);printf("|");textcolor(t);
                printf("%.3d", buffer.tab[j].Etablissement_universitaire);textcolor(11);printf(" |");
                if(cd){
                    if(ct % 50 == 0){
                        ct = 1;
                        textcolor(12);printf("+");textcolor(11);printf("----");textcolor(12);printf("+");textcolor(11);printf("--------");textcolor(12);
                        printf("+");textcolor(11);printf("----------");textcolor(12);printf("+");textcolor(11);printf("----------");textcolor(12);
                        printf("+");textcolor(11);printf("-----");textcolor(12);printf("+");textcolor(11);printf("-----");textcolor(12);printf("+");textcolor(11);
                        printf("------------------------------");textcolor(12);printf("+");textcolor(11);printf("--------------");textcolor(12);
                        printf("+");textcolor(11);printf("----------------------------");textcolor(12);printf("+");textcolor(11);
                        printf("------------");textcolor(12);printf("+");textcolor(11);printf("-------------------------");textcolor(12);
                        printf("+");textcolor(11);printf("----");textcolor(12);printf("+");

                       textcolor(12);printf("\n\n\t\t\t\t");x = wherex();y = wherey();printf("> ");textcolor(11);printf("Continuer par morceaux.");
                       textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Continuer jusqu'a la fin.");
                       textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("> Quitter l'affichage.");
                       z = pointer(x - 1,y , 3, tab, 12, 11, 12, 10);
                       gotoxy(1, y + 4);
                        textcolor(12);printf("+");textcolor(11);printf("----");textcolor(12);printf("+");textcolor(11);printf("--------");textcolor(12);
                        printf("+");textcolor(11);printf("----------");textcolor(12);printf("+");textcolor(11);printf("----------");textcolor(12);
                        printf("+");textcolor(11);printf("-----");textcolor(11);printf("+-----");textcolor(12);printf("+");textcolor(11);
                        printf("------------------------------");textcolor(12);printf("+");textcolor(11);printf("--------------");textcolor(12);
                        printf("+");textcolor(11);printf("----------------------------");textcolor(12);printf("+");textcolor(11);
                        printf("------------");textcolor(12);printf("+");textcolor(11);printf("-------------------------");textcolor(12);
                        printf("+");textcolor(11);printf("----");textcolor(12);printf("+");
                       if(z == 1)cd = 0;
                       else if (z == 2) goto nextAffcons;
                    }
                    ct++;
                }
            }
    }
    textcolor(12);printf("+");textcolor(11);printf("----");textcolor(12);printf("+");textcolor(11);printf("--------");textcolor(12);
    printf("+");textcolor(11);printf("----------");textcolor(12);printf("+");textcolor(11);printf("----------");textcolor(12);
    printf("+");textcolor(11);printf("-----");textcolor(11);printf("+-----");textcolor(12);printf("+");textcolor(11);
    printf("------------------------------");textcolor(12);printf("+");textcolor(11);printf("--------------");textcolor(12);
    printf("+");textcolor(11);printf("----------------------------");textcolor(12);printf("+");textcolor(11);
    printf("------------");textcolor(12);printf("+");textcolor(11);printf("-------------------------");textcolor(12);
    printf("+");textcolor(11);printf("----");textcolor(12);printf("+");
    textcolor(12);printf("\n\n\t\t\t\t> ");textcolor(11);printf("Affichage termine.");
    getch();
    nextAffcons:
    textbackground(0);
}

void DispWilaya()
{
    //Affichage des Wilayas
     textcolor(15);
     printf("\n________________________________________________________________________________________________________________________________________________________________________\n");
     textcolor(10);
     printf("\n                                                         __          ___ _  ");
     printf("\n                                                         \\ \\        / (_) | ");
     printf("\n                                                          \\ \\  /\\  / / _| | __ _ _   _  __ _ ___ ");
     printf("\n                                                           \\ \\/  \\/ / | | |/ _` | | | |/ _` / __|");
     printf("\n                                                            \\  /\\  /  | | | |_| | |_| | |_| \\__ \\");
     printf("\n                                                             \\/  \\/   |_|_|\\__,_| __, |\\__,_|___/");
     printf("\n                                                                                  __/ |          ");
     printf("\n                                                                                 |___/           ");
     textcolor(15);
     printf("\n________________________________________________________________________________________________________________________________________________________________________\n");

     int x = wherex() + 40, y , y1= wherey() + 1;
     textcolor(10);printf("\n|                        .////////");textcolor(15); printf("///////////            |");
     textcolor(10);printf("\n|                    *////////////");textcolor(15); printf("////////////           |");
     textcolor(10); printf("\n|                  ///////////////");textcolor(15); printf("////////////           |");
     textcolor(10);printf("\n|                   //////////////");textcolor(15); printf("//////////             |");
     textcolor(10);printf("\n|                   //////////////");textcolor(15); printf("//////////             |");
     textcolor(10);printf("\n|                    /////////////");textcolor(15); printf("////////////           |");
     textcolor(10);printf("\n|              ///////////////////");textcolor(15); printf("//////////////         |");
     textcolor(10);printf("\n|              ///////////////////");textcolor(15); printf("///////////////        |");
     textcolor(10);printf("\n|          ///////////////////////");textcolor(15); printf("///////////////        |");
     textcolor(10);printf("\n|  ,//////////////////////////////");textcolor(15); printf("////////////////       |");
     textcolor(10);printf("\n|/////////////////////////////////");textcolor(15); printf("////////////////       |");
     textcolor(10);printf("\n| /////////////////////////////////");textcolor(15); printf("////////////////      |");
     textcolor(10);printf("\n|    //////////////////////////////");textcolor(15); printf("////////////////      |");
     textcolor(10);printf("\n|       ///////////////////////////");textcolor(15); printf("///////////////       |");
     textcolor(10);printf("\n|          ///////////////////////");textcolor(15); printf("////////////////       |");
     textcolor(10);printf("\n|              ///////////////////");textcolor(15); printf("/////////////////////  |");
     textcolor(10); printf("\n|                 ////////////////");textcolor(15); printf("////////////////////   |");
     textcolor(10);printf("\n|                       ////////////");textcolor(15); printf("/////////////////    |");
     textcolor(10);printf("\n|                        ,////////");textcolor(15); printf("/////////////          |");
     textcolor(10);printf("\n|                           //////");textcolor(15); printf("/////////              |");
     textcolor(10);printf("\n|                               //");textcolor(15); printf("//////                 |\n");
     printf("\n________________________________________________________________________________________________________________________________________________________________________\n");

     for(int i = 0; i < 58; i++)
     {
         if(i % 21 == 0)
         {
             y = y1;
             x= x + 25;
         }
         gotoxy(x, y);
         y = y + 1;
         textcolor(10);
         printf("%.2d :", i + 1);
         if(i < 48)
            textcolor(15);
         else
            textcolor(12);

         printf(" %s", tab_wilaya[i]);
         printf("\n");
     }
}

void Q41()
{
    //Insertion d'un enregistrement dans le ficheir.
    SEns e;
    char prenom[30];
    int trouv = 0, i, j;
    clrscr();
    printf("\n________________________________________________________________________________________________________________________________________________________________________");
    textcolor(9);
    printf("\n                                                              _____                     _   _               ______             ");
    printf("\n                                                             |_   _|                   | | (_)             |  ____|            ");
    printf("\n                                                               | |  _ __  ___  ___ _ __| |_ _  ___  _ __   | |__   _ __   __ _ ");
    printf("\n                                                               | | | '_ \\/ __|/ _ \\ '__| __| |/ _ \\| '_ \\  |  __| | '_ \\ / _` |");
    printf("\n                                                              _| |_| | | \\__ \\  __/ |  | |_| | (_) | | | | | |____| | | | (_| |");
    printf("\n                                                             |_____|_| |_|___/\\___|_|   \\__|_|\\___/|_| |_| |______|_| |_|\\__, |");
    printf("\n                                                                                                                          __/ |");
    printf("\n                                                                                                                         |___/ ");
    textcolor(15);
    printf("\n\n________________________________________________________________________________________________________________________________________________________________________");
    lines(n);
    textcolor(12);
    printf("\n\n          > ");textcolor(11);printf("Veuillez vous introduire : \n\t\t\t");
    //R�cup�ration d'un matricule valide.
    do
    {
        textcolor(12);printf("> ");textcolor(11);printf("Matricule :");textcolor(12);
        scanf(" %d", &e.Matricule);
        RechInd(fInd, e.Matricule, &trouv, &i, &j);
        if(trouv)
            printf("\n\t\t\t\tMatricule existant!");
    }
    while(trouv);
    //R�cup�ration du nom et pr�nom
    textcolor(12);printf("\n\t\t\t> ");textcolor(11);printf("Nom et prenom :");textcolor(12);
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Nom :");textcolor(12);
    scanf("%s", e.Nom_Prenom);
    strcat(e.Nom_Prenom, ".");
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Prenom :");textcolor(12);
    scanf("%s", prenom);
    strcat(e.Nom_Prenom, prenom);
    for(int l = strlen(e.Nom_Prenom); l < 30; l++)
        e.Nom_Prenom[l] = ' ';//Justification du texte, pour l'affichage
    e.Nom_Prenom[30] = '\0';
    //R�cup�ration de la date de naissance
    printf("\n\t\t\t> ");textcolor(11);printf("La date d naissance :");
    adr0ins:
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);
    printf("yyyy (");textcolor(12);printf("1958");textcolor(11);printf(" _ ");textcolor(12);printf("1998");textcolor(11);printf(" ): ");textcolor(12);
    scanf(" %d", &e.Date_naissance.yyyy);
    //Ann�es valide?
    if(e.Date_naissance.yyyy > 1998 || e.Date_naissance.yyyy < 1958)
    {
        textcolor(12);printf("\n\t\t\t\tDate de naissance invalide! Veuillez vous inserer une date entre"); textcolor(12);printf("1958");textcolor(11);printf(" et ");textcolor(12);printf("1998");
        goto adr0ins;
    }
    adr1ins:
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);
    printf("mm : ");textcolor(12);
    scanf(" %d", &e.Date_naissance.mm);
    //Mois valide?
    if(e.Date_naissance.mm > 12 || e.Date_naissance.mm < 1)
    {
        textcolor(12);printf("\n\t\t\t\tMois invalide! veuillez saisir un mois entre"); textcolor(12);printf("1");textcolor(11);printf(" et ");textcolor(12);printf("12");
        goto adr1ins;
    }
    adr2ins:
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);
    printf("jj: ");textcolor(12);
    scanf(" %d", &e.Date_naissance.jj);
     //Jour valide?
    if(e.Date_naissance.mm == 2 && (e.Date_naissance.jj < 1 || e.Date_naissance.jj > 28 + AnBissextile(e.Date_naissance.yyyy))){
        textcolor(12);
        printf("\n\t\t\t\tInvalid jour! Veuillez vous le saisez encore une foix.");goto adr2ins;}
    else if(((e.Date_naissance.mm > 5 && e.Date_naissance.mm % 2 == 0 )||(e.Date_naissance.mm < 6&& e.Date_naissance.mm % 2 == 1))&& (e.Date_naissance.jj < 1 || e.Date_naissance.jj > 31)){
        textcolor(12);
        printf("\n\t\t\t\tInvalid jour! Veuillez vous le saisez encore une foix.");goto adr2ins;}
    else if(((e.Date_naissance.mm > 5&&e.Date_naissance.mm % 2 == 1)||(e.Date_naissance.mm < 6&&e.Date_naissance.mm % 2 == 0))&& (e.Date_naissance.jj < 1 || e.Date_naissance.jj > 30)){
        textcolor(12);
        printf("\n\t\t\t\tInvalid jour! Veuillez vous le saisez encore une foix.");goto adr2ins;}


    DispWilaya();
    adr01ins:
    //Wilaya de naissance
    printf("\n\n\n\n\n\n\n\n\t\t\t> ");textcolor(11);printf("Wilaya de naissance :");textcolor(12);
    scanf(" %d", &e.Wilaya_naissance);
    if(e.Wilaya_naissance > 58 || e.Wilaya_naissance < 1)
    {
        textcolor(12);printf("\n\t\t\t\tCode wilaya de naissance invalide! Veuillez vous inserer une date entre"); textcolor(12);printf("1");textcolor(11);printf(" et ");textcolor(12);printf("58");
        goto adr01ins;
    }
    e.Wilaya_naissance--;
    int x, y, x1, y1;
    //Sexe
    char Tab[2][70] = {"> Homme.         ", "> Femme.         "};
    printf("\n\t\t\t> ");textcolor(11);printf("Sexe :");textcolor(12);
    printf("\n\t\t\t\t");x = wherex(); x1 = x; y = wherey(); y1 = y;printf("> ");textcolor(11);printf("Homme.         ");textcolor(12);
    printf("\n\t\t\t\t> ");textcolor(11);printf("Femme.         ");textcolor(12);
    e.sexe = pointer(x - 1, y, 2, Tab, 12, 11, 12, 10);
    gotoxy(x1, y1 + 3);
    char TabGp[8][70] = {"> O+ ", "> A+ ", "> B+ ", "> O- ", "> A- ", "> AB+", "> B- ", "> AB-"};
    printf("\n\t\t\t> ");textcolor(11);printf("Groupe sanguin :");textcolor(12);

    //Groupe sanguin
    printf("\n\t\t\t\t");x = wherex(); x1 = x; y = wherey(); y1 = y;printf("> ");textcolor(11);printf(" O+ ");textcolor(12);
    printf("\n\t\t\t\t> ");textcolor(11);printf(" A+ ");textcolor(12);
    printf("\n\t\t\t\t> ");textcolor(11);printf(" B+ ");textcolor(12);
    printf("\n\t\t\t\t> ");textcolor(11);printf(" O-");textcolor(12);
    printf("\n\t\t\t\t> ");textcolor(11);printf(" A-");textcolor(12);
    printf("\n\t\t\t\t> ");textcolor(11);printf(" AB+");textcolor(12);
    printf("\n\t\t\t\t> ");textcolor(11);printf(" B-");textcolor(12);
    printf("\n\t\t\t\t> ");textcolor(11);printf(" AB-");textcolor(12);
    e.Groupe_sanguin = pointer(x - 1, y, 8, TabGp, 12, 11, 12, 10);

    gotoxy(x1, y1 + 9);

    //Ann�e de recrutement avec les testes de validit�
    textcolor(12);printf("\n\t\t\t> ");textcolor(11);printf("La date de recrutemment : ");
    Adr0Q41 :
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);
    printf("yyyy (");textcolor(12);printf("1985");textcolor(11);printf(" _ ");textcolor(12);printf("2022");textcolor(11);printf(" ): ");textcolor(12);
    scanf(" %d", &e.Date_recrutement.yyyy);
    if(e.Date_recrutement.yyyy > 2022 || e.Date_recrutement.yyyy < 1985)
    {
        textcolor(12);printf("\n\t\t\t\tDate de recrutement invalide! Veuillez vous inserer une date entre"); textcolor(12);printf("1985");textcolor(11);printf(" et ");textcolor(12);printf("2022");
        goto Adr0Q41;
    }
    if(e.Date_recrutement.yyyy - e.Date_naissance.yyyy < 20){
        printf("\nInvalide date de recrutement, il faut avoire au moins 20 ans.");
        goto Adr0Q41;
    }
    adr1Q41:
    textcolor(12);printf("\n\t\t\t\t>");textcolor(11);
    printf("mm : ");textcolor(12);
    scanf(" %d", &e.Date_recrutement.mm);
    if(e.Date_recrutement.mm > 12 || e.Date_recrutement.mm < 1)
    {
        textcolor(12);printf("\n\t\t\t\tMois invalide! veuillez saisir un mois entre"); textcolor(12);printf("1");textcolor(11);printf(" et ");textcolor(12);printf("12");
        goto adr1Q41;
    }
    adr2Q41:
    textcolor(12);printf("\n\t\t\t\t>");textcolor(11);
    printf("jj: ");textcolor(12);
    scanf(" %d", &e.Date_recrutement.jj);

    //gendate.jj = rand()%32 + 1;
    if(e.Date_recrutement.mm == 2&& (e.Date_recrutement.jj < 1 || e.Date_recrutement.jj > 28 + AnBissextile(e.Date_recrutement.yyyy))){
        textcolor(12);
        printf("\n\t\t\t\tInvalid jour! Veuillez vous le saisez encore une foix.");goto adr2Q41;}
    else if(((e.Date_recrutement.mm > 5&&e.Date_recrutement.mm % 2 == 0)||(e.Date_recrutement.mm < 6&&e.Date_recrutement.mm % 2 == 1))&& (e.Date_recrutement.jj < 1 || e.Date_recrutement.jj > 31)){
        textcolor(12);
        printf("\n\t\t\t\tInvalid jour! Veuillez vous le saisez encore une foix.");goto adr2Q41;}
    else if(((e.Date_recrutement.mm > 5&&e.Date_recrutement.mm % 2 == 1)||(e.Date_recrutement.mm < 6&&e.Date_recrutement.mm % 2 == 0))&& (e.Date_recrutement.jj < 1 || e.Date_recrutement.jj > 30)){
        textcolor(12);
        printf("\n\t\t\t\tInvalid jour! Veuillez vous le saisez encore une foix.");goto adr2Q41;}

    DispEtablissements();
    //L'�tablissement universitaire
    textcolor(12);printf("\n\n\t\t\t> ");textcolor(11);printf("Etablissement universitaire: ");textcolor(12);
    scanf(" %d", &e.Etablissement_universitaire);
    if(e.Etablissement_universitaire > 111 || e.Etablissement_universitaire < 0)
    {
        textcolor(12);
        printf("Code invalide! Veuillez vous introduire un autre code entre ");textcolor(12);printf("0");textcolor(11);printf(" et ");textcolor(12);printf("111");
    }

    //Grade
    char tabGrade[6][70] = {"> Professeur(e).", "> Maitre de conferances Rang A.", "> Maitre de conferance Rang B.", "> Maitre assistant(e) Rang A.", "> Maitre assistant(e) Rang B.", "> Assistant(e)."};
    textcolor(12);printf("\n\n\t\t\t> ");textcolor(11);printf("Grade : ");
    textcolor(12);printf("\n\t\t\t\t");x = wherex();x1 = x; y = wherey(); y1 = y;printf("> ");textcolor(11);printf("Professeur(e).");
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Maitre de conferances Rang A.");
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Maitre de conferance Rang B.");
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Maitre assistant(e) Rang A.");
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Maitre assistant(e) Rang B.");
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Assistant(e).");
    e.Grade = pointer(x - 1, y, 6, tabGrade, 12, 11, 12, 10);
    gotoxy(x1, y1 + 7);
    //Sp�cialit�
    char tab2[10][70] = {"> Informatique.", "> Architecture.", "> Medcine.", "> Genie Electrique.", "> Les Mathematiques.", "> La Physique.", "> Amenagement du Territoire.", "> Geologie.", "> La Chimie.", "> Geophysique."};
    textcolor(12);printf("\n\n\t\t\t> ");textcolor(11);printf("Specialite : ");
    textcolor(12);printf("\n\t\t\t\t");x = wherex(); x1 = x; y = wherey();y1 = y;printf("> ");textcolor(11);printf("Informatique.");
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Architecture.");
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Medcine.");
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Genie Electrique.");
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Les Mathematiques.");
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("La Physique.");
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Amenagement du Territoire.");
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Geologie.");
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("La Chimie.");
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Geophysique.");
    e.Specialite = pointer(x - 1, y, 10, tab2, 12, 11, 12, 10);
    gotoxy(x1, y1 + 11);

    //Dernier diplome
    char tabDer_Dip[6][70] = {"> Doctorant d'etat.", "> Doctorant en sciences.", "> Doctorant D-LMD.", "> PhD.", "> Habillation Universitaire.", "> Magistere."};
    textcolor(12);printf("\n\n\t\t\t> ");textcolor(11);printf("Dernier diplome : ");
    textcolor(12);printf("\n\t\t\t\t");x = wherex();x1 = x; y = wherey();y1 = y;printf("> ");textcolor(11);printf("Doctorant d'etat.");
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Doctorant en sciences.");
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Doctorant D-LMD.");
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("PhD.");
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Habillation Universitaire.");
    textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Magistere.");
    e.Dernier_diplome = pointer(x - 1, y, 6, tabDer_Dip, 12, 11, 12, 10);
    gotoxy(x1, y1 + 8);
     //Insertion de l'nregistrement r�cup�r� dans le fichier
    InsErgMat(e);
    textcolor(12);printf("\n\n\n\t\t\t\t\t\t> ");textcolor(11),printf("Insertion terminee");
    getch();
}
void DispEng(SEns e, int i, int j)
{
    //Affichage d'un seul enregistrement
                int text = 0;
                textcolor(12);printf("\n                           +");textcolor(11);printf("------------");textcolor(12);printf("+");
                textcolor(12);printf("\n                           |");textcolor(11);printf(" <");textcolor(12);printf("%.2d", i);textcolor(11);
                printf(" ,");textcolor(12);printf("%.4d", j);textcolor(11);printf("> |");
                textcolor(12);printf("\n                           +");textcolor(11);printf("---------------------------------------------------------------------------------------------------");textcolor(12);printf("+");
                textcolor(11);printf("\n                           Date de naissance           : ");textcolor(14);printf("%d", e.Date_naissance.jj );
                printf(".");textcolor(14);printf("%d", e.Date_naissance.mm );
                printf(".");textcolor(14);printf("%d", e.Date_naissance.yyyy );
                textcolor(12);printf("\n                           +");textcolor(11);printf("---------------------------------------------------------------------------------------------------");textcolor(12);printf("+");
                textcolor(11);printf("\n                           Date de recrutement         : ");textcolor(14);printf("%d", e.Date_recrutement.jj );textcolor(12);
                printf(".");textcolor(14);printf("%d", e.Date_recrutement.mm );textcolor(12);
                printf(".");textcolor(14);printf("%d", e.Date_recrutement.yyyy );
                textcolor(12);printf("\n                           +");textcolor(11);printf("---------------------------------------------------------------------------------------------------");textcolor(12);printf("+");
                textcolor(11);printf("\n                           Nom & Prenom                : ");textcolor(11);while(e.Nom_Prenom[text] != '.'){printf("%c", e.Nom_Prenom[text]);++text;}textcolor(12);printf("%c", e.Nom_Prenom[text]);
                //textcolor(14);do{text++;printf("%c", e.Nom_Prenom[text]);}while(text < strlen(e.Nom_Prenom));
                textcolor(14);printf("%s", &e.Nom_Prenom[text] + 1);
                textcolor(12);printf("\n                           +");textcolor(11);printf("---------------------------------------------------------------------------------------------------");textcolor(12);printf("+");
                textcolor(11);printf("\n                           Dernier diplome             : ");textcolor(14);printf("%s", Der_Dip[e.Dernier_diplome]);
                textcolor(12);printf("\n                           +");textcolor(11);printf("---------------------------------------------------------------------------------------------------");textcolor(12);printf("+");
                textcolor(11);printf("\n                           Etablissement_universitaire : ");textcolor(14);printf("%s", tab_Etablissement[e.Etablissement_universitaire] );
                textcolor(12);printf("                           +");textcolor(11);printf("---------------------------------------------------------------------------------------------------");textcolor(12);printf("+");
                textcolor(11);printf("\n                           Grade                       : ");textcolor(14);printf("%s", Grade[e.Grade] );
                textcolor(12);printf("\n                           +");textcolor(11);printf("---------------------------------------------------------------------------------------------------");textcolor(12);printf("+");
                textcolor(11);printf("\n                           Groupe_sanguin              : ");textcolor(14);printf("%s", Groupe_Sanguin[e.Groupe_sanguin] );
                textcolor(12);printf("\n                           +");textcolor(11);printf("---------------------------------------------------------------------------------------------------");textcolor(12);printf("+");
                textcolor(11);printf("\n                           Matricule                   : ");textcolor(11);printf("%d", e.Matricule );
                textcolor(12);printf("\n                           +");textcolor(11);printf("---------------------------------------------------------------------------------------------------");textcolor(12);printf("+");
                textcolor(11);printf("\n                           sexe                        : ");textcolor(14);printf("%s", tabgender[e.sexe]);
                textcolor(12);printf("\n                           +");textcolor(11);printf("---------------------------------------------------------------------------------------------------");textcolor(12);printf("+");
                textcolor(11);printf("\n                           Specialite                  : ");textcolor(14);printf("%s", specialites[e.Specialite]);
                textcolor(12);printf("\n                           +");textcolor(11);printf("---------------------------------------------------------------------------------------------------");textcolor(12);printf("+");
                textcolor(11);printf("\n                           Wilaya_naissance            :");textcolor(14);printf(" %s", tab_wilaya[e.Wilaya_naissance]);
                textcolor(12);printf("\n                           +");textcolor(11);printf("---------------------------------------------------------------------------------------------------");textcolor(12);printf("+\n");
                textcolor(15);
                printf("\n\n\n_____________________________________________________________________________");textcolor(10);printf(" FIN");textcolor(15);printf(" ______________________________________________________________________________________");
                lines(n);
                getch();
}

void AffichageUnEng()
{
     //Affichage d'un seul enregistrement plus �volu�
     clrscr();
     textcolor(15);
     printf("\n________________________________________________________________________________________________________________________________________________________________________");
     textcolor(9);
     printf("\n                                                                _____       _        _ _     ");
     printf("\n                                                               |  __ \\     | |      (_) |    ");
     printf("\n                                                               | |  | | ___| |_ __ _ _| |___ ");
     printf("\n                                                               | |  | |/ _ \\ __/ _` | | / __|");
     printf("\n                                                               | |__| |  __/ || (_| | | \\__ \\");
     printf("\n                                                               |_____/ \\___|\\__\\__,_|_|_|___/");
     textcolor(15);
     printf("\n\n________________________________________________________________________________________________________________________________________________________________________");
    lines(n);
    int i, j, trouv, matricule, x, y, z, ct, tmp;
    Block buff;
    BlockIndex buff0;
    char tab0[2][70] = {"> Continuer.", "> Quitter."};
    char tab[3][70] = {"> <i, j>", "> Matricule", "> Quitter"};
    if(f.Entete.tab[0] == 0){
       textcolor(12);
        printf("\n\n\t\t\tFichier vide!");}
    else{
        textcolor(12);printf("\n\n\t\t> ");textcolor(11);printf("Veuillez vous chercher par matricule ou par emplacement ");textcolor(12);printf("<");
        textcolor(11);printf("i");textcolor(12);printf(",");textcolor(11);printf(" j");textcolor(12);printf(">");
        printf("\n\t\t\t");x = wherex(); y = wherey();printf("> ");textcolor(11);printf("<i, j>");
        textcolor(12);printf("\n\t\t\t>");textcolor(11);printf(" <Matricule>");
        textcolor(12);printf("\n\t\t\t>");textcolor(11);printf(" Quitter.");
        choice = pointer(x - 1, y, 3, tab, 12, 11, 12, 9);
        gotoxy(x, y + 4);
        if(choice == 0)
        {
            //Affichage par cordonn�es
            adr0AffichageUnEng :
            textcolor(12);printf("\n\t\t> ");textcolor(11);printf("i : ");
            scanf("%d", &i);
            if(i >= f.Entete.tab[4] || i < 1){
                textcolor(12);
                printf("\n\t\t\tInvalide valeur pour i !");
                goto adr0AffichageUnEng;
            }
            LireDir(f.F, i, &buff);
            CountLE[0]++;
            adr1AffichageUnEng:
            textcolor(12);printf("\n\t\t> ");textcolor(11);printf("j : ");
            scanf(" %d", &j);
            if(i == f.Entete.tab[0] && j >= buff.Nb || j > buff.Nb)
            {
                textcolor(12);
                printf("\n\t\t\tInvalide valeur pour j");
                goto adr1AffichageUnEng;
            }
            DispEng(buff.tab[j], i, j);
        }
        else if(choice == 1)
        {
            //Affichage par matricule
            adr3:
            textcolor(12);printf("\n\t\t> ");textcolor(11);printf("Matricule : ");
            scanf(" %d", &matricule);
            RechInd(fInd, matricule, &trouv, &i, &j);
            if(trouv)
            {
                LireDirInd(fInd.F, i, &buff0);
                CountLE[2]++;
                LireDir(f.F, buff0.tab[j].Block, &buff);
                CountLE[0]++;
                DispEng(buff.tab[buff0.tab[j].Enrg], buff0.tab[j].Block, buff0.tab[j].Enrg);

            }
            else{
                    printf("\n\tMatricule inexistant!");
                    textcolor(12);
                    printf("\n\t\t\tMatricule inexistant!!");
                    if(i > fInd.Entete.tab[0])
                    {
                           LireDirInd(fInd.F, i - 1, &buff0);
                           CountLE[2]++;
                           j = buff0.Nb - 1;
                    }
                    else{
                            LireDirInd(fInd.F, i, &buff0);CountLE[2]++;}
                            ct = j;
                            while(buff0.tab[j].eff == 1 && j > 0)
                                j--;
                            tmp = buff0.tab[j].matricule*10 + buff0.tab[j].eff;
                            j = ct;
                            while(buff0.tab[j].eff == 1 && j < buff0.Nb - 1)
                                j++;

                            if(tmp/10 <  buff0.tab[j].matricule)
                                tmp = buff0.tab[j].matricule*10 + buff0.tab[j].eff;
                            if(tmp % 10 == 0){
                                textcolor(11);
                                printf("\n\t\tLe plus proche matricule trouve est : ");textcolor(12);printf("%d", tmp/10);
                               }
                           textcolor(12);printf("\n\n\t\t\t");x = wherex();y = wherey();printf(">");textcolor(11);printf("Continuer.");
                           textcolor(12);printf("\n\t\t\t>");textcolor(11);printf("Quitter.");
                           z = pointer(x - 1,y , 2, tab0, 12, 11, 12, 10);
                           gotoxy(x, y + 3);
                           if(z == 0)
                              goto adr3;
            }
        }
    }

}

void ModificationEns()
{
      int matricule, Etab, trouv, i, j, tmp, ct;
      Block buff;
      BlockIndex buff0;
      clrscr();
      printf("\n________________________________________________________________________________________________________________________________________________________________________");
      textcolor(9);
      printf("\n                                             __  __           _ _  __ _           _   _               ______ _   _     ");
      printf("\n                                            |  \\/  |         | (_)/ _(_)         | | (_)             |  ____| | | |    ");
      printf("\n                                            | \\  / | ___   __| |_| |_ _  ___ __ _| |_ _  ___  _ __   | |__  | |_| |__  ");
      printf("\n                                            | |\\/| |/ _ \\ / _` | |  _| |/ __/ _` | __| |/ _ \\| '_ \\  |  __| | __| '_ \\ ");
      printf("\n                                            | |  | | (_) | (_| | | | | | (_| (_| | |_| | (_) | | | | | |____| |_| |_) |");
      printf("\n                                            |_|  |_|\\___/ \\__,_|_|_| |_|\\___\\__,_|\\__|_|\\___/|_| |_| |______|\\__|_.__/ ");
      textcolor(15);
      printf("\n\n________________________________________________________________________________________________________________________________________________________________________");
      lines(n);
      //Si le ficheir n'est pas vide on cherche l'enregistrement � modifier
      if(fInd.Entete.tab[1] > 0){
            adrmod:
            textcolor(12);printf("\n\n\t\t> ");textcolor(11);printf("Veuillez vous introduire le matricule de l'enseignant : ");textcolor(12);
            scanf("%d", &matricule);
            //Recherche dans l'index
            RechInd(fInd, matricule, &trouv, &i, &j);
            if(trouv)
            {
                //Si le matricule � modifier existe dans le fichier
                    LireDirInd(fInd.F, i, &buff0);
                    CountLE[2]++;
                    DispEtablissements();
                    adrModifEtab:
                    printf("\n\n\t\t> ");textcolor(11);printf("Veuillez vous saisir le code de nouvelle etablissement :"); textcolor(12);
                    scanf("%d" ,&Etab);
                    if(Etab < 0 || Etab > 111)
                    {
                        textcolor(12);printf("\n\n\t\t\tCode Invalide!");
                        goto adrModifEtab;
                    }
                    LireDir(f.F, buff0.tab[j].Block, &buff);
                    CountLE[0]++;
                    //Modification dans les bitmaps des �tablissements
                    Bitmaps[buff.tab[buff0.tab[j].Enrg].Etablissement_universitaire][(buff0.tab[j].Block - 1)*b + buff0.tab[j].Enrg] = false;
                    //Modification de l'�tablissement
                    buff.tab[buff0.tab[j].Enrg].Etablissement_universitaire = Etab;
                    Bitmaps[buff.tab[buff0.tab[j].Enrg].Etablissement_universitaire][(buff0.tab[j].Block - 1)*b + buff0.tab[j].Enrg] = true;
                    EcrireDir(f.F, buff0.tab[j].Block, buff);
                    CountLE[1]++;
                    textcolor(12);printf("\n\n\t\t\t> ");
                    textcolor(11);
                    printf("Modification terminee");
                    getch();
            }
            else{
                    textcolor(12);
                    printf("\n\t\t\tMatricule inexistant!!");
                    if(i > fInd.Entete.tab[0])
                    {
                       LireDirInd(fInd.F, i - 1, &buff0);
                       CountLE[2]++;
                       j = buff0.Nb - 1;
                    }
                    else{
                        LireDirInd(fInd.F, i, &buff0);CountLE[2]++;}
                    //R�cup�ration de plus proche matricule dans le meme block si il existe
                    ct = j;//Sauvgarder l'enregistrement courant
                    while(buff0.tab[j].eff == 1 && j > 0)
                        j--;//Chercher au dessous dans le meme block
                    tmp = buff0.tab[j].matricule*10 + buff0.tab[j].eff;//Sauvgarder le champ effacer dans le poid faible, et le matricule dans le poid fort
                    j = ct;
                    while(buff0.tab[j].eff == 1 && j < buff0.Nb - 1)
                        j++;//Recherche au dessus

                    if(tmp/10 <  buff0.tab[j].matricule)//Comparaison des deux matricules trouv�s
                        tmp = buff0.tab[j].matricule*10 + buff0.tab[j].eff;
                    if(tmp % 10 == 0){//Si il ne sont pas supprim�s
                        textcolor(11);
                        printf("\n\t\tLe plus proche matricule trouve est : ");textcolor(12);printf("%d", tmp/10);
                    }
                    //Affichage d'une liste de d�cision
                    char tab[2][70] = {"> Continuer.", "> Quitter."};
                    int z, x, y;
                       textcolor(12);printf("\n\n\t\t\t");x = wherex();y = wherey();printf(">");textcolor(11);printf("Continuer.");
                       textcolor(12);printf("\n\t\t\t>");textcolor(11);printf("Quitter.");
                       z = pointer(x - 1,y , 2, tab, 12, 11, 12, 10);
                       gotoxy(x, y + 3);
                       if(z == 0)
                        goto adrmod;
            }
        }
        else
        {
            textcolor(12);printf("\n\t\tFichier vide!");
            getch();
        }
        textcolor(15);

}

void Disp3Blocks()
{
    //Affichage des 3 blocks
    int tb[3] = {14, 11, 11};
    textcolor(15);
    Block buffer;
    SEns e;
    int i = f.Entete.tab[0], count = 0, ct = 1, cd = 1, x, y, z, text, inter;
    char tab[4][70] = {"> Continuer par morceaux.", "> Aller au prochain block.", "> Continuer jusqu'a la fin.", "> Quitter l'affichage."};
    int t = 14;
    while (i != 0 && count < 3)
    {
        clrscr();
        printf("\n\n________________________________________________________________________________________________________________________________________________________________________");
        textcolor(9);
        printf("\n                                                  __  __ _      _                        ____    ____  _            _        ");
        printf("\n                                           /\\    / _|/ _(_)    | |                      |___ \\  |  _ \\| |          | |       ");
        printf("\n                                          /  \\  | |_| |_ _  ___| |__   __ _  __ _  ___    __) | | |_) | | ___   ___| | _____ ");
        printf("\n                                         / /\\ \\ |  _|  _| |/ __| '_ \\ / _` |/ _` |/ _ \\  |__ <  |  _ <| |/ _ \\ / __| |/ / __|");
        printf("\n                                        / ____ \\| | | | | | (__| | | | (_| | (_| |  __/  ___) | | |_) | | (_) | (__|   <\\__ \\");
        printf("\n                                       /_/    \\_\\_| |_| |_|\\___|_| |_|\\__,_|\\__, |\\___| |____/  |____/|_|\\___/ \\___|_|\\_\\___/");
        printf("\n                                                                             __/ |                                           ");
        printf("\n                                                                            |___/                                            ");
        textcolor(15);
        printf("\n\n________________________________________________________________________________________________________________________________________________________________________");
printf("\n\n\n\n                                                    ");textcolor(12);printf("+");textcolor(tb[0]);printf("----------");
        textcolor(12);printf("+");textcolor(12);printf("    __ __ _");textcolor(12);printf("+");textcolor(tb[1]);printf("----------");
        textcolor(12);printf("+");textcolor(12);printf("    __ __ _ ");textcolor(12);printf("+");textcolor(tb[2]);printf("----------");
        textcolor(12);printf("+");textcolor(tb[0]);
        printf("\n                                                    |       |  |");textcolor(12);printf("   /       ");textcolor(tb[1]);
        printf("|       |  |");textcolor(12);printf("   /        ");textcolor(tb[2]);printf("|       |  |");textcolor(tb[0]);
        printf("\n                                                    |       |");textcolor(12);printf("--");textcolor(tb[0]);
        printf("|  ");textcolor(12);printf("/");textcolor(tb[1]);printf("        |       |");textcolor(12);printf("--");textcolor(tb[1]);
        printf("|  ");textcolor(12);printf("/");textcolor(tb[2]);printf("         |       |");textcolor(12);printf("--");textcolor(tb[2]);printf("|");
        textcolor(tb[0]);printf("\n                                                    |       |  |");textcolor(12);printf("_/");textcolor(tb[1]);
        printf("         |       |  |");textcolor(12);printf("_/");textcolor(tb[2]);printf("          |       |  |");textcolor(12);
        printf("\n                                                    +");textcolor(tb[0]);printf("----------");textcolor(12);
        printf("+           +");textcolor(tb[1]);printf("----------");textcolor(12);printf("+            +");textcolor(tb[2]);
        printf("----------");textcolor(12);printf("+");textcolor(tb[0]);
        printf("\n                                                     Block -");textcolor(12);printf(" 1 ");textcolor(tb[0]);
        printf("-");textcolor(tb[1]);printf("            Block - ");textcolor(12);printf("2");textcolor(tb[1]);printf(" - ");textcolor(tb[2]);printf("            Block -");textcolor(12);
        printf(" 3 ");textcolor(tb[2]);printf("-");
        printf("\n");
        textcolor(12);printf("\n\t\t\t\t\t\t\t\t\t  > ");textcolor(tb[2]);printf("Patientez...");
        printf("\n\n\n\n_____________________________________________________________________________");textcolor(10);printf(" Suiv");textcolor(15);printf(" ____________________________________________________________________________________");
        lines(n);
        for(int k = 0; k < 799999999; k++){}
        LireDir(f.F, i, &buffer);
        CountLE[0]++;

        clrscr();
        textcolor(12);printf("+");textcolor(11);printf("----");textcolor(12);printf("+");textcolor(11);printf("--------");textcolor(12);
        printf("+");textcolor(11);printf("----------");textcolor(12);printf("+");textcolor(11);printf("----------");textcolor(12);
        printf("+");textcolor(11);printf("-----");textcolor(11);printf("+-----");textcolor(12);printf("+");textcolor(11);
        printf("------------------------------");textcolor(12);printf("+");textcolor(11);printf("--------------");textcolor(12);
        printf("+");textcolor(11);printf("----------------------------");textcolor(12);printf("+");textcolor(11);
        printf("------------");textcolor(12);printf("+");textcolor(11);printf("-------------------------");textcolor(12);
        printf("+");textcolor(11);printf("----");textcolor(12);printf("+");
        textcolor(11);printf("|");textcolor(12);printf(" Nu ");textcolor(11);printf("|");textcolor(12);printf("  Matr  ");textcolor(11);printf("|");textcolor(12);
        printf(" Date Rec ");textcolor(11);printf("|");textcolor(12);printf(" Date Nai ");textcolor(11);
        printf("|");textcolor(12);printf("Sexe ");textcolor(11);printf("|");textcolor(12);printf("Sang ");
        textcolor(11);printf("|");textcolor(12);printf("        Nom & Prenom          ");textcolor(11);
        printf("|");textcolor(12);printf("  Wilaya Nais ");textcolor(11);printf("|");textcolor(12);
        printf("           Grade            ");textcolor(11);printf("|");textcolor(12);printf(" Specialite ");
        textcolor(11);printf("|");textcolor(12);printf("     Dernier Diplome     ");textcolor(11);printf("|");textcolor(12);
        printf("Etab");textcolor(11);printf("|");
        textcolor(12);printf("+");textcolor(11);printf("----");textcolor(12);printf("+");textcolor(11);printf("--------");textcolor(12);
        printf("+");textcolor(11);printf("----------");textcolor(12);printf("+");textcolor(11);printf("----------");textcolor(12);
        printf("+");textcolor(11);printf("-----");textcolor(11);printf("+-----");textcolor(12);printf("+");textcolor(11);
        printf("------------------------------");textcolor(12);printf("+");textcolor(11);printf("--------------");textcolor(12);
        printf("+");textcolor(11);printf("----------------------------");textcolor(12);printf("+");textcolor(11);
        printf("------------");textcolor(12);printf("+");textcolor(11);printf("-------------------------");textcolor(12);
        printf("+");textcolor(11);printf("----");textcolor(12);printf("+");
        for(int j=0; j<buffer.Nb; j++){
            text = 0;
            textcolor(11);printf("|");textcolor(t);printf("%.4d", j);textcolor(11);printf("|");textcolor(11);
            printf(" %.6d ", buffer.tab[j].Matricule);textcolor(11);printf("|");textcolor(t);printf("%.2d", buffer.tab[j].Date_recrutement.jj);textcolor(12);
            printf(".");textcolor(t);printf("%.2d", buffer.tab[j].Date_recrutement.mm);textcolor(12);printf(".");textcolor(t);
            printf("%.4d", buffer.tab[j].Date_recrutement.yyyy);textcolor(11);printf("|");textcolor(t);printf("%.2d", buffer.tab[j].Date_naissance.jj);textcolor(12);
            printf(".");textcolor(t);printf("%.2d", buffer.tab[j].Date_naissance.mm);textcolor(12);printf(".");textcolor(t);
            printf("%.4d", buffer.tab[j].Date_naissance.yyyy);textcolor(11);printf("|");textcolor(t);printf("%.5s", tabgender[buffer.tab[j].sexe]);textcolor(11);
            printf("|");textcolor(t);printf(" %.3s ", Groupe_Sanguin[buffer.tab[j].Groupe_sanguin]);textcolor(11);printf("|");textcolor(11);
            while(buffer.tab[j].Nom_Prenom[text] != '.'){printf("%c", buffer.tab[j].Nom_Prenom[text]);++text;}textcolor(12);printf("%c", buffer.tab[j].Nom_Prenom[text]);

            textcolor(t);printf("%s", &buffer.tab[j].Nom_Prenom[text] + 1);textcolor(11);printf("|");textcolor(t);printf("%s", tab_wilaya[buffer.tab[j].Wilaya_naissance]);textcolor(11);printf("|");
            textcolor(t);printf("%s", Grade[buffer.tab[j].Grade]);textcolor(11);printf("|");textcolor(t);printf("%s", specialites[buffer.tab[j].Specialite]);
            textcolor(11);printf("|");textcolor(t);printf("%s" , Der_Dip[buffer.tab[j].Dernier_diplome]);textcolor(11);printf("|");textcolor(t);
            printf("%.3d", buffer.tab[j].Etablissement_universitaire);textcolor(11);printf(" |");
            if(cd){
                if(ct % 50 == 0){
                    ct = 1;
                    textcolor(12);printf("+");textcolor(11);printf("----");textcolor(12);printf("+");textcolor(11);printf("--------");textcolor(12);
                    printf("+");textcolor(11);printf("----------");textcolor(12);printf("+");textcolor(11);printf("----------");textcolor(12);
                    printf("+");textcolor(11);printf("-----");textcolor(12);printf("+");textcolor(11);printf("-----");textcolor(12);printf("+");textcolor(11);
                    printf("------------------------------");textcolor(12);printf("+");textcolor(11);printf("--------------");textcolor(12);
                    printf("+");textcolor(11);printf("----------------------------");textcolor(12);printf("+");textcolor(11);
                    printf("------------");textcolor(12);printf("+");textcolor(11);printf("-------------------------");textcolor(12);
                    printf("+");textcolor(11);printf("----");textcolor(12);printf("+");

                   textcolor(12);printf("\n\n\t\t\t\t");x = wherex();y = wherey();printf("> ");textcolor(11);printf("Continuer par morceaux.");
                   textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Aller au prochain block.");
                   textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("Continuer jusqu'a la fin.");
                   textcolor(12);printf("\n\t\t\t\t> ");textcolor(11);printf("> Quitter l'affichage.");
                   z = pointer(x - 1,y , 4, tab, 12, 11, 12, 10);
                   gotoxy(1, y + 5);
                   if(z == 1){
                        if(count < 2)
                            goto proch;
                        else
                            goto nextAff3;
                   }
                   else if(z == 2)cd = 0;
                   else if (z == 3) goto nextAff3;
                }
                ct++;
            }


        }

        textcolor(12);printf("+");textcolor(11);printf("----");textcolor(12);printf("+");textcolor(11);printf("--------");textcolor(12);
        printf("+");textcolor(11);printf("----------");textcolor(12);printf("+");textcolor(11);printf("----------");textcolor(12);
        printf("+");textcolor(11);printf("-----");textcolor(12);printf("+");textcolor(11);printf("-----");textcolor(12);printf("+");textcolor(11);
        printf("------------------------------");textcolor(12);printf("+");textcolor(11);printf("--------------");textcolor(12);
        printf("+");textcolor(11);printf("----------------------------");textcolor(12);printf("+");textcolor(11);
        printf("------------");textcolor(12);printf("+");textcolor(11);printf("-------------------------");textcolor(12);
        printf("+");textcolor(11);printf("----");textcolor(12);printf("+\n\n");
        getch();
        proch :
        count++;
        i = buffer.lien;
        inter = tb[2];
        for(int k = 2; k > 0; k--)
            tb[k] = tb[k - 1];

        tb[0] = inter;


    }
    //Si le fichier est vide
    if(count == 0 && i == 0) {
        clrscr();
        textcolor(12);printf("\n\n\t\t\t\t> ");textcolor(11);printf("Fichier vide!");}
    textcolor(12);printf("\n\n\t\t\t> ");textcolor(11);printf("Affichage termine.");
    getch();
    nextAff3:
    textbackground(0);
}

void FisrtScreen()
{
    //Affichage de l'�cran d'acceuil
    int i,x,y,c;
    gotoxy(1, 2);
    x=wherex();
    y=wherey();
    gotoxy(x,y);
    printf(" ______  _____  __ ");
    gotoxy(x,y+1);
    printf("|  ____|/ ____||__|");
    gotoxy(x,y+2);
    printf("| |__  | (___   __ ");
    gotoxy(x,y+3);
    printf("|  __|  \\___ \\ |  |");
    gotoxy(x,y+4);
    printf("| |____ ____) ||  |");
    gotoxy(x,y+5);
    printf("|______|_____/ |__|");
    textcolor(9);
    gotoxy(x+15,y);
    printf(" __");
    gotoxy(x+15,y+1);
    printf("|__|");
    textcolor(15);
    gotoxy(x+21,y+1);
    printf("%ccole nationale",144);
    gotoxy(x+21,y+3);
    printf("Sup%crieure",130);
    gotoxy(x+21,y+5);
    printf("d'Informatique");
    textcolor(9);
    printf("\n\n_______________________________________________________________________________________________________________________________________________________________________");
    textcolor(10);
    printf("\n\n\n\t\t\t\t\t\t\t _______ _____");textcolor(9);printf("     _____ ______ _____ _____   \n");textcolor(10);
    printf("\t\t\t\t\t\t\t|__   __|  __ \\");textcolor(9);printf("   / ____|  ____/ ____|  __ \\  \n");textcolor(10);
   printf("\t\t\t\t\t\t\t   | |  | |__) |");textcolor(9);printf(" | (___ | |__ | (___ | |  | | \n");textcolor(10);

   printf("\t\t\t\t\t\t\t   | |  |  ___/");textcolor(9);printf("   \\___ \\|  __| \\___ \\| |  | | \n");textcolor(10);
    printf("\t\t\t\t\t\t\t   | |  | |");textcolor(9);printf("       ____) | |    ____) | |__| | \n");textcolor(10);
    printf("\t\t\t\t\t\t\t   |_|  |_|");textcolor(9);printf("      |_____/|_|   |_____/|_____/  \n");textcolor(10);
    printf("\t\t\t\t\t\t\t\n\n");
    textcolor(12);
    lines(n);
    textcolor(9);
    printf("\t\t\t\t                                                                                                   \n"
    "\t\t\t\t                                     TRAVAIL REALISE PAR :                                       `\n"
    "\t\t\t\t                                                                                                   \n"
    "\t\t\t\t                    ");textcolor(10);printf("</>");textcolor(9);printf(" TEBBAL");textcolor(12);printf(" Nadjib                      ");textcolor(10);printf("</>");
    textcolor(9);printf(" CHABANE");textcolor(12);printf(" Oualid\n\n"
    "\t\t\t\t    Theme"); textcolor(10); printf(" : Manipulation d'un fichier LObarreF, contenant les enseignants inscrits en MESRS\n\n");textcolor(9);
    printf("\t\t\t\t                                           GROUPE :");textcolor(10);printf(" 01");printf("\n\n");
    textcolor(12);
    printf("\n\n_____________________________________________________________________________");textcolor(10);printf(" FIN");textcolor(12);printf(" ______________________________________________________________________________________");
    textcolor(12);
    lines(n);
    textcolor(10);printf("\n\t                                             ----->>"); textcolor(12);printf(" APPUYEZ SUR N'IMPORTE QUELLE TOUCHE AFIN DE COMMENCER... ");
    textcolor(15);
    getchar();
}

void DispEntete()
{
    //Affichage de l'entete
     clrscr();
     textcolor(15);
     printf("\n________________________________________________________________________________________________________________________________________________________________________\n");
     textcolor(9);
     printf("\n                                                                   ______       _       _ ");
     printf("\n                                                                  |  ____|     | |     | | ");
     printf("\n                                                                  | |__   _ __ | |_ ___| |_ ___");
     printf("\n                                                                  |  __| | '_ \\| __/ _ \\ __/ _ \\");
     printf("\n                                                                  | |____| | | | ||  __/ ||  __/");
     printf("\n                                                                  |______|_| |_|\\__\\___|\\__\\___|");
     textcolor(15);
     printf("\n\n\n________________________________________________________________________________________________________________________________________________________________________\n");
     lines(n);
     textcolor(10);
     printf("\n                                                                <>");textcolor(9);printf("Fichier enseignants ");textcolor(12);printf("%s", Namefich);textcolor(10);printf(" <\\>\n");textcolor(15);
     textcolor(11);
     printf("\n     +------------------------+-------------------------+-------------------------+-------------------------+-------------------------+-------------------------+");
     printf("\n     |");textcolor(9);printf("   Tete de la liste     ");textcolor(11);printf("|");textcolor(9);printf("   Queue de la liste     ");textcolor(11);printf("|");textcolor(9);printf("  Tete de la liste vide  ");textcolor(11);printf("|");textcolor(9);
     printf(" Queue de la liste vide  ");textcolor(11);printf("|");textcolor(9);printf(" Nombre d'enregistrements");textcolor(11);printf("|");textcolor(9);printf("  Plus grand block vide  ");textcolor(11);printf("|");
     printf("\n     +------------------------+-------------------------+-------------------------+-------------------------+-------------------------+-------------------------+");
     printf("\n     |                        |                         |                         |                         |                         |                         |");
     printf("\n     |");textcolor(12);printf("          %.2d            ", f.Entete.tab[0]);textcolor(11);printf("|");textcolor(12);printf("           %.2d            ", f.Entete.tab[1]);textcolor(11);printf("|");textcolor(12);printf("           %.2d            ", f.Entete.tab[2]);textcolor(11);printf("|");textcolor(12);
     printf("            %.2d           ", f.Entete.tab[3]);textcolor(11);printf("|");textcolor(12);printf("         %.7d         ", f.Entete.tab[5]);textcolor(11);printf("|");textcolor(12);printf("           %.2d            ", f.Entete.tab[4]);textcolor(11);printf("|");
     printf("\n     |                        |                         |                         |                         |                         |                         |");
     printf("\n     +------------------------+-------------------------+-------------------------+-------------------------+-------------------------+-------------------------+");
     textcolor(10);
     printf("\n\n\n                                                                        <>");textcolor(9);printf("Index");textcolor(12);printf(" %s", NameInd);textcolor(10);printf(" <\\>\n");
     printf("\n                                                                  -> ");textcolor(11);printf("Nombre des blocks          : ");textcolor(12);printf(" %d", fInd.Entete.tab[0]);textcolor(10);
     printf("\n                                                                  -> ");textcolor(11);printf("Nombre des Enregistrements : ");textcolor(12);printf(" %d", fInd.Entete.tab[1]);
    textcolor(15);
     printf("\n\n\n_____________________________________________________________________________");textcolor(10);printf(" FIN");textcolor(15);printf(" ______________________________________________________________________________________");
    lines(n);
    getch();
}

void Dispstats()
{
    //Affichage de nombre des liredires et d'ecriredirs effectu�s � partire du d�but d'�x�cution du programme, Il est donc Variable.
     clrscr();
     textcolor(15);
     printf("\n________________________________________________________________________________________________________________________________________________________________________\n");
     textcolor(9);
     printf("\n                                                        _____ _        _   _     _   _                       ");
     printf("\n                                                       / ____| |      | | (_)   | | (_)                    ");
     printf("\n                                                      | (___ | |_ __ _| |_ _ ___| |_ _  __ _ _   _  ___  ___ ");
     printf("\n                                                       \\___ \\| __/ _` | __| / __| __| |/ _` | | | |/ _ \\/ __|");
     printf("\n                                                       ____) | || (_| | |_| \\__ \\ |_| | (_| | |_| |  __/\\__ \\");
     printf("\n                                                      |_____/ \\__\\__,_|\\__|_|___/\\__|_|\\__, |\\__,_|\\___||___/");
     printf("\n                                                                                          | |                ");
     printf("\n                                                                                          |_|                ");
     textcolor(15);
     printf("\n\n________________________________________________________________________________________________________________________________________________________________________\n");
     textcolor(12);printf("\n                +");textcolor(11);printf("-------------");textcolor(12);printf("+");textcolor(11);printf("-----------------------------");textcolor(12);
     printf("+");textcolor(11);printf("------------------------------");textcolor(12);printf("+");textcolor(11);
     printf("---------------------------");textcolor(12);printf("+");textcolor(11);printf("---------------------------");textcolor(12);printf("+");
     printf("\n                |             |");textcolor(12);printf("            Index            ");textcolor(11);printf("|");textcolor(12);
     printf("       Fichier principal      ");textcolor(11);printf("|");textcolor(12);printf("          BitMap           ");textcolor(11);
     printf("|");textcolor(12);printf("            TOTAL          ");textcolor(11);printf("|");textcolor(12);
     printf("\n                +");textcolor(11);printf("-------------");textcolor(12);printf("+");textcolor(11);
     printf("-----------------------------");textcolor(12);printf("+");textcolor(11);printf("------------------------------");textcolor(12);printf("+");textcolor(11);
     printf("---------------------------");textcolor(12);printf("+");textcolor(11);printf("---------------------------");textcolor(12);printf("+");textcolor(11);
     printf("\n                |");textcolor(11);printf(" LireDir     ");textcolor(11);printf("|");textcolor(14);printf("           %.6d            ", CountLE[2]);textcolor(11);
     printf("|");textcolor(14);printf("            %.6d            ", CountLE[0]);textcolor(11);printf("|");textcolor(14);printf("          %.6d           ", CountLE[4]);textcolor(11);printf("|");textcolor(14);printf("           %.6d          ", CountLE[0] + CountLE[2] + CountLE[4]);
     textcolor(11);printf("|");
     textcolor(12);printf("\n                +");textcolor(11);printf("-------------");textcolor(12);printf("+");textcolor(11);
     printf("-----------------------------");textcolor(12);printf("+");textcolor(11);printf("------------------------------");textcolor(12);
     printf("+");textcolor(11);printf("---------------------------");textcolor(12);printf("+");textcolor(11);
     printf("---------------------------");textcolor(12);printf("+");
     textcolor(11);printf("\n                |");textcolor(11);printf(" EcrireDir   ");textcolor(11);printf("|");textcolor(14);printf("           %.6d            ", CountLE[3]);textcolor(11);
     printf("|");textcolor(14);printf("            %.6d            ", CountLE[1]);textcolor(11);printf("|");textcolor(14);printf("          %.6d           ", CountLE[5]);textcolor(11);printf("|");textcolor(14);printf("           %.6d          ", CountLE[1] + CountLE[3] + CountLE[5]);
     textcolor(11);printf("|");
     textcolor(12);printf("\n                +");textcolor(11);printf("-------------");textcolor(12);printf("+");textcolor(11);
     printf("-----------------------------");textcolor(12);printf("+");textcolor(11);printf("------------------------------");textcolor(12);
     printf("+");textcolor(11);printf("---------------------------");textcolor(12);printf("+");textcolor(11);
     printf("---------------------------");textcolor(12);printf("+");
     textcolor(15);
     printf("\n\n\n\n\n_____________________________________________________________________________");textcolor(10);printf(" FIN");textcolor(15);printf(" ______________________________________________________________________________________");
     lines(n);
     getch();
}

void Guid(){
    clrscr();
    textcolor(15);
     printf("\n\n________________________________________________________________________________________________________________________________________________________________________\n");
    textcolor(12);printf("\n\n\n\n\n\n                                 Titre :");textcolor(14);printf("TP SFSD, Realisation d'un proramme qui gere les donnees des enseignants inscrits au minister d'enseignement superieur.");
    textcolor(12);printf("\n                                 Realise par :");textcolor(14);printf("CHABANE Oualid");textcolor(12);printf(" - ");textcolor(14);printf("TEBBAL Nadjib");
    textcolor(12);printf("\n                                 Encadre par :");textcolor(14);printf("Mr.Kermi");
    textcolor(12);printf("\n                                 Annee universitaire :");textcolor(12);printf("2022 ");textcolor(14);printf("-");textcolor(12);printf(" 2023");
    textcolor(12);printf("\n                                 Etablissement :");textcolor(14);printf("Ecole Nationale superieure d'inforamtique (ESI ex INI).");
    textcolor(12);printf("\n                                 Guide de l'utilisation :");
    textcolor(12);printf("\n                                 \t>");textcolor(11);printf(" Pour commencer le programme il faut creer 3 fichier.");
    textcolor(12);printf("\n                                 \t\t>");textcolor(11);printf(" Fichier enseignants");
    textcolor(12);printf("\n                                 \t\t>");textcolor(11);printf(" Fichier Index");
    textcolor(12);printf("\n                                 \t\t>");textcolor(11);printf(" Fichier Bitmap");
    textcolor(12);printf("\n                                 \t>");textcolor(11);printf(" Il y a 124 chaines BitMaps de taille variable selon le nombre des enregistrements inseres");
    textcolor(12);printf("\n                                 \t\t>");textcolor(12);printf(" 000 ");textcolor(11);printf("-");textcolor(12);printf(" 110");textcolor(11);printf(" : chaines relatifs au etablissements");
    textcolor(12);printf("\n                                 \t\t>");textcolor(12);printf(" 111");textcolor(11);printf(" -");textcolor(12);printf(" 120 ");textcolor(11);printf(": chaines relatifs au specialites");
    textcolor(12);printf("\n                                 \t\t>");textcolor(12);printf(" 120");textcolor(11);printf(" -");textcolor(12);printf(" 123");textcolor(11);printf(" : chaines relatifs au regions");
    textcolor(12);printf("\n                                 \t>");textcolor(11);printf(" Les etablissements sont ordonnees selon la region.");
    textcolor(12);printf("\n                                 \t\t>");textcolor(12);printf(" 00");textcolor(11);printf(" - ");textcolor(12);printf("45 ");textcolor(11);printf(" : Est.");
    textcolor(12);printf("\n                                 \t\t>");textcolor(12);printf(" 41");textcolor(11);printf(" -");textcolor(12);printf(" 82");textcolor(11);printf("  : Centre.");
    textcolor(12);printf("\n                                 \t\t>");textcolor(12);printf(" 82");textcolor(11);printf(" -");textcolor(12);printf(" 110 ");textcolor(11);printf(": Ouest.");
    textcolor(12);printf("\n                                 \t>");textcolor(11);printf(" Pour coder un fichier il faut quitter le programmer, puis executer de nouveau ouvrire le.");
    textcolor(11);printf("\n                                 \t\t et  fichier precedant en mode ancien, pour la sauvgarde de l'entete du fichier a coder");
    textcolor(12);printf("\n                                 \t>");textcolor(11);printf(" Les noms sont affiches en blue, et les prenoms en jaune claire.");
    textcolor(12);printf("\n                                 \t>");textcolor(11);printf(" Mettre l'ecran en mode plaine pour meux visualiser les résultats.");
    textcolor(15);
    printf("\n\n\n\n\n\n\n\n\n_____________________________________________________________________________");textcolor(10);printf(" FIN");textcolor(15);printf(" ______________________________________________________________________________________");
    lines(n);
    getch();

}
void MenuPrimaire()
{
     int x, y, z;
     char tabch[8][70]={"> Operations d'initialisation.", "> Operations d'ajouts.", "> Operations de modification.", "> Operation de suppression.", "> Codage et Decodage.", "> Operations d'affichage.","> Guide de l'utilisation.", "> Quitter."};
     char tabch1[4][70] = {"> Creation du fichier.", "> Ouverture d'un fichier existant.", "> Chargement initial.","> Retour au menu principal."};
     char tabch2[2][70] = {"> Modification de l etablissement universitaire.","> Retour au menu principal."};
     char tabch3[3][70] = {"> Suppression d�un enregistrement par matricule.", "> Suppression de tous les enregistrements relatifs a une specialite." , "> Retour au menu principal."};
     char tabch4[10][70] = {"> Affichage d'enregistrements par region.", "> Consultation par etablissements et anciente.", "> Affichage 3 Blocks", "> Affichage entete.","> Affichage Index.", "> Affichage BitMap.", "> Affichage d'un seul Enregistrement.", "> Affichage des statistiques.",
      "> Les noms des fichiers.", "> Retour au menu principal."};
     char tabch5[2][70] = {"> Ajout d'un enseignant au fichier.", "> Retour au menu principal."};
     char tabch6[3][70] = {"> Codage du fichier.", "> Decodage du fichier.", "> Retour au menu principal."};
     char tabch7[4][70]={"> Fichier enseignant.", "> Fichier Indexe.", "> Fichier BitMap.", "> Retour au menu principal."};
     textcolor(15);
     begin :
     clrscr();
     textcolor(15);
     printf("\n________________________________________________________________________________________________________________________________________________________________________\n");
     textcolor(9);
     printf("\n                                                   __  __                    _____      _            _             _ ");
     printf("\n                                                  |  \\/  |                  |  __ \\    (_)          (_)           | |");
     printf("\n                                                  | \\  / | ___ _ __  _   _  | |__) | __ _ _ __   ___ _ _ __   __ _| |");
     printf("\n                                                  | |\\/| |/ _ \\ '_ \\| | | | |  ___/ '__| | '_ \\ / __| | '_ \\ / _` | |");
     printf("\n                                                  | |  | |  __/ | | | |_| | | |   | |  | | | | | (__| | |_) | (_| | |");
     printf("\n                                                  |_|  |_|\\___|_| |_|\\__,_| |_|   |_|  |_|_| |_|\\___|_| .__/ \\__,_|_|");
     printf("\n                                                                                                      | |            ");
     printf("\n                                                                                                      |_|            ");
     textcolor(15);
     printf("\n\n\n________________________________________________________________________________________________________________________________________________________________________");
    //Affichage de menu de d�cisions primaire
     x = wherex(); y = wherey();
      gotoxy(1, 39);
     printf("\n\n_____________________________________________________________________________");textcolor(10);printf(" FIN");textcolor(15);printf(" ______________________________________________________________________________________");
     lines(n);
    gotoxy(x, y);
      textcolor(12);printf("\n                                                                 ");x = wherex(); y = wherey();printf(">");textcolor(11);printf(" Operations d'initialisation.");
      textcolor(12);printf("\n                                                                 >");textcolor(11);printf(" Operations d'ajouts.");
      textcolor(12);printf("\n                                                                 >");textcolor(11);printf(" Operations de modification.");
      textcolor(12);printf("\n                                                                 >");textcolor(11);printf(" Operation de suppression.");
      textcolor(12);printf("\n                                                                 >");textcolor(11);printf(" Codage et Decodage.");
      textcolor(12);printf("\n                                                                 >");textcolor(11);printf(" Operations d'affichage.");
      textcolor(12);printf("\n                                                                 >");textcolor(11);printf(" Guide de l'utilisation.");
      textcolor(12);printf("\n                                                                 >");textcolor(11);printf(" Quitter.");
    z = pointer(x - 1, y, 8, tabch, 12, 11, 12, 9);
    switch(z)
    {
        case 0:
            //Cr�ation du fichier
             printf("\n                                                                 \t");x = wherex(); y = wherey();printf(">");textcolor(11);printf(" Creation du fichier.");
             textcolor(12);printf("\n                                                                 \t>");textcolor(11);printf(" Ouverture d'un fichier existant.");
             textcolor(12);printf("\n                                                                 \t>");textcolor(11);printf(" Chargement initial.");
             textcolor(12);printf("\n                                                                 \t>");textcolor(11);printf(" Retour au menu principal.\n");
             gotoxy(x, y + 4);
             for(int k = z + 1; k < 7; k++)
                printf("\n                                                                 %s",tabch[k]);

             z = pointer(x - 1, y, 4, tabch1, 12, 11, 12, 9);//Afficher le reste de la liste primaire
            switch(z){
            case 0 :
                if(f.F != NULL){
                    FermerBitMap();
                    Fermer();
                    FermerInd();
                }
                //Cr�ation
                Ouvrire(Namefich, 'N');
                OuvrireBitMap(NameBitmap, 'N');
                OuvrireInd( NameInd, 'N');
                break;
            case 1:
                //Ouverture
                //Si l'utilisateur effectue une ouverture et le fichier est d�ja ouvert, on l'ouvre pas en mode ancien.
                if(f.F != NULL){
                    FermerBitMap();
                    Fermer();
                    FermerInd();
                }
               Ouvrire(Namefich, 'A');
               OuvrireBitMap(NameBitmap, 'A');
               OuvrireInd( NameInd, 'A');
                getch();
                break;
            case 2:
                //Si le coix dans la liste de d�cision est le chargement initial
                Chargement_Init();
                break;
            default:
                break;
             }
            break;

        case 1:
            //L'ajout
             textcolor(12);printf("\n                                                                 \t");x = wherex(); y = wherey();printf(">");textcolor(11);printf(" Ajout d'un enseignant au fichier.");
             textcolor(12);printf("\n                                                                 \t>");textcolor(11);printf(" Retour au menu principal.\n");
             gotoxy(x, y + 2);
             for(int k = z + 1; k < 7; k++)
                 printf("\n                                                                 %s",tabch[k]);

             z = pointer(x - 1, y, 2, tabch5, 12, 11, 12, 9);
             switch(z){
            case 0:
                //Ajout
                Q41();
                break;
            default:
                break;
             }
            break;
        case 2:
            //La modification
             textcolor(12);printf("\n                                                                 \t");x = wherex(); y = wherey();printf(">");textcolor(11);printf(" Modification de l etablissement universitaire.");
             textcolor(12);printf("\n                                                                 \t>");textcolor(11);printf(" Retour au menu principal.\n");
             gotoxy(x, y + 2);
             for(int k = z + 1; k < 7; k++)
                 printf("\n                                                                 %s",tabch[k]);

             z = pointer(x - 1, y, 2, tabch2, 12, 11, 12, 9);
             switch(z){
            case 0:
                ModificationEns();
                break;
            case -1:
                break;
             }
            break;
        case 3:
            //Op�rations de suppression
             printf("\n                                                                 \t");x = wherex(); y = wherey();printf(">");textcolor(11);printf(" Suppression d'un enregistrement par matricule.");
             printf("\n                                                                 \t>");textcolor(11);printf(" Suppression de tous les enregistrements relatifs a une specialite.");
             printf("\n                                                                 \t>");textcolor(11);printf(" Retour au menu principal.\n");
             printf("\n");
             gotoxy(x, y + 3);
             for(int k = z + 1; k < 7; k++)
                 printf("\n                                                                 %s",tabch[k]);

             z = pointer(x - 1, y, 3, tabch3, 12, 11, 12, 9);
             switch(z){
                 case 0:
                     //Suppression par matricule
                     SupEns();
                    break;
                 case 1:
                     //Suppression d'une specialit�
                     SupSpec();
                    break;
                 case -1:
                    break;
             }
            break;
        case 4:
            //Op�ration du codage et de d�codage
             textcolor(12);printf("\n                                                                 \t");x = wherex(); y = wherey();printf(">");textcolor(11);printf(" Codage du fichier.");
             textcolor(12);printf("\n                                                                 \t>");textcolor(11);printf(" Decodage du fichier.");
             textcolor(12);printf("\n                                                                 \t>");textcolor(11);printf(" Retour au menu principal.\n");
             gotoxy(x, y + 3);
             for(int k = z + 1; k < 7; k++)
                 printf("\n                                                                 %s",tabch[k]);

             z = pointer(x - 1, y, 3, tabch6, 12, 11, 12, 9);
             switch(z){
                 case 0:
                     //Codage
                    textcolor(12);printf("\n                                                                 \t\t");x = wherex(); y = wherey();printf("> ");textcolor(11);printf("Fichier enseignant.");
                    textcolor(12);printf("\n                                                                 \t\t> ");textcolor(11);printf("Fichier Indexe.");
                    textcolor(12);printf("\n                                                                 \t\t> ");textcolor(11);printf("Fichier BitMap.");
                    textcolor(12);printf("\n                                                                 \t\t> ");textcolor(11);printf("> Retour au menu principal.");
                    gotoxy(x, y + 3);
                    for(int k = z + 1; k < 3; k++)
                        printf("\n                                                                 \t%s",tabch6[k]);
                    z = pointer(x - 1, y, 4, tabch7, 12, 11, 12, 9);
                    switch(z){
                        case 0:
                            //Fichier enseignants
                            Codef(&f.F, key, fichEcode);
                            break;
                        case 1:
                            //Fichier Index
                            Codef(&fInd.F, key, fichIcode);
                            break;
                        case 2:
                            //Fichier Bitmap
                            Codef(&Fbitmap, key, fichBcode);
                            break;
                        default :
                            break;
                    }
                    break;
                 case 1:
                     //D�codage
                    textcolor(12);printf("\n                                                                 \t\t");x = wherex(); y = wherey();printf("> ");textcolor(11);printf("Fichier enseignant.");
                    textcolor(12);printf("\n                                                                 \t\t> ");textcolor(11);printf("Fichier Indexe.");
                    textcolor(12);printf("\n                                                                 \t\t> ");textcolor(11);printf("Fichier BitMap.");
                    textcolor(12);printf("\n                                                                 \t\t> ");textcolor(11);printf("> Retour au menu principal.");
                    gotoxy(x, y + 3);
                    for(int k = z + 1; k < 3; k++)
                        printf("\n                                                                 \t%s",tabch6[k]);
                    z = pointer(x - 1, y, 4, tabch7, 12, 11, 12, 9);

                    switch(z){
                        case 0:
                            Decodef(fichEcode, key, fichEres);
                            break;
                        case 1:
                            Decodef(fichIcode, key, fichIres);
                            break;
                        case 2:
                            Decodef(fichBcode, key, fichBres);
                            break;
                        default :
                            break;
                    }
                    break;
                 default:
                    break;
             }
            break;
        case 5:
            //Op�rations d'affichage
              textcolor(12);printf("\n                                                                 \t");x = wherex(); y = wherey();printf(">");textcolor(11);printf(" Affichage d'enregistrements par region.");
              textcolor(12);printf("\n                                                                 \t>");textcolor(11);printf(" Consultation par etablissements et anciente.");
              textcolor(12);printf("\n                                                                 \t>");textcolor(11);printf(" Affichage 3 Blocks");
              textcolor(12);printf("\n                                                                 \t>");textcolor(11);printf(" Affichage entete.");
              textcolor(12);printf("\n                                                                 \t>");textcolor(11);printf(" Affichage Index.");
              textcolor(12);printf("\n                                                                 \t>");textcolor(11);printf(" Affichage BitMap.");
              textcolor(12);printf("\n                                                                 \t>");textcolor(11);printf(" Affichage d'un seul Enregistrement.");
              textcolor(12);printf("\n                                                                 \t>");textcolor(11);printf(" Affichage des statistiques.");
              textcolor(12);printf("\n                                                                 \t>");textcolor(11);printf(" Les noms des fichiers.");
              textcolor(12);printf("\n                                                                 \t>");textcolor(11);printf(" Retour au menu principal.\n");
             gotoxy(x, y + 10);
             for(int k = z + 1; k < 7; k++)
                 printf("\n                                                                 %s",tabch[k]);

             z = pointer(x - 1, y, 10, tabch4, 12, 11, 12, 9);
            switch(z){
            case 0 :
                AffichageParRegion();
                break;
            case 1:
                ConsultationEtabAnc();
                break;
            case 2:
                //Affichage_FEnseignat();
                Disp3Blocks();
                break;
            case 3:
                DispEntete();
                break;
            case 4:
                Affichage_Index(fInd);
                break;
            case 5:
                Temp();
                break;
            case 6:
                AffichageUnEng();
                break;
            case 7:
                Dispstats();
                break;
            case 8:
                    clrscr();
                    textcolor(15);
                    printf("\n________________________________________________________________________________________________________________________________________________________________________");

                   textcolor(9);
                     printf("\n                                                            _   _                       ______");
                     printf("\n                                                           | \\ | |                     |  ____|");
                     printf("\n                                                           |  \\| | ___  _ __ ___  ___  | |__   ");
                     printf("\n                                                           | . ` |/ _ \\| '_ ` _ \\/ __| |  __| ");
                     printf("\n                                                           | |\\  | (_) | | | | | \__ \\  | |    ");
                     printf("\n                                                           |_| \\_|\\___/|_| |_| |_|___/ |_|    ");
                    textcolor(15);
                     printf("\n_________________________________________________________________________________________________________________________________________________________________________");
                    lines(n);
                     textcolor(12);printf("\n\n\n\n                                                             <:>");textcolor(11);printf(" Fichier enseignant Principal : ");textcolor(12);printf("%s", Namefich);
                     textcolor(12);printf("\n                                                             <:>");textcolor(11);printf(" Fichier Index Principal      : ");textcolor(12);printf("%s", NameInd);
                     textcolor(12);printf("\n                                                             <:>");textcolor(11);printf(" Fichier BitMap principal     : ");textcolor(12);printf("%s", NameBitmap);
                     textcolor(12);printf("\n                                                             <:>");textcolor(11);printf(" Fichier enseignant code      : ");textcolor(12);printf("%s", fichEcode);
                     textcolor(12);printf("\n                                                             <:>");textcolor(11);printf(" Fichier Index code           : ");textcolor(12);printf("%s", fichIcode);
                     textcolor(12);printf("\n                                                             <:>");textcolor(11);printf(" Fichier BitMap code          : ");textcolor(12);printf("%s", fichBcode);
                     textcolor(12);printf("\n                                                             <:>");textcolor(11);printf(" Fichier enseignant decode    : ");textcolor(12);printf("%s", fichEres);
                     textcolor(12);printf("\n                                                             <:>");textcolor(11);printf(" Fichier Index decode         : ");textcolor(12);printf("%s", fichIres);
                     textcolor(12);printf("\n                                                             <:>");textcolor(11);printf(" Fichier BitMap decode        : ");textcolor(12);printf("%s", fichBres);
                    textcolor(15);

                    printf("\n\n\n\n\n_____________________________________________________________________________");textcolor(10);printf(" FIN");textcolor(15);printf(" ______________________________________________________________________________________");
                    lines(n);
                    getch();
                    break;
            }
            break;

        case 6:
            Guid();
            break;

        case 7:
            goto next;
            break;
    }
    goto begin;
    next:
    textbackground(0);
}



