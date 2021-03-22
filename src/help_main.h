#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h> 
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "countmin.h"
#include "kmv.h"
#include "mergesort.h"
#include "murmurhash.h"


int getNumberOfElements(char* string);
char* getWord(char* src, char* string, int position, int endoffile);
void write_in_file(FILE* filepointer, char* filename, char* maincategory, char* attribute, int kmv, int cm_dep, int cm_wid, int real_card, double estimate_card, double time_needed, double accuracy);
void est_time_acc_array(double* array, double time_cm, cms* cm, char* attribute, int real_card, int mode);
void union_time_acc_array(double* array, double time_cm, cms* cm, char* attribute1, char* attribute2, int real_card, int mode);
void inter_time_acc_array(double* array, double time_cm, cms* cm, char* attribute1, char* attribute2, int real_card, int mode);
void pointquery(double time_cm, cms* cm, char* attribute, int real_card, FILE* filepointer, char* filename, char* maincategory, char* nameinfile, int mode);
void unionquery(double time_cm, cms* cm, char* attribute1, int real_union, FILE* filepointer, char* filename, char* attribute2, char* nameinfile1, char* nameinfile2, int mode);
void interquery(double time_cm, cms* cm, char* attribute1, int real_inter, FILE* filepointer, char* filename, char* attribute2, char* nameinfile1, char* nameinfile2, int mode);
void fill_table(FILE* filepointer, char* filename, char* maincategory, char* attribute);
