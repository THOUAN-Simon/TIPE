#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>





//[Section structures]

//structure contenant une piece du jeu, avec des informations utiles à sa manipulation

struct piece_blokus { 
  char** piece;         
  int nb_cases;
  int taille;
  int nb_rotations;
  int id;
  bool joue;
  char joueur;
};

typedef struct piece_blokus p;

//retourne un nouveau p* contenant les mêmes données que source

p* deep_copy_p (p* source){
    p* resultat = malloc(sizeof(p));
    resultat->nb_cases = source->nb_cases;
    resultat->taille = source->taille;
    resultat->id = source->id;
    resultat->joue = source->joue;
    resultat->joueur = source->joueur;
    resultat->piece = malloc(sizeof(char*)*source->taille);
    for (int i=0; i<source->taille; i++){
        resultat->piece[i] = malloc(sizeof(char)*source->taille);
    }
    for (int i=0; i<source->taille; i++){
        for (int j=0; j<source->taille; j++){
            resultat->piece[i][j] = source->piece[i][j];
        }
    }    
    return resultat;
}

//structure représentant un ensembe de pièces typique du jeu blokus

struct ensemble_pieces { 
    p pieces[42];
};

typedef struct ensemble_pieces ensemble;

char** copie (char** source, int taille){
    char** result = malloc(sizeof(char*)*taille);
    for (int i=0; i<taille; i++){
        result[i] = malloc(sizeof(char)*taille);
    }
    for (int i=0; i<taille; i++){
        for (int j=0; j<taille; j++){
            result[i][j] = source[i][j];
        }
    }
    return result;
}

ensemble initialise_ensemble(char*** toutes_pieces, int**dimensions, char joueur){
    ensemble final;
    for (int i=0; i<21; i++){
        if (joueur == '.'){
            final.pieces[i].joueur = '.';
            final.pieces[i].piece = copie(toutes_pieces[i],dimensions[i][1]);
        }
        else{
            final.pieces[i].piece = malloc(sizeof(char*)*dimensions[i][1]);
            for (int m=0; m<dimensions[i][1]; m++){
                final.pieces[i].piece[m] = malloc(sizeof(char)*dimensions[i][1]);
            }
            final.pieces[i].joueur = '&';
            for(int k=0; k<dimensions[i][1]; k++){
                for(int l=0; l<dimensions[i][1]; l++){
                    if (toutes_pieces[i][k][l] == '.'){
                        final.pieces[i].piece[k][l] = '&';
                    }
                    else{
                        final.pieces[i].piece[k][l] = ' ';
                    }
                }
            }
        }
        final.pieces[i].nb_cases = dimensions[i][0];
        final.pieces[i].taille = dimensions[i][1];
        final.pieces[i].nb_rotations = dimensions[i][2];
        final.pieces[i].id = dimensions[i][3];
        final.pieces[i].joue = false;
    }
    for (int j=21; j<42; j++){
        int i = j-21;
        if (joueur == '.'){
            final.pieces[j].joueur = '.';
            final.pieces[j].piece = copie(toutes_pieces[i],dimensions[i][1]);
        }
        else{
            final.pieces[j].piece = malloc(sizeof(char*)*dimensions[i][1]);
            for (int m=0; m<dimensions[i][1]; m++){
                final.pieces[j].piece[m] = malloc(sizeof(char)*dimensions[i][1]);
            }
            final.pieces[j].joueur = '&';
            for(int k=0; k<dimensions[i][1]; k++){
                for(int l=0; l<dimensions[i][1]; l++){
                    if (toutes_pieces[i][k][l] == '.'){
                        final.pieces[j].piece[k][l] = '&';
                    }
                    else{
                        final.pieces[j].piece[k][l] = ' ';
                    }
                }
            }
        }
        final.pieces[j].nb_cases = dimensions[i][0];
        final.pieces[j].taille = dimensions[i][1];
        final.pieces[j].nb_rotations = dimensions[i][2];
        final.pieces[j].id = dimensions[i][3];
        final.pieces[j].joue = false;
    }
    return final;
}


//structure permettant de faire une liste de couples d'entiers, et de vérifier l'existence du point désigné dans la grille 20x20

struct dot { 
  int ligne;           
  int colonne;    
  bool existe;
};

typedef struct dot dot;

//initialise un point de la grille aux coordonnées (l,c)

dot initialise_point(int l, int c){
    dot result;
    result.ligne = l;
    result.colonne = c;
    result.existe = true;
    return result;
}

//Fonctions qui calculent, pour un point(i,j) du plan donné, les coordonnées du point en haut, en bas, à gauche, à droite ou l'une des 4 diagonales
//du point donné, si celles-ci appartiennent à la grille. (ie les 8 points autour du point passé en argument).

dot haut(int l, int c){
    dot point;
    if (l-1>=0){
        point.existe = true;
        point.ligne = l-1;
        point.colonne = c;
    }
    else{
        point.existe = false;
    }
    return point;
}

dot bas(int l, int c){
    dot point;
    if  (l+1<20){
        point.existe = true;
        point.ligne = l+1;
        point.colonne = c;
    }
    else{
        point.existe = false;
    }
    return point;
}

dot gauche(int l, int c){
    dot point;
    if (c-1>=0){
        point.existe = true;
        point.ligne = l;
        point.colonne = c-1;
    }
    else{
        point.existe = false;
    }
    return point;   
}

dot droit(int l, int c){
    dot point;
    if (c+1<20){
        point.existe = true;
        point.ligne = l;
        point.colonne = c+1;
    }
    else{
        point.existe = false;
    }
    return point;   
}

