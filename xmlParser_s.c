#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#define TABLENAME "tablename"
#define COLNAME "attributes"
#define TYPE "type"
#define NAME "name"
// void print_attribute_info(xmlNode* node) {
//   if (node == NULL || strcmp(node->name, ) != 0) {
//     return;  // Not an "attributes" element, skip
//   }

//   // Get the "type" attribute
//   xmlAttr* attr = node->properties;
//   while (attr != NULL && strcmp(attr->name, "type") != 0) {
//     attr = attr->next;
//   }

//   if (attr == NULL) {
//     printf("Error: Missing 'type' attribute for 'attributes' element\n");
//     return;
//   }

//   // Print type and child element name
//   printf("Type: %s Child %s\n", attr->children->content,node->children->content);

//   xmlNode* child = node->children;
//   while (child != NULL) {
//     if (child->type == XML_TEXT_NODE) {
//       // Skip text nodes
//     } else {
//       printf("  Child: %s\n", child->name);
//     }
//     child = child->next;
//   }
// }
void extract_table_info(xmlNode *root){
  xmlAttr *attr = NULL;
  xmlChar * tableName = root->name;
  printf("TableName: %s\n", root->name);
  for(attr = root->properties; attr; attr = attr->next){
    printf("j");
    printf("%s",attr->name);
    printf("%s",attr->children->content);
  }
  xmlNode *child=root->next;
  while(child!=NULL){
    // printf("%s->%s\n",table,child->name);
    child=child->next;
  }
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
  xmlNode* root_node = xmlDocGetRootElement(doc);

  // Process all child elements of the root
  printf("Root %s\n",root_node->name);//db name

  xmlNode* child = root_node->children;
  while (xmlStrcmp(child->name,(const xmlChar *) TABLENAME)) {
    printf("h");
    // print_attribute_info(child);
    extract_table_info(child);
    child = child->next;
  }

  // Free memory allocated by libxml2
  xmlFreeDoc(doc);
  xmlCleanupParser();

  return 0;
}
