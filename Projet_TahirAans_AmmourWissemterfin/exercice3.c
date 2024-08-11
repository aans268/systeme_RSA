#include "structures.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>


/* initialise une cle deja allouee */
void init_key(Key *key, long val, long n){
    if(key){
        key->k=val;
        key->n=n;
    }
    else{
        printf("Erreur la clé n'est pas initialisée.");
    }
}


/*utilise le protocole RSA pour initialiser une cle publique et 
une cle secrete (deja allouees) */
void init_pair_keys(Key *pKey, Key *sKey, long low_size, long up_size){
    //Generation de cle :
    long p = random_prime_number(low_size ,up_size , 5000);
    long q = random_prime_number(low_size ,up_size , 5000);
    while ( p == q ){
        q = random_prime_number(low_size ,up_size , 5000);
    }
    long n,s,u;
    generate_key_values(p ,q ,&n ,&s ,&u ) ;
    //Pour avoir des cles positives :
    if (u <0) {
        long t = (p -1) *( q -1) ;
        u = u + t ; //on aura toujours s*u mod t = 1
    }
    init_key(pKey,s,n);
    init_key(sKey,u,n); 
}


/* permet de passer d’une variable de type Key a sa representation 
sous forme de chaıne de caracteres */
char* key_to_str(Key* key){
    long k= key->k;
    long n=key->n;
    char * chaine= (char *)malloc(sizeof(char)*256);
    sprintf(chaine,"(%lx,%lx)",k,n);
    return chaine;
}


/* inverse de key_to_str */
Key* str_to_key(char* str){
    Key *cle= (Key*)malloc(sizeof(Key));
    long k;
    long n;
    sscanf(str,"(%lx,%lx)",&k,&n);
    init_key(cle,k,n);
    return cle;
}


/* permet d’allouer et de remplir une signature avec un tableau de long 
deja alloue et initialise */
Signature* init_signature(long* content, int size){
    Signature* sign= (Signature*) malloc(size*sizeof(Signature));
    if(sign==NULL){
        printf("Probleme d'allocation \n");
        return NULL;
    }
    sign->content=content;
    sign->size=size;
    return sign;
}


/* cree une signature a partir du message mess (declaration de vote) et 
de la cle secrete de l’emetteur */
Signature* sign(char* mess, Key* sKey){
    long *res;
    res=encrypt(mess, sKey->k, sKey->n);
    return init_signature(res,strlen(mess));
}


/*permet de passer d’une Signature a sa representation sous forme de 
chaıne de caracteres */
char* signature_to_str(Signature* sgn){
    char* result = malloc(10*sgn->size*sizeof(char));
    result[0]='#';
    int pos = 1;
    char buffer[156];
    for(int i=0; i < sgn->size; i++){
        sprintf(buffer, "%lx", sgn->content[i]);
        for(int j=0;j < strlen(buffer); j++){
            result[pos] = buffer[j];
            pos++;
        }
        result[pos] = '#';
        pos++;
    }
    result[pos]='\0';
    result = realloc(result, (pos+1)*sizeof(char));
    return result;
}


/* inverse de signature_to_str */
Signature* str_to_signature(char* str){
    int len = strlen(str);
    long* content = (long*)malloc(sizeof(long)*len);
    int num = 0;
    char buffer[256];
    int pos = 0;
    for(int i=0; i<len; i++){
        if(str[i] != '#'){
            buffer[pos] = str[i];
            pos++;
        }else{
            if(pos!=0){
                buffer[pos] = '\0';
                sscanf(buffer, "%lx", &(content[num]));
                num++;
                pos=0;
            }
        }
    }
    content=realloc(content,num*sizeof(long));
    return init_signature(content, num);
}



/* alloue et initialise la structure Protected (cette fonction ne verifie pas si la 
signature est valide : veuillez utiliser la fonction verify pour cela) */
Protected* init_protected(Key* pKey, char* mess,Signature* sgn){
    Protected* prt = (Protected*) malloc(sizeof(Protected));
    prt->pKey = pKey;
    prt->mess = mess;
    prt->sign = sgn;

    return prt;
}


/* verifie que la signature contenue dans pr correspond bien au message 
et a la personne contenus dans pr donc verifie que la signature est valide ou non */
int verify(Protected* pr){
    int taille=pr->sign->size;
    char* decr= (char*)malloc(sizeof(char)*taille);
    decr= decrypt(pr->sign->content,taille,pr->pKey->k,pr->pKey->n);
    int i;
    for(i=0;i<taille;i++){
        if(pr->mess[i]!=decr[i]){
            printf("LA SIGNATURE EST FAUSSE\n");
            return 0;
        }
    }
    printf("La signature est vérifiée\n");
    free(decr);
    return 1;
}


/* permet de passer d’un Protected a sa representation sous forme de chaıne de caracteres */
char* protected_to_str(Protected* pr){
    char *chaine= (char *)malloc(sizeof(char)*(256+2*pr->sign->size));
    sprintf(chaine,"%s %s %s",key_to_str(pr->pKey),pr->mess,signature_to_str(pr->sign));
    //printf("%s",chaine);
    return chaine;
}


//inverse de protected_to_str
Protected* str_to_protected(char* chaine){

	Protected* newPr = (Protected *)malloc(sizeof(Protected));

	char* pkey = (char *)malloc(256*sizeof(char));
	char* mess = (char *)malloc(256*sizeof(char));
	char* sgn = (char *)malloc(256*sizeof(char));
	

	sscanf(chaine,"%s%s%s",pkey,mess,sgn);

	Key* newKey = str_to_key(pkey);
	Signature* newSgn =  str_to_signature(sgn);

	newPr->pKey = newKey;
	newPr->mess = strdup(mess);
	newPr->sign = newSgn;

	free(pkey);
	free(mess);
	free(sgn);

	return newPr;
}


// libère la mémoire d'un objet de type protected
void delete_protected(Protected * p){
    free(p->pKey);
    free(p->sign->content);
	free(p->sign);
    free(p);
}