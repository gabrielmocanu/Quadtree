#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct pixel
{
	unsigned char red;		
	unsigned char green;
	unsigned char blue;
}__attribute__((packed)) pixel;

typedef struct QuadtreeNode {
  	pixel rgb;
  	uint32_t area;
  	int32_t top_left , top_right;
  	int32_t bottom_left , bottom_right;
}__attribute__((packed)) QuadtreeNode;

typedef struct file_description
{
	char type[3];
	uint32_t width;
	uint32_t height;
	uint32_t size_max;
}__attribute__((packed)) file_description;

typedef struct TreeNode
{
	pixel rgb;
	uint32_t area;
	struct TreeNode *top_left, *bottom_left, *bottom_right, *top_right;
}__attribute__((packed)) TreeNode;

void ReadDescripton(FILE *f,file_description *file); // functie pentru a citi tipul fisierului PPM si dimensiunile imaginii
void SetMatrix(FILE *f, pixel **a, uint32_t n); // functie pentru a citi matricea de pixeli
void MakeTree(TreeNode **tree, uint32_t lin, uint32_t col, pixel **rgb, uint32_t lenght, uint32_t *nr_nodes, uint32_t prag); // functie recursiva pentru a construi arborele
void MakeVector(TreeNode *tree, QuadtreeNode *v, uint32_t *position, uint32_t *nr_color); // functie recursiva pentru a construi vectorul de compresie
void MakeTreeVector(TreeNode **tree, QuadtreeNode *v, int32_t position); // functie recursiva pentru a construi arborele din vectorul de compresie
void MakeMatrixTree(TreeNode *tree, pixel **a, uint32_t lin, uint32_t col); // functie recursiva pentru a construi matricea de pixeli avand arborele de compresie
void SwapNode( TreeNode **tree1 , TreeNode **tree2); // functie de interschimbare a doua noduri din arbore
void SwapNodesOrizontal(TreeNode **tree); // functie de schimbare a tuturor nodurilor dintr-un arbore in cazul oglindirii orizontale
void SwapNodesVertical(TreeNode **tree); // functie de schimbare a tuturor nodurilor dintr-un arbore in cazul oglindirii verticale
void MakeTreeBonus( TreeNode **tree, TreeNode *tree1, TreeNode *tree2, uint32_t *nodes_bonus, uint32_t *r, uint32_t *g, uint32_t *b, uint32_t *choose); // functie pentru construirea arborelui imaginii suprapuse
TreeNode* FreeTree(TreeNode *tree); // functie pentru eliberarea memorie de arbore
void MakeMatrixMirror(TreeNode *tree, pixel **a, uint32_t lin, uint32_t col); // functie pentru construirea matricei de pixeli in cazul oglindirilor
