#include "structures.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <dirent.h>

/*permet à un citoyen de soumettre un vote*/
void submit_vote(Protected* p){
	FILE* f = fopen("Pending_block.txt", "w");
	
	//pointe sur la fin du fichier
	fseek(f, sizeof(f), SEEK_END);
	
	char* strpr = protected_to_str(p);
	fprintf(f, "%s\n", strpr);
	free(strpr);
}


/*crée un block à partir des votes dans pending_block.txt*/
void create_block(CellTree* tree, Key* author, int d){
	CellTree* ltree = last_node(tree);
	
	Block* b = malloc(sizeof(Block));
	
	b->author = author;
	b->votes = read_protected("Pending_votes.txt");
	remove("Pending_votes.txt");
	
	char* hash = block_to_str(lecture_block("Pending_block"));
	printf("wsh\n");
    b->hash = hash_SHA256(hash);
    printf("wsh\n");
	//ajoute le dernier bloc crée précedemment dans previous_block
	if(tree){
        char* p_hash = block_to_str(ltree->father->block);
	    b->previous_hash = hash_SHA256(p_hash);
    }	
	b->nonce = 0;
	compute_proof_of_work(b, d);
    
	ecriture_block("Pending_block", b); 
	
	free(hash);
	//free(p_hash);
}



void add_block(int d, char* name){
	Block* b=lecture_block("Pending_block.txt");
    if (verify_block(b,d)==0){
        DIR* d=opendir("Blockchain");
        if(!d){
            printf("erreur a l'ouverture du dossier blockchain\n");
            delete_block(b);
            return;
        }
        FILE* f=fopen(name,"w");
        if(!f){
            printf("erreur a l'ouverture du fichier %s\n",name);
            delete_block(b);
            return;
        }
        char* str=block_to_str(b);
        fprintf(f,"%s\n",str);
        free(str);
        fclose(f);
        closedir(d);
    }
    delete_block(b);
    if (remove("Pending_block.txt") == 0){
        printf("Le fichier Pending_block.txt a été supprimé avec succès.\n");
    }
}



CellTree* read_tree(){
    DIR* rep = opendir("./Blockchain/");
    if(rep != NULL){
        struct dirent* dir;
        while((dir = readdir(rep))){
            if(strcmp(dir->d_name,"." ) !=0 && strcmp(dir->d_name,"..")!=0){
                printf ("Chemin du fichier : ./Blockchain/%s \n" ,dir->d_name);
            }
        }
        closedir(rep);
    }
    return NULL;
}



Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
    CellProtected* pr=fusion_cp_tree(tree,NULL);
    pr=tri_decla_valide(pr);
    return compute_winner(pr,candidates,voters,sizeC,sizeV);
}