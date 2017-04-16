#include "decompressFile.h"

int* getHeader(FILE *pFile)
{
  int *header = (int*) malloc(2 * sizeof(int));
  unsigned char byte;

  int trashSize = 0, treeSize = 0;
  fscanf(pFile, "%c", &byte);
  trashSize = byte >> 5;
  header[0] = trashSize;

  unsigned char mid = (byte << 3);
  treeSize = mid >> 3;
  fscanf(pFile, "%c", &byte);
  treeSize = (treeSize << 8) | byte;
  header[1] = treeSize;

  //  printByte(trashSize, 8); printf("\n");
  //  printByte(treeSize, 8); printf("\n");
  //  printf("Trash Size: %d || Tree Size: %d\n", trashSize, treeSize);

  return(header);
}

void decompress(char pathFile[])
{
  FILE *pFile = fopen(pathFile, "rb");
  if (pFile == NULL)
  {
    printf("We weren't able to open the file\n");
    return;
  }

  long long int *progressBar = createProgressBar(pFile);

  char *decompressedFileName = createDecompressedFileName(pathFile);
  FILE *newFile = fopen(decompressedFileName, "wb");

  int *header = getHeader(pFile);
  int trashSize = header[0];
  int treeSize = header[1];

  //printf("Decompressed Tree:\n");
  int i = 0;
  huffTree_t *decompressedTree = NULL; createTreeFromPreFix(pFile, &decompressedTree, treeSize, &i);
  //printf("PreOrder: "); printTreePreOrder(decompressedTree); printf("\n");
  //printf("InOrder: "); printTreeInOrder(decompressedTree); printf("\n");
  //printf("PosOrder: "); printTreePosOrder(decompressedTree); printf("\n");
  //printf("Tree Height %d\n", height(decompressedTree));

  decompressBytes(pFile, newFile, decompressedTree, trashSize, progressBar);
  printf("Decompressing File.... Done\n");

  free(progressBar);
  free(decompressedFileName);
  free(header);
  destroyHuffTree(decompressedTree);
  fclose(pFile);
  fclose(newFile);
}

void multipleDecompress(char quantityString[], char pathFile[])
{
  int quantity = atoi(quantityString);

  if (quantity <= 0)
    printf("Invalid quantity\n");
  else
  {
    int i;
    decompress(pathFile);
    fixExtension2(pathFile);
    printf("%s\n", pathFile);
    printf("Decompressed once\n\n");

    for (i = 0; i < quantity - 1; i ++)
    {
      decompress(pathFile);
      if (i < quantity - 2)
        fixExtension2(pathFile);

      if (i == 0)
        printf("Decompressed twice\n\n");
      else
        printf("Decompressed %d times\n\n", i + 2);
    }

    printf("You've decompressed %d times\n", quantity);
  }
}

char* createDecompressedFileName(char pathFile[])
{
  int endOfName = strlen(pathFile);
  int i, j;
  for (i = endOfName; i >= 0 && pathFile[i] != '\\';)
    i --;

  char *newFileName = (char*) malloc(100 * sizeof(char)); i ++;
  for (j = 0; i < endOfName - 5; j ++, i ++)
    newFileName[j] = pathFile[i];

  newFileName[j] = '\0';

  return(newFileName);
}

void fixExtension2(char pathFile[])
{
  char *originalName = (char*) malloc(100 * sizeof(char));
  strcpy(originalName, pathFile);
  printf("Ori: %s\n", originalName);

  int endOfName = strlen(pathFile);
  int i = endOfName, j;
  j = 5; char dotHuff[6] = {'.', 'h', 'u', 'f', 'f', '\0'};
  while (i >= 0 && pathFile[i] == dotHuff[j])
  {
    i --; j --;
    if (j == -1)
      j = 4;
  }

  pathFile[i + 1] = '\0';
  printf("Path: %s\n", pathFile);

  if (strcmp(originalName, pathFile) != 0)
    remove(originalName);

  if (rename(pathFile, originalName))
    printf("Success\n");

  strcpy(pathFile, originalName);
}

/*int endOfName = strlen(pathFile);
int i = endOfName, j;
j = 5; char dotHuff[6] = {'.', 'h', 'u', 'f', 'f', '\0'};
while (i >= 0 && pathFile[i] == dotHuff[j])
{
  i --; j --;
  if (j == -1)
  j = 4;
}
char *originalName = (char*) malloc(100 * sizeof(char));
strcpy(originalName, pathFile);
//remove(pathFile);
strcat(pathFile, ".huff");
rename(originalName, pathFile);*/