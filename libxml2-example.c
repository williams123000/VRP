/**
 * sección: Tree
 * sinopsis: Navegar el árbol para imprimir los nombres de los elementos
 * propósito: Convertir el archivo xml en un árbol, usar xmlDocGetRootElement() para obtener el elemento raíz, 
 *  luego recorrer el documento e imprimir todos los nombres de los elementos en el orden en que aparecen en el
 *  documento.
 * uso: tree1 filename_or_URL
 * test: tree1 test2.xml > tree1.tmp ; diff tree1.tmp tree1.res ; rm tree1.tmp
 * autor: Dodji Seketeli
 * copia: vea la sección Copyright para más información acerca de este software.
 */
#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
 
/*Hace compilació condicional. Si libxml2 esta correctamente instalada y configurada*/
#ifdef LIBXML_TREE_ENABLED
 
/*
 *Para compilar este archivo usando gcc tipee 
 *gcc `xml2-config --cflags --libs` -o tree1 libxml2-example.c
 *Tenga en cuenta que la librería debe estar correctamente instalada y que el archivo se debe
 *llamar libxml2-example.c
 */
 
/**
 * print_element_names:
 * @a_node: el nodo inicial a conciderar.
 *
 * Imprime los nombres de todos los elementos
 * que son hermanos o hijos del nodo xml.
 */
static void print_element_names(xmlNode * a_node)
{
    xmlNode *cur_node = NULL;
 
    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            printf("node type: Element, name: %s\n", cur_node->name);
        }
 
        print_element_names(cur_node->children);
    }
}
 
 
/**
 * Ejemplo simple para parsear un archivo xml recibido como parámetro de main,
 * recorrer el DOM (http://es.wikipedia.org/wiki/DOM), e imprimir el nombre de  
 * los elementos de los nodos.
 */
int main(int argc, char **argv)
{
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;
 
    if (argc != 2)
        return(1);
 
    /*
     * esto inicializa la biblioteca y chequea por pontenciales fallas en la ABI
     * (http://en.wikipedia.org/wiki/Application_binary_interface)
     * entre la versión que ha sido compilada y la que se esta compartiendo actualmente.
     */
    LIBXML_TEST_VERSION
 
    /*parsea el archivo y obtiene el DOM */
    doc = xmlReadFile(argv[1], NULL, 0);
 
    if (doc == NULL) {
        printf("error: could not parse file %s\n", argv[1]);
    }
 
    /*Obtiene el nodo raíz*/
    root_element = xmlDocGetRootElement(doc);
 
    print_element_names(root_element);
 
    /*Libera la memoria usada para almacenar el árbol que representa el DOM*/
    xmlFreeDoc(doc);
 
    /*
     *Libera las variables globales que pudieron haber sido alocadas por el parser.
     */
    xmlCleanupParser();
 
    return 0;
}
/*
 * ELSE de la compilación codicional. Si no esta correctamente configurada la biblioteca, se creará un programa que  
 * imprime un mensaje de error, diciendo que la biblioteca no esta disponible. 
 * Por eso parece que hay dos main(), pero sólo uno es compilado realmente.
 */
#else
int main(void) {
    fprintf(stderr, "Tree support not compiled in\n");
    exit(1);
}
#endif