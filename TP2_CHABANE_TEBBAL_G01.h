#ifndef TP2_CHABANE_TEBBAL_G01_H_INCLUDED
#define TP2_CHABANE_TEBBAL_G01_H_INCLUDED
#include "conio.c"
#define b 1024
#define MaxInd 1024
#define NbSpecialites 10
#define Taillebtmap 900000
#define cchaine 30

typedef struct{
    int jj;
    int mm;
    int yyyy;
}DT;

typedef struct{
    int Matricule;
    char Nom_Prenom[31];//Convention
    DT Date_naissance;
    int Wilaya_naissance;//Plus grande est Bordj Bou Arrerij_23 cara
    int sexe;
    int Groupe_sanguin;//Maximaume AB+
    DT Date_recrutement;
    int Grade;//Maximaume est Maître de conférences Rang A
    int Specialite;//Maximaume 14 caracters
    int Dernier_diplome;
    int Etablissement_universitaire;
}SEns;

typedef struct{
    int tab[6];
    /*L'entete est composée du numéro de
        1: Premier block.
        2: Dernier block.
        3: Tete des listes vides
        4: queue des listes vide
        5: Le plus grand numéro du block alloué + 1
        6: Nombre des enregistrement inséré.
    */
} SEntete;

typedef struct{
    SEntete Entete;
    FILE *F;
} file;

typedef struct {
    SEns tab[b];//Block d'entregistrement de type SEns contient les informations des enseignant
    int Nb;//Nombre indique la taille effectif du block
    int lien;
} Block;

typedef struct{
    int eff;
    int matricule;
    int Block;
    int Enrg;
}SEnsInd;

typedef struct {
    SEnsInd tab[MaxInd];//Block d'entregistrement de type SEns contient les informations des enseignant
    int Nb;//Nombre indique la taille effectif du block
} BlockIndex;

typedef struct{
    int tab[2];
    /*L'entete est composée du numéro de
        1: Nbr block.
        2: NbrEnregistrements.
    */
} IEntete;

typedef struct{
    IEntete Entete;
    FILE *F;
} fileInd;

DT currentDt;

typedef unsigned short int usint_t;


int OuvrireInd( char Name[ cchaine ], char mode);
int FermerInd();
int EcrireDirInd(FILE *F, int i, BlockIndex buff);
int LireDirInd(FILE *F, int i, BlockIndex *buff);
int OuvrireBitMap(char Name[cchaine], char mode);
void FermerBitMap();
int EcrireDirBitMap(FILE *F, int i, char BitMapSpecialites[Taillebtmap]);
int LireDirBitMap(FILE *F, int i, char BitMapSpecialites[Taillebtmap]);
int Ouvrire(char Name[ cchaine ], char mode);
void Fermer();
int EcrireDir(FILE *F, int i, Block buff);
int LireDir(FILE *F, int i, Block *buff);
int Alloc_Block();
int RechInd(fileInd fInd, int matricule, int *trouv, int *i, int *j);
int insertInd(fileInd *fInd, SEnsInd e, bool condition);
int supInd( int i ,int j);
int pointer(int x, int y, int Nb, char tabch[4][70], int color1,  int color2, int color3, int color4);
void lines(int fin);
int AnBissextile(int year);
SEns Generer();
void RechDic(SEnsInd Index[MaxInd],int val,  int *trouve, int *a);
int region(int i);
void DispEtablissements();
int Chargement_Init();
int Affichage_FEnseignat();
int Codef(FILE **in, char key[cchaine], char nout[cchaine]);
int Decodef(char file[cchaine], char key[cchaine], char nout[cchaine]);
int SupEngMat(Block *buff, int i, int j );
int InsErgMat( SEns e);
void Affichage_Index(fileInd f);
int SupEns();
void Temp();
int SupSpec();
int AffichageParRegion();
int ConsultationEtabAnc();
void DispWilaya();
void Q41();
void DispEng(SEns e, int i, int j);
void AffichageUnEng();
void ModificationEns();
void Disp3Blocks();
void FisrtScreen();
void DispEntete();
void Dispstats();
void Guid ();
void MenuPrimaire();


#endif // TP2_CHABANE_TEBBAL_G01_H_INCLUDED