dot diag_NW(int l, int c){
    dot point;
    if (l-1>=0 && c-1>=0){
        point.existe = true;
        point.ligne = l-1;
        point.colonne = c-1;
    }
    else{
        point.existe = false;
    }
    return point;
}

dot diag_NE(int l, int c){
    dot point;
    if  (l-1>=0 && c+1<20){
        point.existe = true;
        point.ligne = l-1;
        point.colonne = c+1;
    }
    else{
        point.existe = false;
    }
    return point;
}

dot diag_SW(int l, int c){
    dot point;
    if (l+1<20 && c-1>=0){
        point.existe = true;
        point.ligne = l+1;
        point.colonne = c-1;
    }
    else{
        point.existe = false;
    }
    return point;   
}

dot diag_SE(int l, int c){
    dot point;
    if (l+1<20 && c+1<20){
        point.existe = true;
        point.ligne = l+1;
        point.colonne = c+1;
    }
    else{
        point.existe = false;
    }
    return point;   
}

//Type permettant de connaître les points diagonaux (leurs positions et leur nombre)

struct points_diagonaux{
    dot points[200];   //la moitié du plateau ne peut clairement pas être composée uniquement de points diagonaux
    int nb;
};

typedef struct points_diagonaux p_diag;

//Type permettant de modéliser un coup : une pièce de type p et une position sur la grille (couple d'entiers (ligne,colonne));

struct coup{
    p piece;
    int ligne;
    int colonne;
};

typedef struct coup coup;

//[Section gestion du jeu]



//initialise la grille

char** initialise_grille(){
    char** tab;
    tab = malloc(sizeof(void*)*20);
    for (int i = 0; i < 20; i ++){
        tab[i] = malloc(sizeof(char)*20);
        for (int j = 0; j < 20; j++){
            tab[i][j] = ' ';
        }
    }
    return tab;
} 


//affiche la grille

void afficher (char** grille){
    for (int i = 0; i<20; i++){
        printf("---------------------------------------------------------------------------------");
        printf("\n");
        for (int j = 0; j<20; j++){
            printf("|");
            if (grille[i][j] == '.'){
                printf("\033[32m\033[7m");
                printf(" %c ", ' ');
                printf("\033[0m");
            }
            else{
                if (grille[i][j] == '&'){
                        printf("\033[34m\033[7m");
                        printf(" %c ", ' ');
                        printf("\033[0m");
                }
                else{
                    printf("\033[30m\033[7m");
                    printf(" %c ", ' ');
                    printf("\033[0m");
                }
            }
        }
        printf("|");
        printf("\n");
    }
    printf("---------------------------------------------------------------------------------");
    printf("\n");
}


//extrait les pièces du fichier utilisé pour les stocker

int** recup_piece (FILE** fd, int* cases, int* ord, int* rot, int* id){
    fscanf(*fd, "%d,%d,%d,%d:", cases, ord, rot, id);
    int** piece;
    piece = malloc(sizeof(int*)*(*ord));
    for (int i = 0; i < (*ord); i ++){
        piece[i] = malloc(sizeof(int)*(*ord));
    }
    for (int i = 0; i < (*ord); i ++){
        for (int j = 0; j < (*ord); j ++){
            fscanf(*fd, "%d,", &piece[i][j]);
        }
    }
    return piece;
}


//Convertit les matrices représentant les pièces depuis leur type d'origine (int) vers le type utilisé (char)

char*** convertir (int*** pieces, int** dimensions){
    char*** result =  malloc(sizeof(char**)*21);
    for (int p = 0; p<21; p++){
        result[p] = malloc(sizeof(char*)*dimensions[p][1]);
        for (int k = 0; k<dimensions[p][1]; k++){
            result[p][k] = malloc(sizeof(char)*dimensions[p][1]);
        }
    }
    for (int k = 0; k<21; k++){
        for(int i = 0; i<dimensions[k][1]; i++){
            for(int j = 0; j<dimensions[k][1]; j++){
                if (pieces[k][i][j] == 1){
                    result[k][i][j] = '.';
                }
                else{
                    result[k][i][j] = ' ';
                }
            }
        }
    }
    return result;
}


//Fonction utilisée dans la fonction pour translater une pièce (cela sert à chercher une autre façon de poser une pièce si cela est possible,
//et c'est cette fonction qui décide de cela). La fonction revoie un entier qui est nombre de colonnes vides à droite de la pièce
//passée en argument.

int detect_col_vide_d (p* a_placer){
    int final = 0;
    for (int i = 0; i<a_placer->taille; i++){
        int c = 0;
        for(int j = 0; j<a_placer->taille; j++){
            if (a_placer->piece[j][a_placer->taille-1-i] == ' '){
                c++;
            }
        }
        if (c == a_placer->taille){
            final++;
        }
        else {
            return final;
        }
    }
}


//Même principe que detect_col_vide_d, mais cette fois, c'est le nombre de colones vides à gauche qui est compté. On utilise cette fonction dans
//placable_exterieur.

int detect_col_vide_g (p* a_placer){
    int final = 0;
    for (int i = 0; i<a_placer->taille; i++){
        int c = 0;
        for(int j = 0; j<a_placer->taille; j++){
            if (a_placer->piece[j][i] == ' '){
                c++;
            }
        }
        if (c == a_placer->taille){
            final++;
        }
        else {
            return final;
        }
    }
}


//Opération matricielle d'échange de colonnes sur une pièce (sert dans la fonction rotation)

