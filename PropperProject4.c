/*
* Program Name: "COP 2220 Project 4: File analyzer"
*
* Description: A program that loads and analizes numbers form a file 
*
* Author: Markos Calderon (N01600902)
*
* Date: November 25, 2024
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void GetFileNames(char* fname, char* fnameOut);         //Prompts user for names of the files
void GetFileDim(int* rows, int* cols, char* fname);     //Reads the first 2 lines of the file
double** Allocate2DArray(int rows, int cols);           //Allocates a given 2D array
void Free2DArray(double** ary, int rows);               //Frees said 2D array
double** LoadArrayFromFile(int rows, int cols, char* fname);        //Loads said 2D array with data from a given file
void AddAverage(int rows, int* cols, double** ary);     //Adds a column to said 2D array and stores an average of each row in them
void SaveResult(int rows, int cols, double** ary, char* fnameOut);  //Stores said 2D array into a file

int main (void){
    //Declare variables
    double** ary;
    char fname[256];
    char fnameOut[256];
    int rows;
    int cols;

    //Get filenames DONE
    GetFileNames(fname, fnameOut);
    if(strcmp(fname, "quit") == 0){
        return 0;
    }
    if(strcmp(fnameOut, "quit") == 0){
        return 0;
    }
    //Get Sizes EZ
    GetFileDim(&rows, &cols, fname);
    //Allocate array EZ
    ary = Allocate2DArray(rows, cols);
    //Load Array
    ary = LoadArrayFromFile(rows, cols, fname);     //FIXED    //printf("%lf\n", ary[0][4]); //Debugging
    //Add average (Increase cols by 1)
    AddAverage(rows, &cols, ary);                   //FIXED    //printf("%lf\n", ary[0][5]);    //printf("%d\n", cols); //Debugging
    //Save the data into a nex file with fnameOut
    SaveResult(rows, cols, ary, fnameOut);          //FIXED
    //Free 2D array
    Free2DArray(ary, rows);

    return 0;
}

/*
* Function Name: GetFileNames
*
* Input Parameters: 
*       fname: Character pointer type array
*       fnameOut: Character pointer type array
*
* Description: Reads standard input and updates two arrays with the file names given by the user
*
* Return Value: Void Type Nothing
*/
void GetFileNames(char* fname, char* fnameOut){
    printf("Type the input filename, path, or \"quit\":\n");
    scanf( "%s", fname);
    if(strcmp(fname, "quit") == 0){
        return;
    }
    printf("Type the output filename, path, or \"quit\":\n");
    scanf( "%s", fnameOut);
    if(strcmp(fnameOut, "quit") == 0){
        return;
    }
    //printf("%s %s\n", fname, fnameOut); Debugging
    return;
}

/*
* Function Name: GetFileDim
*
* Input Parameters: 
*       rows: Integer pointer
*       cols: Integer pointer
*       fname: Character pointer type array
*
* Description: Reads a file and updates two integer pointers with the file's metadata
*
* Return Value: Void Type Nothing
*/
void GetFileDim(int* rows, int* cols, char* fname){
    FILE* fp;
    fp = fopen(fname, "r");
    fscanf(fp, "%d ", &*rows);
    fscanf(fp, " %d", &*cols);
    fclose(fp);
    //printf("rows: %d, cols: %d  Pionono <3 \n", *rows, *cols);    //Debug
    return;
}

/*
* Function Name: Allocate2DArray
*
* Input Parameters: 
*       rows: Integer pointer
*       cols: Integer pointer       
*
* Description: Allocates a 2D Array
*
* Return Value: Double pointer of a double pointer
*/
double** Allocate2DArray(int rows, int cols){
    double** ary = (double**)malloc(rows * sizeof(double*));
    if (ary == NULL) {
        return NULL; // failed X(
    }

    for (int i = 0; i < rows; i++) {
        ary[i] = (double*)malloc(cols * sizeof(double));
        if (ary[i] == NULL) {
            //Free Allllll
            for (int j = 0; j < i; j++) {
                free(ary[j]);
            }
            free(ary);
            return NULL;
        }
    }
    return ary;
}

/*
* Function Name: Free 2D Array
*
* Input Parameters: 
*       rows: Integer pointer
*       ary: Double pointer type array of pointers type arrays
*
* Description: Reads a file and updates two integer pointers with the file's metadata
*
* Return Value: Void Type Nothing
*/
void Free2DArray(double** ary, int rows){
    for (int i = 0; i < rows; i++) {
        free(ary[i]);
    }
    free(ary);
    return;
}

/*
* Function Name: LoadArrayFromFile
*
* Input Parameters: 
*       rows: Integer variable
*       cols: Integer variable
*       fname: Character pointer type array
*
* Description: Reads a file and stores its data into a given 2D double array
*
* Return Value: Double pointer of double pointer
*/
double** LoadArrayFromFile(int rows, int cols, char* fname){
    double** ary = Allocate2DArray(rows, cols);
    FILE* fp;
    fp = fopen(fname, "r");

    if(fp == NULL){
        printf("Error opening File\n");
        return NULL;
    }

    char tmp[cols];
    int lenght = 0;
    int cur1, cur2; //just to move the cursor (not used)
    fseek(fp, 4, SEEK_SET); //this is better

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (fscanf(fp, "%lf", &ary[i][j]) != 1) {
                printf("Error reading file\n");
                fclose(fp);
                return NULL;
            }
        }
    }
    //printf("Data Loaded successfully ^^\n"); debug
    fclose(fp);
    return ary;
}

/*
* Function Name: AddAverage
*
* Input Parameters: 
*       rows: Integer variable
*       cols: Integer pointer
*       ary: Double pointer type array of pointers type arrays
*
* Description: Adds a column to a 2D array and stores an average of each row in them ignoring the 1st column
*
* Return Value: Void Type Nothing
*/
void AddAverage(int rows, int* cols, double** ary){
    int avg = 0;
    double sum = 0.0;
    *cols += 1;

    for (int i = 0; i < rows; i++) {
        for(int j = 1; j < (*cols - 1) ; j++){
            sum += ary[i][j];
        }
        
        ary[i] = (double*)realloc(ary[i], (*cols) * sizeof(double));
        ary[i][*cols - 1] = (sum / (*cols - 2));
        sum = 0;
    }
    return;
}

/*
* Function Name: SaveResult
*
* Input Parameters: 
*       rows: Integer variable
*       cols: Integer pointer
*       ary: Double pointer type array of pointers type arrays
*       fnameOut: Character type pointer
*
* Description: Stores the data of a 2D array in a file
*
* Return Value: Void Type Nothing
*/
void SaveResult(int rows, int cols, double** ary, char* fnameOut){
    FILE* fpo;
    fpo = fopen(fnameOut, "w");
    
    if(fpo == NULL){
        printf("Error Writing File\n");
        return;
    }

    fprintf(fpo, "%d\n%d\n", rows, cols);

    //printf("aaa %d\n", fpo); //debug

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++){
            if(j == 0){     //Crashed x.x
                fprintf(fpo, "%.0lf ", ary[i][j]);
            }
            else{
            fprintf(fpo, "%.2lf ", ary[i][j]);
            }
        }
        fprintf(fpo, "\n");
    }
    fclose(fpo);
    return;
}