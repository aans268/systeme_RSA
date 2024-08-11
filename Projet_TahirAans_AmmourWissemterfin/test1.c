#include "structures.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

int main(void){
    srand(time(NULL));
    //Tests exercies 1 et 2
    
    //printf("%lx\n",modpow_naive(3,4,5));
    //printf("%d\n",modpow(3,4,5));
    
    FILE *fichier = fopen("compmodpow.txt", "w");
	if(!fichier){
		printf("Error: pas de fichiers ouvert");
	}
    //Tests a effectuer
    int m=4;
    int i, j;
    //intialisation clock
    clock_t temps_initial1;
    clock_t temps_final1;
    double temps_cpu1;
    clock_t temps_initial2;
    clock_t temps_final2;
    double temps_cpu2;
    //Traitement
    for (i=1;i<m;i++){
        for(j=2;j<m;j++){
            temps_initial1 = clock();
            modpow_naive(i,m,j);
            temps_final1 = clock();
            temps_initial2 = clock();
            modpow(i,m,j);
            temps_final2 = clock();
            temps_cpu2 = (( double)(temps_final2  - temps_initial2))/CLOCKS_PER_SEC;
            temps_cpu1 = (( double)(temps_final1  - temps_initial1))/CLOCKS_PER_SEC;
            fprintf(fichier,"%d\t %f\t %f\n",m,temps_cpu1,temps_cpu2);
        }
    }
    fclose(fichier);
    
    srand ( time ( NULL ) ) ;

    //Generation de cle :
    long p = random_prime_number(3 ,5 , 5000) ;
    long q = random_prime_number(3 ,5 , 5000) ;
    while ( p == q ){
        q = random_prime_number(3 ,5 , 5000) ;
    }
    long n,s,u;
    generate_key_values(p ,q ,&n ,&s ,&u ) ;
    //Pour avoir des cles positives :
    if (u <0) {
        long t = (p -1) *( q -1) ;
        u = u + t ; //on aura toujours s*u mod t = 1
    }

    //Afichage des cles en hexadecimal
    printf( " cle publique = (%lx, %lx ) \n",s,n) ;
    printf( " cle privee = (%lx ,%lx ) \n",u,n) ;

    //Chiffrement:
    char msg [10] = "Hello";
    int len = strlen(msg);
    long * crypted = encrypt(msg,s,n);

    printf ("Initial message: %s \n",msg);
    printf (" Encoded representation: \n");
    print_long_vector( crypted , len ); 

    //Dechiffrement
    char *decoded = decrypt(crypted,len,u,n);
    printf ("Decoded : %s \n",decoded );


    return 0;
}