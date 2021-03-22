/*
This code is based on the Count-Min Sketch by Cormode, Muthukrishnan. 
.h: https://www.cs.rutgers.edu/~muthu/countmin.h 
.c: https://www.cs.rutgers.edu/~muthu/countmin.c
This code extends its basic structure by adding the KMV-Sketch. 
Thus, only the basic concept (update etc.) are used while data types are changed for the specific intentions.
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>


/*
kmv.h 		: used to add KMV-features into the Count-Min Sketch
mergesort.h : used to sort the elements in the KMV Sketch (needed for example for the union of two KMVs 
			and only taking the k smallest elements in account) - uint32_t elements are used
murmurhash.h: used to hash the elements
merge_double.h 	: used to calculate Median of Point Estimation, Union and Intersection - for that we need 
				to get a sorted list of all the elements and take the one in the "middle". - double elements are used
*/
#include "kmv.h"
#include "mergesort.h"
#include "murmurhash.h"
#include "merge_double.h"

// MurmurHash uses 32 Bit Variables - if we use unsigned integers, our biggest number would be 0xFFFFFFFF 
// This will be used to calculate the Estimation of the specific KMV of the field in the KMV
#define MAX_ELEM 0xFFFFFFFF

// Basic Functions to determine the smaller/bigger number
#define min(x,y)	((x) < (y) ? (x) : (y))
#define max(x,y)	((x) > (y) ? (x) : (y))

// Struct for the KMV with its length (how many numbers can be stored) and an array of uint32_t elements
typedef struct{
	int kmv_length;
	uint32_t* kmv_sketch;
}cms_field;

// Struct for the KMV-based Count-Min Sketch
// depth and width are the params for the Count-Min Sketch, kmv is the length of one KMV (equal to the KMV struct kmv_length), 
// cm2d is a 2D-Array of cms_field arrays, seed is the parameter for the use of the Murmurhash and 
// hasha contains all the seed numbers for each hashfunction (which is responsible for each row in the Count Min Sketch)
typedef struct{
	int depth;
	int width;
	int kmv;
	cms_field** cm2d;
	int seed;
	unsigned int *hasha;
}cms;

// Struct for the input - the element which is going to be saved into the KMVCM 
// cm_string = String which will be saved in the CM-part (in which field of the 2D is it going to be stored?)
// kmv_string = String which will be saved in the KMV of the field in the 2D
// Example: "(Titanic, Charlie Nguyen)". cm_string = Titanic, kmv_string = Charlie Nguyen. 
typedef struct{
	char* cm_string;
	char* kmv_string;
}input;

/*
create_cms	: reserves space for the Count-Min Sketch part of the KMVCM-structure by using the parameters.
CM_Destroy	: frees the space reserved for the Count-Min Sketch 
initialise_CMKVHash: reserves the space for each KMV in every field of the Count-Min Sketch
print2darray: functionality to print the 2D Array (was more for testing and checking purposes)
CM_Update: updates the KMVCM "cm" by using the input "input"
*/
cms* create_cms(int depth, int width, int k, int seed);
void CM_Destroy(cms* cm);
void initialise_CMKMVHash(cms* countmin);
void print2dArray(cms* countmin);
void CM_Update(cms* cm, input* input);


/*
All of the four functions return the result of a Point Estimation by using either the Minimum, Maximum, Mean or Median of
all the rows in the KMVCM - for that we need th cm_key (which will e.g. "Titanic" ) and then check for each row in cm the 
cardinality and then take the particular function(Min, Max, Mean, Median)
*/
double CM_PointEst_Min(cms* cm, char* cm_key);
double CM_PointEst_Max(cms* cm, char* cm_key);
double CM_PointEst_Mean(cms* cm, char* cm_key);
double CM_PointEst_Median(cms* cm, char* cm_key);

/*
Similar to the four functions above, except that we want the union of two elements for which we need the names for both of the elements, 
which are cm_key1 and cm_key2. 
Similar will be the Intersection, which will be the other following four functions.
*/
double CM_Union_Min(cms* cm, char* cm_key1, char* cm_key2);
double CM_Union_Max(cms* cm, char* cm_key1, char* cm_key2);
double CM_Union_Mean(cms* cm, char* cm_key1, char* cm_key2);
double CM_Union_Median(cms* cm, char* cm_key1, char* cm_key2);

double CM_Intersection_Min(cms* cm, char* cm_key1, char* cm_key2);
double CM_Intersection_Max(cms* cm, char* cm_key1, char* cm_key2);
double CM_Intersection_Mean(cms* cm, char* cm_key1, char* cm_key2);
double CM_Intersection_Median(cms* cm, char* cm_key1, char* cm_key2);


/*
print2dArray: 	was used for debugging in order to display the elements which could be found in the KMV
create_input: 	reserve space for the input
destroy_input:	free the space which was reserved for the input
*/
void printArray(uint32_t* array, int depth);
void create_input(input* in, char* cmstring, char* string);
void destroy_input(input* input);

