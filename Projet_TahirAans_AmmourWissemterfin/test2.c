#include "structures.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

int main(void){
    srand(time(NULL));
    //tests exercices 3
    printf("\n TESTS EXERCICE 3 \n");
    //Testing Init Keys
    Key* pKey = malloc(sizeof(Key));
    Key* sKey = malloc(sizeof(Key));
    init_pair_keys(pKey, sKey,3 ,7);
    printf("pKey: %lx, %lx \n", pKey->k, pKey->n); //k a la place de val ??
    printf("sKey: %lx, %lx \n", sKey->k, sKey->n);
    

    //Testing Key Serialization
    char* chaine = key_to_str(pKey);
    printf("key_to_str : %s \n", chaine);
    Key* k = str_to_key(chaine);
    printf("str_to_key: %lx, %lx \n", k->k, k->n);
    

    //Testing signature
    
    //Candidate keys:
    Key* pKeyC = malloc(sizeof(Key));
    Key* sKeyC = malloc(sizeof(Key));
    init_pair_keys(pKeyC, sKeyC,3,7);
    
    
    //Declaration:
    char* mess = key_to_str(pKeyC);
    printf("%s vote pour %s \n", key_to_str(pKey), mess);
    Signature* sgn = sign(mess, sKey);
    printf("signature: ");
    print_long_vector(sgn->content,sgn->size);
    chaine = signature_to_str(sgn);
    printf("\n");
    printf("signature to str : %s \n", chaine);
    sgn = str_to_signature(chaine);
    printf("str to signature : ");
    print_long_vector(sgn->content,sgn->size);
    printf("\n");   
    
    //Testing protected:
    Protected* pr = init_protected(pKey, mess, sgn);
    
    
    //Verification:
    if(verify(pr)){
        printf("Signature valide \n");
    }else{
        printf("Signature non valide \n");
    }
    chaine = protected_to_str(pr);
    printf("protected to str : %s\n", chaine);
    pr = str_to_protected(chaine);
    printf("str to protected : %s;%s;%s\n",key_to_str(pr->pKey), pr->mess, signature_to_str(pr->sign));
    
    //Tests exercice 4
    
    //printf("\n TESTS EXRECICE 4 \n");

    //generate_random_data(4,4);   
    
    //Tests Exercice 5
    
    printf("\n TESTS EXERCICE 5 \n");

    CellKey* cell=create_cell_key(pKey);
    printf("Liste de clés : \n");
    print_list_keys(cell);
    cell= add_cell_key(sKey,cell);
    printf("Ajout en tête d'une clé : \n");
    print_list_keys(cell);
    delete_cell_key(&cell);
    printf("Suppression d'une clé :\n");
    print_list_keys(cell);
    printf("Suppression de toute la liste :\n");
    printf("\n");
    delete_key_list(&cell);
    print_list_keys(cell);
    CellProtected* pro= create_cell_pr(pr);
    printf("Liste de déclarations : \n");
    print_pr(pro);
    delete_liste_pr(pro);
    printf("Suppression de toute la liste :\n");

    //Tests Exercice 6

    printf("\n TESTS EXERCICE 6\n\n");
    generate_random_data(10,4);

    printf("Liste des candidats:\n");
    CellKey* liste_candidats=read_public_keys("candidats.txt");

    printf("Liste des déclarations avant tri:\n");
    CellProtected* liste_declarations=read_protected();

    liste_declarations=tri_decla_valide(liste_declarations);
    printf("\nListe des déclarations apres tri:\n");
    print_pr(liste_declarations);


    printf("\nListe des votants:\n");
    CellKey* liste_voters=read_public_keys("keys.txt");
    printf("\nCOMPUTE WINNER TEST\n");
    Key* gagnant=compute_winner(liste_declarations, liste_candidats, liste_voters,4,10);
    printf("Le gagnant est %s\n",key_to_str(gagnant));


    //Tests Exercice 7

    //Creation d'un bloc pour tester

    printf("\n TESTS EXERCICE 7 BLOCK\n\n");

    //printf("test:\n"); print_pr(liste_declarations);

	Block* b = malloc(sizeof(Block));
	
	Key * key1 = malloc(sizeof(Key));
	Key * key2 = malloc(sizeof(Key));
	init_pair_keys(key1 ,key2 ,3 ,7);
	
	b->author = key1;
	b->votes = liste_declarations;
	b->hash = NULL;
	b->previous_hash = NULL;
	b->nonce = 0;
	char* strb = block_to_str(b);
    printf("Test de lecture des declarations de votes dans bloc.txt:\n");
    print_pr(b->votes);
	printf("\nChaine representant le bloc:\n %s\n\n", strb);
	printf("\nChaine représentant la valeur hachée du bloc: %s\n\n",hash_SHA256(strb));
	b->hash = hash_SHA256(strb);
	
	//Test de la fonction write_block
	ecriture_block("block.txt",b); //puis vérifier dans le fichier si c'est bon
	
    //Test de la fonction read_block
	
	Block* b2 = malloc(sizeof(Block));
    b2=lecture_block("block.txt");

	char* strb1 =block_to_str(b);
    printf("b1: %s\n",strb1);

	char* strb2 = block_to_str(b2);
    if(!strb2){
        printf("erreur allocation\n");
    }
    printf("\n\nb2: %s\n\n",strb2);


	//si la chaine des deux blocs sont identiques alors les deux blocs sont identiques
	if(strcmp(strb2, strb)){
		printf("read_block marche bien\n\n");
	}
	else{
		printf("read_block ne marche pas :(\n\n");
	}

    compute_proof_of_work(b,3);   //cause toutes les pertes de mémoire
	

    FILE *fichier = fopen("Compute.txt", "w");
	if(!fichier){
		printf("Error: pas de fichiers ouvert\n");
	}    
    //initialisation clock
    clock_t  temps_initial1;
    clock_t  temps_final1;
    double temps_cpu1;
    clock_t  temps_initial2;
    clock_t  temps_final2;
    double temps_cpu2; 
    for(int i=1;i<5;i++){
        //traitement fonction 1
        temps_initial1 = clock();
        compute_proof_of_work(b, i);
        temps_final1 = clock();
        temps_cpu1 = ((double)(temps_final1  - temps_initial1))/CLOCKS_PER_SEC;
        //Ecriture
        fprintf(fichier,"%d\t%f\n",i,temps_cpu1);
    }
    fclose(fichier);


	if(verify_block(b,3) == 1){
		printf("bloc b est valide\n");
	}
	else{
		printf("bloc b n'est pas valide ce qui est faux donc verify_block ne fonctionne pas correctement\n");
	}
    

    //Tests Exercice 8

    printf("TESTS EXERCICE 8\n");
    Block* b3=(Block*)malloc(sizeof(Block));
    if (b3==NULL){
        printf("erreur de malloc bloc dans main\n");
    }
    b3->author=key2;
    b3->votes=NULL;
    b3->hash=NULL;
    b3->previous_hash=NULL;
    b3->nonce=0;

    CellTree* tree= (CellTree*)malloc(sizeof(CellTree));
    tree=create_node(b);
    CellTree* child=create_node(b3);
    add_child(tree,child);
    printf("Print arbre tree:\n");
    print_tree(tree);
    printf("Print arbre child:\n");
    print_tree(child);
    printf("Print highest child\n");
    print_tree(highest_child(tree));
    printf("Print last_node\n");
    print_tree(last_node(tree));
    printf("test\n");
    //CellProtected* LDF=fusion_cp_tree(tree,NULL);
    //print_pr(LDF);
    
    //Tests Exercice 9
    printf("\nTESTS EXERCICE 9\n");

    printf("On passe à la soumission de vote\n");
    submit_vote(pr);
    char* mess2 = key_to_str(key1);
    Signature* sgn2 = sign(mess, key2);
    Protected* pr2=init_protected(key1,mess2,sgn2);

    printf("On passe à la 2eme soumission vote\n");
    submit_vote(pr2);

    printf("On crée le bloc\n");
    create_block(tree,pKey,2);
    printf("On ajoute le bloc\n");
    add_block(3,"test");
    printf("On lit l'arbre\n");
    read_tree();



    // TESTS 9.6 (ne fonctionne malheureusement pas :( )

    /*generate_random_data(1000,5);
    CellProtected* liste_decla=read_protected();
    CellKey* liste_citoyens=read_public_keys("keys.txt");
    CellTree* blockchain=NULL;
    int cpt=0;
    int i=1;
    while(liste_decla){
        if(cpt==10){
            create_block(blockchain,liste_decla->data->pKey,2);
            char d= (char) i;
            add_block(2, &d);
            i++;
            cpt=0;
        }
        cpt++;
        liste_decla=liste_decla->next;
    }
    CellTree* res_lecture= read_tree();
    print_tree(res_lecture);
    CellKey* liste_candidats2;
    compute_winner_BT(res_lecture,liste_candidats2,liste_citoyens,5,1000);
    */
    
    free(tree);
    free(b->author);
    delete_block(b);
    delete_block(b2);
    delete_key_list(&liste_candidats);
    delete_key_list(&liste_voters);
    delete_protected(pr);
    free(key2);
    free(k);
    free(chaine);
    //free(strb);
    free(strb1);
    free(strb2);


    //free(pKey);
    //free(sKey);
    free(pKeyC);
    free(sKeyC);
    return 0;
}