#include "countmin.h"

cms* create_cms(int depth, int width, int k, int seed){
	cms* countmin = (cms*) malloc (sizeof(cms));
	int j;

	if(countmin){
		countmin->depth = depth;
		countmin->width = width;
		countmin->kmv = k;
		countmin->cm2d = (cms_field**)malloc(countmin->depth* sizeof(cms_field*));
		for(int i = 0; i < countmin->depth; i++){
			countmin->cm2d[i] = (cms_field*)malloc(countmin->width* sizeof(cms_field));
		}
		countmin->seed = seed;
		countmin->hasha=(unsigned int *)calloc(sizeof(unsigned int),countmin->depth);
      	if (countmin->cm2d && countmin->hasha && countmin->cm2d[0]){
      		for (j=0;j<depth;j++){
	      		countmin->hasha[j] = j*seed+11;
      		}
      	}
	}
	return countmin;
}

void CM_Destroy(cms* cm){     
// get rid of a sketch and free up the space
	if (!cm) return;
	for(int i = 0; i < cm->depth; i++){
		for(int j = 0; j < cm->width; j++){
			free(cm->cm2d[i][j].kmv_sketch);
		}
		free(cm->cm2d[i]);
	}
	free(cm->cm2d);

    if (cm->hasha) 
    	free(cm->hasha); 
    cm->hasha=NULL;

    free(cm);  
    cm=NULL;
}

void initialise_CMKMVHash(cms* countmin){
	for(int i = 0; i < countmin->depth; i++){
		for(int j = 0; j < countmin->width; j++){
			countmin->cm2d[i][j].kmv_length = countmin->kmv;
			countmin->cm2d[i][j].kmv_sketch = (uint32_t*)calloc(countmin->kmv, sizeof(uint32_t));
		}
	}
}


void print2dArray(cms* countmin){
	for (int l = 0; l < countmin->kmv; ++l){
		for(int i = 0; i < countmin->depth; i++){
			for(int j = 0; j < countmin->width; j++){
				printf("%u ", countmin->cm2d[i][j].kmv_sketch[l]);
			}
			printf("\n");
		}
	printf("\n");
	printf("\n");
	}
}

void CM_Update(cms* cm, input* input){
  int j;

  if (!cm) return;
  for (j=0; j<cm->depth ;j++){
  	uint32_t posX = murmurhash(input->cm_string, strlen(input->cm_string), cm->hasha[j]) % cm->width;
  	//printf("Das ist posX: %d\n", (int)posX);
  	update_kmv(cm->cm2d[j][(int)posX].kmv_sketch, input->kmv_string, cm->kmv, cm->seed);
  }
}

double CM_PointEst_Min(cms* cm, char* cm_key)
{
  // return an estimate of the count of an item by taking the minimum
  int j, posX;
  double ans;

  if (!cm) return 0;
  posX = murmurhash(cm_key, strlen(cm_key), cm->hasha[0]) % cm->width;
  ans = estimate_kmv(cm->cm2d[0][posX].kmv_sketch, cm->kmv);

  for (j=1;j<cm->depth;j++){
  	int pos_h = murmurhash(cm_key, strlen(cm_key), cm->hasha[j]) % cm->width;
  	double result = estimate_kmv(cm->cm2d[j][pos_h].kmv_sketch, cm->kmv);
    if(result == 0){
      return 0;
    }
    ans=min(ans,result);
	}
  if(ans <= cm->kmv){
    return ans;
  }

  return ans;
}

double CM_PointEst_Max(cms* cm, char* cm_key)
{
  // return an estimate of the count of an item by taking the minimum
  int j, posX;
  double ans;

  if (!cm) return 0;
  posX = murmurhash(cm_key, strlen(cm_key), cm->hasha[0]) % cm->width;
  ans = estimate_kmv(cm->cm2d[0][posX].kmv_sketch, cm->kmv);

  for (j=1;j<cm->depth;j++){
    int pos_h = murmurhash(cm_key, strlen(cm_key), cm->hasha[j]) % cm->width;
    double result = estimate_kmv(cm->cm2d[j][pos_h].kmv_sketch, cm->kmv);

    ans=max(ans,result);
  }
  if(ans < cm->kmv){
    return ans;
  }

  return ans;
}


