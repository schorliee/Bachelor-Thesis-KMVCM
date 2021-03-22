#include "kmv.h"


void merge_kmvs1(uint32_t* help_array, uint32_t* arr1, uint32_t* arr2, int k){
	int counter = 0;
	int i = 0;
	int j = 0;
	int intersection = 0;
                
	while(counter < k){
		if(arr1[i] == 0){
			i++;
			continue;
		}
		else if(arr2[j] == 0){
			j++;
			continue;
		}
		else{
			if(arr1[i] == arr2[j]){
				help_array[counter] = arr1[i];
				i++;
				j++;
				intersection++;
				counter++;
			}
			else if(arr1[i] < arr2[j]){
				help_array[counter] = arr1[i];
				i++;
				counter++;
			}
			else{
				help_array[counter] = arr2[j];
				j++;
				counter++;
			}
		}
	}
	help_array[k] = intersection;
}

int check_full_kmv(uint32_t* arr, int kmv_length){
	for(int i = 0; i < kmv_length; i++){
		if(arr[i] == 0){
			return 0;
		}
	}
	return 1;
}

int check_amount_zero(uint32_t* arr, int kmv_length){
	int counter = 0;
	for(int i = 0; i < kmv_length; i++){
		if(arr[i] == 0){
			counter++;
		}
	}
	return counter;
}

int check_highest(uint32_t* arr, int kmv_length){
	int highest = 0;
	for(int i=0; i < kmv_length; i++){
		if(arr[highest] < arr[i]){
			highest = i;
		}
	}
	return highest;
}

double estimate_kmv(uint32_t* arr, int k){
	int amountofzero = check_amount_zero(arr, k);
	if(amountofzero != 0){
		return k - amountofzero;
	}


	int highest = check_highest(arr, k);

	double result = (double)k * ((double)MAX_ELEM1 / (double) arr[highest]);
/*
	FILE* filepointer;
 	char* filename = "private_result.csv";
  	filepointer = fopen(filename, "a");
  	fprintf(filepointer, "%f", result);
  	fprintf(filepointer, "\n");
  	fclose(filepointer);
*/
	
	return result;
}

int count_same_element(uint32_t* arr1, uint32_t* arr2, int kmv_length){
	int count = 0;
	for (int i = 0; i < kmv_length; ++i){
		for(int j = 0; j < kmv_length; j++){
			if(arr1[i] != 0 && arr1[i] == arr2[j]){
				count++;
			}
		}
	}
	return count;
}

int duplicate_check(uint32_t* arr, uint32_t hashvalue, int kmv_length){
	for(int i = 0; i < kmv_length; i++){
		if(arr[i] == hashvalue){
			return 1;
		}
	}
	return 0;
}

int give_empty_index(uint32_t* arr, int kmv_length){
	for (int i = 0; i < kmv_length; ++i){
		if(arr[i] == 0){
			return i;
		}
	}
	return -1;
}

void update_kmv(uint32_t* arr, char* elem, int kmv_length, uint32_t seed){
	uint32_t hash = murmurhash(elem, strlen(elem), seed);
	if(duplicate_check(arr, hash, kmv_length) == 0){
		int index = give_empty_index(arr, kmv_length);
		if(index != -1){
			arr[index] = hash;
		}
		else{
			int highest = check_highest(arr, kmv_length);
			if(hash < arr[highest]){
				arr[highest] = hash;
			}
		}
	}
}

double union_kmv(uint32_t* arr1, uint32_t* arr2, int kmv_length){
	mergeSort(arr1,0,kmv_length-1);
	mergeSort(arr2,0,kmv_length-1);

	uint32_t* merged_array = (uint32_t*)calloc(kmv_length+1, sizeof(uint32_t));
	merge_kmvs1(merged_array, arr1, arr2, kmv_length);

	double result = estimate_kmv(merged_array, kmv_length);
	
	free(merged_array);

	return result;
}


double intersection_kmv(uint32_t* arr1, uint32_t* arr2, int kmv_length){

	//int intersection_card = count_same_element(arr1, arr2, kmv_length);
	mergeSort(arr1,0,kmv_length-1);
	mergeSort(arr2,0,kmv_length-1);

	uint32_t* merged_array = (uint32_t*)calloc(kmv_length+1, sizeof(uint32_t));
	merge_kmvs1(merged_array, arr1, arr2, kmv_length);

	double union_card = estimate_kmv(merged_array, kmv_length);
	double intersection_card = (double) merged_array[kmv_length];
	double jaccard = (double)intersection_card /(double)kmv_length;
	
	free(merged_array);

	return jaccard * union_card;
}