void echange_c (p* a_modifier, int c1, int c2){
    char temp;
    for (int i=0; i<a_modifier->taille; i++){
        temp = a_modifier->piece[i][c1];
        a_modifier->piece[i][c1] = a_modifier->piece[i][c2];
        a_modifier->piece[i][c2] = temp;
    }
}

//Opération matricielle de "miroir" (sert dans la fonction rotation) ; on y discute le cas des pièces à symétries particluières.

void miroir (p* a_modifier){
    if (a_modifier->taille == 2 || a_modifier->taille == 3){
        echange_c(a_modifier,0,a_modifier->taille-1);
    }
    if (a_modifier->taille == 4 || a_modifier->taille == 5){
        echange_c(a_modifier,0,a_modifier->taille-1);
        echange_c(a_modifier,1,a_modifier->taille-2);
    }
}



//Opération matricielle de rotation à 90°

void rotation_90 (p* a_modifier){
    int t = a_modifier->taille;
    char** contenu = malloc(sizeof(char*)*t);
    for (int k=0; k<t; k++){
        contenu[k] = malloc(sizeof(char)*t);
    }
    for (int i=0; i<t; i++){
        for (int j=0; j<t; j++){
            contenu[i][j] = a_modifier->piece[i][j];
        }
    }
    for (int i=0; i<t; i++){
        for (int j=0; j<t; j++){
            a_modifier->piece[j][(a_modifier->taille)-i-1] = contenu[i][j];
        }
    }
}


//Fonctions qui servent pour uniformiser le format des matrices des pièces, de sorte que chacune soit contre le côté haut et le côté droit
//après application de la fonction rotation

void translation_c(p* a_modifier){
    int col_vides_d = detect_col_vide_d(a_modifier);
    int col_vides_g = detect_col_vide_g(a_modifier);
    int taille = a_modifier->taille;
    if (col_vides_d != 0){
        if (taille-(col_vides_d + col_vides_g) == 1){
            echange_c(a_modifier,col_vides_g,taille-1);
        } 
        if (taille-(col_vides_d + col_vides_g) == 2){
            echange_c(a_modifier,taille-col_vides_d-1,taille-1);
            echange_c(a_modifier,taille-col_vides_d-2,taille-2);
        }
    }
}


void translation_l(p* a_modifier){
    rotation_90(a_modifier);
    translation_c(a_modifier);
    rotation_90(a_modifier);
    rotation_90(a_modifier);
    rotation_90(a_modifier);
}


void normalise (p* a_modifier){
    translation_c(a_modifier);
    translation_l(a_modifier);
}


//Vérifie si une pièce n'entre pas en conflit avec une autre lors du placement sur le plateau (la vérification de la legitimité des coordonnées a déjà été faite...)

bool placable_interieur (char** grille, int posx, int posy, p a_placer){
    for (int i = 0; i<a_placer.taille; i++){
        for (int j = 0; j<a_placer.taille; j++){
            if ((grille[posx+i][posy+j] == '.' && a_placer.piece[i][j] != ' ') || (grille[posx+i][posy+j] == '&' && a_placer.piece[i][j] != ' ')){
                return false;
            }
        }
    }
    return true;
}

//Vérifie si une pièce peut être placée sur le plateau en termes de coordonées, sans qu'un morceau n'en sorte. 
//A noter que la fonction ne tient pas compte des placements qui font déborder du vide : ces placements de pièces seront possibles 
//plus tard, lorsque le programme parcourera les possibilités de translation.

bool placable_exterieur (p a_placer, int posx, int posy){
    if (posx>19 || posx<0 || posy>19 || posy<0){
        return false;
    }
    else{
        for (int i = 0; i<a_placer.taille-1; i++){
            if (posx == 19-i || posy == 19-i){
                return false;
            }
        }
        return true;
    }
}

p** rotation (p* a_placer){
    int rot = a_placer->nb_rotations;
    int id = a_placer->id;
    p** result = malloc(sizeof(p*)*rot);
    for (int i=0; i<rot; i++){
        result[i] = malloc(sizeof(p)*rot);
    }
    for (int i=0; i<rot; i++){
        result[i] = deep_copy_p(a_placer);
    }
    if (rot == 2){
        rotation_90(result[1]);
        normalise(result[1]);
    }
    if (rot == 4 && (id == 8 || id == 13)){
        rotation_90(result[1]);
        miroir(result[2]);
        rotation_90(result[3]);
        miroir(result[3]);
        normalise(result[1]);
        normalise(result[2]);
        normalise(result[3]);
    }
    if (rot == 4 && id != 8 && id != 13){
        rotation_90(result[1]);
        rotation_90(result[2]);
        rotation_90(result[2]);
        rotation_90(result[3]);
        rotation_90(result[3]);
        rotation_90(result[3]);
        normalise(result[1]);
        normalise(result[2]);
        normalise(result[3]);
    }
    if (rot == 8){
        rotation_90(result[1]);
        rotation_90(result[2]);
        rotation_90(result[2]);
        rotation_90(result[3]);
        rotation_90(result[3]);
        rotation_90(result[3]);
        miroir(result[4]);
        miroir(result[5]);
        rotation_90(result[5]);
        miroir(result[6]);
        rotation_90(result[6]);
        rotation_90(result[6]);
        miroir(result[7]);
        rotation_90(result[7]);
        rotation_90(result[7]);
        rotation_90(result[7]);
        normalise(result[1]);
        normalise(result[2]);
        normalise(result[3]);
        normalise(result[4]);
        normalise(result[5]);
        normalise(result[6]);
        normalise(result[7]);
    }    
    a_placer->joue = true;
    return result;
}

