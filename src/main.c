/*
Maincategory; Attribut; KMV; DEP; WID; true card; estimated card; time(s); accuracy; sketchsize(MByte)
*/

#include "help_main.h"

#define KMV_start 2
#define CM_wid_start 2
#define CM_dep_start 2

#define KMV_end 1024
#define CM_wid_end 1024
#define CM_dep_end 1024


char* input_data = "../input/data32561.csv";

char* filename_est1 = "min/estimation_32561.csv";
char* filename_est2 = "mean/estimation_32561.csv";
char* filename_est3 = "median/estimation_32561.csv";
char* filename_est4 = "max/estimation_32561.csv";
char* filename_union1 = "min/union_32561.csv";
char* filename_union2 = "mean/union_32561.csv";
char* filename_union3 = "median/union_32561.csv";
char* filename_union4 = "max/union_32561.csv";
char* filename_inter1 = "min/inter_32561.csv";
char* filename_inter2 = "mean/inter_32561.csv";
char* filename_inter3 = "median/inter_32561.csv";
char* filename_inter4 = "max/inter_32561.csv";

int real_private = 22696;
int real_never_worked = 7;

int real_bachelors = 5355;
int real_masters = 1723;
int real_preschool = 51;

int real_never_married = 10683;
int real_married_civ_spouse = 14976;
int real_divorced = 4443;

int real_sales = 3650;
int real_armed_forces = 9;

int real_wife = 1568;
int real_husband = 13193;

int real_white = 27816;
int real_black = 3124;

int real_female = 10771;
int real_male = 21790;
// ########################################################
int union_private_private = 22696;
int union_private_never_worked = 22703;

int union_private_bachelors = 24500;
int union_private_masters = 23525;
int union_private_preschool = 22706;

int union_private_never_married = 25193;
int union_private_married_civ_spouse = 27940;
int union_private_divorced = 24020;

int union_private_sales = 23404;
int union_private_armed_forces = 22705;

int union_private_wife = 23265;
int union_private_husband = 27317;

int union_private_white = 31108;
int union_private_black = 23644;

int union_private_female = 25715;
int union_private_male = 29542;
// ########################################################
int inter_private_private = 22696;
int inter_private_never_worked = 0;

int inter_private_bachelors = 3551;
int inter_private_masters = 894;
int inter_private_preschool = 41;

int inter_private_never_married = 8186;
int inter_private_married_civ_spouse = 9732;
int inter_private_divorced = 3119;

int inter_private_sales = 2942;
int inter_private_armed_forces = 0;

int inter_private_wife = 999;
int inter_private_husband = 8572;

int inter_private_white = 19404;
int inter_private_black = 2176;

int inter_private_female = 7752;
int inter_private_male = 14944;

