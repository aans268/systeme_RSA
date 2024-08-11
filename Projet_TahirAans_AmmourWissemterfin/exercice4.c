#include "structures.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>


/*fonction rajoutée permettant de savoir si un couple de clé est dans 
la liste listeKey utilisee dans generate_random_data*/
int is_in(Key** listeKey,int size, Key* acomp){
    if(listeKey==NULL){
        return 0;
    }
    int i =0;
    for (i=0;i<size;i++){
        Key* atest= listeKey[i];
        if (((atest->k)==(acomp->k)) && ((atest->n)==(acomp->n))){
            return 1;
        }    
    }
    return 0;
}



void generate_random_data(int nv, int nc){
    int i;
    //generation des cles
    Key* liste_pkey[nv];
    Key* liste_skey[nv];
    for (i=0;i<nv;i++){
        Key* pKey = (Key*)malloc(sizeof(Key));
        if(pKey==NULL){
            printf("Erreur malloc dans generate_random_data\n");
            return;
        }
        Key* sKey = (Key*)malloc(sizeof(Key));
        if(sKey==NULL){
            printf("Erreur malloc dans generate_random_data\n");
            return;
        }
        init_pair_keys(pKey, sKey, 1, 5);
        liste_pkey[i]=pKey;
        liste_skey[i]=sKey;
    }
    //Ecriture des Clés
    FILE *keys_file = fopen("keys.txt", "w");
    char* str1;
    char* str2;
    if(!keys_file){
        printf("Error: pas de fichiers ouvert key\n");
        return;
    }
    else{
        for (i=0;i<nv;i++){
            str1=key_to_str(liste_pkey[i]);
            str2=key_to_str(liste_skey[i]);
            fprintf(keys_file,"(%s,%s)\n",str1,str2);
            free(str1);
            free(str2);
        }
        fclose(keys_file);
    }
    //generation des candidats 
    Key* liste_candidats[nc];
    int ind;
    FILE *c_file = fopen("candidats.txt", "wr");
    char* str3;
    if(!c_file){
        printf("Error: pas de fichiers ouvert candidats\n");
        return;
    }
    else{
        for(i =0; i<nc;i++){
            ind=rand()%(nv);
            while (is_in(liste_candidats,i,liste_pkey[ind])==1){
                ind=rand()%(nv);
            }
            liste_candidats[i]= liste_pkey[ind];
            str3=key_to_str(liste_candidats[i]);
            fprintf(c_file,"(%s)\n",str3);
            free(str3);
        }
        fclose(c_file);
    }
    
    char* str4;
    for (i=0;i<nc;i++){
        str4=key_to_str(liste_candidats[i]);
        free(str4);
    }
    //generation des declarations
    FILE *d_file = fopen("declarations.txt", "wr");
    char* key;
    char* sgn2;
    if(!d_file){
        printf("Error: pas de fichiers ouvert candidats\n");
        return;
    }
    else{
        for (i=0;i<nv;i++){
            int vote=rand()%(nc);
            char* mess= key_to_str(liste_candidats[vote]);;
            Signature* sgn= sign(mess,liste_skey[i]);
            key=key_to_str(liste_pkey[i]);
            sgn2=signature_to_str(sgn);
            fprintf(d_file,"%s %s %s\n",key,mess,sgn2);
            free(key);
            free(sgn2);
            free(mess);
            free(sgn->content);
            free(sgn);
        }
        fclose(d_file);
    }
    for(i=0;i<nv;i++){
        free(liste_pkey[i]);
        free(liste_skey[i]);
    }
}
