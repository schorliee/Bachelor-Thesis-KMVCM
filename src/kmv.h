#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "murmurhash.h"
#include "mergesort.h"
#define MAX_ELEM1 0xFFFFFFFF

// function to merge two KMVS (arr1 and arr2) into the array "help_array" 
void merge_kmvs1(uint32_t* help_array, uint32_t* arr1, uint32_t* arr2, int k);

/*
The following functions are help function to estimate the value of a KMV (estimate_kmv).
check_full_kmv		: checks if the KMV is full or not (if it's not full the amount of elements in the KMV = the amount of saved elements)
check_amount_zero	: returns the amount of zero elements (fields which are empty in the KMV to subtract it with k, so that we'll the the total amount of saved elements)
check_highest		: if kmv is full - we want to know the highest element in order to calculate the estimatinon.	
estimate_kmv		: actual function to calculate the value of a KMV
*/
int check_full_kmv(uint32_t* arr, int kmv_length);
int check_amount_zero(uint32_t* arr, int kmv_length);
int check_highest(uint32_t* arr, int kmv_length);

double estimate_kmv(uint32_t* arr, int k);


/*
The following functions help to update one or to unite and intersect two KMVs. 
count_same_element		: returns the amount of the same element of two arrays
						- this function is important for the intersection since we need the amount of the same elements in both KMVs
duplicate_check			: checks if the element which should be saved in a KMV has already been in the KMV (we dont save duplicates)
						- this function is important to check before saving an element (for example if the array is empty, that we dont stupidly fill the array till it's full)
give_empty_index		: returns the index of an empty space in the KMV (if the KMV is not empty!!!)
						- this is important to know at which position to store the new element

*/
int count_same_element(uint32_t* arr1, uint32_t* arr2, int kmv_length);
int duplicate_check(uint32_t* arr, uint32_t hashvalue, int kmv_length);
int give_empty_index(uint32_t* arr, int kmv_length);

void update_kmv(uint32_t* arr, char* elem, int kmv_length, uint32_t seed);
double union_kmv(uint32_t* arr1, uint32_t* arr2, int kmv_length);
double intersection_kmv(uint32_t* arr1, uint32_t* arr2, int kmv_length);