p** translation (p* a_placer){
    int col_vides = detect_col_vide_g(a_placer);
    p** result = malloc(sizeof(p*)*col_vides);
    for (int i=0; i<col_vides; i++){
        result[i] = malloc(sizeof(p)*col_vides);
    }
    for (int i=0; i<=col_vides; i++){
        result[i] = deep_copy_p(a_placer);
    }
    if (col_vides == 1 && a_placer->taille == 2){
        echange_c(result[1],0,1);
    }
    if (col_vides == 1 && a_placer->taille == 3){
        echange_c(result[1],0,1);
        echange_c(result[1],1,2);
    }
    if (col_vides == 2 && a_placer->taille == 3){
        echange_c(result[1],1,2);
        echange_c(result[2],0,2);
    }
    if (col_vides == 2 && a_placer->taille == 4){
        echange_c(result[1],1,2);
        echange_c(result[1],2,3);
        echange_c(result[2],0,2);
        echange_c(result[2],1,3);
    }
    if (col_vides == 3){
        echange_c(result[1],2,3);
        echange_c(result[2],1,3);
        echange_c(result[3],0,3);
        
    }
    if (col_vides == 4){
        echange_c(result[1],3,4);
        echange_c(result[2],2,4);
        echange_c(result[3],1,4);
        echange_c(result[4],0,4);
    }
    a_placer->joue = true;
    return result;
}


//Vérifie si la règle du jeu est respectée sur la grille passée en argument, avec l'ajout de la nouvelle pièce.
//(on suppose donc que la grille est correcte au sens de la règle du jeu avant ajout)

bool verif (char** grille, int posl, int posc, p a_placer, int tour){
    if(tour > 0 && grille[0][0] != '.'){
        return false;
    }
    if(tour > 0 && grille[19][19] != '&'){
        return false;
    }
    if(grille[posl][posc] != a_placer.joueur){
        return false;
    }
    for(int l=0; l<a_placer.taille; l++){
        for(int c=0; c<a_placer.taille; c++){
            if(a_placer.piece[l][c] != ' '){
                if(haut(l,c).existe){
                    if(a_placer.piece[l][c] == grille[haut(posl+l,posc+c).ligne][haut(posl+l,posc+c).colonne]){
                        return false;
                    }
                }
                if(bas(l,c).existe){
                    if(a_placer.piece[l][c] == grille[bas(posl+l,posc+c).ligne][bas(posl+l,posc+c).colonne]){
                        return false;
                    }
                }
                if(gauche(l,c).existe){
                    if(a_placer.piece[l][c] == grille[gauche(posl+l,posc+c).ligne][gauche(posl+l,posc+c).colonne]){
                        return false;
                    }
                }
                if(droit(l,c).existe){
                    if(a_placer.piece[l][c] == grille[droit(posl+l,posc+c).ligne][droit(posl+l,posc+c).colonne]){
                        return false;
                    }
                }
            }
        }
    }
    int nb_coins = 0;
    dot h_g = diag_NW(posl,posc);
    dot h_d = diag_NE(posl,posc+a_placer.taille-1);
    dot b_g = diag_SW(posl+a_placer.taille-1,posc);
    dot b_d = diag_SE(posl+a_placer.taille-1,posc+a_placer.taille-1);
    if(h_g.existe){
        if(grille[h_g.ligne][h_g.colonne] == a_placer.joueur){
            nb_coins++;
        }
    }
    if(h_d.existe){
        if(grille[h_d.ligne][h_d.colonne] == a_placer.joueur){
            nb_coins++;
        }
    }
    if(b_g.existe){
        if(grille[b_g.ligne][b_g.colonne] == a_placer.joueur){
            nb_coins++;
        }
    }
    if(b_d.existe){
        if(grille[b_d.ligne][b_d.colonne] == a_placer.joueur){
            nb_coins++;
        }
    }
    if(nb_coins == 0){
        return false;
    }
    else{
        return true;
    }
}


//Ajoute la pièce sur le plateau

bool ajoute (char** grille, int posx, int posy, p* a_placer, int tour){
    if (!(placable_exterieur(*a_placer, posx, posy))){
        return 0;
    }
    if (!(placable_interieur(grille,posx,posy,*a_placer))){
        return 0;
    }
    if (!(verif(grille,posx,posy,*a_placer,tour))){
        return 0;
    }
    else{
        for (int i = 0; i<a_placer->taille; i++){
            for(int j = 0; j<a_placer->taille; j++){
                if (grille[posx+i][posy+j] == ' ' && a_placer->piece[i][j] == '.'){
                    grille[posx+i][posy+j] = a_placer->piece[i][j];
                }
                else{
                    if(grille[posx+i][posy+j] == ' ' && a_placer->piece[i][j] == '&'){
                        grille[posx+i][posy+j] = a_placer->piece[i][j];
                    }
                }
            }   
        }
        a_placer->joue = true;
        return 1;
    }
}

//Supprime la pièce précédemment placée passée en argument, aux coordonnées données (sert dans le minmax, donc on aura la garantie
//que la pièce à effectivement été placée.)

void supprime (char** grille, int posx, int posy, p* a_supprimer){
    bool verif = true;
    for (int i=0; i<a_supprimer->taille; i++){
        for (int j=0; j<a_supprimer->taille; j++){
            if(a_supprimer->piece[i][j] == a_supprimer->joueur && grille[posx+i][posy+j] != a_supprimer->joueur){
                verif = false;
            }
        }
    }
    if(verif){
        for (int i=0; i<a_supprimer->taille; i++){
            for (int j=0; j<a_supprimer->taille; j++){
                if(grille[posx+i][posy+j] == a_supprimer->piece[i][j]){
                    grille[posx+i][posy+j] = ' ';
                }
            }
        }
    }
}

