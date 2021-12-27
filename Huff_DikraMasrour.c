
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <math.h>

//var globale, le reste de l'array est rempli de 0
char str[100]; 
char strNew[100];
float freq[128]; //courvre la majorite des symboles utilisés (ASCII)
float freqNew[128];

// la structure pour represente un element
struct element { 
    char charactere; // pour contenir le caractère
    float freq; // frequence d'apparetion du caractere dans la chaine
    struct element *gauche, *droite;  // les fils gauche et droite de l'element
}; 
  
// la structure pour representer un tas
typedef struct  { 
  
    struct element** tab;  // le tableau (ici c'est un tableau des element et pas des entiers)
    int mem; //Nombre maximum d'éléments dans le tableau
    int taille; // nombre d’éléments de l'arbre
}*noeud; 
  



// fonction pour creer un nouveau element 
struct element* nouveauElement(char charactere, float freq){ 

    struct element* temp = (struct element*)malloc (sizeof(struct element)); 
    temp->gauche = temp->droite = NULL; 
    temp->charactere = charactere; 
    temp->freq = freq;   
    return temp; 
} 
  
//fonction pour créer un nouveau noeud (tas)
noeud nouveauTas(int mem){ 
  
    noeud x = malloc(sizeof( noeud)); 
    x->taille = 0;   
    x->mem = mem;   
    x->tab  = (struct element**)malloc(x->mem * sizeof(struct element*));  // allocation dynamique de la memoire pour le tableau
    return x; 
} 
  
// fonction permettenat l'échange d'elements similaire à celle des tableaux
void echangenoeud(struct element** a, struct element** b) { 
  
    struct element* t = *a; 
    *a = *b; 
    *b = t; 
} 
  
 //fonction entasser Min
void entasserMin(noeud noeud, int idx){ 
  
    int indiceMin = idx; 
    int gauche = 2 * idx + 1; 
    int droite = 2 * idx + 2; 
  
    if (gauche < noeud->taille && noeud->tab[gauche]->freq < noeud->tab[indiceMin]->freq) 
        indiceMin = gauche; 
  
    if (droite < noeud->taille && noeud->tab[droite]->freq < noeud->tab[indiceMin]->freq) 
        indiceMin = droite; 
  
    if (indiceMin != idx) { 
        echangenoeud(&noeud->tab[indiceMin],&noeud->tab[idx]); 
        entasserMin(noeud, indiceMin); 
    } 
} 
  

  
//fonction pour extraire l'element a valeur min (racide) du tas
struct element* extraireMin(noeud noeud) { 
  
    struct element* temp = noeud->tab[0]; 
    noeud->tab[0] = noeud->tab[noeud->taille - 1]; 
  
    --(noeud->taille); 
    entasserMin(noeud, 0);  // reconstruction du tas min
  
    return temp; 
} 
  
// fonction pour inserer un element dans le tas min
void insererTasMin(noeud noeud, struct element* element) { 
  
    ++noeud->taille; 
    int i = noeud->taille - 1; 
  
    while (i && element->freq < noeud->tab[(i - 1) / 2]->freq) { 
  
        noeud->tab[i] = noeud->tab[(i - 1) / 2]; 
        i = (i - 1) / 2; 
    } 
  
    noeud->tab[i] = element; 
} 
  
 // Construction d'un Tas min
void construireTasMin(noeud noeud) { 
  
    int n = noeud->taille - 1; 
    int i; 
  
    for (i = (n - 1) / 2; i >= 0; --i) 
        entasserMin(noeud, i); 
} 
  
//afficher les elements du tableau tab[]
int afficherTab(int *tab, int n, FILE *file) { 
    int i; 
    for (i = 0; i < n; ++i)
    {
        printf("%d", tab[i]); 
        fprintf(file, "%d", tab[i]);

    }
    fprintf(file, "\n");
            printf("\n"); 
return *tab;} 

/* Utile: max(p, q) renvoie le maximum entre ces deux entiers */
int max(int p, int q) {
  return (p > q) ? p : q;
}

/* hauteur(x) renvoie la hauteur de l'arbre x */
int hauteur(struct element * x){
  if (x) {
    return 1 + max(hauteur(x->gauche), hauteur(x->droite));
  }
  else return 0;
}

/* cardinal(x) renvoie le nombre de noeuds de l'arbre x */
int cpt = 0;  //compteur de noeuds
int cardinal(struct element * x)
  {

      if(x != NULL){
          return cardinal(x->gauche)+cardinal(x->droite)+1;
      }
      return 0;
                
  }

// tester si l'element est une feuile ou pas 
int estFeuille(struct element* racine){ 
  
    return !(racine->gauche) && !(racine->droite); 
} 
  
//creation du tas
noeud tasMin(char *charactere, float *freq, int taille){ 
  
    noeud noeud = nouveauTas(taille); 
  
    for (int i = 0; i < taille; ++i) 
        noeud->tab[i] = nouveauElement(charactere[i], freq[i]); 
        noeud->taille = taille; 
    construireTasMin(noeud); 
  
    return noeud; 
} 
  
// fonction pour construire l'arbre de Huffman 
struct element* Huffman(char *charactere, float *freq, int taille){ 
    
    struct element *gauche, *droite, *top;   
    //top correspond au noeud de fusion, il ne contient pas de caractere

