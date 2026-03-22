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

//libère la mémoire allouée à un appel à deep copy

void free_deep_copy(p* a_liberer){
    free(a_liberer);
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
        final.pieces[j].id = dimensions[i][3] + 21;
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

//Type permettant de représenter une liste de points de taille quelconque

struct liste_points{
    dot* points;
    int nb;
};

typedef struct liste_points l_dots;

//Type permettant de modéliser un coup : une pièce de type p et une position sur la grille (couple d'entiers (ligne,colonne));

struct coup{
    p piece;
    int ligne;
    int colonne;
};

typedef struct coup coup;

coup initialise_coup(p piece, int ligne, int colonne){
    coup result;
    result.piece = piece;
    result.ligne = ligne;
    result.colonne = colonne;
    return result;
}

//Type permettant de lister des coups pour un joueur

struct coups_possibles{
    coup coups_poss_j1[1500]; //Taille correspondant aux nombres de façons de pose une pièce et au nombre de points diagonaux maximums.
    coup coups_poss_j2[1500];
    int nb_j1;
    int nb_j2;
};

typedef struct coups_possibles coups_poss;

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
    printf("\n");
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

//Opération matricielle d'échange de colonnes sur une pièce (sert dans la fonction rotation)

void echange_c (p* a_modifier, int c1, int c2){
    char temp;
    for (int i=0; i<a_modifier->taille; i++){
        temp = a_modifier->piece[i][c1];
        a_modifier->piece[i][c1] = a_modifier->piece[i][c2];
        a_modifier->piece[i][c2] = temp;
    }
}

//Opération matricielle de "r" (sert dans la fonction rotation) ; on y discute le cas des pièces à symétries particluières.

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

int detect_col_vide_b (p* a_placer){
    int result;
    rotation_90(a_placer);
    result = detect_col_vide_g(a_placer);
    rotation_90(a_placer);
    rotation_90(a_placer);
    rotation_90(a_placer);
    return result;
}

int detect_col_vide_h (p* a_placer){
    int result;
    rotation_90(a_placer);
    result = detect_col_vide_d(a_placer);
    rotation_90(a_placer);
    rotation_90(a_placer);
    rotation_90(a_placer);
    return result;
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
    return result;
}

//Libère la mémoire allouée à un appel à rotation

void free_rotation(p** a_liberer, p* a_placer){
    for(int i=0; i<a_placer->nb_rotations; i++){
        free(a_liberer[i]);
    }
    free(a_liberer);
}

p** translation_g (p* a_placer){
    int col_vides = detect_col_vide_g(a_placer);
    p** result = malloc(sizeof(p*)*col_vides);
    for (int i=0; i<col_vides; i++){
        result[i] = malloc(sizeof(p)*col_vides);
    }
    for (int i=0; i<=col_vides; i++){
        result[i] = deep_copy_p(a_placer);
    }
    if(detect_col_vide_d(a_placer) == 0){
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
    }
    return result;
}

//Libère la mémoire allouée à un appel à translation_g

void free_translation_g(p** a_liberer, p* a_placer){
    for(int i=0; i<detect_col_vide_g(a_placer); i++){
        free(a_liberer[i]);
    }
    free(a_liberer);
}

p** translation_b (p* a_placer){
    rotation_90(a_placer);
    int col_vides = detect_col_vide_g(a_placer);
    p** result = malloc(sizeof(p*)*col_vides);
    for (int i=0; i<col_vides; i++){
        result[i] = malloc(sizeof(p)*col_vides);
    }
    for (int i=0; i<=col_vides; i++){
        result[i] = deep_copy_p(a_placer);
    }
    result = translation_g(a_placer);
    for(int i=0; i<col_vides; i++){
        rotation_90(result[i]);
        rotation_90(result[i]);
        rotation_90(result[i]);
    }
    return result;
}

//Libère la mémoire allouée à un appel à translation_bb

void free_translation_b(p** a_liberer, p* a_placer){
    for(int i=0; i<detect_col_vide_b(a_placer); i++){
        free(a_liberer[i]);
    }
    free(a_liberer);
}

//Fonction auxiliaire qui permet de vérifier si un point appartient ou non à la liste de points

bool appartient(l_dots liste, dot element){
    for(int i=0; i<liste.nb; i++){
        if((liste.points[i].ligne == element.ligne) && (liste.points[i].colonne == element.colonne) && (liste.points[i].existe == element.existe)){
            return true;
        }
    }
    return false;
}

//Fonction auxiliaire qui sert pour verif et qui calcule les carrés directemet adjacents à ceux de piece

l_dots adj(p piece, int posl, int posc){
    int t = piece.taille;
    int indice = 0;
    l_dots result;
    result.points = malloc(sizeof(dot)*9*piece.nb_cases);
    result.nb = indice;
    for(int l=0; l<piece.taille; l++){
        for(int c=0; c<piece.taille; c++){
            if(piece.piece[l][c] != ' '){
                if((haut(posl+l,posc+c).existe)){
                    if((haut(posl+l,posc+c).ligne-posl>=0 && haut(posl+l,posc+c).ligne-posl<t)&&(haut(posl+l,posc+c).colonne-posc>=0 && haut(posl+l,posc+c).colonne-posc)){
                        if(piece.piece[haut(posl+l,posc+c).ligne-posl][haut(posl+l,posc+c).colonne-posc] != piece.joueur){
                            if(!appartient(result,haut(posl+l,posc+c))){
                                result.points[indice] = haut(posl+l,posc+c);
                                indice++;
                                result.nb = indice;
                            }
                        }
                    }
                    else{
                        if(!appartient(result,haut(posl+l,posc+c))){
                            result.points[indice] = haut(posl+l,posc+c);
                            indice++;
                            result.nb = indice;
                        }
                    }
                }
                if(bas(posl+l,posc+c).existe){
                    if((bas(posl+l,posc+c).ligne-posl>=0 && bas(posl+l,posc+c).ligne-posl<t)&&(bas(posl+l,posc+c).colonne-posc>=0 && bas(posl+l,posc+c).colonne-posc<t)){
                        if(piece.piece[bas(posl+l,posc+c).ligne-posl][bas(posl+l,posc+c).colonne-posc] != piece.joueur){
                            if(!appartient(result,bas(posl+l,posc+c))){
                                result.points[indice] = bas(posl+l,posc+c);
                                indice++;
                                result.nb = indice;
                            }
                        }
                    }
                    else{
                        if(!appartient(result,bas(posl+l,posc+c))){
                            result.points[indice] = bas(posl+l,posc+c);
                            indice++;
                            result.nb = indice;
                        }
                    }
                }
                if(gauche(posl+l,posc+c).existe){
                    if((gauche(posl+l,posc+c).ligne-posl>=0 && gauche(posl+l,posc+c).ligne-posl<t)&&(gauche(posl+l,posc+c).colonne-posc>=0 && gauche(posl+l,posc+c).colonne-posc<t)){
                        if(piece.piece[gauche(posl+l,posc+c).ligne-posl][gauche(posl+l,posc+c).colonne-posc] != piece.joueur){
                            if(!appartient(result,gauche(posl+l,posc+c))){
                                result.points[indice] = gauche(posl+l,posc+c);
                                indice++;
                                result.nb = indice;
                            }
                        }
                    }
                    else{
                        if(!appartient(result,gauche(posl+l,posc+c))){
                            result.points[indice] = gauche(posl+l,posc+c);
                            indice++;
                            result.nb = indice;
                        }
                    }
                }
                if(droit(posl+l,posc+c).existe){
                    if((droit(posl+l,posc+c).ligne-posl>=0 && droit(posl+l,posc+c).ligne-posl<t)&&(droit(posl+l,posc+c).colonne-posc>=0 && droit(posl+l,posc+c).colonne-posc<t)){
                        if(piece.piece[droit(posl+l,posc+c).ligne-posl][droit(posl+l,posc+c).colonne-posc] != piece.joueur){
                            if(!appartient(result,droit(posl+l,posc+c))){
                                result.points[indice] = droit(posl+l,posc+c);
                                indice++;
                                result.nb = indice;
                            }
                        }
                    }
                    else{
                        if(!appartient(result,droit(posl+l,posc+c))){
                            result.points[indice] = droit(posl+l,posc+c);
                            indice++;
                            result.nb = indice;
                        }
                    }
                }
            }
        }
    }
    return result;
}

//Fonction qui renvoie un tableau de couples d'entiers représentant une ligne, une colonne, sur laquelle on peut potentiellement
//placer une pièce alliée

l_dots points_diagonaux (char** plateau, char type){
    int indice = 0;
    l_dots result;
    result.points = malloc(sizeof(dot)*100); // On ne peux clairement pas remplir le quart du plateau avec des points diagonaux
    result.nb = indice;
    for(int i=0; i<20; i++){
        for(int j=0; j<20; j++){
            if(plateau[i][j] == ' '){
                if(haut(i,j).existe && bas(i,j).existe && gauche(i,j).existe && droit(i,j).existe){
                    if(plateau[haut(i,j).ligne][haut(i,j).colonne] == ' ' && plateau[bas(i,j).ligne][bas(i,j).colonne] == ' ' && plateau[gauche(i,j).ligne][gauche(i,j).colonne] == ' ' && plateau[droit(i,j).ligne][droit(i,j).colonne] == ' '){
                        if(plateau[diag_NE(i,j).ligne][diag_NE(i,j).colonne] == type || plateau[diag_NW(i,j).ligne][diag_NW(i,j).colonne] == type || plateau[diag_SE(i,j).ligne][diag_SE(i,j).colonne] == type || plateau[diag_SW(i,j).ligne][diag_SW(i,j).colonne] == type){
                            result.points[indice] = initialise_point(i,j);
                            indice++;
                            result.nb = indice;
                        }
                    }
                }
                if(!haut(i,j).existe && bas(i,j).existe && gauche(i,j).existe && droit(i,j).existe){
                    if(plateau[bas(i,j).ligne][bas(i,j).colonne] == ' ' && plateau[gauche(i,j).ligne][gauche(i,j).colonne] == ' ' && plateau[droit(i,j).ligne][droit(i,j).colonne] == ' '){
                        if(plateau[diag_SE(i,j).ligne][diag_SE(i,j).colonne] == type || plateau[diag_SW(i,j).ligne][diag_SW(i,j).colonne] == type){
                            result.points[indice] = initialise_point(i,j);
                            indice++;
                            result.nb = indice;
                        }
                    }
                }
                if(haut(i,j).existe && !bas(i,j).existe && gauche(i,j).existe && droit(i,j).existe){
                    if(plateau[haut(i,j).ligne][haut(i,j).colonne] == ' ' && plateau[gauche(i,j).ligne][gauche(i,j).colonne] == ' ' && plateau[droit(i,j).ligne][droit(i,j).colonne] == ' '){
                        if(plateau[diag_NE(i,j).ligne][diag_NE(i,j).colonne] == type || plateau[diag_NW(i,j).ligne][diag_NW(i,j).colonne] == type){
                            result.points[indice] = initialise_point(i,j);
                            indice++;
                            result.nb = indice;
                        }
                    }
                }
                if(haut(i,j).existe && bas(i,j).existe && !gauche(i,j).existe && droit(i,j).existe){
                    if(plateau[haut(i,j).ligne][haut(i,j).colonne] == ' ' && plateau[bas(i,j).ligne][bas(i,j).colonne] == ' ' && plateau[droit(i,j).ligne][droit(i,j).colonne] == ' '){
                        if(plateau[diag_NE(i,j).ligne][diag_NE(i,j).colonne] == type || plateau[diag_SE(i,j).ligne][diag_SE(i,j).colonne] == type){
                            result.points[indice] = initialise_point(i,j);
                            indice++;
                            result.nb = indice;
                        }
                    }
                }
                if(haut(i,j).existe && bas(i,j).existe && gauche(i,j).existe && !droit(i,j).existe){
                    if(plateau[haut(i,j).ligne][haut(i,j).colonne] == ' ' && plateau[bas(i,j).ligne][bas(i,j).colonne] == ' ' && plateau[gauche(i,j).ligne][gauche(i,j).colonne] == ' '){
                        if(plateau[diag_NW(i,j).ligne][diag_NW(i,j).colonne] == type || plateau[diag_SW(i,j).ligne][diag_SW(i,j).colonne] == type){
                            result.points[indice] = initialise_point(i,j);
                            indice++;
                            result.nb = indice;
                        }
                    }
                }
                if(!haut(i,j).existe && bas(i,j).existe && !gauche(i,j).existe && droit(i,j).existe){
                    if(plateau[bas(i,j).ligne][bas(i,j).colonne] == ' ' && plateau[droit(i,j).ligne][droit(i,j).colonne] == ' '){
                        if(plateau[diag_SE(i,j).ligne][diag_SE(i,j).colonne] == type){
                            result.points[indice] = initialise_point(i,j);
                            indice++;
                            result.nb = indice;
                        }
                    }
                }
                if(!haut(i,j).existe && bas(i,j).existe && gauche(i,j).existe && !droit(i,j).existe){
                    if(plateau[bas(i,j).ligne][bas(i,j).colonne] == ' ' && plateau[gauche(i,j).ligne][gauche(i,j).colonne] == ' '){
                        if(plateau[diag_SW(i,j).ligne][diag_SW(i,j).colonne] == type){
                            result.points[indice] = initialise_point(i,j);
                            indice++;
                            result.nb = indice;
                        }
                    }
                }
                if(haut(i,j).existe && !bas(i,j).existe && !gauche(i,j).existe && droit(i,j).existe){
                    if(plateau[haut(i,j).ligne][haut(i,j).colonne] == ' ' && plateau[droit(i,j).ligne][droit(i,j).colonne] == ' '){
                        if(plateau[diag_NE(i,j).ligne][diag_NE(i,j).colonne] == type){
                            result.points[indice] = initialise_point(i,j);
                            indice++;
                            result.nb = indice;
                        }
                    }
                }
                if(haut(i,j).existe && !bas(i,j).existe && gauche(i,j).existe && !droit(i,j).existe){
                    if(plateau[haut(i,j).ligne][haut(i,j).colonne] == ' ' && plateau[gauche(i,j).ligne][gauche(i,j).colonne] == ' '){
                        if(plateau[diag_NW(i,j).ligne][diag_NW(i,j).colonne] == type){
                            result.points[indice] = initialise_point(i,j);
                            indice++;
                            result.nb = indice;
                        }
                    }
                }
            }
        }
    }
    return result;
}

l_dots diag(p piece, int posl, int posc){
    char** grille_temp = initialise_grille();
    for(int i=0; i<piece.taille; i++){
        for(int j=0; j<piece.taille; j++){
            grille_temp[posl+i][posc+j] = piece.piece[i][j];
        }
    }
    l_dots result = points_diagonaux(grille_temp, piece.joueur);
    return result;
}

//Vérifie si la règle du jeu est respectée sur la grille passée en argument, avec l'ajout de la nouvelle pièce.
//(on suppose donc que la grille est correcte au sens de la règle du jeu avant ajout)

bool verif (char** grille, int posl, int posc, p a_placer, int tour){
    //La règle du démarrage au coin est vérifié par la fonction joue_coup
    //Vérification de la collision directe
    if(tour>1){
        l_dots a = adj(a_placer,posl,posc);
        for(int i=0; i<a.nb; i++){
            if(grille[a.points[i].ligne][a.points[i].colonne] == a_placer.joueur){
                return false;
            }
        }
        int nb_coins = 0;
        //Vérification du rattachement diagonal
        l_dots d = diag(a_placer,posl,posc);
        for(int i=0; i<d.nb; i++){
            if(grille[d.points[i].ligne][d.points[i].colonne] == a_placer.joueur){
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
    else{
        return true;
    }
}


//Ajoute la pièce sur le plateau

bool ajoute (char** grille, int posx, int posy, p* a_placer, int tour){
    if (!(placable_exterieur(*a_placer, posx, posy))){
        return 0;
    }
    else if (!(placable_interieur(grille,posx,posy,*a_placer))){
        return 0;
    }
    else if (!(verif(grille,posx,posy,*a_placer,tour))){
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
        return 1;
    }
}

//Supprime la pièce précédemment placée passée en argument, aux coordonnées données (sert dans l'algorithme glouton, donc on aura la garantie
//que la pièce à effectivement été placée à ces coordonnées).

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





//[Section Heuristiques et algorithmique]



//[Gestion de la partie]



//Renvoi un élément de type coups_poss qui contient : - un tableau avec les coups possibles de j1 codés avec le type coup
//                                                    - le nombre de coups possibles de j1
//                                                    - un tableau avec les coups possibles de j2 codés avec le type coup
//                                                    - le nombre de coups possibles de j1

coups_poss joue_coup(char** grille, ensemble j1, ensemble j2, int tour){
    coups_poss result;
    result.nb_j1 = 0;
    result.nb_j2 = 0;
    if(tour == 0){
        //Cas du premier tour (premier tour de j1)
        for(int i=0; i<42; i++){
            if(j1.pieces[i].joue == false){
                for(int r=0; r<j1.pieces[i].nb_rotations; r++){
                    for(int t=0; t<=detect_col_vide_g(rotation(&j1.pieces[i])[r]); t++){
                        p candidat = *translation_g(rotation(&j1.pieces[i])[r])[t];
                        if(candidat.piece[0][0] == '.'){
                            coup temp = initialise_coup(candidat,0,0);
                            result.coups_poss_j1[result.nb_j1] = temp;
                            result.nb_j1++;
                            free_rotation(rotation(&j1.pieces[i]), &j1.pieces[i]);
                            free_translation_g(translation_g(rotation(&j1.pieces[i])[r]),rotation(&j1.pieces[i])[r]);
                        }
                    }
                }
            }
        }
    }
    else if(tour == 1){
        //Cas du second tour (premier tour de j2)
        for(int j=0; j<42; j++){
            if(j2.pieces[j].joue == false){
                for(int r=0; r<j2.pieces[j].nb_rotations; r++){
                    for(int t=0; t<=detect_col_vide_g(rotation(&j2.pieces[j])[r]); t++){
                        p candidat = *translation_g(rotation(&j2.pieces[j])[r])[t];
                        if(candidat.piece[candidat.taille-1][candidat.taille-1] == '&'){
                            coup temp = initialise_coup(candidat,19-candidat.taille+1,19-candidat.taille+1);
                            result.coups_poss_j2[result.nb_j2] = temp;
                            result.nb_j2++;
                        }
                    }
                    if(detect_col_vide_g(rotation(&j2.pieces[j])[r]) != 0){
                        for(int t=0; t<=detect_col_vide_b(rotation(&j2.pieces[j])[r]); t++){
                            p candidat = *translation_b(rotation(&j2.pieces[j])[r])[t];
                            if(candidat.piece[candidat.taille-1][candidat.taille-1] == '&'){
                                coup temp = initialise_coup(candidat,19-candidat.taille+1,19-candidat.taille+1);
                                result.coups_poss_j2[result.nb_j2] = temp;
                                result.nb_j2++;
                            }
                        }
                    }
                }
            }
        }
    }
    else{
        //Pour chaque case
        for(int l=0; l<20; l++){
            for(int c=0; c<20; c++){
                if(grille[l][c] == ' '){
                    //Pour chaque piece de j1
                    for(int i=0; i<42; i++){
                        if(j1.pieces[i].joue == false){
                            for(int r=0; r<j1.pieces[i].nb_rotations; r++){
                                for(int t=0; t<=detect_col_vide_g(rotation(&j1.pieces[i])[r]); t++){
                                    p* candidat = translation_g(rotation(&j1.pieces[i])[r])[t];
                                    int ok_ajoute = ajoute(grille,l,c,candidat,tour);
                                    if(ok_ajoute == 1){
                                        coup temp = initialise_coup(*candidat,l,c);
                                        result.coups_poss_j1[result.nb_j1] = temp;
                                        result.nb_j1++;
                                        supprime(grille,temp.ligne,temp.colonne,&temp.piece);
                                    }
                                    else{
                                        supprime(grille,l,c,candidat);
                                    }
                                }
                                if(detect_col_vide_g(rotation(&j1.pieces[i])[r]) != 0){
                                    for(int t=0; t<=detect_col_vide_b(rotation(&j1.pieces[i])[r]); t++){
                                        p* candidat = translation_b(rotation(&j1.pieces[i])[r])[t];
                                        int ok_ajoute = ajoute(grille,l,c,candidat,tour);
                                        if(ok_ajoute == 1){
                                            coup temp = initialise_coup(*candidat,l,c);
                                            result.coups_poss_j1[result.nb_j1] = temp;
                                            result.nb_j1++;
                                            supprime(grille,temp.ligne,temp.colonne,&temp.piece);
                                        }
                                        else{
                                            supprime(grille,l,c,candidat);
                                        }
                                    }
                                }
                            }   
                        }
                    }
                    //Pour chaque piece de j2
                    for(int j=0; j<42; j++){
                        if(j2.pieces[j].joue == false){
                            for(int r=0; r<j2.pieces[j].nb_rotations; r++){
                                for(int t=0; t<=detect_col_vide_g(rotation(&j2.pieces[j])[r]); t++){
                                    p* candidat = translation_g(rotation(&j2.pieces[j])[r])[t];
                                    int ok_ajoute = ajoute(grille,l,c,candidat,tour);
                                    if(ok_ajoute == 1){
                                        coup temp = initialise_coup(*candidat,l,c);
                                        result.coups_poss_j2[result.nb_j2] = temp;
                                        result.nb_j2++;
                                        supprime(grille,l,c,candidat);
                                    }
                                    else{
                                        supprime(grille,l,c,candidat);
                                    }
                                }
                                if(detect_col_vide_g(rotation(&j2.pieces[j])[r]) != 0){
                                    for(int t=0; t<=detect_col_vide_b(rotation(&j2.pieces[j])[r]); t++){
                                        p* candidat = translation_b(rotation(&j2.pieces[j])[r])[t];
                                        int ok_ajoute = ajoute(grille,l,c,candidat,tour);
                                        if(ok_ajoute == 1){
                                            coup temp = initialise_coup(*candidat,l,c);
                                            result.coups_poss_j2[result.nb_j2] = temp;
                                            result.nb_j2++;
                                            supprime(grille,l,c,candidat);
                                        }
                                        else{
                                            supprime(grille,l,c,candidat);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }  
            }
        }
    }
    return result;
}

// [Heuristiques]

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

//Heuristiques naïves, placent la pièce au hasard.


int rubik_distance (p piece){
    if (piece.id == 18 || piece.id == 10 || piece.id == 20 || piece.id == 39 || piece.id == 31 || piece.id == 41){
        return 5;
    }
    else if(piece.id == 5 || piece.id == 19 || piece.id == 26 || piece.id == 40){
        return 4;
    }
    else if(piece.id == 0 || piece.id == 21){
        return 2;
    }
    else{
        return 2*piece.taille-detect_col_vide_g(&piece);
    }
}

//Renvoi un coup qui maximise rapporte_points pour le joueur passé en argument

coup maximise_points(char joueur, char** grille, ensemble j1, ensemble j2, int tour, int* id){
    coups_poss poss = joue_coup(grille,j1,j2,tour);
    int points = score_joueur(grille, joueur);
    int ligne_finale = 0;
    int colonne_finale = 0;
    p piece_finale;
    if(joueur == '.'){
        for(int i=0; i<poss.nb_j1; i++){
            int p = ajoute(grille,poss.coups_poss_j1[i].ligne,poss.coups_poss_j1[i].colonne,&poss.coups_poss_j1[i].piece,tour);
            int new_points = score_joueur(grille,joueur);
            if(new_points >= points){
                points = new_points;
                ligne_finale = poss.coups_poss_j1[i].ligne;
                colonne_finale = poss.coups_poss_j1[i].colonne;
                piece_finale = poss.coups_poss_j1[i].piece;
                supprime(grille,poss.coups_poss_j1[i].ligne,poss.coups_poss_j1[i].colonne,&poss.coups_poss_j1[i].piece);
            }
            else{
                supprime(grille,poss.coups_poss_j1[i].ligne,poss.coups_poss_j1[i].colonne,&poss.coups_poss_j1[i].piece);
            }
        }
        coup result = initialise_coup(piece_finale,ligne_finale,colonne_finale);
        *id = piece_finale.id;
        return result;
    }
    else{
        for(int i=0; i<poss.nb_j2; i++){
            int p = ajoute(grille,poss.coups_poss_j2[i].ligne,poss.coups_poss_j2[i].colonne,&poss.coups_poss_j2[i].piece,tour);
            int new_points = score_joueur(grille,joueur);
            if(new_points >= points){
                points = new_points;
                ligne_finale = poss.coups_poss_j2[i].ligne;
                colonne_finale = poss.coups_poss_j2[i].colonne;
                piece_finale = poss.coups_poss_j2[i].piece;
                supprime(grille,poss.coups_poss_j2[i].ligne,poss.coups_poss_j2[i].colonne,&poss.coups_poss_j2[i].piece);
            }
            else{
                supprime(grille,poss.coups_poss_j1[i].ligne,poss.coups_poss_j1[i].colonne,&poss.coups_poss_j1[i].piece);
            }
        }
        coup result = initialise_coup(piece_finale,ligne_finale,colonne_finale);
        *id = piece_finale.id;
        return result;
    }
}

//Renvoi un coup qui maximise rubik_distance pour le joueur passé en argument

coup maximise_rubik(char joueur, char** grille, ensemble j1, ensemble j2, int tour, int* id){
    coups_poss poss = joue_coup(grille,j1,j2,tour);
    int dist = rubik_distance(poss.coups_poss_j1[0].piece);
    int ligne_finale = 0;
    int colonne_finale = 0;
    p piece_finale;
    if(joueur == '.'){
        for(int i=0; i<poss.nb_j1; i++){
            int p = ajoute(grille,poss.coups_poss_j1[i].ligne,poss.coups_poss_j1[i].colonne,&poss.coups_poss_j1[i].piece,tour);
            int new_dist = rubik_distance(poss.coups_poss_j1[i].piece);
            if(new_dist >= dist){
                dist = new_dist;
                ligne_finale = poss.coups_poss_j1[i].ligne;
                colonne_finale = poss.coups_poss_j1[i].colonne;
                piece_finale = poss.coups_poss_j1[i].piece;
                supprime(grille,poss.coups_poss_j1[i].ligne,poss.coups_poss_j1[i].colonne,&poss.coups_poss_j1[i].piece);
            }
            else{
                supprime(grille,poss.coups_poss_j1[i].ligne,poss.coups_poss_j1[i].colonne,&poss.coups_poss_j1[i].piece);
            }
        }
        coup result = initialise_coup(piece_finale,ligne_finale,colonne_finale);
        *id = piece_finale.id;
        return result;
    }
    else{
        for(int i=0; i<poss.nb_j2; i++){
            int p = ajoute(grille,poss.coups_poss_j2[i].ligne,poss.coups_poss_j2[i].colonne,&poss.coups_poss_j2[i].piece,tour);
            int new_dist = rubik_distance(poss.coups_poss_j1[i].piece);
            if(new_dist >= dist){
                dist = new_dist;
                ligne_finale = poss.coups_poss_j2[i].ligne;
                colonne_finale = poss.coups_poss_j2[i].colonne;
                piece_finale = poss.coups_poss_j2[i].piece;
                supprime(grille,poss.coups_poss_j2[i].ligne,poss.coups_poss_j2[i].colonne,&poss.coups_poss_j2[i].piece);
            }
            else{
                supprime(grille,poss.coups_poss_j1[i].ligne,poss.coups_poss_j1[i].colonne,&poss.coups_poss_j1[i].piece);
            }
        }
        coup result = initialise_coup(piece_finale,ligne_finale,colonne_finale);
        *id = piece_finale.id;
        return result;
    }
}

//Renvoi un coup tel que le nombre de points diagonaux après ajout à grille est plus elevé qu'avant, et qui vérifie que cette augmentation
//est optimale
coup maximise_p_diag (char joueur, char** grille, ensemble j1, ensemble j2, int tour, int* id){
    coups_poss poss = joue_coup(grille,j1,j2,tour);
    int nb_p_diag = points_diagonaux(grille, joueur).nb;
    int ligne_finale = 0;
    int colonne_finale = 0;
    p piece_finale;
    if(joueur == '.'){
        for(int i=0; i<poss.nb_j1; i++){
            int p = ajoute(grille,poss.coups_poss_j1[i].ligne,poss.coups_poss_j1[i].colonne,&poss.coups_poss_j1[i].piece,tour);
            int new_nb_p_diag = points_diagonaux(grille, joueur).nb;
            if (new_nb_p_diag >= nb_p_diag){
                nb_p_diag = new_nb_p_diag;
                ligne_finale = poss.coups_poss_j1[i].ligne;
                colonne_finale = poss.coups_poss_j1[i].colonne;
                piece_finale = poss.coups_poss_j1[i].piece;
                supprime(grille,poss.coups_poss_j1[i].ligne,poss.coups_poss_j1[i].colonne,&poss.coups_poss_j1[i].piece);
            }
            else{
                supprime(grille,poss.coups_poss_j1[i].ligne,poss.coups_poss_j1[i].colonne,&poss.coups_poss_j1[i].piece);
            }
        }
        coup result = initialise_coup(piece_finale,ligne_finale,colonne_finale);
        *id = piece_finale.id;
        return result;
    }
    else{
        for(int i=0; i<poss.nb_j2; i++){
            int p = ajoute(grille,poss.coups_poss_j2[i].ligne,poss.coups_poss_j2[i].colonne,&poss.coups_poss_j2[i].piece,tour);
            int new_nb_p_diag = points_diagonaux(grille, joueur).nb;
            if (new_nb_p_diag >= nb_p_diag){
                nb_p_diag = new_nb_p_diag;
                ligne_finale = poss.coups_poss_j2[i].ligne;
                colonne_finale = poss.coups_poss_j2[i].colonne;
                piece_finale = poss.coups_poss_j2[i].piece;
                supprime(grille,poss.coups_poss_j2[i].ligne,poss.coups_poss_j2[i].colonne,&poss.coups_poss_j2[i].piece);
            }
            else{
                supprime(grille,poss.coups_poss_j2[i].ligne,poss.coups_poss_j2[i].colonne,&poss.coups_poss_j2[i].piece);
            }
        }
        coup result = initialise_coup(piece_finale,ligne_finale,colonne_finale);
        *id = piece_finale.id;
        return result;    
    } 
}

//maximise le nombre de points diagonaux et minimise ceux de l'adversaire. Concrètement, maximise la différence 
//entre les nombre de points diagonaux de j1 et le nombre de points diagonaux de j2

coup polyvalente (char joueur, char adversaire, char** grille, ensemble j1, ensemble j2, int tour, int* id){
    coups_poss poss = joue_coup(grille,j1,j2,tour);
    int nb_p_diag_joueur = points_diagonaux(grille, joueur).nb;
    int nb_p_diag_adversaire = points_diagonaux(grille, adversaire).nb;
    int ligne_finale = 0;
    int colonne_finale = 0;
    p piece_finale;
    if(joueur == '.'){
        for(int i=0; i<poss.nb_j1; i++){
            int p = ajoute(grille,poss.coups_poss_j1[i].ligne,poss.coups_poss_j1[i].colonne,&poss.coups_poss_j1[i].piece,tour);
            int new_nb_p_diag_j1 = points_diagonaux(grille, joueur).nb;
            int new_nb_p_diag_j2 = points_diagonaux(grille, adversaire).nb;
            if (new_nb_p_diag_j1 >= nb_p_diag_joueur && new_nb_p_diag_j2 <= nb_p_diag_adversaire){
                nb_p_diag_joueur = new_nb_p_diag_j1;
                nb_p_diag_adversaire = new_nb_p_diag_j2;
                ligne_finale = poss.coups_poss_j1[i].ligne;
                colonne_finale = poss.coups_poss_j1[i].colonne;
                piece_finale = poss.coups_poss_j1[i].piece;
                supprime(grille,poss.coups_poss_j1[i].ligne,poss.coups_poss_j1[i].colonne,&poss.coups_poss_j1[i].piece);
            }
            else{
                supprime(grille,poss.coups_poss_j1[i].ligne,poss.coups_poss_j1[i].colonne,&poss.coups_poss_j1[i].piece);
            }
        }
        coup result = initialise_coup(piece_finale,ligne_finale,colonne_finale);
        *id = piece_finale.id;
        return result;
    }
    else{
        for(int i=0; i<poss.nb_j2; i++){
            int p = ajoute(grille,poss.coups_poss_j2[i].ligne,poss.coups_poss_j2[i].colonne,&poss.coups_poss_j2[i].piece,tour);
            int new_nb_p_diag_j2 = points_diagonaux(grille, joueur).nb;
            int new_nb_p_diag_j1 = points_diagonaux(grille, adversaire).nb;
            if (new_nb_p_diag_j2 >= nb_p_diag_joueur && new_nb_p_diag_j1 <= nb_p_diag_adversaire){
                nb_p_diag_joueur = new_nb_p_diag_j2;
                nb_p_diag_adversaire = new_nb_p_diag_j1;
                ligne_finale = poss.coups_poss_j2[i].ligne;
                colonne_finale = poss.coups_poss_j2[i].colonne;
                piece_finale = poss.coups_poss_j2[i].piece;
                supprime(grille,poss.coups_poss_j2[i].ligne,poss.coups_poss_j2[i].colonne,&poss.coups_poss_j2[i].piece);
            }
            else{
                supprime(grille,poss.coups_poss_j2[i].ligne,poss.coups_poss_j2[i].colonne,&poss.coups_poss_j2[i].piece);
            }
        }
        coup result = initialise_coup(piece_finale,ligne_finale,colonne_finale);
        *id = piece_finale.id;
        return result;
    } 
}


//[Fin de partie]

//Renvoie 1 si n1 est plus grand que n2, 0 si n1 égal n2, -1 sinon

int gagnant(int n1, int n2){
    if(n1 > n2){
        return 1;
    }
    else if(n1 < n2){
        return -1;
    }
    else{
        return 0;
    }
}

//indique la progression de la partie : -1 si j2 gagne, 1 si j1 gagne, 0 si il y a match nul, 2 si la partie continue

int partie_terminee(char** grille, ensemble j1, ensemble j2, int tour){
    coups_poss poss = joue_coup(grille,j1,j2,tour);
    if(poss.nb_j1 == 0 || poss.nb_j2 == 0){
        return gagnant(score_joueur(grille, '.'), score_joueur(grille, '&'));
    }
    else{
        return 2;
    }
}


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


    //Exemple de simulation, distance rubik pour j1 contre aggressif pour j2
    int tour = 0;
    while(partie_terminee(grille,j1,j2,tour) == 2){
        if(tour % 2 == 0){
            int* id_piece_j1;
            coup max_heuristique_j1 = maximise_rubik('.',grille,j1,j2,tour,id_piece_j1);
            ajoute(grille,max_heuristique_j1.ligne,max_heuristique_j1.colonne,&max_heuristique_j1.piece,tour);
            j1.pieces[*id_piece_j1].joue = true;
        }
        if(tour % 2 == 1){
            int* id_piece_j2;
            coup max_heuristique_j2 = maximise_p_diag('&',grille,j1,j2,tour,id_piece_j2);
            ajoute(grille,max_heuristique_j2.ligne,max_heuristique_j2.colonne,&max_heuristique_j2.piece,tour);
            j1.pieces[*id_piece_j2].joue = true;
        }
        tour++;
    }
    if(partie_terminee(grille,j1,j2,tour) == 0){
        printf("match nul !");
    }
    else if(partie_terminee(grille,j1,j2,tour) == 1){
        printf("j2 gagne !");
    }
    else{
        printf("j1 gagne !");
    }
    return 0;
}