//renvoi le score du joueur sur la grille passée en argument.

int score_joueur(char** grille, char joueur){
    int result = 0;
    for(int i=0; i<20; i++){
        for(int j=0; j<20; j++){
            if(grille[i][j] == joueur){
                result++;
            }
        }
    }
    return result;
}




//[Section Heuristiques et minimax]



//Fonction qui renvoie un tableau de couples d'entiers représentant une ligne, une colonne, sur laquelle on peut potentiellement
//placer une pièce alliée

p_diag points_diagonaux (char** plateau, char type){
    int indice = 0;
    p_diag result;
    for(int i=0; i<20; i++){
        for(int j=0; j<20; j++){
            if(plateau[i][j] == ' '){
                if(haut(i,j).existe && bas(i,j).existe && gauche(i,j).existe && droit(i,j).existe){
                    if(plateau[haut(i,j).ligne][haut(i,j).colonne] == ' ' && plateau[bas(i,j).ligne][bas(i,j).colonne] == ' ' && plateau[gauche(i,j).ligne][gauche(i,j).colonne] == ' ' && plateau[droit(i,j).ligne][droit(i,j).colonne] == ' '){
                        if(plateau[diag_NE(i,j).ligne][diag_NE(i,j).colonne] == type || plateau[diag_NW(i,j).ligne][diag_NW(i,j).colonne] == type || plateau[diag_SE(i,j).ligne][diag_SE(i,j).colonne] == type || plateau[diag_SW(i,j).ligne][diag_SW(i,j).colonne] == type){
                            result.points[indice] = initialise_point(i,j);
                            indice++;
                        }
                    }
                }
                if(!haut(i,j).existe && bas(i,j).existe && gauche(i,j).existe && droit(i,j).existe){
                    if(plateau[bas(i,j).ligne][bas(i,j).colonne] == ' ' && plateau[gauche(i,j).ligne][gauche(i,j).colonne] == ' ' && plateau[droit(i,j).ligne][droit(i,j).colonne] == ' '){
                        if(plateau[diag_SE(i,j).ligne][diag_SE(i,j).colonne] == type || plateau[diag_SW(i,j).ligne][diag_SW(i,j).colonne] == type){
                            result.points[indice] = initialise_point(i,j);
                            indice++;
                        }
                    }
                }
                if(haut(i,j).existe && !bas(i,j).existe && gauche(i,j).existe && droit(i,j).existe){
                    if(plateau[haut(i,j).ligne][haut(i,j).colonne] == ' ' && plateau[gauche(i,j).ligne][gauche(i,j).colonne] == ' ' && plateau[droit(i,j).ligne][droit(i,j).colonne] == ' '){
                        if(plateau[diag_NE(i,j).ligne][diag_NE(i,j).colonne] == type || plateau[diag_NW(i,j).ligne][diag_NW(i,j).colonne] == type){
                            result.points[indice] = initialise_point(i,j);
                            indice++;
                        }
                    }
                }
                if(haut(i,j).existe && bas(i,j).existe && !gauche(i,j).existe && droit(i,j).existe){
                    if(plateau[haut(i,j).ligne][haut(i,j).colonne] == ' ' && plateau[bas(i,j).ligne][bas(i,j).colonne] == ' ' && plateau[droit(i,j).ligne][droit(i,j).colonne] == ' '){
                        if(plateau[diag_NE(i,j).ligne][diag_NE(i,j).colonne] == type || plateau[diag_SE(i,j).ligne][diag_SE(i,j).colonne] == type){
                            result.points[indice] = initialise_point(i,j);
                            indice++;
                        }
                    }
                }
                if(haut(i,j).existe && bas(i,j).existe && gauche(i,j).existe && !droit(i,j).existe){
                    if(plateau[haut(i,j).ligne][haut(i,j).colonne] == ' ' && plateau[bas(i,j).ligne][bas(i,j).colonne] == ' ' && plateau[gauche(i,j).ligne][gauche(i,j).colonne] == ' '){
                        if(plateau[diag_NW(i,j).ligne][diag_NW(i,j).colonne] == type || plateau[diag_SW(i,j).ligne][diag_SW(i,j).colonne] == type){
                            result.points[indice] = initialise_point(i,j);
                            indice++;
                        }
                    }
                }
                if(!haut(i,j).existe && bas(i,j).existe && !gauche(i,j).existe && droit(i,j).existe){
                    if(plateau[bas(i,j).ligne][bas(i,j).colonne] == ' ' && plateau[droit(i,j).ligne][droit(i,j).colonne] == ' '){
                        if(plateau[diag_SE(i,j).ligne][diag_SE(i,j).colonne] == type){
                            result.points[indice] = initialise_point(i,j);
                            indice++;
                        }
                    }
                }
                if(!haut(i,j).existe && bas(i,j).existe && gauche(i,j).existe && !droit(i,j).existe){
                    if(plateau[bas(i,j).ligne][bas(i,j).colonne] == ' ' && plateau[gauche(i,j).ligne][gauche(i,j).colonne] == ' '){
                        if(plateau[diag_SW(i,j).ligne][diag_SW(i,j).colonne] == type){
                            result.points[indice] = initialise_point(i,j);
                            indice++;
                        }
                    }
                }
                if(haut(i,j).existe && !bas(i,j).existe && !gauche(i,j).existe && droit(i,j).existe){
                    if(plateau[haut(i,j).ligne][haut(i,j).colonne] == ' ' && plateau[droit(i,j).ligne][droit(i,j).colonne] == ' '){
                        if(plateau[diag_NE(i,j).ligne][diag_NE(i,j).colonne] == type){
                            result.points[indice] = initialise_point(i,j);
                            indice++;
                        }
                    }
                }
                if(haut(i,j).existe && !bas(i,j).existe && gauche(i,j).existe && !droit(i,j).existe){
                    if(plateau[haut(i,j).ligne][haut(i,j).colonne] == ' ' && plateau[gauche(i,j).ligne][gauche(i,j).colonne] == ' '){
                        if(plateau[diag_NW(i,j).ligne][diag_NW(i,j).colonne] == type){
                            result.points[indice] = initialise_point(i,j);
                            indice++;
                        }
                    }
                }
            }
        }
    }
    result.nb = indice;
    return result;
}


