#include "structures.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>


/* supprime toutes les declarations dont la signature n’est pas valide. */
CellProtected* tri_decla_valide(CellProtected* liste){
    CellProtected* tmp0=liste;
    CellProtected* tmp=NULL;
    while(verify(tmp0->data)==0){ //On enleve toutes les signatures fausses au début
        tmp=tmp0;
        tmp0=tmp0->next;
        delete_cell_protected(tmp);
        }
    liste=tmp0; //on stocke la premiere signature vraie
    while(tmp0->next){
        if (!verify(tmp0->next->data)){
            CellProtected * tmp=tmp0->next;
            tmp0->next=tmp0->next->next;
            delete_cell_protected(tmp);
        }
        else{
            tmp0=tmp0->next;
        }
    }
    return liste;

}


/* alloue une cellule de la table de hachage, et qui initialise ses 
champs en mettant la valeur a zero. */
HashCell* create_hashcell(Key* key){
    HashCell* cell= (HashCell*) malloc (sizeof(HashCell));
    if(!cell){
        printf("Erreur d'allocation de cell\n");
        return NULL;
    }
    cell->key=key;
    cell->val=0;
    return cell;
}

/*retourne la position de la clé dans la table de hachage*/
int hash_function(Key* key, int size){
    if(size==0){
        printf("size vaut 0 erreur");
        return -1;
    }
    return (int) (key->k+key->n)%size;
}

/* cherche dans la table t s’il existe un élément dont la cle publique est key */
int find_position(HashTable* t, Key* key) {
    int size = t->size;
    int pos = hash_function(key, size);

    //Si la case n'est pas vide et qu'il ne s'agit pas de la bonne clé
    if (t->tab[pos] != NULL && t->tab[pos]->key != NULL && key != NULL && ((t->tab[pos]->key->k != key->k) || (t->tab[pos]->key->n != key->n))) {

        int pos_depart = pos;
        pos = (pos + 1) % size;

        //Parcours de la table de hachage jusqu'à trouver la bonne clé
        while (pos != pos_depart) {
            if (t->tab[pos] != NULL && t->tab[pos]->key != NULL && key != NULL && (t->tab[pos]->key->k == key->k) && (t->tab[pos]->key->n == key->n)) {
                break;
            }
            pos = (pos + 1) % size;
        }

        //Si on ne trouve pas la clé
        if (pos == pos_depart) {
            pos = (pos + 1) % size;

            //Parcours de la table de hachage jusqu'à trouver une case vide
            while (t->tab[pos] != NULL && pos != pos_depart) {
                pos = (pos + 1) % size;
            }

            //S'il n'y plus de place dans la table de hachage
            if (pos == pos_depart) {
                return -1;
            }
        }
    }
    return pos;
}


/* crée et initialise une table de hachage de taille size contenant une cellule pour 
chaque clé de la liste chaînee keys. */
HashTable *create_hashtable(CellKey *keys,int size){
    HashTable *table=(HashTable *)malloc(sizeof(HashTable));
    if (table==NULL){
        printf("ERREUR MALLOC\n");
        exit(-1);
    }
    table->size=size;
    table->tab=(HashCell **)malloc(sizeof(HashCell *)*size);
    if (table->tab==NULL){
        printf("ERREUR MALLOC\n");
        exit(-1);
    }
    for(int i=0;i<size;i++){ // Initialisation des éléments à NULL (étant donné que ce n'est pas le cas automatiquement)
        table->tab[i]=NULL;
    }
    while (keys!=NULL){
        int hash=hash_function(keys->data,size);
        if (table->tab[hash]!=NULL){
            for(int i=0;i<size;i++){
                if(table->tab[(hash+i)%size]==NULL){
                    table->tab[(hash+i)%size]=create_hashcell(keys->data);
                    break;
                }
            }
        } else {
            table->tab[hash]=create_hashcell(keys->data);
        }
        keys=keys->next;
    }

    return table;
}

/*supprime une table de hachage */
void delete_hashtable(HashTable* t){
    int i;
    for(i=0;i < t->size; i++){
        if((t->tab[i])!= NULL){
            free((void*) t->tab[i]->key);
        }else{
            free((void*) t->tab[i]);
        }
    }
    free(t->tab);
    free(t);
}


/*fonction pour simplifier compute_win
"Vérifie l'occurence de la clé dans la liste"
"Renvoie 1 si occurence il y a,0 sinon"*/

int occurence_key(Key *key,CellKey *list){
    while (list!=NULL){
        if (key->k==list->data->k && key->n==list->data->n){
            return 1;
        }
        list=list->next;
    }
    return 0;
}


Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV) {
    if (voters == NULL || candidates == NULL || decl == NULL) {
        return NULL;
    }
    HashTable* tableC = create_hashtable(candidates, sizeC);
    HashTable* tableV = create_hashtable(voters, sizeV);
    CellProtected* declarations = decl;
    while (declarations != NULL) {

        if (occurence_key(declarations->data->pKey, voters)) {

            if (tableV->tab[find_position(tableV, declarations->data->pKey)]->val == 0) { // Est-ce que l'électeur n'a pas voté ?
                tableV->tab[find_position(tableV, declarations->data->pKey)]->val = 1;
                Key* ck = str_to_key(declarations->data->mess);
                if (occurence_key(ck, candidates)) { // Est-ce que le candidat est légitime ?
                    tableC->tab[find_position(tableC, ck)]->val++;
                }
                free(ck);
            }
        }
        declarations = declarations->next;
    }

    HashCell* max = NULL;
    for (int i = 0; i < sizeC; i++) { // Recherche du premier élément HashCell non nul pour initialiser le max
        if (tableC->tab[i] != NULL) {
            max = tableC->tab[i];
            break;
        }
    }
    for (int i = 1; i < sizeC; i++) { // Comparaison des nombres d'occurences pour déterminer le max
        if (tableC->tab[i] != NULL && tableC->tab[i]->val > max->val) {
            max = tableC->tab[i];
        }
    }
    Key* res = (Key*)malloc(sizeof(Key));
    if (res == NULL) {
        printf("ERREUR MALLOC\n");
        exit(-1);
    }
    init_key(res, max->key->k, max->key->n);
    delete_hashtable(tableC);
    delete_hashtable(tableV);
    return res;
}
