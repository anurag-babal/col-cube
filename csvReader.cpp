#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<bits/stdc++.h>
#define MAX_LINE_LENGTH 512
#define MAX_TOKENS 23 
#define MAX_COL_DATA 100
#define PATH "/Users/sudhanshu/dm_project/COL-CUBE/cubestore/fact_sales/"
#define MAX_PATH_LENGTH 256
using namespace std;
const char* getField(char* line, int num) {
  const char* tok;
  for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\r\n")) {
    if (!num--)
      return tok;
  }
  return NULL;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <csv_file>\n", argv[0]);
    return 1;
  }
  FILE *fp, *colFile;
  char line[MAX_LINE_LENGTH];
  char* tokens[MAX_TOKENS];
  const char* csvFile = argv[1];
  int i, numLines = 0;
  char colName[MAX_LINE_LENGTH];
  char colData[MAX_COL_DATA];

  fp = fopen(csvFile, "r");
  if (fp == NULL) {
    perror("Error opening file");
    return 1;
  }

  while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
    numLines++;
  }
  rewind(fp);

  // Loop for each column
  for (int col = 0; col < MAX_TOKENS; col++) {
    fgets(line, MAX_LINE_LENGTH, fp);
    const char *fieldName=getField(line,col);
    cout<<fieldName<<"\n";
    if(fieldName==NULL) break;
    sprintf(colName, "%s.dat", fieldName); 
    string res=PATH;
    res+=colName;
    cout<<res<<" ";
    colFile = fopen(res.c_str(), "wb");
    printf("Column %s Data:\n", colName);

    // Loop through each line
    for (int i = 1; i < numLines; i++) {
      fgets(line, MAX_LINE_LENGTH, fp);
      // colData = getField(line, col);
      strcpy(colData, getField(line, col));
      strcat(colData, "\n");
      fwrite(colData, strlen(colData), 1, colFile);
    }
    printf("\n");
    rewind(fp);
  }

  fclose(fp);

  return 0;
}