int rapporte_points (p piece){
    return piece.nb_cases;
}

int rubik_distance (p piece){
    if (piece.id == 18 || piece.id == 10 || piece.id == 20){
        return 5;
    }
    if(piece.id == 5 || piece.id == 19){
        return 4;
    }
    if(piece.id == 0){
        return 2;
    }
    else{
        return 2*piece.taille-detect_col_vide_g(&piece);
    }
}

int maximise_p_diag (p piece, char** grille, int tour){
    p_diag dots_temp = points_diagonaux(grille, piece.joueur);
    int tours = dots_temp.nb;
    int max = tours;
    for(int k=0; k<tours; k++){
        p_diag dots = points_diagonaux(grille, piece.joueur);
        if(piece.piece[0][0] != ' '){
            if(ajoute(grille,dots.points[k].ligne,dots.points[k].colonne, &piece, tour)){
                if(points_diagonaux(grille,piece.joueur).nb>= max){
                    max = dots.nb;
                    supprime(grille,dots.points[k].ligne,dots.points[k].colonne, &piece);
                }
                else{
                    supprime(grille,dots.points[k].ligne,dots.points[k].colonne, &piece);
                }
            }
        }
    }
    return max;
}

int minimise_p_diag (p piece, char** grille, char type_ennemi){

}


//Vérification de terminaison de la partie (renvoie -1 si la partie n'est pas finie, 0 si elle et nulle, 
// le score de j1 sur la grille avec un mouvement supplémentaire si j1 gagne, le score de j2 sur la grille 
//avec un mouvement supplémentaire si j2 gagne)

