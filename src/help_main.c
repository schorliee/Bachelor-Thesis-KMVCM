#include "help_main.h"

int getNumberOfElements(char* string){
	if(string == NULL) return 0;

	int counter = 0;
	for(int i = 0; i < strlen(string); i++){
		if(string[i] == ';')
			counter++;
	}
	counter++;
	return counter;
}

char* getWord(char* src, char* string, int position, int endoffile){
	if(string == NULL) return 0;

	int comma1 = 0;
	int comma2 = 0;
	int counter = 0;

	if(endoffile == 1){
		position = position - 1;
	}
	for(int i = 0; i < strlen(string); i++){
		if(string[i] == ';'){
			comma2 = i;
			if(counter == position){
				break;
			}
			comma1 = comma2+1;
			counter++;
		}
		if(string[i] == '\n'){
			comma2 = i-1;
			break;
		}
	}

	if(endoffile == 1){
		comma1 = comma2 +1;
		comma2 = strlen(string);
	}

	int length = comma2-comma1;
	strncpy(src, &string[comma1],length);
	src[length] = '\0';
	return src;
}



void write_in_file(FILE* filepointer, char* filename, char* maincategory, char* attribute, int kmv, int cm_dep, int cm_wid, 
	int real_card, double estimate_card, double time_needed, double accuracy){
	/*sketchsize errechnet sich durch:
	cms structure mit: 4 ints, 2d array von cms_field, unsigned int array mit Länge cm_dep
	cms_field: 1 int, uint32_t array mit Länge kmv
	--> 4*4Byte + w*d*(4Byte + kmv *4Byte) + 4Byte*d
	*/
	double sketchsize = (4*4 + (double)cm_wid*(double)cm_dep*(4 + (double)kmv*4) + 4*(double)cm_dep)/1024/1024;
	
	filepointer = fopen(filename, "a");
	fprintf(filepointer, "%s;", maincategory);
	fprintf(filepointer, "%s;", attribute);
	fprintf(filepointer, "%d;", kmv);
	fprintf(filepointer, "%d;", cm_dep);
	fprintf(filepointer, "%d;", cm_wid);
	fprintf(filepointer, "%d;", real_card);
	fprintf(filepointer, "%f;", estimate_card);
	fprintf(filepointer, "%f;", time_needed);
	fprintf(filepointer, "%f;", accuracy);
	fprintf(filepointer, "%.3f", sketchsize);
	fprintf(filepointer, "\n");
	fclose(filepointer);
}

void est_time_acc_array(double* array, double time_cm, cms* cm, char* attribute, int real_card, int mode){
	double estimation = 0;
	clock_t t = clock();
	switch(mode){
		case 1: estimation = CM_PointEst_Min(cm, attribute);
				break;
		case 2: estimation = CM_PointEst_Mean(cm, attribute);
				break;
		case 3: estimation = CM_PointEst_Median(cm, attribute);
				break;
		case 4: estimation = CM_PointEst_Max(cm, attribute);
				break;		
		default: printf("ERROR in EST\n");
	}

	t = clock() - t;
	double time_needed = ((double)t/CLOCKS_PER_SEC) + time_cm;
	double accuracy;
	if(real_card == 0){
		if(estimation == 0)
			accuracy = 0;
		else
			accuracy = estimation;
	}
	else
		accuracy = fabs(estimation - real_card) / real_card; 
	array[0] = estimation;
	array[1] = time_needed;
	array[2] = accuracy;
}

