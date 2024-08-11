#include "structures.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>


/*fonction rajoutée permettant de calculer le maximum entre deux entiers
utilisée dans update_height*/
int max_height(int n,int m){
    if(n==m){
        return n || m;
    }
    if(n>m){
        return n;
    }
    return m;
}


/*permet de creer et initialiser un noeud avec une hauteur egale a zero.*/
CellTree* create_node(Block* b){
    CellTree* noeud=(CellTree*)malloc(sizeof(CellTree));
    if(!noeud){
        printf("Erreur lors de l'allocation du noeud \n");
        return NULL;
    }
    noeud->block=b;
    noeud->father=NULL;
    noeud->firstChild=NULL;
    noeud->nextBro=NULL;
    noeud->height=0;
    return noeud;
}


/*met a jour la hauteur du noeud father quand l'un de ses fils a été modifié
renvoie 1 si la hauteure a chnagé, 0 sinon*/
int update_height(CellTree* father, CellTree* child){
    int height_cour=father->height;
    father->height=max_height(height_cour,child->height)+1;
    if((father->height)!=(height_cour)){
        return 1;
    }
    return 0;
}


/* ajoute un fils a un noeud, en mettant a jour la hauteur de tous les ascendants.*/
void add_child(CellTree* father, CellTree* child){
    CellTree* prem=father->firstChild;
    if(!prem){
        father->firstChild=child;
    }else{
        while(prem->nextBro!=NULL){
            prem=prem->nextBro;
        }
        prem->nextBro=child;
        father=prem->father;
    }
    update_height(father,child);
    CellTree* tmp_pere=father;
    while(tmp_pere){
        if(update_height(tmp_pere,tmp_pere->firstChild)){
            printf("mise à jour de la hauteur réussie");
        }
        tmp_pere=tmp_pere->father;
    }
}


/*Permet d'afficher le contenu d'un arbre*/
void print_tree(CellTree* tree){
    char *b=block_to_str(tree->block);
    printf("h=%d r=%s\n",tree->height,hash_SHA256(b)); 
    free(b);
    if(tree->nextBro!=NULL){
        print_tree(tree->nextBro);
    }
    if((tree->firstChild)!=NULL){
        print_tree(tree->firstChild);
    } 
}


/*fonction qui supprime un noeud*/
void delete_node(CellTree* node){
    if (node == NULL){
        return;
    }
    if(node->block != NULL){
        delete_block(node->block);
    }
    free(node);
}


/*fonction qui supprime l'arbre a faire*/
void delete_tree(CellTree* father){
	delete_node(father);
	delete_tree(father->firstChild);
	delete_tree(father->nextBro);
	free(father);
}


/*renvoie le noeud fils avec la plus grande hauteur de l'abre passé en paramètre*/
CellTree* highest_child(CellTree* cell){
    CellTree* max=cell->firstChild;
    if(!max){
        return NULL;
    }
    CellTree* tmp=cell->firstChild->nextBro;
    while (tmp){
        if (tmp->height>max->height){
            max=tmp;
        }
        tmp=tmp->nextBro;
    }
    return max;
} 


/*renvoie le bloc du dernier noeud du fils le plus grand*/
CellTree* last_node(CellTree* tree){
    if(tree->height == 0)
		return tree;
	last_node(highest_child(tree));
	return tree;
}


/*fusionne 2 listes chaînées de déclarations signées*/
CellProtected* fusion_liste(CellProtected* c1,CellProtected* c2){
    if(!c1){
        if(!c2){
            return NULL;
        }
        return c2;
    }
    if(!c2){
        return c1;
    }
    CellProtected* res=c1;
	while(c1){
		c1=c1->next;
	}
	c1->next = c2;	
	return res;
}


/*fusionne les listes chaînées de déclarations dans le bloc le plus long d'un arbre*/
CellProtected* fusion_cp_tree(CellTree* tree, CellProtected* liste){
	if (!tree){
        return liste;
    }
    CellTree* child=highest_child(tree);
    if(!tree->block->votes){
        return liste;
    }
    CellProtected* tmp=fusion_liste(liste,tree->block->votes);
    if (child){
        liste=fusion_cp_tree(child,tmp);
    }
    return tmp;
}