double CM_PointEst_Mean(cms* cm, char* cm_key)
{
  // return an estimate of the count of an item by taking the minimum
  int j, posX;
  double ans;
  double s = 0xFFFFFFFF;
  int ack = 0;

  if (!cm) return 0;
  posX = murmurhash(cm_key, strlen(cm_key), cm->hasha[0]) % cm->width;
  ans = estimate_kmv(cm->cm2d[0][posX].kmv_sketch, cm->kmv);
  if(ans < cm->kmv){
    ack =1;
    s = min(ans, s);
  }

  for (j=1;j<cm->depth;j++){
    int pos_h = murmurhash(cm_key, strlen(cm_key), cm->hasha[j]) % cm->width;
    double result = estimate_kmv(cm->cm2d[j][pos_h].kmv_sketch, cm->kmv);
    if(result < cm->kmv){
      ack = 1;
      s = min(result, s);
    }

    if(ack == 0){
      ans = ans + result;
    }
  }

  if(ack == 1){
    return s;
  }
  else{
    ans = ans / (double)cm->depth;
  }

  return ans;
}

double CM_PointEst_Median(cms* cm, char* cm_key)
{
  // return an estimate of the count of an item by taking the minimum
  int j, posX;
  double median;
  double s = 0xFFFFFFFF;
  int ack = 0;
  double array_median[cm->depth];

  if (!cm) return 0;
  posX = murmurhash(cm_key, strlen(cm_key), cm->hasha[0]) % cm->width;
  median = estimate_kmv(cm->cm2d[0][posX].kmv_sketch, cm->kmv);
  if(median < cm->kmv){
    ack =1;
    s = min(median, s);
  }
  array_median[0] = median;
  

  for (j=1;j<cm->depth;j++){
    int pos_h = murmurhash(cm_key, strlen(cm_key), cm->hasha[j]) % cm->width;
    double result = estimate_kmv(cm->cm2d[j][pos_h].kmv_sketch, cm->kmv);
    if(result < cm->kmv){
      ack = 1;
      s = min(result, s);
    }
    array_median[j] = result;

  }

  if(ack == 1){
    return s;
  }
  mergeSort_double(array_median,0, cm->depth-1);

  int evenodd = cm->depth & 1;
  if(evenodd == 0){ // wenn wir gerade anzahl an elementen haben.
    median = (array_median[cm->depth/2] + array_median[(cm->depth/2)-1])/2;
  }
  else{
    median = array_median[(cm->kmv-1)/2];
  }

  return median;
}


double CM_Union_Min(cms* cm, char* cm_key1, char* cm_key2){
    int j, posX1, posX2;
    double ans;
    double result;

    if (!cm) return 0;
    posX1 = murmurhash(cm_key1, strlen(cm_key1), cm->hasha[0]) % cm->width;
    posX2 = murmurhash(cm_key2, strlen(cm_key2), cm->hasha[0]) % cm->width;
    ans = union_kmv(cm->cm2d[0][posX1].kmv_sketch, cm->cm2d[0][posX2].kmv_sketch, cm->kmv);

    for (j=1;j<cm->depth;j++){
      posX1 = murmurhash(cm_key1, strlen(cm_key1), cm->hasha[j]) % cm->width;
      posX2 = murmurhash(cm_key2, strlen(cm_key2), cm->hasha[j]) % cm->width;
      result = union_kmv(cm->cm2d[j][posX1].kmv_sketch, cm->cm2d[j][posX2].kmv_sketch, cm->kmv);
      if(result == 0){
        return 0;
      }
      ans = min(ans, result);
    }
    return ans;
}

double CM_Union_Max(cms* cm, char* cm_key1, char* cm_key2){
    int j, posX1, posX2;
    double ans;
    double result;

    if (!cm) return 0;
    posX1 = murmurhash(cm_key1, strlen(cm_key1), cm->hasha[0]) % cm->width;
    posX2 = murmurhash(cm_key2, strlen(cm_key2), cm->hasha[0]) % cm->width;
    ans = union_kmv(cm->cm2d[0][posX1].kmv_sketch, cm->cm2d[0][posX2].kmv_sketch, cm->kmv);

    for (j=1;j<cm->depth;j++){
      posX1 = murmurhash(cm_key1, strlen(cm_key1), cm->hasha[j]) % cm->width;
      posX2 = murmurhash(cm_key2, strlen(cm_key2), cm->hasha[j]) % cm->width;
      result = union_kmv(cm->cm2d[j][posX1].kmv_sketch, cm->cm2d[j][posX2].kmv_sketch, cm->kmv);
      ans = max(ans, result);
    }

    if(ans < cm->kmv){
      return ans;
    }

    return ans;
}

