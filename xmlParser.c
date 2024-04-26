#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <sys/stat.h>
#include <errno.h>

#define TYPE "type"
#define NAME "name"
#define PATH "/Users/sudhanshu/dm_project/COL-CUBE"
#define COLNAME "attribute"
#define TABLENAME "tablename"
#define IS_PART_OF_LATTICE "is_part_of_lattice"
#define TRUE "true"
#define FALSE "false"
#define DIM_ATTR "dim_attr.txt"
#define AGGR "aggr"
#define MAX_PATH_LENGTH 256
#define MAX_STRING_LENGTH 100
#define ATTR_ARRAY_SIZE 100
void createFolder(const char *path)
{
  // Check if folder exists
  struct stat st;
  if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
  {
    return;
  }

  int result = mkdir(path, 0755);
  if (result != 0)
  {
    fprintf(stderr, "Error creating folder: %s (%s)\n", path, strerror(errno));
  }
}

void createDBFolder(xmlNode *node, char *currentPath)
{
  strncat(currentPath, "/", MAX_PATH_LENGTH - strlen(currentPath) - 1);
  // printf("node->properties->children->content          %s\n",node->properties->children->content);
  strncat(currentPath, node->properties->children->content, MAX_PATH_LENGTH - strlen(currentPath) - 1);
  createFolder(currentPath);
}