int partie_terminee(char** grille, ensemble j1, ensemble j2, int tour){
    int etat = 0;
    int mem_j1 = 0; //se souviens de si j1 a réussi ou non à jouer (0 si non, 1 si oui);
    int score_j1;
    int score_j2;
    p_diag p_diag_j1 = points_diagonaux(grille,'.');
    p_diag p_diag_j2 = points_diagonaux(grille,'&');
    //Gestion du 1er joueur
    for(int d=0; d<p_diag_j1.nb; d++){
        if(diag_NW(p_diag_j1.points[d].ligne,p_diag_j1.points[d].colonne).existe){
            if(grille[diag_NW(p_diag_j1.points[d].ligne,p_diag_j1.points[d].colonne).ligne][diag_NW(p_diag_j1.points[d].ligne,p_diag_j1.points[d].colonne).colonne] == '.'){
                for(int i=0; i<21; i++){
                    int n_l = p_diag_j1.points[d].ligne;
                    int n_c = p_diag_j1.points[d].colonne;
                    if(j1.pieces[i].joue == false || j1.pieces[21+i].joue == false){
                        p** rot_piece_j1 = rotation(&j1.pieces[i]);
                        for(int r=0; r<j1.pieces[i].nb_rotations; r++){
                            p** translat_piece_j1 = translation(rot_piece_j1[r]);
                            for(int t=0; t<=detect_col_vide_g(rot_piece_j1[r]); t++){
                                if(ajoute(grille,n_l,n_c,translat_piece_j1[t],tour)){
                                    score_j1 = score_joueur(grille,'.');
                                    supprime(grille,n_l,n_c,translat_piece_j1[t]);
                                    etat = -1; 
                                    mem_j1 = 1;
                                    break;
                                    // j1 a réussi à jouer
                                }
                            }
                        }
                    }
                }
                score_j1 = score_joueur(grille,'.'); 
            }    
        }
        if(diag_NE(p_diag_j1.points[d].ligne,p_diag_j1.points[d].colonne).existe){
            if(grille[diag_NE(p_diag_j1.points[d].ligne,p_diag_j1.points[d].colonne).ligne][diag_NE(p_diag_j1.points[d].ligne,p_diag_j1.points[d].colonne).colonne] == '.'){
                for(int i=0; i<21; i++){
                    int n_l = p_diag_j1.points[d].ligne;
                    int n_c = p_diag_j1.points[d].colonne-j1.pieces[i].taille+1;
                    if(j1.pieces[i].joue == false || j1.pieces[21+i].joue == false){
                        p** rot_piece_j1 = rotation(&j1.pieces[i]);
                        for(int r=0; r<j1.pieces[i].nb_rotations; r++){
                            p** translat_piece_j1 = translation(rot_piece_j1[r]);
                            for(int t=0; t<=detect_col_vide_g(rot_piece_j1[r]); t++){
                                if(ajoute(grille,n_l,n_c, translat_piece_j1[t], tour)){
                                    score_j1 = score_joueur(grille,'.');
                                    supprime(grille,n_l,n_c,translat_piece_j1[t]);
                                    etat = -1; 
                                    mem_j1 = 1;
                                    break;
                                    // j1 a réussi à jouer
                                }
                            }
                        }
                    }
                }
                score_j1 = score_joueur(grille,'.'); 
            } 
        }
        if(diag_SW(p_diag_j1.points[d].ligne,p_diag_j1.points[d].colonne).existe){
            if(grille[diag_SW(p_diag_j1.points[d].ligne,p_diag_j1.points[d].colonne).ligne][diag_SW(p_diag_j1.points[d].ligne,p_diag_j1.points[d].colonne).colonne] == '.'){
                for(int i=0; i<21; i++){
                    int n_l = p_diag_j1.points[d].ligne-j1.pieces[i].taille+1;
                    int n_c = p_diag_j1.points[d].colonne;
                    if(j1.pieces[i].joue == false || j1.pieces[21+i].joue == false){
                        p** rot_piece_j1 = rotation(&j1.pieces[i]);
                        for(int r=0; r<j1.pieces[i].nb_rotations; r++){
                            p** translat_piece_j1 = translation(rot_piece_j1[r]);
                            for(int t=0; t<=detect_col_vide_g(rot_piece_j1[r]); t++){
                                if(ajoute(grille,n_l,n_c,translat_piece_j1[t], tour)){
                                    score_j1 = score_joueur(grille,'.');
                                    supprime(grille,n_l,n_c,translat_piece_j1[t]);
                                    etat = -1; 
                                    mem_j1 = 1;
                                    break;
                                    // j1 a réussi à jouer
                                }
                            }
                        }
                    }
                }
                score_j1 = score_joueur(grille,'.'); 
            } 
        }
        if(diag_SE(p_diag_j1.points[d].ligne,p_diag_j1.points[d].colonne).existe){
            if(grille[diag_SE(p_diag_j1.points[d].ligne,p_diag_j1.points[d].colonne).ligne][diag_SE(p_diag_j1.points[d].ligne,p_diag_j1.points[d].colonne).colonne] == '.'){
                for(int i=0; i<21; i++){
                    int n_l = p_diag_j1.points[d].ligne-j1.pieces[i].taille+1;
                    int n_c = p_diag_j1.points[d].colonne-j1.pieces[i].taille+1;
                    if(j1.pieces[i].joue == false || j1.pieces[21+i].joue == false){
                        p** rot_piece_j1 = rotation(&j1.pieces[i]);
                        for(int r=0; r<j1.pieces[i].nb_rotations; r++){
                            p** translat_piece_j1 = translation(rot_piece_j1[r]);
                            for(int t=0; t<=detect_col_vide_g(rot_piece_j1[r]); t++){
                                if(ajoute(grille,n_l,n_c,translat_piece_j1[t],tour)){
                                    score_j1 = score_joueur(grille,'.');
                                    supprime(grille,n_l,n_c,translat_piece_j1[t]);
                                    etat = -1; 
                                    mem_j1 = 1;
                                    break;
                                    // j1 a réussi à jouer
                                }
                            }
                        }
                    }
                }
                score_j1 = score_joueur(grille,'.'); 
            } 
        }
    }
    //Gestion du 2eme joueur
    for(int d=0; d<p_diag_j2.nb; d++){
        if(diag_NW(p_diag_j2.points[d].ligne,p_diag_j2.points[d].colonne).existe){
            if(grille[diag_NW(p_diag_j2.points[d].ligne,p_diag_j2.points[d].colonne).ligne][diag_NW(p_diag_j2.points[d].ligne,p_diag_j2.points[d].colonne).colonne] == '&'){
                for(int i=0; i<21; i++){
                    int n_l = p_diag_j2.points[d].ligne;
                    int n_c = p_diag_j2.points[d].colonne;
                    if(j2.pieces[i].joue == false || j2.pieces[21+i].joue == false){
                        p** rot_piece_j2 = rotation(&j2.pieces[i]);
                        for(int r=0; r<j2.pieces[i].nb_rotations; r++){
                            p** translat_piece_j2 = translation(rot_piece_j2[r]);
                            for(int t=0; t<=detect_col_vide_g(rot_piece_j2[r]); t++){
                                if(ajoute(grille,n_l,n_c,translat_piece_j2[t],tour)){
                                    score_j2 = score_joueur(grille,'&');
                                    supprime(grille,n_l,n_c,translat_piece_j2[t]);
                                    etat = -1; 
                                    break;
                                    // j2 a réussi à jouer
                                }
                            }
                        }
                    }
                }
                score_j2 = score_joueur(grille,'&'); 
            }    
        }
        if(diag_NE(p_diag_j2.points[d].ligne,p_diag_j2.points[d].colonne).existe){
            if(grille[diag_NE(p_diag_j2.points[d].ligne,p_diag_j2.points[d].colonne).ligne][diag_NE(p_diag_j2.points[d].ligne,p_diag_j2.points[d].colonne).colonne] == '&'){
                for(int i=0; i<21; i++){
                    int n_l = p_diag_j2.points[d].ligne;
                    int n_c = p_diag_j2.points[d].colonne-j2.pieces[i].taille+1;
                    if(j2.pieces[i].joue == false || j2.pieces[21+i].joue == false){
                        p** rot_piece_j2 = rotation(&j2.pieces[i]);
                        for(int r=0; r<j2.pieces[i].nb_rotations; r++){
                            p** translat_piece_j2 = translation(rot_piece_j2[r]);
                            for(int t=0; t<=detect_col_vide_g(rot_piece_j2[r]); t++){
                                if(ajoute(grille,n_l,n_c, translat_piece_j2[t], tour)){
                                    score_j2 = score_joueur(grille,'&');
                                    supprime(grille,n_l,n_c,translat_piece_j2[t]);
                                    etat = -1; 
                                    break;
                                    // j2 a réussi à jouer
                                }
                            }
                        }
                    }
                }
                score_j2 = score_joueur(grille,'&'); 
            } 
        }
        if(diag_SW(p_diag_j2.points[d].ligne,p_diag_j2.points[d].colonne).existe){
            if(grille[diag_SW(p_diag_j2.points[d].ligne,p_diag_j2.points[d].colonne).ligne][diag_SW(p_diag_j2.points[d].ligne,p_diag_j2.points[d].colonne).colonne] == '&'){
                for(int i=0; i<21; i++){
                    int n_l = p_diag_j2.points[d].ligne-j2.pieces[i].taille+1;
                    int n_c = p_diag_j2.points[d].colonne;
                    if(j2.pieces[i].joue == false || j2.pieces[21+i].joue == false){
                        p** rot_piece_j2 = rotation(&j2.pieces[i]);
                        for(int r=0; r<j2.pieces[i].nb_rotations; r++){
                            p** translat_piece_j2 = translation(rot_piece_j2[r]);
                            for(int t=0; t<=detect_col_vide_g(rot_piece_j2[r]); t++){
                                if(ajoute(grille,n_l,n_c,translat_piece_j2[t], tour)){
                                    score_j2 = score_joueur(grille,'&');
                                    supprime(grille,n_l,n_c,translat_piece_j2[t]);
                                    etat = -1; 
                                    break;
                                    // j2 a réussi à jouer
                                }
                            }
                        }
                    }
                }
                score_j2 = score_joueur(grille,'&'); 
            } 
        }
        if(diag_SE(p_diag_j2.points[d].ligne,p_diag_j2.points[d].colonne).existe){
            if(grille[diag_SE(p_diag_j2.points[d].ligne,p_diag_j2.points[d].colonne).ligne][diag_SE(p_diag_j2.points[d].ligne,p_diag_j2.points[d].colonne).colonne] == '&'){
                for(int i=0; i<21; i++){
                    int n_l = p_diag_j2.points[d].ligne-j2.pieces[i].taille+1;
                    int n_c = p_diag_j2.points[d].colonne-j2.pieces[i].taille+1;
                    if(j2.pieces[i].joue == false || j2.pieces[21+i].joue == false){
                        p** rot_piece_j2 = rotation(&j2.pieces[i]);
                        for(int r=0; r<j2.pieces[i].nb_rotations; r++){
                            p** translat_piece_j2 = translation(rot_piece_j2[r]);
                            for(int t=0; t<=detect_col_vide_g(rot_piece_j2[r]); t++){
                                if(ajoute(grille,n_l,n_c,translat_piece_j2[t],tour)){
                                    score_j2 = score_joueur(grille,'&');
                                    supprime(grille,n_l,n_c,translat_piece_j2[t]);
                                    etat = -1; 
                                    break;
                                    // j2 a réussi à jouer
                                }
                            }
                        }
                    }
                }
                score_j2 = score_joueur(grille,'&'); 
            } 
        }
    }
    //Gestion des résultats
    if(etat == 0){
        //Aucun des joueurs n'ont réussi à jouer
        if(score_j2 > score_j1){
            printf("Le joueur 2 gagne \n");
            return score_j2;
        }
        if(score_j2 < score_j1){
            printf("Le joueur 1 gagne \n");
            return score_j1;
        }
        if(score_j2 == score_j1){
            printf("Nul \n");
            return 0;
        }
    }
    if(etat == -1){
        //seul j1 a réussi à jouer
        printf("Le joueur 1 gagne \n");
        return score_j1;
    }
    if(etat == -2){
        if(mem_j1 == 0){
            //seul j2 a réussi à jouer
            printf("Le joueur 2 gagne \n");
            return score_j2;
        }
        if(mem_j1 == 1){
            //les deux joueurs ont réussi à jouer
            return -1;
        }
    }
}