double CM_Union_Mean(cms* cm, char* cm_key1, char* cm_key2){
    int j, posX1, posX2;
    double ans;
    double result;
    double s = MAX_ELEM;
    int ack = 0;

    if (!cm) return 0;
    posX1 = murmurhash(cm_key1, strlen(cm_key1), cm->hasha[0]) % cm->width;
    posX2 = murmurhash(cm_key2, strlen(cm_key2), cm->hasha[0]) % cm->width;
    ans = union_kmv(cm->cm2d[0][posX1].kmv_sketch, cm->cm2d[0][posX2].kmv_sketch, cm->kmv);
    if(ans < cm->kmv){
      ack = 1;
      s = min(ans, s);
    }

    for (j=1;j<cm->depth;j++){
      posX1 = murmurhash(cm_key1, strlen(cm_key1), cm->hasha[j]) % cm->width;
      posX2 = murmurhash(cm_key2, strlen(cm_key2), cm->hasha[j]) % cm->width;
      result = union_kmv(cm->cm2d[j][posX1].kmv_sketch, cm->cm2d[j][posX2].kmv_sketch, cm->kmv);
      if(result < cm->kmv){
        ack = 1;
        s = min(result, s);
      }
      if(ack == 0){
        ans = ans + result;
      }
    }
    if(ack == 1){
      return s;
    }
    ans = ans / (double)cm->depth;
    return ans;
}

double CM_Union_Median(cms* cm, char* cm_key1, char* cm_key2){
    int j, posX1, posX2;
    double ans;
    double result;
    double s = MAX_ELEM;
    int ack = 0;

    if (!cm) return 0;
    double array_median[cm->depth];

    posX1 = murmurhash(cm_key1, strlen(cm_key1), cm->hasha[0]) % cm->width;
    posX2 = murmurhash(cm_key2, strlen(cm_key2), cm->hasha[0]) % cm->width;
    ans = union_kmv(cm->cm2d[0][posX1].kmv_sketch, cm->cm2d[0][posX2].kmv_sketch, cm->kmv);
    if(ans < cm->kmv){
      ack = 1;
      s = min(s, ans);
    }
    else{
      array_median[0] = ans;
    }

    for (j=1;j<cm->depth;j++){
      posX1 = murmurhash(cm_key1, strlen(cm_key1), cm->hasha[j]) % cm->width;
      posX2 = murmurhash(cm_key2, strlen(cm_key2), cm->hasha[j]) % cm->width;
      result = union_kmv(cm->cm2d[j][posX1].kmv_sketch, cm->cm2d[j][posX2].kmv_sketch, cm->kmv);
      if(result < cm->kmv){
        ack = 1;
        s = min(result, s);
      }
      if(ack == 0){
        array_median[j] = result;
      }
    }
    if(ack == 1){
      //free(array_median);
      return s;
    }

    mergeSort_double(array_median,0, cm->depth-1);

    int evenodd = cm->depth & 1;
    if(evenodd == 0){ // wenn wir gerade anzahl an elementen haben.
      ans = (array_median[cm->depth/2] + array_median[(cm->depth/2)-1])/2;
    }
    else{
      ans = array_median[(cm->depth-1)/2];
    }

    //free(array_median);
    return ans;
}

double CM_Intersection_Min(cms* cm, char* cm_key1, char* cm_key2){
    int j, posX1, posX2;
    double intersection;

    if (!cm) return 0;
    posX1 = murmurhash(cm_key1, strlen(cm_key1), cm->hasha[0]) % cm->width;
    posX2 = murmurhash(cm_key2, strlen(cm_key2), cm->hasha[0]) % cm->width;
    intersection = intersection_kmv(cm->cm2d[0][posX1].kmv_sketch, cm->cm2d[0][posX2].kmv_sketch, cm->kmv);

    for(j=1; j<cm->depth;j++){
      posX1 = murmurhash(cm_key1, strlen(cm_key1), cm->hasha[0]) % cm->width;
      posX2 = murmurhash(cm_key2, strlen(cm_key2), cm->hasha[0]) % cm->width;
      double result = intersection_kmv(cm->cm2d[j][posX1].kmv_sketch, cm->cm2d[j][posX2].kmv_sketch, cm->kmv);
      if(result == 0){
        return result;
      }
      intersection = min(intersection, result);
    }
  return intersection;
}