int main(){
	FILE* filepointer_est1 = NULL;
	FILE* filepointer_est2 = NULL;
	FILE* filepointer_est3 = NULL;
	FILE* filepointer_est4 = NULL;
	FILE* filepointer_union1 = NULL;
	FILE* filepointer_union2 = NULL;
	FILE* filepointer_union3 = NULL;
	FILE* filepointer_union4 = NULL;
	FILE* filepointer_inter1 = NULL;
	FILE* filepointer_inter2 = NULL;
	FILE* filepointer_inter3 = NULL;
	FILE* filepointer_inter4 = NULL;
	
	mkdir("min", 0777);
	mkdir("mean", 0777);
	mkdir("median", 0777);
	mkdir("max", 0777);

	fill_table(filepointer_est1, filename_est1, "maincategory;", "attribute;");
	fill_table(filepointer_est2, filename_est2, "maincategory;", "attribute;");
	fill_table(filepointer_est3, filename_est3, "maincategory;", "attribute;");
	fill_table(filepointer_est4, filename_est4, "maincategory;", "attribute;");

	fill_table(filepointer_union1, filename_union1, "attribute1;", "attribute2;");
	fill_table(filepointer_union2, filename_union2, "attribute1;", "attribute2;");
	fill_table(filepointer_union3, filename_union3, "attribute1;", "attribute2;");
	fill_table(filepointer_union4, filename_union4, "attribute1;", "attribute2;");

	fill_table(filepointer_inter1, filename_inter1, "attribute1;", "attribute2;");
	fill_table(filepointer_inter2, filename_inter2, "attribute1;", "attribute2;");
	fill_table(filepointer_inter3, filename_inter3, "attribute1;", "attribute2;");
	fill_table(filepointer_inter4, filename_inter4, "attribute1;", "attribute2;");

	for (int kmv = KMV_start; kmv <= KMV_end; kmv = kmv*2){
		for (int cm_dep = CM_dep_start; cm_dep <= CM_dep_end; cm_dep = cm_dep*2){
			for (int cm_wid = CM_wid_start; cm_wid <= CM_wid_end; cm_wid = cm_wid*2){
				clock_t t_cm; //time how long the count min needed
				t_cm = clock();
				cms* cm = create_cms(cm_dep, cm_wid, kmv, 42);
				initialise_CMKMVHash(cm);
				char line[1024];
				FILE* stream;
				stream = fopen(input_data, "r");
				if(stream == NULL){
					perror("Error while opening the file.\n");
					exit(-1);
				}

				while(fgets(line, 1024, stream)){
					int endoffile = 0;
					int numbers = getNumberOfElements(line);
					if(numbers == 0) return 0;
					else if(numbers == 1) return 0;
					else{
						char id_buff[50];
						char* id = getWord(id_buff, line, 0, endoffile);
						for(int i = 1; i < numbers; i++){ 
							if(feof(stream) && i == numbers-1){
								endoffile = 1;
							}
							char src[50];
							char* word = getWord(src, line, i, endoffile);
							if(strcmp(word, "?") != 0){
								input* in = (input*) malloc (sizeof(input));
								create_input(in, word, id);
								CM_Update(cm, in);
								destroy_input(in);
							}
						}
					}
				}
				fclose(stream);	
				t_cm = clock() - t_cm;
				double time_cm = ((double)t_cm/CLOCKS_PER_SEC);

// ########################################################
// 1 = Min, 2 = Mean, 3 = Median, 4 = Max


				pointquery(time_cm, cm, "Private", real_private, filepointer_est1, filename_est1, "workclass", "private", 1);
				pointquery(time_cm, cm, "Private", real_private, filepointer_est2, filename_est2, "workclass", "private", 2);
				pointquery(time_cm, cm, "Private", real_private, filepointer_est3, filename_est3, "workclass", "private", 3);
				pointquery(time_cm, cm, "Private", real_private, filepointer_est4, filename_est4, "workclass", "private", 4);

				pointquery(time_cm, cm, "Never-worked", real_never_worked, filepointer_est1, filename_est1, "workclass", "never-worked", 1);
				pointquery(time_cm, cm, "Never-worked", real_never_worked, filepointer_est2, filename_est2, "workclass", "never-worked", 2);
				pointquery(time_cm, cm, "Never-worked", real_never_worked, filepointer_est3, filename_est3, "workclass", "never-worked", 3);
				pointquery(time_cm, cm, "Never-worked", real_never_worked, filepointer_est4, filename_est4, "workclass", "never-worked", 4);
				
				pointquery(time_cm, cm, "Bachelors", real_bachelors, filepointer_est1, filename_est1, "education", "bachelors", 1);
				pointquery(time_cm, cm, "Bachelors", real_bachelors, filepointer_est2, filename_est2, "education", "bachelors", 2);
				pointquery(time_cm, cm, "Bachelors", real_bachelors, filepointer_est3, filename_est3, "education", "bachelors", 3);
				pointquery(time_cm, cm, "Bachelors", real_bachelors, filepointer_est4, filename_est4, "education", "bachelors", 4);
				
				pointquery(time_cm, cm, "Masters", real_masters, filepointer_est1, filename_est1, "education", "masters", 1);
				pointquery(time_cm, cm, "Masters", real_masters, filepointer_est2, filename_est2, "education", "masters", 2);
				pointquery(time_cm, cm, "Masters", real_masters, filepointer_est3, filename_est3, "education", "masters", 3);
				pointquery(time_cm, cm, "Masters", real_masters, filepointer_est4, filename_est4, "education", "masters", 4);

				pointquery(time_cm, cm, "Preschool", real_preschool, filepointer_est1, filename_est1, "education", "preschool", 1);
				pointquery(time_cm, cm, "Preschool", real_preschool, filepointer_est2, filename_est2, "education", "preschool", 2);
				pointquery(time_cm, cm, "Preschool", real_preschool, filepointer_est3, filename_est3, "education", "preschool", 3);
				pointquery(time_cm, cm, "Preschool", real_preschool, filepointer_est4, filename_est4, "education", "preschool", 4);

				pointquery(time_cm, cm, "Never-married", real_never_married, filepointer_est1, filename_est1, "marital-status", "never-married", 1);
				pointquery(time_cm, cm, "Never-married", real_never_married, filepointer_est2, filename_est2, "marital-status", "never-married", 2);
				pointquery(time_cm, cm, "Never-married", real_never_married, filepointer_est3, filename_est3, "marital-status", "never-married", 3);
				pointquery(time_cm, cm, "Never-married", real_never_married, filepointer_est4, filename_est4, "marital-status", "never-married", 4);

				pointquery(time_cm, cm, "Married-civ-spouse", real_married_civ_spouse, filepointer_est1, filename_est1, "marital-status", "married-civ-spouse", 1);
				pointquery(time_cm, cm, "Married-civ-spouse", real_married_civ_spouse, filepointer_est2, filename_est2, "marital-status", "married-civ-spouse", 2);
				pointquery(time_cm, cm, "Married-civ-spouse", real_married_civ_spouse, filepointer_est3, filename_est3, "marital-status", "married-civ-spouse", 3);
				pointquery(time_cm, cm, "Married-civ-spouse", real_married_civ_spouse, filepointer_est4, filename_est4, "marital-status", "married-civ-spouse", 4);

				pointquery(time_cm, cm, "Divorced", real_divorced, filepointer_est1, filename_est1, "marital-status", "divorced", 1);
				pointquery(time_cm, cm, "Divorced", real_divorced, filepointer_est2, filename_est2, "marital-status", "divorced", 2);
				pointquery(time_cm, cm, "Divorced", real_divorced, filepointer_est3, filename_est3, "marital-status", "divorced", 3);
				pointquery(time_cm, cm, "Divorced", real_divorced, filepointer_est4, filename_est4, "marital-status", "divorced", 4);

				pointquery(time_cm, cm, "Sales", real_sales, filepointer_est1, filename_est1, "occupation", "sales", 1);
				pointquery(time_cm, cm, "Sales", real_sales, filepointer_est2, filename_est2, "occupation", "sales", 2);
				pointquery(time_cm, cm, "Sales", real_sales, filepointer_est3, filename_est3, "occupation", "sales", 3);
				pointquery(time_cm, cm, "Sales", real_sales, filepointer_est4, filename_est4, "occupation", "sales", 4);

				pointquery(time_cm, cm, "Armed-Forces", real_armed_forces, filepointer_est1, filename_est1, "occupation", "armed-forces", 1);
				pointquery(time_cm, cm, "Armed-Forces", real_armed_forces, filepointer_est2, filename_est2, "occupation", "armed-forces", 2);
				pointquery(time_cm, cm, "Armed-Forces", real_armed_forces, filepointer_est3, filename_est3, "occupation", "armed-forces", 3);
				pointquery(time_cm, cm, "Armed-Forces", real_armed_forces, filepointer_est4, filename_est4, "occupation", "armed-forces", 4);

				pointquery(time_cm, cm, "Wife", real_wife, filepointer_est1, filename_est1, "relationship", "wife", 1);
				pointquery(time_cm, cm, "Wife", real_wife, filepointer_est2, filename_est2, "relationship", "wife", 2);
				pointquery(time_cm, cm, "Wife", real_wife, filepointer_est3, filename_est3, "relationship", "wife", 3);
				pointquery(time_cm, cm, "Wife", real_wife, filepointer_est4, filename_est4, "relationship", "wife", 4);

				pointquery(time_cm, cm, "Husband", real_husband, filepointer_est1, filename_est1, "relationship", "husband", 1);
				pointquery(time_cm, cm, "Husband", real_husband, filepointer_est2, filename_est2, "relationship", "husband", 2);
				pointquery(time_cm, cm, "Husband", real_husband, filepointer_est3, filename_est3, "relationship", "husband", 3);
				pointquery(time_cm, cm, "Husband", real_husband, filepointer_est4, filename_est4, "relationship", "husband", 4);

				pointquery(time_cm, cm, "White", real_white, filepointer_est1, filename_est1, "race", "white", 1);
				pointquery(time_cm, cm, "White", real_white, filepointer_est2, filename_est2, "race", "white", 2);
				pointquery(time_cm, cm, "White", real_white, filepointer_est3, filename_est3, "race", "white", 3);
				pointquery(time_cm, cm, "White", real_white, filepointer_est4, filename_est4, "race", "white", 4);

				pointquery(time_cm, cm, "Black", real_black, filepointer_est1, filename_est1, "race", "black", 1);
				pointquery(time_cm, cm, "Black", real_black, filepointer_est2, filename_est2, "race", "black", 2);
				pointquery(time_cm, cm, "Black", real_black, filepointer_est3, filename_est3, "race", "black", 3);
				pointquery(time_cm, cm, "Black", real_black, filepointer_est4, filename_est4, "race", "black", 4);

				pointquery(time_cm, cm, "Female", real_female, filepointer_est1, filename_est1, "sex", "female", 1);
				pointquery(time_cm, cm, "Female", real_female, filepointer_est2, filename_est2, "sex", "female", 2);
				pointquery(time_cm, cm, "Female", real_female, filepointer_est3, filename_est3, "sex", "female", 3);
				pointquery(time_cm, cm, "Female", real_female, filepointer_est4, filename_est4, "sex", "female", 4);

				pointquery(time_cm, cm, "Male", real_male, filepointer_est1, filename_est1, "sex", "male", 1);
				pointquery(time_cm, cm, "Male", real_male, filepointer_est2, filename_est2, "sex", "male", 2);
				pointquery(time_cm, cm, "Male", real_male, filepointer_est3, filename_est3, "sex", "male", 3);
				pointquery(time_cm, cm, "Male", real_male, filepointer_est4, filename_est4, "sex", "male", 4);


// ########################################################	// ########################################################	// ########################################################	

				unionquery(time_cm, cm, "Private", union_private_private, filepointer_union1, filename_union1, "Private", "private", "private", 1);
				unionquery(time_cm, cm, "Private", union_private_private, filepointer_union2, filename_union2, "Private", "private", "private", 2);
				unionquery(time_cm, cm, "Private", union_private_private, filepointer_union3, filename_union3, "Private", "private", "private", 3);

				unionquery(time_cm, cm, "Private", union_private_never_worked, filepointer_union1, filename_union1, "Never-worked", "private", "never-worked", 1);
				unionquery(time_cm, cm, "Private", union_private_never_worked, filepointer_union2, filename_union2, "Never-worked", "private", "never-worked", 2);
				unionquery(time_cm, cm, "Private", union_private_never_worked, filepointer_union3, filename_union3, "Never-worked", "private", "never-worked", 3);

				unionquery(time_cm, cm, "Private", union_private_bachelors, filepointer_union1, filename_union1, "Bachelors", "private", "bachelors", 1);
				unionquery(time_cm, cm, "Private", union_private_bachelors, filepointer_union2, filename_union2, "Bachelors", "private", "bachelors", 2);
				unionquery(time_cm, cm, "Private", union_private_bachelors, filepointer_union3, filename_union3, "Bachelors", "private", "bachelors", 3);
				
				unionquery(time_cm, cm, "Private", union_private_masters, filepointer_union1, filename_union1, "Masters", "private", "masters", 1);
				unionquery(time_cm, cm, "Private", union_private_masters, filepointer_union2, filename_union2, "Masters", "private", "masters", 2);
				unionquery(time_cm, cm, "Private", union_private_masters, filepointer_union3, filename_union3, "Masters", "private", "masters", 3);

				unionquery(time_cm, cm, "Private", union_private_preschool, filepointer_union1, filename_union1, "Preschool", "private", "preschool", 1);
				unionquery(time_cm, cm, "Private", union_private_preschool, filepointer_union2, filename_union2, "Preschool", "private", "preschool", 2);
				unionquery(time_cm, cm, "Private", union_private_preschool, filepointer_union3, filename_union3, "Preschool", "private", "preschool", 3);

				unionquery(time_cm, cm, "Private", union_private_never_married, filepointer_union1, filename_union1, "Never-married", "private", "never-married", 1);
				unionquery(time_cm, cm, "Private", union_private_never_married, filepointer_union2, filename_union2, "Never-married", "private", "never-married", 2);
				unionquery(time_cm, cm, "Private", union_private_never_married, filepointer_union3, filename_union3, "Never-married", "private", "never-married", 3);

				unionquery(time_cm, cm, "Private", union_private_married_civ_spouse, filepointer_union1, filename_union1, "Married-civ-spouse", "private", "married-civ-spouse", 1);
				unionquery(time_cm, cm, "Private", union_private_married_civ_spouse, filepointer_union2, filename_union2, "Married-civ-spouse", "private", "married-civ-spouse", 2);
				unionquery(time_cm, cm, "Private", union_private_married_civ_spouse, filepointer_union3, filename_union3, "Married-civ-spouse", "private", "married-civ-spouse", 3);

				unionquery(time_cm, cm, "Private", union_private_divorced, filepointer_union1, filename_union1, "Divorced", "private", "divorced", 1);
				unionquery(time_cm, cm, "Private", union_private_divorced, filepointer_union2, filename_union2, "Divorced", "private", "divorced", 2);
				unionquery(time_cm, cm, "Private", union_private_divorced, filepointer_union3, filename_union3, "Divorced", "private", "divorced", 3);

				unionquery(time_cm, cm, "Private", union_private_sales, filepointer_union1, filename_union1, "Sales", "private", "sales", 1);
				unionquery(time_cm, cm, "Private", union_private_sales, filepointer_union2, filename_union2, "Sales", "private", "sales", 2);
				unionquery(time_cm, cm, "Private", union_private_sales, filepointer_union3, filename_union3, "Sales", "private", "sales", 3);

				unionquery(time_cm, cm, "Private", union_private_armed_forces, filepointer_union1, filename_union1, "Armed-Forces", "private", "armed-forces", 1);
				unionquery(time_cm, cm, "Private", union_private_armed_forces, filepointer_union2, filename_union2, "Armed-Forces", "private", "armed-forces", 2);
				unionquery(time_cm, cm, "Private", union_private_armed_forces, filepointer_union3, filename_union3, "Armed-Forces", "private", "armed-forces", 3);

				unionquery(time_cm, cm, "Private", union_private_wife, filepointer_union1, filename_union1, "Wife", "private", "wife", 1);
				unionquery(time_cm, cm, "Private", union_private_wife, filepointer_union2, filename_union2, "Wife", "private", "wife", 2);
				unionquery(time_cm, cm, "Private", union_private_wife, filepointer_union3, filename_union3, "Wife", "private", "wife", 3);

				unionquery(time_cm, cm, "Private", union_private_husband, filepointer_union1, filename_union1, "Husband", "private", "husband", 1);
				unionquery(time_cm, cm, "Private", union_private_husband, filepointer_union2, filename_union2, "Husband", "private", "husband", 2);
				unionquery(time_cm, cm, "Private", union_private_husband, filepointer_union3, filename_union3, "Husband", "private", "husband", 3);

				unionquery(time_cm, cm, "Private", union_private_white, filepointer_union1, filename_union1, "White", "private", "white", 1);
				unionquery(time_cm, cm, "Private", union_private_white, filepointer_union2, filename_union2, "White", "private", "white", 2);
				unionquery(time_cm, cm, "Private", union_private_white, filepointer_union3, filename_union3, "White", "private", "white", 3);

				unionquery(time_cm, cm, "Private", union_private_black, filepointer_union1, filename_union1, "Black", "private", "black", 1);
				unionquery(time_cm, cm, "Private", union_private_black, filepointer_union2, filename_union2, "Black", "private", "black", 2);
				unionquery(time_cm, cm, "Private", union_private_black, filepointer_union3, filename_union3, "Black", "private", "black", 3);

				unionquery(time_cm, cm, "Private", union_private_female, filepointer_union1, filename_union1, "Female", "private", "female", 1);
				unionquery(time_cm, cm, "Private", union_private_female, filepointer_union2, filename_union2, "Female", "private", "female", 2);
				unionquery(time_cm, cm, "Private", union_private_female, filepointer_union3, filename_union3, "Female", "private", "female", 3);

				unionquery(time_cm, cm, "Private", union_private_male, filepointer_union1, filename_union1, "Male", "private", "male", 1);
				unionquery(time_cm, cm, "Private", union_private_male, filepointer_union2, filename_union2, "Male", "private", "male", 2);
				unionquery(time_cm, cm, "Private", union_private_male, filepointer_union3, filename_union3, "Male", "private", "male", 3);

				//MAX
				unionquery(time_cm, cm, "Private", union_private_private, filepointer_union4, filename_union4, "Private", "private", "private", 4);
				unionquery(time_cm, cm, "Private", union_private_never_worked, filepointer_union4, filename_union4, "Never-worked", "private", "never-worked", 4);
				unionquery(time_cm, cm, "Private", union_private_bachelors, filepointer_union4, filename_union4, "Bachelors", "private", "bachelors", 4);
				unionquery(time_cm, cm, "Private", union_private_masters, filepointer_union4, filename_union4, "Masters", "private", "masters", 4);
				unionquery(time_cm, cm, "Private", union_private_preschool, filepointer_union4, filename_union4, "Preschool", "private", "preschool", 4);
				unionquery(time_cm, cm, "Private", union_private_never_married, filepointer_union4, filename_union4, "Never-married", "private", "never-married", 4);
				unionquery(time_cm, cm, "Private", union_private_married_civ_spouse, filepointer_union4, filename_union4, "Married-civ-spouse", "private", "married-civ-spouse", 4);
				unionquery(time_cm, cm, "Private", union_private_divorced, filepointer_union4, filename_union4, "Divorced", "private", "divorced", 4);
				unionquery(time_cm, cm, "Private", union_private_sales, filepointer_union4, filename_union4, "Sales", "private", "sales", 4);
				unionquery(time_cm, cm, "Private", union_private_armed_forces, filepointer_union4, filename_union4, "Armed-Forces", "private", "armed-forces", 4);
				unionquery(time_cm, cm, "Private", union_private_wife, filepointer_union4, filename_union4, "Wife", "private", "wife", 4);
				unionquery(time_cm, cm, "Private", union_private_husband, filepointer_union4, filename_union4, "Husband", "private", "husband", 4);
				unionquery(time_cm, cm, "Private", union_private_white, filepointer_union4, filename_union4, "White", "private", "white", 4);
				unionquery(time_cm, cm, "Private", union_private_black, filepointer_union4, filename_union4, "Black", "private", "black", 4);
				unionquery(time_cm, cm, "Private", union_private_female, filepointer_union4, filename_union4, "Female", "private", "female", 4);
				unionquery(time_cm, cm, "Private", union_private_male, filepointer_union4, filename_union4, "Male", "private", "male", 4);


// ########################################################	// ########################################################	// ########################################################	

				interquery(time_cm, cm, "Private", inter_private_private, filepointer_inter1, filename_inter1, "Private", "private", "private", 1);
				interquery(time_cm, cm, "Private", inter_private_private, filepointer_inter2, filename_inter2, "Private", "private", "private", 2);
				interquery(time_cm, cm, "Private", inter_private_private, filepointer_inter3, filename_inter3, "Private", "private", "private", 3);
		
				interquery(time_cm, cm, "Private", inter_private_never_worked, filepointer_inter1, filename_inter1, "Never-worked", "private", "never-worked", 1);
				interquery(time_cm, cm, "Private", inter_private_never_worked, filepointer_inter2, filename_inter2, "Never-worked", "private", "never-worked", 2);
				interquery(time_cm, cm, "Private", inter_private_never_worked, filepointer_inter3, filename_inter3, "Never-worked", "private", "never-worked", 3);
				
				interquery(time_cm, cm, "Private", inter_private_bachelors, filepointer_inter1, filename_inter1, "Bachelors", "private", "bachelors", 1);
				interquery(time_cm, cm, "Private", inter_private_bachelors, filepointer_inter2, filename_inter2, "Bachelors", "private", "bachelors", 2);
				interquery(time_cm, cm, "Private", inter_private_bachelors, filepointer_inter3, filename_inter3, "Bachelors", "private", "bachelors", 3);
				
				interquery(time_cm, cm, "Private", inter_private_masters, filepointer_inter1, filename_inter1, "Masters", "private", "masters", 1);
				interquery(time_cm, cm, "Private", inter_private_masters, filepointer_inter2, filename_inter2, "Masters", "private", "masters", 2);
				interquery(time_cm, cm, "Private", inter_private_masters, filepointer_inter3, filename_inter3, "Masters", "private", "masters", 3);

				interquery(time_cm, cm, "Private", inter_private_preschool, filepointer_inter1, filename_inter1, "Preschool", "private", "preschool", 1);
				interquery(time_cm, cm, "Private", inter_private_preschool, filepointer_inter2, filename_inter2, "Preschool", "private", "preschool", 2);
				interquery(time_cm, cm, "Private", inter_private_preschool, filepointer_inter3, filename_inter3, "Preschool", "private", "preschool", 3);

				interquery(time_cm, cm, "Private", inter_private_never_married, filepointer_inter1, filename_inter1, "Never-married", "private", "never-married", 1);
				interquery(time_cm, cm, "Private", inter_private_never_married, filepointer_inter2, filename_inter2, "Never-married", "private", "never-married", 2);
				interquery(time_cm, cm, "Private", inter_private_never_married, filepointer_inter3, filename_inter3, "Never-married", "private", "never-married", 3);

				interquery(time_cm, cm, "Private", inter_private_married_civ_spouse, filepointer_inter1, filename_inter1, "Married-civ-spouse", "private", "married-civ-spouse", 1);
				interquery(time_cm, cm, "Private", inter_private_married_civ_spouse, filepointer_inter2, filename_inter2, "Married-civ-spouse", "private", "married-civ-spouse", 2);
				interquery(time_cm, cm, "Private", inter_private_married_civ_spouse, filepointer_inter3, filename_inter3, "Married-civ-spouse", "private", "married-civ-spouse", 3);

				interquery(time_cm, cm, "Private", inter_private_divorced, filepointer_inter1, filename_inter1, "Divorced", "private", "divorced", 1);
				interquery(time_cm, cm, "Private", inter_private_divorced, filepointer_inter2, filename_inter2, "Divorced", "private", "divorced", 2);
				interquery(time_cm, cm, "Private", inter_private_divorced, filepointer_inter3, filename_inter3, "Divorced", "private", "divorced", 3);

				interquery(time_cm, cm, "Private", inter_private_sales, filepointer_inter1, filename_inter1, "Sales", "private", "sales", 1);
				interquery(time_cm, cm, "Private", inter_private_sales, filepointer_inter2, filename_inter2, "Sales", "private", "sales", 2);
				interquery(time_cm, cm, "Private", inter_private_sales, filepointer_inter3, filename_inter3, "Sales", "private", "sales", 3);

				interquery(time_cm, cm, "Private", inter_private_armed_forces, filepointer_inter1, filename_inter1, "Armed-Forces", "private", "armed-forces", 1);
				interquery(time_cm, cm, "Private", inter_private_armed_forces, filepointer_inter2, filename_inter2, "Armed-Forces", "private", "armed-forces", 2);
				interquery(time_cm, cm, "Private", inter_private_armed_forces, filepointer_inter3, filename_inter3, "Armed-Forces", "private", "armed-forces", 3);

				interquery(time_cm, cm, "Private", inter_private_wife, filepointer_inter1, filename_inter1, "Wife", "private", "wife", 1);
				interquery(time_cm, cm, "Private", inter_private_wife, filepointer_inter2, filename_inter2, "Wife", "private", "wife", 2);
				interquery(time_cm, cm, "Private", inter_private_wife, filepointer_inter3, filename_inter3, "Wife", "private", "wife", 3);

				interquery(time_cm, cm, "Private", inter_private_husband, filepointer_inter1, filename_inter1, "Husband", "private", "husband", 1);
				interquery(time_cm, cm, "Private", inter_private_husband, filepointer_inter2, filename_inter2, "Husband", "private", "husband", 2);
				interquery(time_cm, cm, "Private", inter_private_husband, filepointer_inter3, filename_inter3, "Husband", "private", "husband", 3);

				interquery(time_cm, cm, "Private", inter_private_white, filepointer_inter1, filename_inter1, "White", "private", "white", 1);
				interquery(time_cm, cm, "Private", inter_private_white, filepointer_inter2, filename_inter2, "White", "private", "white", 2);
				interquery(time_cm, cm, "Private", inter_private_white, filepointer_inter3, filename_inter3, "White", "private", "white", 3);

				interquery(time_cm, cm, "Private", inter_private_black, filepointer_inter1, filename_inter1, "Black", "private", "black", 1);
				interquery(time_cm, cm, "Private", inter_private_black, filepointer_inter2, filename_inter2, "Black", "private", "black", 2);
				interquery(time_cm, cm, "Private", inter_private_black, filepointer_inter3, filename_inter3, "Black", "private", "black", 3);

				interquery(time_cm, cm, "Private", inter_private_female, filepointer_inter1, filename_inter1, "Female", "private", "female", 1);
				interquery(time_cm, cm, "Private", inter_private_female, filepointer_inter2, filename_inter2, "Female", "private", "female", 2);
				interquery(time_cm, cm, "Private", inter_private_female, filepointer_inter3, filename_inter3, "Female", "private", "female", 3);

				interquery(time_cm, cm, "Private", inter_private_male, filepointer_inter1, filename_inter1, "Male", "private", "male", 1);
				interquery(time_cm, cm, "Private", inter_private_male, filepointer_inter2, filename_inter2, "Male", "private", "male", 2);
				interquery(time_cm, cm, "Private", inter_private_male, filepointer_inter3, filename_inter3, "Male", "private", "male", 3);
		
				//Max
				interquery(time_cm, cm, "Private", inter_private_private, filepointer_inter4, filename_inter4, "Private", "private", "private", 4);
				interquery(time_cm, cm, "Private", inter_private_never_worked, filepointer_inter4, filename_inter4, "Never-worked", "private", "never-worked", 4);
				interquery(time_cm, cm, "Private", inter_private_bachelors, filepointer_inter4, filename_inter4, "Bachelors", "private", "bachelors", 4);
				interquery(time_cm, cm, "Private", inter_private_masters, filepointer_inter4, filename_inter4, "Masters", "private", "masters", 4);
				interquery(time_cm, cm, "Private", inter_private_preschool, filepointer_inter4, filename_inter4, "Preschool", "private", "preschool", 4);
				interquery(time_cm, cm, "Private", inter_private_never_married, filepointer_inter4, filename_inter4, "Never-married", "private", "never-married", 4);
				interquery(time_cm, cm, "Private", inter_private_married_civ_spouse, filepointer_inter4, filename_inter4, "Married-civ-spouse", "private", "married-civ-spouse", 4);
				interquery(time_cm, cm, "Private", inter_private_divorced, filepointer_inter4, filename_inter4, "Divorced", "private", "divorced", 4);
				interquery(time_cm, cm, "Private", inter_private_sales, filepointer_inter4, filename_inter4, "Sales", "private", "sales", 4);
				interquery(time_cm, cm, "Private", inter_private_armed_forces, filepointer_inter4, filename_inter4, "Armed-Forces", "private", "armed-forces", 4);
				interquery(time_cm, cm, "Private", inter_private_wife, filepointer_inter4, filename_inter4, "Wife", "private", "wife", 4);
				interquery(time_cm, cm, "Private", inter_private_husband, filepointer_inter4, filename_inter4, "Husband", "private", "husband", 4);
				interquery(time_cm, cm, "Private", inter_private_white, filepointer_inter4, filename_inter4, "White", "private", "white", 4);
				interquery(time_cm, cm, "Private", inter_private_black, filepointer_inter4, filename_inter4, "Black", "private", "black", 4);
				interquery(time_cm, cm, "Private", inter_private_female, filepointer_inter4, filename_inter4, "Female", "private", "female", 4);
				interquery(time_cm, cm, "Private", inter_private_male, filepointer_inter4, filename_inter4, "Male", "private", "male", 4);
	

			    CM_Destroy(cm);
			}
		}
	}


	return 0;

}
