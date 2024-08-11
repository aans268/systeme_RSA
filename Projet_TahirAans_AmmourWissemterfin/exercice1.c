#include "structures.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

/* calcule a à la puissance b */
double puissance(double a, double b){
    double res=1;
    for (int i=0; i<b; i++){
        res=res*a;
    }
    return res;
}

/* etant donne un entier impair p, renvoie 1 si p est premier et 0 sinon */
int is_prime_naive(long p){
    if((p<3)||(p==4)){
        return 0;
    }
    if(p==3){
        return 1;
    }
    int i;
    for(i=3;i<p;i++){
        if(p%i==0){
            return 0;
        }
    }
    return 1;
}


/* prend en entree trois entiers a, m et n, et qui retourne la valeur a^b mod n par la methode naive */
long modpow_naive(long a, long m, long n){
    long res=1;
    int i;
    for(i=1;i<=m;i++){
        res=(res*a)%n;
    }
    return res;
}


/* meilleure version de modpow_naive en realisant des elevations au carre */
int modpow(long a, long m, long n){
    a=a%n;
    if(m==0){
        return 1;
    }
    int res= modpow(a,m/2,n);
    if(m%2 ==1){
        return a * res * res %n;
    }
    else{
        return res * res % n;
    }
}

/*teste si a est un temoin de Miller pour p, pour un entier a donne.*/
int witness ( long a , long b , long d , long p ) {
	long x = modpow (a ,d , p ) ;
	if ( x == 1) {
		return 0;
	}
	for ( long i = 0; i < b ; i ++) {
		if ( x == p -1) {return 0;
		}
		x = modpow (x ,2 , p ) ;
	}
	return 1;
}

/*  retourne un entier long genere aleatoirement entre low et up inclus.*/
long rand_long ( long low , long up ) {
	return rand () % ( up - low +1) + low ;
}



/* realise le test de Miller-Rabin en generant k valeurs de a au hasard, 
et en testant si chaque valeur de a est un temoin de Miller pour p.
    la fonction retourne 0 des qu’un temoin de Miller est trouve (p n’est pas premier), et retourne 1 si aucun temoin de Miller n’a ete trouve (p est tres probablement premier) */
    
int is_prime_miller ( long p , int k ) {
	if ( p == 2) {
		return 1;
	}
	if (!( p & 1) || p <= 1) { //on verifie que p est impair et different de 1
		return 0;
	}
 //on determine b et d :
	long b = 0;
	long d = p - 1;
	while (!( d & 1) ) { //tant que d n’est pas impair
		d = d /2;
		b = b +1;
	}
 // On genere k valeurs pour a, et on teste si c’est un temoin :
	long a ;
	int i ;
	for ( i = 0; i < k ; i ++) {
		a = rand_long (2 , p -1) ;
		if ( witness (a ,b ,d , p ) ) {
			return 0;
		}
	}
	return 1;
}


/*la fonction prends deux entiers low_size et up_size representant respectivement la taille minimale et maximale du nombre premier a generer 
et un entier k representant le nombre de tests de Miller a realiser,
    retourne un nombre premier de taille comprise entre low size et up size*/

long random_prime_number(int low_size, int up_size, int k){
    long max = puissance(2,up_size);
    long min = puissance(2,low_size);
    long nombre = rand_long(min,max);
    while (!(is_prime_miller(nombre,k))){
        nombre = rand_long(min,max);
    }
    return nombre;
}