int createTableFolder(xmlNodePtr node, char *currentPath)
{
  xmlAttr *attr = NULL;
  const char *name;
  const char *type;
  char result[MAX_STRING_LENGTH];
  strncat(currentPath, "/", MAX_PATH_LENGTH - strlen(currentPath) - 1);

  // Print attribute values if they exist
  for (attr = node->properties; attr; attr = attr->next)
  {
    if ((!xmlStrcmp(attr->name, (const xmlChar *)NAME)))
    {
      name = attr->children->content;
      printf("Table: %s ", name);
    }
    if ((!xmlStrcmp(attr->name, (const xmlChar *)TYPE)))
    {
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

void createFile(const char *path)
{
  // Open the file in binary write mode ("wb") and create it if it doesn't exist
  FILE *fp = fopen(path, "wb");
  if (fp == NULL)
  {
    fprintf(stderr, "Error creating file: %s\n", path);
  }
  fclose(fp);
}
void writeFile(FILE *fp, char arr[][50], int length)
{
  char result[MAX_STRING_LENGTH];
  sprintf(result, "%d\n", length);
  printf("\n%s\n", result);
  fwrite(result, strlen(result), 1, fp);
  result[0] = '\0';
  for (int i = 0; i < length; i++)
  {
    strcat(result, arr[i]);
    strcat(result, " ");
    fwrite(result, strlen(result), 1, fp);
    result[0] = '\0';
  }
}
void createColumnFile(char *name, char *currentPath)
{
  char filename[MAX_STRING_LENGTH];
  strcpy(filename, name);
  strncat(filename, ".dat", MAX_STRING_LENGTH - strlen(filename) - 1);
  strncat(currentPath, "/", MAX_PATH_LENGTH - strlen(currentPath) - 1);
  strncat(currentPath, filename, MAX_PATH_LENGTH - strlen(currentPath) - 1);
  createFile(currentPath);
  currentPath[strlen(currentPath) - strlen(filename) - 1] = '\0';
}

void parseTable(xmlDocPtr doc, xmlNodePtr cur, char *currentPath)
{
  xmlChar *column;
  int folderNameSize = createTableFolder(cur, currentPath);

  cur = cur->xmlChildrenNode;
  while (cur != NULL)
  {
    if ((!xmlStrcmp(cur->name, (const xmlChar *)COLNAME)))
    {
      column = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      printf("  Column: %s\n", column);
      createColumnFile(column, currentPath);
      xmlFree(column);
    }
    cur = cur->next;
  }
  currentPath[strlen(currentPath) - folderNameSize - 1] = '\0';
}
void getIsPartOfLattice(xmlDocPtr doc, xmlNodePtr node, char **attrArray)
{
  xmlAttr *attr = NULL;
  const char *name;
  const char *isPartOfLattice;
  char res[100][50], set[100][50];
  int index = 0, index2 = 0;
  while (node != NULL)
  {
    // printf("Att %s",node->name);
    if ((!xmlStrcmp(node->name, (const xmlChar *)COLNAME)))
    {
      // is_part_of_lattice hai node->properties->name
      // printf("Column: %s\n", node->properties->name);
      for (attr = node->properties; attr; attr = attr->next)
      {
        if ((!xmlStrcmp(attr->name, (const xmlChar *)IS_PART_OF_LATTICE)) && (!xmlStrcmp(attr->children->content, (const xmlChar *)TRUE)))
        {
          name = xmlNodeListGetString(doc, node->xmlChildrenNode, 1);
          strcpy(res[index], name);
          index++;
        }
        else if ((!xmlStrcmp(attr->name, (const xmlChar *)IS_PART_OF_LATTICE)) && (!xmlStrcmp(attr->children->content, (const xmlChar *)FALSE)))
        {
          name = xmlNodeListGetString(doc, node->xmlChildrenNode, 1);
          strcpy(set[index2], name);
          index2++;
        }
      }
    }
    node = node->next;
  }
  for (int i = 0; i < index; i++)
  {
    strcpy(attrArray[i], res[i]);
  }

  // Print copied elements for verification
  // for (int i = 0; i < index; i++)
  // {
  //   printf("  - %s\n", attrArray[i]);
  // }
  attrArray[index] = NULL;
  char result[MAX_PATH_LENGTH];
  strcpy(result, PATH);
  strcat(result, "/dim_attr.txt");
  createFile(result);
  FILE *attrFile;
  attrFile = fopen("dim_attr.txt", "wb");
  writeFile(attrFile, res, index);
  writeFile(attrFile, set, index2);
  fclose(attrFile);
}
int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <xml_file>\n", argv[0]);
    return 1;
  }

  const char *xml_file = argv[1];
  char **attrArray = malloc(100 * sizeof(char *)); // Assuming maximum 100 strings
  for (int i = 0; i < 100; i++)
  {
    attrArray[i] = malloc(50 * sizeof(char)); // Assuming maximum string length of 50
  }
  // Initialize libxml2
  xmlInitParser();

  // Parse the XML document
  xmlDocPtr doc = xmlParseFile(xml_file);

  if (doc == NULL)
  {
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
  int sz=0;
  xmlNodePtr cur = root->xmlChildrenNode;
  int flag = 1;
  while (cur != NULL)
  {
    if ((!xmlStrcmp(cur->name, (const xmlChar *)TABLENAME)))
    {
      parseTable(doc, cur, currentPath);
      // if (flag == 1)
      // {
        getIsPartOfLattice(doc, cur->children, attrArray);
      //   flag = 0;
      // }
    }
    else if ((!xmlStrcmp(cur->name, (const xmlChar *)AGGR)))
    {
      printf("%s\n",cur->children->content);
      strcpy(aggrArr[sz],cur->children->content);
      sz++;
    }
    cur = cur->next;
  }
  // createColumnFile(DIM_ATTR,PATH);
  FILE *fp;
  fp=fopen("dim_attr.txt","ab");
  // printf("\n%d\n",sz);
  // fseek(fp,0,SEEK_END);
  writeFile(fp,aggrArr,sz);
  fclose(fp);
  for (int i = 0; attrArray[i] != NULL; i++)
  {
    printf("  - %s\n", attrArray[i]);
  }
  // Free memory allocated by libxml2
  xmlFreeDoc(doc);
  xmlCleanupParser();
  for (int i = 0; i < 100; i++)
  {
    free(attrArray[i]);
  }
  free(attrArray);
  return 0;
}