//Algorithme minmax avec heuristique (d désigne la profondeur d'exploration)

coup minmax(char** grille, int d, ensemble j1, ensemble j2){}




//[Main]



int main (){
    char** grille = initialise_grille();
    char*** tt_pieces = malloc(sizeof(char**)*21);
    int*** tt_pieces_init = malloc(sizeof(int**)*21);
    int** dim = malloc(sizeof(int*)*21);
    for (int k = 0; k<21; k++){
        dim[k] = malloc(sizeof(int)*4);
    }
    FILE* stream = fopen("pieces.txt", "r" );
    for (int k = 0; k<21; k++){
        int case_temp = 0;
        int ord_temp = 0;
        int rot_temp = 0;
        int id_temp = 0;
        tt_pieces_init[k] = recup_piece(&stream,&case_temp,&ord_temp, &rot_temp, &id_temp); 
        dim[k][0] = case_temp;
        dim[k][1] = ord_temp;
        dim[k][2] = rot_temp;
        dim[k][3] = id_temp;
    }
    tt_pieces = convertir(tt_pieces_init,dim);
    ensemble j1 = initialise_ensemble(tt_pieces,dim,'.');
    ensemble j2 = initialise_ensemble(tt_pieces,dim,'&');
    ajoute(grille,0,0,&j1.pieces[0],0);
    ajoute(grille,19,19,&j2.pieces[0],1);
    ajoute(grille,1,1,&j1.pieces[4],2);
    ajoute(grille,18,18,&j2.pieces[4],3);
    afficher(grille);
    return 0;
}