#include "structures.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>


/* calcule la valeur PGCD(s,t) et determine les entiers 
u et v verifiant l’equation de Bezout : s×u + t×v = PGCD(s, t) */
long extended_gcd(long s, long t, long* u, long* v){
    if (s==0){
        *u=0;
        *v=1;
        return t;
    }
    long uPrim, vPrim;
    long gcd=extended_gcd(t%s,s,&uPrim,&vPrim);
    *u=vPrim-(t/s)*uPrim;
    *v=uPrim;
    return gcd;
}


/* calcule le plus grand commun diviseur de a et b */
int pgcd(int a, int b){
    int pgcd;
    for(int i=1; i <= a && i <= b; ++i){
        if(a%i==0 && b%i==0){
            pgcd = i;
        }
    }
    return pgcd;
}


/* la fonction genere la cle publique pkey = (s, n) et la cle secrete skey = (u, n),
 a partir des nombres premiers p et q, en suivant le protocole RSA */
void generate_key_values(long p, long q, long* n, long* s, long* u){
    *n=p*q;
    long t=(p-1)*(q-1);
    *s=rand()%t;
    while (pgcd(*s,t)!=1){
        *s=rand()%t;
    }
    long v;
    extended_gcd(*s,t,u,&v);
}


/* chiffre la chaîne de caracteres chaine avec la cle publique pKey = (s,n)
la fonction cast (convertit) chaque caractere en un entier de type int
retourne le tableau de long obtenu en chiffrant ces entiers */
long* encrypt(char *chaine, long s, long n){
    long *tab= (long *) malloc(sizeof(long)*strlen(chaine));
    int i;
    for(i=0; i<strlen(chaine);i++){
        tab[i]= modpow((long) chaine[i],s,n);
    }
    return tab;
}


/* dechiffre un message a l’aide de la cle secrete skey = (u, n), en connaissant la taille du tableau d’entiers.
la fonction renvoie la chaîne de caracteres obtenue */
char* decrypt(long* crypted, int size, long u, long n){
    char *msg= (char *)malloc(sizeof(char)*(size+1));
    int i;
    for(i=0;i<size;i++){
        msg[i]=(char) modpow(crypted[i],u,n);
    }
    msg[size]='\0';
    return msg;
}


/*prends en parametre le message chiffre et sa taille
affiche le tableau chiffre du message*/
void print_long_vector ( long * result , int size ) {
    printf ( " Vector : [ " ) ;
    for ( int i =0; i < size ; i ++) {
    printf ( "%lx \t " , result [ i ]) ;
    }
    printf ( " ] \n" ) ;
}


