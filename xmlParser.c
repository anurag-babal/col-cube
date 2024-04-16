#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <sys/stat.h>
#include <errno.h>

#define TYPE "type"
#define NAME "name"
#define PATH "/home/arch/dm"
#define COLNAME "attributes"
#define TABLENAME "tablename"
#define MAX_PATH_LENGTH 256
#define MAX_STRING_LENGTH 100

void createFolder(const char* path) {
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

void createDBFolder(xmlNode* node, char* currentPath) {
  strncat(currentPath, "/", MAX_PATH_LENGTH - strlen(currentPath) - 1);
  strncat(currentPath, node->name, MAX_PATH_LENGTH - strlen(currentPath) - 1);
  createFolder(currentPath);
}

int createTableFolder(xmlNodePtr node, char *currentPath) {
  xmlAttr* attr = NULL;
  const char * name;
  const char * type;
  char result[MAX_STRING_LENGTH];
  strncat(currentPath, "/", MAX_PATH_LENGTH - strlen(currentPath) - 1);

  // Print attribute values if they exist
  for (attr = node->properties; attr; attr = attr->next) {
    if ((!xmlStrcmp(attr->name, (const xmlChar *) NAME))) {
      name = attr->children->content;
      printf("Table: %s ", name);
    }
    if ((!xmlStrcmp(attr->name, (const xmlChar *) TYPE))) {
      type = attr->children->content;
      printf("Type: %s \n", type);
    }
  }

  strcpy(result, type);
  strcat(result, "_");
  strncat(result, name, MAX_STRING_LENGTH - strlen(result) - 1); // Append name, considering available space
  strncat(currentPath, result , MAX_PATH_LENGTH - strlen(currentPath) - 1);
  createFolder(currentPath);

  return strlen(result);
}

void createFile(const char *path) {
  // Open the file in binary write mode ("wb") and create it if it doesn't exist
  FILE *fp = fopen(path, "wb");
  if (fp == NULL) {
    fprintf(stderr, "Error creating file: %s\n", path);
  }
  fclose(fp);
}

void createColumnFile(char *name, char *currentPath) {
  char filename[MAX_STRING_LENGTH];
  strcpy(filename, name);
  strncat(filename, ".dat", MAX_STRING_LENGTH - strlen(filename) - 1);
  strncat(currentPath, "/", MAX_PATH_LENGTH - strlen(currentPath) - 1);
  strncat(currentPath, filename , MAX_PATH_LENGTH - strlen(currentPath) - 1);
  createFile(currentPath);
  currentPath[strlen(currentPath) - strlen(filename) - 1] = '\0';
}

void parseTable(xmlDocPtr doc, xmlNodePtr cur, char *currentPath) {
  xmlChar *column;
  int folderNameSize = createTableFolder(cur, currentPath);

  cur = cur->xmlChildrenNode;
  while (cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar *) COLNAME))) {
      column = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      printf("  Column: %s\n", column);
      createColumnFile(column, currentPath);
		  xmlFree(column);
    }
    cur = cur->next;
  }
  currentPath[strlen(currentPath) - folderNameSize - 1] = '\0';
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <xml_file>\n", argv[0]);
    return 1;
  }

  const char* xml_file = argv[1];

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
  xmlNode* root = xmlDocGetRootElement(doc);

  char currentPath[MAX_PATH_LENGTH] = PATH;
  
  printf("Database: %s\n",root->name);
  createDBFolder(root, currentPath);

  xmlNodePtr cur = root->xmlChildrenNode;
  while (cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar *) TABLENAME))) {
      parseTable(doc, cur, currentPath);
    }
    cur = cur->next;
  }

  // Free memory allocated by libxml2
  xmlFreeDoc(doc);
  xmlCleanupParser();

  return 0;
}