double CM_Intersection_Max(cms* cm, char* cm_key1, char* cm_key2){
    int j, posX1, posX2;
    double intersection;

    if (!cm) return 0;
    posX1 = murmurhash(cm_key1, strlen(cm_key1), cm->hasha[0]) % cm->width;
    posX2 = murmurhash(cm_key2, strlen(cm_key2), cm->hasha[0]) % cm->width;
    intersection = intersection_kmv(cm->cm2d[0][posX1].kmv_sketch, cm->cm2d[0][posX2].kmv_sketch, cm->kmv);

    for(j=1; j<cm->depth;j++){
      posX1 = murmurhash(cm_key1, strlen(cm_key1), cm->hasha[0]) % cm->width;
      posX2 = murmurhash(cm_key2, strlen(cm_key2), cm->hasha[0]) % cm->width;
      double result = intersection_kmv(cm->cm2d[j][posX1].kmv_sketch, cm->cm2d[j][posX2].kmv_sketch, cm->kmv);
      intersection = max(intersection, result);
    }
  return intersection;
}

double CM_Intersection_Mean(cms* cm, char* cm_key1, char* cm_key2){
    int j, posX1, posX2;
    double intersection;

    if (!cm) return 0;
    posX1 = murmurhash(cm_key1, strlen(cm_key1), cm->hasha[0]) % cm->width;
    posX2 = murmurhash(cm_key2, strlen(cm_key2), cm->hasha[0]) % cm->width;
    intersection = intersection_kmv(cm->cm2d[0][posX1].kmv_sketch, cm->cm2d[0][posX2].kmv_sketch, cm->kmv);

    for(j=1; j<cm->depth;j++){
      posX1 = murmurhash(cm_key1, strlen(cm_key1), cm->hasha[0]) % cm->width;
      posX2 = murmurhash(cm_key2, strlen(cm_key2), cm->hasha[0]) % cm->width;
      double result = intersection_kmv(cm->cm2d[j][posX1].kmv_sketch, cm->cm2d[j][posX2].kmv_sketch, cm->kmv);
      intersection = intersection + result;
    }
    intersection = intersection / (double)cm->depth;
  return intersection;
}

double CM_Intersection_Median(cms* cm, char* cm_key1, char* cm_key2){
    int j, posX1, posX2;
    double intersection;
    double* array_median = (double*) calloc (cm->depth, sizeof(double));

    if (!cm) return 0;
    posX1 = murmurhash(cm_key1, strlen(cm_key1), cm->hasha[0]) % cm->width;
    posX2 = murmurhash(cm_key2, strlen(cm_key2), cm->hasha[0]) % cm->width;
    intersection = intersection_kmv(cm->cm2d[0][posX1].kmv_sketch, cm->cm2d[0][posX2].kmv_sketch, cm->kmv);
    array_median[0] = intersection;

    for(j=1; j<cm->depth;j++){
      posX1 = murmurhash(cm_key1, strlen(cm_key1), cm->hasha[0]) % cm->width;
      posX2 = murmurhash(cm_key2, strlen(cm_key2), cm->hasha[0]) % cm->width;
      double result = intersection_kmv(cm->cm2d[j][posX1].kmv_sketch, cm->cm2d[j][posX2].kmv_sketch, cm->kmv);
      array_median[j] = result;
    }
    mergeSort_double(array_median,0, cm->depth-1);

    int evenodd = cm->depth & 1;
    if(evenodd == 0){ // wenn wir gerade anzahl an elementen haben.
       intersection = (array_median[cm->depth/2] + array_median[(cm->depth/2)-1])/2;
    }
    else{
      intersection = array_median[(cm->depth-1)/2];
    }
    free(array_median);
  return intersection;
}


void printArray(uint32_t* array, int depth){
	for(int i = 0; i < depth; i++){
		printf("%u ", array[i]);
	}
	printf("\n");
}

void create_input(input* in, char* cmstring, char* string){
	in->cm_string = cmstring;
	in->kmv_string = string;
}

void destroy_input(input* input){
	free(input);
}

