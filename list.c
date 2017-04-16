#include "list.h"

struct _list
{
  int size;
  huffTree_t *head;
  huffTree_t *tail;
};

list_t* createList()
{
  list_t *temp = (list_t*) malloc(1 * sizeof(list_t));
  temp->size = 0;
  temp->head = NULL;
  temp->tail = NULL;
  return(temp);
}

list_t* createListFromArray(long long int array[])
{
  list_t *list = createList();
  int i;
  for (i = 0; i < 256; i ++)
  {
    if (array[i] != 0)
    {
      huffTree_t *temp;
      temp = createNode(i, array[i]);

      addNode(list, temp);
    }
  }
  return(list);
}

void sortList(list_t *list)
{
  if (list->size <= 1) return;

  huffTree_t *o = list->head, *b;
  while (o != NULL)
  {
    b = getNext(o);
    while (b != NULL)
    {
      if (isMoreFrequent(o, b))
        swap(o, b);

      b = getNext(b);
    }
    o = getNext(o);
  }
}

void addNode(list_t *list, void *newNode /*huffTree_t*/)
{
  if (list->size == 0)
  {
    list->head = newNode;
    list->tail = newNode;
  }
  else
  {
    setNext(list->tail, newNode);
    list->tail = newNode;
  }
  list->size ++;
}

void addTreeSorted(list_t *list, void *newTree /*huffTree_t*/)
{
  huffTree_t *temp = (huffTree_t*) newTree;
  if (list->size == 0)
  {
    list->head = temp; list->tail = temp;
  }
  else
  {
    huffTree_t *curr = list->head, *prev = NULL;
    while (getNext(curr) != NULL && getFrequency(curr) < getFrequency(temp))
    {
      prev = curr;
      curr = getNext(curr);
    }
    if (getFrequency(curr) < getFrequency(temp))
    {
      setNext(curr, temp);
      list->tail = temp;
    }
    else if (prev != NULL)
    {
      setNext(temp, getNext(prev)); //temp->next = prev->next;
      setNext(prev, temp); //prev->next = temp;
    }
    else
    {
      setNext(temp, curr); //temp->next = curr;
      list->head = temp;
    }
  }
  list->size ++;
}

void* createTreeFromList(list_t *list)
{
  huffTree_t *curr = list->head;
  do
  {
    int somaDeFrequencias = getFrequency(curr) + getFrequency(getNext(curr));

    list->head = getNext(getNext(curr));

    huffTree_t *newTree = createTree('*', somaDeFrequencias, curr, getNext(curr));

    addTreeSorted(list, newTree);

    curr = list->head;
    list->size -= 2;
  } while (list->size > 1);
  list->head = list->tail;

  return(list->head);
}

void printList(list_t *list)
{
  huffTree_t *curr = list->head;
  while (!isHuffTreeEmpty(curr))
  {
    printNode(curr);
    curr = getNext(curr);
  }
}

void destroyList(list_t *list)
{
  huffTree_t *curr = list->head, *toRemove;
  while (list->size > 1)
  {
    toRemove = curr;
    curr = getNext(curr);
    free(toRemove);
    list->size --;
  }
  free(list);
}