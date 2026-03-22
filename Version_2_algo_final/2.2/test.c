l_dots adjacents = adj(*translation_g(rotation(&j1.pieces[10])[1])[0],0,3);
l_dots diagonaux = diag(*translation_g(rotation(&j1.pieces[10])[1])[0],0,3);
printf("nb adjacents : %d \n", adjacents.nb);
for(int i=0; i<adjacents.nb; i++){
    printf("ligne : %d ; colonne : %d ; existe : %d \n", adjacents.points[i].ligne, adjacents.points[i].colonne, adjacents.points[i].existe);
}
printf("nb diagonaux : %d \n", diagonaux.nb);
for(int i=0; i<adjacents.nb; i++){
    printf("ligne : %d ; colonne : %d ; existe : %d \n", diagonaux.points[i].ligne, diagonaux.points[i].colonne, diagonaux.points[i].existe);
}