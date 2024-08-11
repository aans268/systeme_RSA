#ifndef STRUCTURES_H
#define STRUCTURES_H

//exercice3
typedef struct key{
    long k;
    long n;
}Key;

typedef struct signature{
    long *content; //tableau
    int size;
}Signature;


typedef struct protected{
    Key* pKey;
    char* mess;
    Signature* sign;
}Protected;

//exercice5
typedef struct cellKey {
	Key * data ;
	struct cellKey* next ;
} CellKey ;


typedef struct cellProtected{
	Protected* data ;
	struct cellProtected* next ;
} CellProtected ;

//exercice6
typedef struct hashcell{
    Key* key;
    int val;
} HashCell;

typedef struct hashtable{
    HashCell** tab;
    int size;
} HashTable;

//exercice7
typedef struct block{
    Key* author;
    CellProtected* votes;
    unsigned char* hash;
    unsigned char* previous_hash;
    int nonce;
}Block;

//exercice8
typedef struct block_tree_cell {
    Block * block ;
    struct block_tree_cell * father ;
    struct block_tree_cell * firstChild ;
    struct block_tree_cell * nextBro ;
    int height ;
} CellTree ;

//exercice1
double puissance(double a, double b);
int is_prime_naive(long p);
long modpow_naive(long a, long m, long n);
int modpow(long a, long m, long n);
int witness ( long a , long b , long d , long p );
long rand_long ( long low , long up );
int is_prime_miller ( long p , int k );
long random_prime_number(int low_size, int up_size, int k);

//exercice2
long extended_gcd(long s, long t, long* u, long* v);
int pgcd(int a, int b);
void generate_key_values(long p, long q, long* n, long* s, long* u);
long* encrypt(char *chaine, long s, long n);
char* decrypt(long* crypted, int size, long u, long n);
void print_long_vector ( long * result , int size );

//exercice3
void init_key(Key *key, long val, long n);
void init_pair_keys(Key *pKey, Key *sKey, long low_size, long up_size);
char* key_to_str(Key* key);
Key* str_to_key(char* str);
Signature* init_signature(long* content, int size);
Signature* sign(char* mess, Key* sKey);
char* signature_to_str(Signature* sgn);
Signature* str_to_signature(char* str);
Protected* init_protected(Key* pKey, char* mess,Signature* sgn);
int verify(Protected* pr);
char* protected_to_str(Protected* pr);
Protected* str_to_protected(char* chaine);
void delete_protected(Protected* p);

//exercice4
int is_in(Key** listeKey,int size, Key* acomp);
void generate_random_data(int nv, int nc);

//exercice5
CellKey* create_cell_key(Key* key);
CellKey* add_cell_key(Key* key, CellKey* liste);
CellKey* read_public_keys(char* nom);
void print_list_keys(CellKey* LCK);
void delete_cell_key(CellKey** c);
void delete_key_list(CellKey** c);
CellProtected* create_cell_pr(Protected* pr);
CellProtected* add_cell_pr(Protected* pr, CellProtected* liste);
CellProtected* add_cell_pr_end(Protected* pr, CellProtected* liste);
CellProtected* read_protected();
void print_pr(CellProtected* liste);
void delete_cell_protected(CellProtected* c);
void delete_liste_pr(CellProtected* liste);

//exercice6
CellProtected* tri_decla_valide(CellProtected* liste);
HashCell* create_hashcell(Key* key);
int hash_function(Key* key, int size);
int find_position(HashTable* t,Key* key);
int equals(Key* k1,Key* k2);
HashTable* create_hashtable(CellKey* keys, int size);
void delete_hashtable(HashTable* t);
Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV);
int occurence_key(Key *key,CellKey *list);

//exercice7
void ecriture_block(char* nom,Block* block);
Block* lecture_block();
char* block_to_str(Block* block);
void supprime(char *texte, char x);
unsigned char* hash_SHA256(const unsigned char *s);
void compute_proof_of_work(Block* b, int d);
int verify_block(Block* b,int d);
void delete_block(Block* b);
char* clear_str(char* ligne);

//exercice8
CellTree* create_node(Block* b);
int max_height(int n,int m);
int update_height(CellTree* father, CellTree* child);
void add_child(CellTree* father, CellTree* child);
void print_tree(CellTree* tree);
void delete_node(CellTree* node);
void delete_tree(CellTree* father);
CellTree* highest_child(CellTree* cell);
CellTree* last_node(CellTree* tree);
CellProtected* fusion_liste(CellProtected* c1,CellProtected* c2);
CellProtected* fusion_cp_tree(CellTree* tree, CellProtected* liste);

//exercice9
void submit_vote(Protected* p);
void create_block(CellTree* tree, Key* author, int d);
void add_block(int d, char* name);
CellTree* read_tree();
Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV);


#endif 