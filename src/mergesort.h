#include<stdlib.h> 
#include<stdio.h> 
#include<stdint.h>

/* 
Code for the algorithm taken from: https://www.geeksforgeeks.org/merge-sort/ 
Code was modified for uint32_t variables in order to use the MurmurHash, which uses uint32_t and not double
*/

void merge(uint32_t arr[], int l, int m, int r);
void mergeSort(uint32_t arr[], int l, int r);
void printArray1(uint32_t A[], int size);