void union_time_acc_array(double* array, double time_cm, cms* cm, char* attribute1, char* attribute2, int real_card, int mode){
	double union_result = 0;
	clock_t t = clock();

	switch(mode){
		case 1: union_result = CM_Union_Min(cm, attribute1, attribute2);
				break;
		case 2: union_result = CM_Union_Mean(cm, attribute1, attribute2);
				break;
		case 3: union_result  = CM_Union_Median(cm, attribute1, attribute2);
				break;
		case 4: union_result  = CM_Union_Max(cm, attribute1, attribute2);
				break;		
		default: printf("ERROR in Union\n");
	}

	t = clock() - t;
	double time_needed = ((double)t/CLOCKS_PER_SEC) + time_cm;
	double accuracy;
	if(real_card == 0){
		if(union_result == 0)
			accuracy = 0;
		else
			accuracy = union_result;
	}
	else
		accuracy = fabs(union_result - real_card) / real_card; 

	array[0] = union_result;
	array[1] = time_needed;
	array[2] = accuracy;
}

void inter_time_acc_array(double* array, double time_cm, cms* cm, char* attribute1, char* attribute2, int real_card, int mode){
	double intersection = 0;
	clock_t t = clock();
	switch(mode){
		case 1: intersection = CM_Intersection_Min(cm, attribute1, attribute2);
				break;
		case 2: intersection = CM_Intersection_Mean(cm, attribute1, attribute2);
				break;
		case 3: intersection = CM_Intersection_Median(cm, attribute1, attribute2);
				break;
		case 4: intersection = CM_Intersection_Max(cm, attribute1, attribute2);
				break;		
		default: printf("ERROR in Union\n");
	}
	t = clock() - t;
	double time_needed = ((double)t/CLOCKS_PER_SEC) + time_cm;
	double accuracy;
	if(intersection == 0){
		if(real_card == 0)
			accuracy = 0;
		else
			accuracy = real_card;
	}
	else if(real_card == 0){
		accuracy = intersection;
	}
	else
		accuracy = fabs(intersection - real_card) / real_card; 

	array[0] = intersection;
	array[1] = time_needed;
	array[2] = accuracy;
}

void pointquery(double time_cm, cms* cm, char* attribute, int real_card, FILE* filepointer, char* filename, char* maincategory, char* nameinfile, int mode){
	double array[3];
	est_time_acc_array(array, time_cm, cm, attribute, real_card, mode);
	write_in_file(filepointer, filename, maincategory, nameinfile, cm->kmv, cm->depth, cm->width, real_card, array[0], array[1], array[2]);
	//free(array);
}

void unionquery(double time_cm, cms* cm, char* attribute1, int real_union, FILE* filepointer, char* filename, char* attribute2, char* nameinfile1, char* nameinfile2, int mode){
	//double* array = (double*) malloc(3* sizeof(double));
	double array[3];
	union_time_acc_array(array, time_cm, cm, attribute1, attribute2, real_union, mode);
	write_in_file(filepointer, filename, nameinfile1, nameinfile2, cm->kmv, cm->depth, cm->width, real_union, array[0], array[1], array[2]);
	//free(array);
}

void interquery(double time_cm, cms* cm, char* attribute1, int real_inter, FILE* filepointer, char* filename, char* attribute2, char* nameinfile1, char* nameinfile2, int mode){
	//double* array = (double*) malloc(3* sizeof(double));
	double array[3];
	inter_time_acc_array(array, time_cm, cm, attribute1, attribute2, real_inter, mode);
	write_in_file(filepointer, filename, nameinfile1, nameinfile2, cm->kmv, cm->depth, cm->width, real_inter, array[0], array[1], array[2]);
	//free(array);
}

void fill_table(FILE* filepointer, char* filename, char* maincategory, char* attribute){
	filepointer = fopen(filename, "a");
	fprintf(filepointer, "%s", maincategory);
	fprintf(filepointer, "%s", attribute);
	fprintf(filepointer, "kmv;");
	fprintf(filepointer, "cm_dep;");
	fprintf(filepointer, "cm_wid;");
	fprintf(filepointer, "real_card;");
	fprintf(filepointer, "est_card;");
	fprintf(filepointer, "time;");
	fprintf(filepointer, "accuracy;");
	fprintf(filepointer, "sketchsize_MByte");
	fprintf(filepointer, "\n");
	fclose(filepointer);
}