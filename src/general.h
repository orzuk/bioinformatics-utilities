// File general.h - Macros, prototypes and constant of the DNA package
#ifndef _GENERAL_H_
#define _GENERAL_H_


// New types defined 
typedef unsigned long int word;   // machine word (32 bit on windows - pc)

typedef unsigned char byte;       // a byte

// Macroes 
#define BIT(block, num)  ( ((block) >> (num)) & (1UL) )  // get the i-th bit from a word
#define BITS(block, num1, num2)  ( ((block) >> (num1)) & ((1UL << (num2+1-num1))-1) )  // get the i-th to j-th bits from a word

// inverse the order of the bits in a word
#define INVERSE(block) \
	(block) = (((block)&0x0000FFFF) << 16 ) ^ (((block) >> 16)&0x0000FFFF); \
	(block) = (((block)&0x00FF00FF) << 8 ) ^ (((block) >> 8)&0x00FF00FF); \
	(block) = (((block)&0x0F0F0F0F) << 4 ) ^ (((block) >> 4)&0x0F0F0F0F); \
	(block) = (((block)&0x33333333) << 2 ) ^ (((block) >> 2)&0x33333333); \
	(block) = (((block)&0x55555555) << 1 ) ^ (((block) >> 1)&0x55555555); 

// inverse the even and the odd bits in a block
#define INV_PAR(block) \
	(block) = (((block)&0x55555555) << 1 ) ^ (((block) >> 1)&0x55555555); 

// inverse the order of bytes in a word (this is for changing indians)
#define INDY(block) \
	(block) = (((block)&0x0000FFFF) << 16 ) ^ (((block) >> 16)&0x0000FFFF); \
	(block) = (((block)&0x00FF00FF) << 8 ) ^ (((block) >> 8)&0x00FF00FF); 



// swap elements 
#define SWAP(A, B, TEMP)	\
			(TEMP) = (A);   \
			(A) = (B);      \
			(B) = (TEMP);


// swap two arrays (convert size from words to bytes)
#define SWAP_ARRAYS(A, B, TEMP, len_in_bytes)	\
	memcpy((TEMP), (A), (len_in_bytes));  \
	memcpy((A), (B), (len_in_bytes));  \
	memcpy((B), (TEMP), (len_in_bytes));  

// swap bits inside one word
#define SWAP_S(addr) \
	(addr) = ((addr)&0x20) ^ (((addr)&0x1) << 4) ^ (((addr)&0x1E) >> 1);

#define ARGMAX(x,y) ((x) > (y) ? (0) : (1))
#define ARGMIN(x,y) ((x) < (y) ? (0) : (1))

#define ARGMAX3(x,y,z, A) \
        (t_xy) = (ARGMAX((y),(x))); \
        (t_xz) = (ARGMAX((z),(x))); \
        (t_yz) = (ARGMAX((y),(z))); \
        (A) = (1-(t_xy)*(t_xz)) * (1 + (t_yz));


#define MAX(x,y) (x) > (y) ? (x) : (y)
#define MIN(x,y) (x) < (y) ? (x) : (y)

#define ABS(x)  ((x) > 0 ?  (x) : (-(x)))

#ifndef TRUE
#define TRUE 0
#endif
#ifndef FALSE
#define FALSE 1
#endif


#define MAX_MODEL_SIZE 6

#define WORD_SIZE 32
#define HALF_WORD_SIZE (WORD_SIZE/2)

#define SIZE_IN_WORDS(A) ((A) >> 4)   /* A usually represents the size in basepairs */

// Get the X'th base from the word A. 0 <= X <= 15
#define GET_BASE(A, X) ( ((A) >> (2*(X)))&0x3 )

// Get the substring containind the base-pairs X to Y : 0 <= X <= Y <= 15
#define GET_BASES(A, X, Y) ( ((A) >> (2*(X)))&0x3 )


// A - 00 T - 01 C - 10 G - 11
// The complement base A<->T and C<->G
#define COMP_BASE(A) ((A) ^ 1) 

// end of line in ASCII
#define EOLN 10

// TAB in ASCII
#define TAB 9

#define SEQ_LEN 100


// Maximal length in words - corresponding to 64*16 = 1024 basepairs
#define MAX_SEQ_LEN 64 

#define MAX_NAME_LENGTH 40

#define MAX_SAMPLES 300

#define RAMAS_SAMPLES 256

// 12980
#define HUMAN13K_GENES 1000  

#define MAX_GENES_NUM HUMAN13K_GENES 

#define EXP_GENES_NUM 7129

#define SLAK_ITERS 50

#define MAX_L 256    // length of sequence in words. Very large, (thanx to Paz ....) 

#define L_GENES 235

#define L_TF  24

// Maximal number of genes 
#define MAX_NUM_SEQS 1000  


#define MIN_SIGMA 0.000005 


#define NUM_BASEPAIRS_PER_HMM_TRANS 250000.0

#define PRINT_IN_ROW 20

// Number of different values for X  (better take multiplies of 8 !!!)
#define MAX_X_VALS 5 

// Number of different values / Mixtures for Y 
#define MAX_Y_VALS 5  

#define MAX_2D_X_VALS 25

// Maximal power of M to compute 
#define MAX_POWER_OF_TRANS_MATRIX 50


// Discrete and Continunuos flags
#define DISCRETE 0
#define CONTINUOUS 1

// Number of steps we do until we perform scaling !!
#define DO_SCALE 1
  

