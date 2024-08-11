#include "structures.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>


/* alloue et initialise une cellule de liste chainee */
CellKey* create_cell_key(Key* key){
	CellKey* ck= (CellKey*) malloc(sizeof(CellKey));
	if(!ck){
		printf("Allocation échouée");
		return NULL;
	}
	ck->data=key;
	ck->next=NULL;
	return ck;
}


/* ajoute une cle en tete de liste */
CellKey* add_cell_key(Key* key, CellKey* liste){
	if(!key){
		printf("La clé est vide\n");
	}else{
		CellKey* cell= create_cell_key(key);
		cell->next=liste;
		liste=cell;
	}
	return liste;
}


/* prend en entrée le fichier keys.txt ou le fichier candidates.txt,
 et qui retourne une liste chaînée contenant toutes les clés publiques du fichier. */
CellKey* read_public_keys(char* nom){
	FILE* fichier= fopen(nom,"r");
	if(fichier==NULL){
        printf("Erreur lors de l'ouverture du fichier\n");
		return NULL;
    }
    char ligne[256];
	CellKey* liste= NULL;
	while(fgets(ligne,256,fichier)!=NULL){
		puts(ligne);
		liste=add_cell_key(str_to_key(ligne),liste);
	}
	fclose(fichier);
	return liste;
}


/* affiche une liste chaînée de clés */
void print_list_keys(CellKey* LCK){
    CellKey* debut=LCK;
    while(LCK){
        char* str=key_to_str(LCK->data);
        printf("%s\n",str);
        free(str);
        LCK=LCK->next;
    }
    LCK=debut;
}

/* supprime une cellule de liste chaînée de clés */
void delete_cell_key(CellKey** c){
	CellKey* tmp;
	if(*c!=NULL){
		tmp=*c;
		*c=(*c)->next;
		free(tmp->data);
		free(tmp);
	}
}

/* supprime une liste chaînée de clés */
void delete_key_list(CellKey** c){
	while((*c)!=NULL){
		*c=(*c)->next;
		delete_cell_key(c);
	}

}

/* alloue et initialise une cellule de liste chaînée. */
CellProtected* create_cell_pr(Protected* pr){
	if(!pr){
		printf("Allocation échouée");
		return NULL;
	}
	CellProtected* cell=(CellProtected*)malloc(sizeof(CellProtected));
	if(!cell){
		printf("Allocation échouée");
		return NULL;
	}
    cell->data=pr;
	cell->next=NULL;
    return cell;
}


/* ajoute une declaration signée en tete de liste */
CellProtected* add_cell_pr(Protected* pr, CellProtected* liste){
	if(!pr){
		printf("La déclaration est vide\n");
	}else{
		CellProtected* cell= create_cell_pr(pr);
		cell->next=liste;
		liste=cell;
	}
	return liste;
}

/* ajoute une declaration signée à la fin de la liste */
CellProtected* add_cell_pr_end(Protected* pr, CellProtected* liste){
	CellProtected* tmp=liste;
	if(!pr){
		printf("La déclaration est vide\n");
	}else{
		CellProtected* cell= create_cell_pr(pr);
		cell->next=NULL;
		if(liste==NULL){
			return cell;
		}
		while(tmp->next!=NULL){
			tmp=tmp->next;
		}
		tmp->next=cell;
	}
	return liste;
}


/* lit le fichier declarations.txt, et qui cree une liste
contenant toutes les declarations signees du fichier. */
CellProtected* read_protected(){
	FILE* fichier= fopen("declarations.txt","r");
	if(fichier==NULL){
        return NULL;
    }
    char ligne[100];
	CellProtected* liste= NULL;
	while(fgets(ligne,100,fichier)!=NULL){
		puts(ligne);
		liste=add_cell_pr(str_to_protected(ligne),liste);
	}
	fclose(fichier);
	return liste;
}

/* affiche une liste chaînée de declarations signees */
void print_pr(CellProtected* liste){
	CellProtected* tmp=liste;
	while(liste){
		char* tmp2=protected_to_str(liste->data);
		printf("%s\n",tmp2);
		liste=liste->next;
		free(tmp2);
	}
	printf("\n");
	liste=tmp;
}


/* supprime une cellule de liste chaînée de declarations signees */
void delete_cell_protected(CellProtected* c){
    delete_protected(c->data);
    free(c);
}


/* supprime entierement la liste chaînée de declarations signees */
void delete_liste_pr(CellProtected* liste){
	CellProtected* tmp=NULL;
    while(liste){
        tmp=liste;
        liste=liste->next;
        delete_cell_protected(tmp);
    }
}
