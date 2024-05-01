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
vector<string> files;
int numLines = 0;
const char* getField(char* line, int num) {
  const char* tok;
  for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\r\n")) {
    if (!num--)
      return tok;
  }
  return NULL;
}
void mapper(string colName){
  string FileNameWrite=PATH+colName+".txt";
    string FileNameRead=PATH+colName+".dat";
    freopen(FileNameRead.c_str(),"r",stdin);
    freopen(FileNameWrite.c_str(),"wb",stdout);
  map<string,vector<unsigned char>> stringToBit;
  for(int i=0;i<numLines;i++){
    string s;
    cin>>s;
    if(stringToBit.find(s)==stringToBit.end()) stringToBit[s]=vector<unsigned char>(100000,'0');
    stringToBit[s][i]='1';
  }
  for(auto x:stringToBit){
    cout<<x.first<<" ";
    for(auto ch:x.second){
      cout<<ch;
    }
    cout<<"\n";
  }
  fclose(stdout);
  fclose(stdin);
}
void makeBitMap(){
  for(int i=0;i<files.size();i++){
    cerr<<files[i]<<" ";
    mapper(files[i]);
  }
}
int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <csv_file>\n", argv[0]);
    return 1;
  }
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  FILE *fp, *colFile;
  char line[MAX_LINE_LENGTH];
  char* tokens[MAX_TOKENS];
  const char* csvFile = argv[1];
  int i;
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
    // cout<<fieldName<<"\n";
    if(fieldName==NULL) break;
    sprintf(colName, "%s.dat", fieldName); 
    files.push_back(fieldName);
    string res=PATH;
    res+=colName;
    // cout<<res<<" ";
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
  // makeBitMap();
  return 0;
}
