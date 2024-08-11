#include "structures.h"
#include <openssl/sha.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>


/* ecriture d'un bloc dans un fichier  */
void ecriture_block(char* nom,Block* b){
    FILE* f=fopen(nom,"w");
    if(f==NULL){
        return;
    }
    char* cle=key_to_str(b->author);
    fprintf(f,"%s\n",cle);
    free(cle);
    fprintf(f,"%s\n",b->hash);
    fprintf(f,"%s\n",b->previous_hash);
    fprintf(f,"%d\n",b->nonce);
    CellProtected* debut=b->votes;
    char* str;
    while(b->votes){
        str=protected_to_str(b->votes->data);
        fprintf(f,"%s\n",str);
        free(str);
        b->votes=b->votes->next;
    }
    b->votes=debut;

    fclose(f);
} 
  


/* lecture d'un bloc depuis un fichier. */
Block* lecture_block(){
    Block* block=(Block*)malloc(sizeof(Block));
    FILE *b_file= fopen("block.txt", "r");
    if(!b_file){
        printf("Error: pas de fichier ouvert block.txt\n");
    }
    char ligne[256];
    
    fgets(ligne,256,b_file); //auteur
    block->author=str_to_key(ligne);
    
    fgets(ligne,256,b_file); //valeur hachée du bloc
    block->hash=(unsigned char*)strdup(ligne);
    
    fgets(ligne,256,b_file); //valeur hachée du bloc précédent
    if(strcmp(ligne,"(null)")==0){
        block->previous_hash=NULL;
    }else{
        block->previous_hash=(unsigned char*)strdup(ligne);
    }
    fgets(ligne,256,b_file); //proof of work
    block->nonce=atoi(ligne);

    while(fgets(ligne,256,b_file)!=NULL){ //liste declarations
        block->votes=add_cell_pr_end(str_to_protected(ligne),block->votes);
    }
    fclose(b_file);
    return block;
}

/* génère une chaîne de caracteres representant un bloc. */
char* block_to_str(Block* block){
    if(!block){
        printf("block vide");
        return NULL;
    }
    char* author = key_to_str(block->author);
    unsigned char* hash =block->hash;
    unsigned char* previous_hash =block->previous_hash;
    int nonce= block->nonce;
    char* chaine=(char*) malloc(sizeof(char)*500);
    sprintf(chaine,"%s%s%s%d", author,hash, previous_hash, nonce);
    CellProtected* tmp=block->votes;
    if(block->votes==NULL){
    }
    while(block->votes){
        strcat(chaine,protected_to_str(block->votes->data));
        block->votes=block->votes->next;
    }
    block->votes=tmp;
    supprime(chaine,'\n');
    return chaine;
}


/*supprime les toutes les occurences de x dans texte*/
void supprime(char *texte, char x){
      int p ,i;
      for(i=0;texte[i] != '\0';i++){
          if (texte[i] == x){
              for(p=i; texte[p]!=0; p++)
                  texte[p]=texte[p+1];
              i--;
        }
    }
}

unsigned char * hash_SHA256(const unsigned char * s){
    unsigned char* d=SHA256(s,strlen(s),0);
    char* res=(char *)malloc(sizeof(char)*SHA256_DIGEST_LENGTH*2+2);
    char* tmp=(char *)malloc(sizeof(char)*2+2);
    for (int i=0;i<SHA256_DIGEST_LENGTH;i++){
        sprintf(tmp,"%02x",d[i]);
        strcat(res,tmp);
    }
    free(tmp);
    return res;
}


void compute_proof_of_work(Block* B, int d){
    B->nonce=0;
    while(verify_block(B,d)==0){
        B->nonce++;
    }
}


/* verifie qu’un bloc est valide */
int verify_block(Block* B,int d){
    char* str=hash_SHA256(block_to_str(B));
    for(int i=0;i<d;i++){
        if(str[i]!='0'){
            free(str);
            return 0;
        }
    }
    free(str);
    return 1;
}


/* supprime un bloc */
void delete_block(Block* b){
    free(b->hash);
    free(b->previous_hash);
    CellProtected* tmp;
    while(b->votes){
        tmp=b->votes;
        b->votes=b->votes->next;
        free(tmp);
    }
    free(b);
}







////////////////////////////// Test à part
char* clear_str(char* ligne){
    char *res=(char*) malloc(sizeof(char)*(strlen(ligne)-1));
    for(int i=0; i<strlen(ligne)-1;i++){
        if(ligne[i]=='\n'&&ligne[i+1]=='\n'){
            res[i]=ligne[i];
            return res;
        }
        res[i]=ligne[i];
    }
    return res;
}