#define ONE_PI 3.141592654
#define SQRT_2PI 2.506628274
#define ONE_OVER_2PI 0.1591549430
#define ONE_OVER_2SQRT_PI  0.3989422802
#define ONE_OVER_2SQRT_PI_LOG (-0.9189385337)

#define EPSILON 0.000001


// This is the number of iterations afterwhich we check if our score is improved !!! 
#define FIRST_ITERS 10
// This number indicates the closeness forwhich we continue to iterate
#define IMPROVEMENT_FRACTION 0.01   

// Maximum possible number of starting points on EM
#define MAX_STARTING_POINTS 50


#define MAX_NUM_GAUSSIANS 10 // Stuff for Mixture of Gaussians 





// The total index
// RelevantInds = [1:6 17:22 33:38 49:54 65:70 81:86]; 
/**
static long total_index_tab[36] = {
        0, 1, 2, 3, 4, 5, 16,17,18,19,20,21, 32,33,34,35,36,37, 48,49,50,51,52,53, 64,65,66,67,68,69, 80,81,82,83,84,85 
};    
**/

// The same indexes, but in a multi dimensional array
/***
static long multi_dim_total_index_tab[2][2][3][3] = {
           0, 32, 64,  2, 34, 66,  4, 36, 68, 
      16, 48, 80, 18, 50, 82, 20, 52, 84, 
           1, 33, 65,  3, 35, 67,  5, 37, 69, 
          17, 49, 81, 19, 51, 83, 21, 53, 85}; 
***/

////////// static long multi_dim_total_index_tab[2][2][3][3] = {
static long multi_dim_total_index_tab[3][2][3][2] = {
         0,  1,  2,  3,  4,  5,  6,  7,  8, 
         9, 10, 11, 12, 13, 14, 15, 16, 17, 
        18, 19, 20, 21, 22, 23, 24, 25, 26, 
        27, 28, 29, 30, 31, 32, 33, 34, 35};  // This one is used ALOT.

/**/
static long multi_dim_A_copy_tab[2][2][3][3] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 1, 2, 0, 1, 2, 0, 1, 2,
        0, 0, 0, 1, 1, 1, 2, 2, 2, 
        0, 1, 2, 1, 2, 3, 2, 3, 4};   // This one is NEVER used
/**/

static long multi_dim_B_copy_tab[2][2][3][3] = {
        0, 1, 2, 1, 2, 3, 2, 3, 4, 
        0, 0, 0, 1, 1, 1, 2, 2, 2,
        0, 1, 2, 0, 1, 2, 0, 1, 2, 
        0, 0, 0, 0, 0, 0, 0, 0, 0};   // This one is also NEVER used

/**
static long A_copy_tab[36] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 1, 2, 0, 1, 2, 0, 1, 2, 
        0, 0, 0, 1, 1, 1, 2, 2, 2,
        0, 1, 2, 1, 2, 3, 2, 3, 4};
**/

/**
static long B_copy_tab[36] = {
        0, 1, 2, 1, 2, 3, 2, 3, 4, 
        0, 0, 0, 1, 1, 1, 2, 2, 2, 
        0, 1, 2, 0, 1, 2, 0, 1, 2,
        0, 0, 0, 0, 0, 0, 0, 0, 0};
**/

/**
static long A_copy_tab[36] = {
        0, 0, 1, 0, 2, 0, 0, 0, 1, 
        0, 2, 0, 1, 1, 2, 1, 3, 1, 
        0, 0, 1, 0, 2, 0, 2, 2, 3,
        2, 4, 2, 0, 0, 1, 0, 2, 0};

static long B_copy_tab[36] = {
        0, 0, 0, 1, 0, 2, 0, 0, 0, 
        1, 0, 2, 0, 0, 0, 1, 0, 2, 
        1, 1, 1, 2, 1, 3, 0, 0, 0,
        1, 0, 2, 2, 2, 2, 3, 2, 4};
**/

/***********************************
static long A_copy_tab[36] = {
        0, 0, 0, 1, 0, 2, 0, 0, 0, 
        1, 0, 2, 0, 0, 0, 1, 0, 2, 
        1, 1, 1, 2, 1, 3, 0, 0, 0,
        1, 0, 2, 2, 2, 2, 3, 2, 4};
static long B_copy_tab[36] = {
        0, 0, 1, 0, 2, 0, 0, 0, 1, 
        0, 2, 0, 1, 1, 2, 1, 3, 1, 
        0, 0, 1, 0, 2, 0, 2, 2, 3,
        2, 4, 2, 0, 0, 1, 0, 2, 0};
/***********************************/

static long A_copy_tab[36] = {
        0, 0, 1, 0, 2, 0, 0, 0, 1, 
        0, 2, 0, 1, 1, 2, 1, 3, 1, 
        0, 0, 1, 0, 2, 0, 2, 2, 3,
        2, 4, 2, 0, 0, 1, 0, 2, 0};  // This one is sometimes used 

static long B_copy_tab[36] = {
        0, 0, 0, 1, 0, 2, 0, 0, 0, 
        1, 0, 2, 0, 0, 0, 1, 0, 2, 
        1, 1, 1, 2, 1, 3, 0, 0, 0,
        1, 0, 2, 2, 2, 2, 3, 2, 4};  // This one is also sometimes used 


// Minimum and maximum values for MU 
// These correspond to total copy # levels: 0, 1, 2, 3, 4
static double MIN_MU_VAL[5] = {0, 0.6, 1.5, 2.3, 2.5};
static double MAX_MU_VAL[5] = {0.5, 1.3, 2.5, 3.3, 4.4};

#define SNPS_ONE_SAMPLE 1
#define SNPS_TWO_SAMPLES 2



#endif
