#include "interface.h"
#include "functions.h"

int main(int argc, char const *argv[])
{
	FILE *f;
	file_description file; // structura pentru a retine informatiile de la inceputului fisierului PPM
	pixel **pixels; // matrice de pixeli
	pixel **pixels2; // matrice de pixeli(in cazul bonusului)
	int i;
	uint32_t lin = 0,col = 0;
	uint32_t r = 0,g = 0,b = 0,choose = 0; // r,g,b variabile pentru a retine culorile pentru un bloc mai mare (in cazul bonusului) si choose pentru a retine care bloc e mai mare
	QuadtreeNode *v = NULL; // vectorul pentru compresie
	uint32_t prag; // pragul oferit pentru compresie
	uint32_t nr_nodes = 0; // nr noduri
	uint32_t nr_nodes1 = 0;
	uint32_t nr_nodes2 = 0;
	uint32_t nr_color = 0; // nr culori/frunze
	uint32_t position = 0; // variabila pentru a pune elementele in vector
	TreeNode *tree = NULL;
	TreeNode *tree1 = NULL;
	TreeNode *tree2 = NULL;
	
	// COMPRESIA
	if(!strcmp(argv[1], "-c"))
	{
		prag = atoi(argv[2]);
		f = fopen(argv[3],"rb");
		ReadDescription(f,&file); // Citim antetul fisierul PPM
		pixels = (pixel**)malloc(file.height*sizeof(pixel*)); // Alocam memorie pentru matricea de pixeli
		for (i=0;i<file.height;i++)
		{
			pixels[i] = (pixel*)malloc(file.height*sizeof(pixel));
		}	
		SetMatrix(f, pixels, file.height); // Citim din fisierul PPM si atribuim in matricea de pixeli
		fclose(f);
		MakeTree(&tree, lin, col, pixels, file.height, &nr_nodes, prag); // Construim arborele de compresie
		v = (QuadtreeNode*)malloc(nr_nodes*sizeof(QuadtreeNode)); // Alocam memorie pentru vectorul de compresie
		MakeVector(tree, v, &position, &nr_color); // Construim vectorul de compresie
		f = fopen(argv[4], "wb");
		fwrite(&nr_color, sizeof(uint32_t), 1, f); // Scrierea in fisier
		fwrite(&nr_nodes, sizeof(uint32_t), 1, f);
		fwrite(v, sizeof(QuadtreeNode), nr_nodes, f);
		fclose(f);
		for (i = 0; i < file.height; i++)
			free(pixels[i]);
		free(pixels);
		free(v);
		tree = FreeTree(tree);
	}

	// DECOMPRESIA
	if (!strcmp(argv[1], "-d"))
	{
		f = fopen(argv[2], "rb"); // Citirea din fisire si alocarea de memorie pentru vector
		fread(&nr_color, sizeof(uint32_t), 1, f); 
		fread(&nr_nodes, sizeof(uint32_t), 1, f);
		v = (QuadtreeNode*)malloc(nr_nodes*sizeof(QuadtreeNode));	
		fread(v, sizeof(QuadtreeNode), nr_nodes, f);
		fclose(f);
		pixels = (pixel**)malloc((int)sqrt(v[0].area)*sizeof(pixel*)); // Alocam memorie pentru matricea de pixeli
		for (i=0;i<(int)sqrt(v[0].area);i++)
		{
			pixels[i] = (pixel*)malloc((int)sqrt(v[0].area)*sizeof(pixel));
		}
		MakeTreeVector(&tree, v, position); // Construim arborele folosindu-ne de vectorul de compresie
		MakeMatrixTree(tree, pixels, lin, col); // Construim matricea de pixeli cu ajutorul arborelui
		f = fopen(argv[3], "wb"); // Scrierea antetului in fisierul PPM
		fprintf(f,"P6\n");
		fprintf(f,"%d",(int)sqrt(v[0].area));
		fprintf(f," ");
		fprintf(f,"%d",(int)sqrt(v[0].area));
		fprintf(f,"\n");
		fprintf(f,"255\n");
		for (i = 0; i < (int)sqrt(v[0].area); i++) // Scrierea matricei de pixeli
			fwrite(pixels[i], sizeof(pixel), (int)sqrt(v[0].area), f);
		fclose(f);
		for (i = 0; i < (int)sqrt(v[0].area); i++)
			free(pixels[i]);
		free(pixels);
		free(v);
		tree = FreeTree(tree);
	}

	// OGLINDIREA
	if (!strcmp(argv[1], "-m"))
	{
		if(!strcmp(argv[2], "h")) // ORIZONTALA
		{
			prag = atoi(argv[3]);
			f = fopen(argv[4],"rb");
			ReadDescription(f,&file); // Citim antetul fisierul PPM
			pixels = (pixel**)malloc(file.height*sizeof(pixel*)); // Alocam memorie pentru matricea de pixeli
			for (i=0;i<file.height;i++)
			{
				pixels[i] = (pixel*)malloc(file.height*sizeof(pixel));
			}	
			SetMatrix(f, pixels, file.height); // Citim din fisierul PPM si atribuim in matricea de pixeli
			fclose(f);

			MakeTree(&tree, lin, col, pixels, file.height, &nr_nodes, prag); // Construim arborele de compresie
			SwapNodesOrizontal(&tree); // Schimbam nodurile
			MakeMatrixMirror(tree, pixels, 0, 0); // Construim matricea de pixeli
			f = fopen(argv[5], "wb");
			fprintf(f,"P6\n");
			fprintf(f,"%d",file.height);
			fprintf(f," ");
			fprintf(f,"%d",file.height);
			fprintf(f,"\n");
			fprintf(f,"255\n");
			for (i = 0; i < file.height; i++)
				fwrite(pixels[i], sizeof(pixel), file.height, f);
			fclose(f);
			for (i = 0; i < file.height; i++)
				free(pixels[i]);
			free(pixels);
			tree = FreeTree(tree);
		}
		else // VERTICALA
		{
			prag = atoi(argv[3]);
			f = fopen(argv[4],"rb");
			ReadDescription(f,&file); // Citim antetul fisierul PPM
			pixels = (pixel**)malloc(file.height*sizeof(pixel*)); // Alocam memorie pentru matricea de pixeli
			for (i=0;i<file.height;i++)
			{
				pixels[i] = (pixel*)malloc(file.height*sizeof(pixel));
			}	
			SetMatrix(f, pixels, file.height);// Citim din fisierul PPM si atribuim in matricea de pixeli
			fclose(f);
			MakeTree(&tree, lin, col, pixels, file.height, &nr_nodes, prag); // Construim arborele de compresie
			SwapNodesVertical(&tree);  // Schimbam nodurile
			MakeMatrixMirror(tree, pixels, 0, 0); // Construim matricea de pixeli
			f = fopen(argv[5], "wb");
			fprintf(f,"P6\n");
			fprintf(f,"%d",file.height);
			fprintf(f," ");
			fprintf(f,"%d",file.height);
			fprintf(f,"\n");
			fprintf(f,"255\n");
			for (i = 0; i < file.height; i++)
				fwrite(pixels[i], sizeof(pixel), file.height, f);
			fclose(f);
			for (i = 0; i < file.height; i++)
				free(pixels[i]);
			free(pixels);
			tree = FreeTree(tree);
		}
	}

	// SUPRAPUNERE
	if(!strcmp(argv[1], "-o"))
	{
		prag = atoi(argv[2]);

		f = fopen(argv[3],"rb");	
		ReadDescription(f,&file); // Citim antetul fisierul PPM
		pixels = (pixel**)malloc(file.height*sizeof(pixel*));
		for (i=0;i<file.height;i++)
		{
			pixels[i] = (pixel*)malloc(file.height*sizeof(pixel));
		}	
		SetMatrix(f, pixels, file.height); // Citim din fisierul PPM si atribuim in matricea de pixeli
		fclose(f);
		MakeTree(&tree1, lin, col, pixels, file.height, &nr_nodes1, prag); // Construim arborele
		
		f = fopen(argv[4],"rb");
		ReadDescription(f,&file); // Citim antetul fisierul PPM
		pixels2 = (pixel**)malloc(file.height*sizeof(pixel*));
		for (i=0;i<file.height;i++)
		{
			pixels2[i] = (pixel*)malloc(file.height*sizeof(pixel));
		}	
		SetMatrix(f, pixels2, file.height); // Citim din fisierul PPM si atribuim in matricea de pixeli
		fclose(f);
		MakeTree(&tree2, lin, col, pixels2, file.height, &nr_nodes2, prag); // Construim arborele
		
		MakeTreeBonus(&tree,tree1,tree2,&nr_nodes,&r,&g,&b,&choose); // Construim arborele imaginii finale
		MakeMatrixMirror(tree, pixels, lin, col); // Construim matricea
		f = fopen(argv[5], "wb");
		fprintf(f,"P6\n");
		fprintf(f,"%d",file.height);
		fprintf(f," ");
		fprintf(f,"%d",file.height);
		fprintf(f,"\n");
		fprintf(f,"255\n");
		for (i = 0; i < file.height; i++)
			fwrite(pixels[i], sizeof(pixel), file.height, f);
		fclose(f);

		for (i = 0; i < file.height; i++)
			free(pixels[i]);
		free(pixels);
		for (i = 0; i < file.height; i++)
			free(pixels2[i]);
		free(pixels2);

		FreeTree(tree);
		FreeTree(tree2);
		FreeTree(tree1);
	}

	return 0;
}