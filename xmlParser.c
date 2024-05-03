#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>

#define TYPE "type"
#define NAME "name"
#define PATH "/Users/sudhanshu/dm_project/COL-CUBE"
// #define PATH "/home/arch/D/dm"
#define COLNAME "attribute"
#define TABLENAME "tablename"
#define IS_PART_OF_LATTICE "is_part_of_lattice"
#define TRUE "true"
#define FALSE "false"
#define DIM_ATTR "dim_attr.bin"
#define AGGR "aggr"
#define MAX_PATH_LENGTH 256
#define MAX_STRING_LENGTH 100
#define ATTR_ARRAY_SIZE 100

struct Attribute {
  int length;
  char type[20];
  char values[20][50];
};

void createFolder(const char *path) {
  // Check if folder exists
  struct stat st;
  if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
    return;
  }

  int result = mkdir(path, 0755);
  if (result != 0) {
    fprintf(stderr, "Error creating folder: %s (%s)\n", path, strerror(errno));
  }
}

void createDBFolder(xmlNode *node, char *currentPath) {
  strncat(currentPath, "/", MAX_PATH_LENGTH - strlen(currentPath) - 1);
  strncat(currentPath, node->properties->children->content, MAX_PATH_LENGTH - strlen(currentPath) - 1);
  createFolder(currentPath);
}

int createTableFolder(xmlNodePtr node, char *currentPath) {
  xmlAttr *attr = NULL;
  const char *name;
  const char *type;
  char result[MAX_STRING_LENGTH];
  strncat(currentPath, "/", MAX_PATH_LENGTH - strlen(currentPath) - 1);

  // Print attribute values if they exist
  for (attr = node->properties; attr; attr = attr->next) {
    if ((!xmlStrcmp(attr->name, (const xmlChar *)NAME))) {
      name = attr->children->content;
      printf("Table: %s ", name);
    }
    if ((!xmlStrcmp(attr->name, (const xmlChar *)TYPE))) {
      type = attr->children->content;
      printf("Type: %s \n", type);
    }
  }

  strcpy(result, type);
  strcat(result, "_");
  strncat(result, name, MAX_STRING_LENGTH - strlen(result) - 1); // Append name, considering available space
  strncat(currentPath, result, MAX_PATH_LENGTH - strlen(currentPath) - 1);
  createFolder(currentPath);

  return strlen(result);
}

void createFile(const char *path) {
  FILE *fp = fopen(path, "wb");
  if (fp == NULL) {
    fprintf(stderr, "Error creating file: %s\n", path);
  }
  fclose(fp);
}

void writeFile(FILE *fp, char arr[][50], int length, char *type) {
  struct Attribute attr;
  attr.length = length;
  strcpy(attr.type, type);
  for (int i = 0; i < length; i++) {
    strcpy(attr.values[i], arr[i]);
  }
  fwrite(&attr, sizeof(struct Attribute), 1, fp);
}

void createColumnFile(char *name, char *currentPath) {
  char filename[MAX_STRING_LENGTH];
  strcpy(filename, name);
  strncat(filename, ".dat", MAX_STRING_LENGTH - strlen(filename) - 1);
  strncat(currentPath, "/", MAX_PATH_LENGTH - strlen(currentPath) - 1);
  strncat(currentPath, filename, MAX_PATH_LENGTH - strlen(currentPath) - 1);
  createFile(currentPath);
  currentPath[strlen(currentPath) - strlen(filename) - 1] = '\0';
}

void parseTable(xmlDocPtr doc, xmlNodePtr cur, char *currentPath) {
  xmlChar *column;
  int folderNameSize = createTableFolder(cur, currentPath);

  cur = cur->xmlChildrenNode;
  while (cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar *)COLNAME))) {
      column = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      printf("  Column: %s\n", column);
      createColumnFile(column, currentPath);
      xmlFree(column);
    }
    cur = cur->next;
  }
  currentPath[strlen(currentPath) - folderNameSize - 1] = '\0';
}

void getIsPartOfLattice(xmlDocPtr doc, xmlNodePtr node) {
  xmlAttr *attr = NULL;
  const char *name;
  const char *isPartOfLattice;
  char res[100][50], set[100][50];
  int index = 0, index2 = 0;
  while (node != NULL) {
    if ((!xmlStrcmp(node->name, (const xmlChar *)COLNAME))) {
      for (attr = node->properties; attr; attr = attr->next) {
        if ((!xmlStrcmp(attr->name, (const xmlChar *)IS_PART_OF_LATTICE)) && (!xmlStrcmp(attr->children->content, (const xmlChar *)TRUE))) {
          name = xmlNodeListGetString(doc, node->xmlChildrenNode, 1);
          strcpy(res[index], name);
          index++;
        }
        else if ((!xmlStrcmp(attr->name, (const xmlChar *)IS_PART_OF_LATTICE)) && (!xmlStrcmp(attr->children->content, (const xmlChar *)FALSE))) {
          name = xmlNodeListGetString(doc, node->xmlChildrenNode, 1);
          strcpy(set[index2], name);
          index2++;
        }
      }
    }
    node = node->next;
  }

  char result[MAX_PATH_LENGTH];
  strcpy(result, PATH);
  strcat(result, "/");
  strcat(result, DIM_ATTR);
  createFile(result);
  FILE *attrFile = fopen(DIM_ATTR, "wb");
  writeFile(attrFile, res, index, "dim");
  writeFile(attrFile, set, index2, "fact");
  fclose(attrFile);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <xml_file>\n", argv[0]);
    return 1;
  }

  const char *xml_file = argv[1];

  // Initialize libxml2
  xmlInitParser();

  // Parse the XML document
  xmlDocPtr doc = xmlParseFile(xml_file);

  if (doc == NULL) {
    fprintf(stderr, "Error parsing XML file: %s\n", xml_file);
    xmlCleanupParser();
    return 1;
  }

  // Get root element
  xmlNode *root = xmlDocGetRootElement(doc);

  char currentPath[MAX_PATH_LENGTH] = PATH;

  printf("Database: %s\n", root->name);
  createDBFolder(root, currentPath);

  char aggrArr[100][50];
  int sz = 0;
  xmlNodePtr cur = root->xmlChildrenNode;

  while (cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar *)TABLENAME))) {
      parseTable(doc, cur, currentPath);
      getIsPartOfLattice(doc, cur->children);
    }
    else if ((!xmlStrcmp(cur->name, (const xmlChar *)AGGR))) {
      strcpy(aggrArr[sz],cur->children->content);
      sz++;
    }
    cur = cur->next;
  }

  // createColumnFile(DIM_ATTR,PATH);
  FILE *fp = fopen(DIM_ATTR,"ab");
  writeFile(fp, aggrArr, sz, "aggr");
  fclose(fp);

  // Free memory allocated by libxml2
  xmlFreeDoc(doc);
  xmlCleanupParser();

  return 0;
}