    noeud noeud = tasMin(charactere, freq, taille); 
  
    
    while (!(noeud->taille == 1)) { 
       
        gauche = extraireMin(noeud); 
        droite = extraireMin(noeud); 
        top = nouveauElement('*', gauche->freq + droite->freq);  // ici puique le noeud qui contient la somme des fréquence n'a pas de caractère, on lui donne * comme caractère
        top->gauche = gauche; 
        top->droite = droite; 
  
        insererTasMin(noeud, top); 
    } 
    return extraireMin(noeud); 
} 
  

// afficher le code
void afficherCodes(struct element* racine, int *tab, int top){ 
    FILE *file = fopen("codedData.txt", "a+");
    if (racine->gauche) {   
        tab[top] = 0; 
        afficherCodes(racine->gauche, tab, top + 1); 
    } 

    if (racine->droite) { 
  
        tab[top] = 1; 
        afficherCodes(racine->droite, tab, top + 1); 
    } 
    if (estFeuille(racine)) {   
        printf("%c: ", racine->charactere); 
        fprintf(file, "%c: ", racine->charactere); 
        afficherTab(tab, top, file ); 
    } 
    fclose(file);
} 
int i = 0; //pour assurer la continuite du parcours
void decode(struct element* racine, int *tab, int top, char *str){
    FILE *dataFile = fopen("data.txt","a+"); 
    fgets(str, 500, dataFile); //reinitialisation de *str
    FILE *file = fopen("codedData.txt", "a+");
    int j = strlen(str), c;

    //boucle de parcours et identification des feuilles et leur affichage dans l'ordre de la chaine d'origine
        for(j ; j>0 ; j--){
            if (racine->gauche) 
            {   
                tab[top] = 0;
                decode(racine->gauche, tab, top + 1,str);
            }

            if (racine->droite) 
            { 
                tab[top] = 1;
                decode(racine->droite, tab, top + 1,str); 
            }

            if (estFeuille(racine) && racine->charactere == str[i] ) 
            {
                ++i;
                fprintf(file,"%c ", racine->charactere);
                printf("[%c] ", racine->charactere);
                for (c = 0; c < top; ++c)
                {
                    fprintf(dataFile, "%d", tab[c]);
                    printf("%d", tab[c]); 
                } 
                fprintf(dataFile, "\t");
            }
        }
fclose(dataFile);
fclose(file);
}




char eliminerRepetition(char *str, char *strNew)
{
    char flag[256] = {0}; //Assuming sizeof char is 8. So max 256 different characters.
    int  i = 0;
    int  j = 0;

    for(i=0; str[i] != '\0'; i++)
    {
        if(0 == flag[str[i]]) //Check if character is already found.
        {
            flag[str[i]] = 1; //If a character is found for the first time, enable corresponding flag.
            strNew[j] = str[i];
            j++;
        }
        
    }
    strNew[j] = '\0';
    return *strNew;
}


void calculFreq (char * str, float * freq, char *strNew){
    //float freq[128] = {0}; //128 couvre la majorité des code des symboles utilisés (en ASCII) + mettre à 0 les cases correspondantes aux char de chaine
    int i = 0, j = 0;
    char test;
    //calcul des frequences
    while (str[j])
    {
        i=0;
        while (str[i])
        {   
            test = str[j];
            if (str[i] == test)
            {
                freq[test]++;
                i++;
            }
            else
            {
                i++;
            }   
        }
        j++;
    }

    eliminerRepetition(str, strNew);

    FILE * codeFile = fopen("codedData.txt", "a+");
    fprintf(codeFile, "Les Fréquences :\n");
    //affichage ASCII-Caractere-Frequence
    for (int i = 0; i < 128 && str[i] != '\0' ; i++)
    {   
        if (freq[strNew[i]] != 0)
        {
            fprintf(codeFile, "%d - %c : %g\n", strNew[i], strNew[i], sqrt(freq[strNew[i]]));
            printf("%d - %c : %g\n", strNew[i], strNew[i], sqrt(freq[strNew[i]]) );
        }   
    }  
    fclose(codeFile);
}




int main(){ 
    int i = 0, top = 0, h, c, n;
    struct element* rootNode;
    int codes[h], tab[n];

    FILE *dataFile = fopen("data.txt","r"); 

    //lecture à partir du fichier texte
    fgets(str, 500, dataFile);
    printf("le fichier contient : %s\n", str );
    calculFreq(str, freq, strNew);
    printf("La chaine sans repetitions :[%s] de taille %d \n", strNew, strlen(strNew));
  
    //organiser tableau de frequences dans un nouveau tableau compact
    for(i = 0; strNew[i] != '\0'; i++)
        freqNew[i] = sqrt(freq[strNew[i]]);

    
    //contruction de l'arbre de Huffman et affichage des codes
    n = strlen(strNew);
    rootNode = Huffman(strNew, freqNew, n); 
    h = hauteur(rootNode);//le nombre de bits ne depasse pas la hauteur du tas

    //Affichage + sauvegarde dans un nouveau fichier codedData.txt
    FILE *codeFile = fopen("codedData.txt", "a+");
    fprintf(codeFile, "\tCodage de HUFFMAN\nLes Codes :\n");
    fclose(codeFile);
    afficherCodes(rootNode, codes, top );
    top = 0; //reinitialisation de top
    dataFile = fopen("data.txt", "a+");
    codeFile = fopen("codedData.txt", "a+");
    fprintf(codeFile, "\nDecodage :\n");
    fclose(codeFile);
    decode(rootNode, tab, top, str);

    


    
    return 0; 
} 


