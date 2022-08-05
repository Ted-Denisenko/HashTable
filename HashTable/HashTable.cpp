#define _CRT_SECURE_NO_WARNINGS

#define CAPACITY 4003
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//linked list
typedef struct LinkedList LinkedList;
struct LinkedList
{
	HT_item* item;
	LinkedList* next;
};

LinkedList* CreateListElem()
{
	LinkedList* list = new LinkedList;
	return list;
}

LinkedList* LinkedList_insert(LinkedList* list, HT_item* item)
{
	// если список пуст
	if (!list)
	{
		LinkedList* head = CreateListElem();
		head->item = item;
		head->next = NULL;
		list = head;
		return list;
	}
	// если в списке один элемент
	else if (list->next == NULL)
	{
		LinkedList* node;
		node->item = item;
		node->next = NULL;
		list->next = node;
		return list;
	}

	// если в списке хотя бы два элемента
	LinkedList* tmp = list;
	while (tmp->next->next)
		tmp = tmp->next;
	LinkedList* node = CreateListElem();
	node->item = item;
	node->next = NULL;
	tmp->next = node;

	return list;
}

HT_item* LinkedList_remove(LinkedList* list, HT_item item)
{
	// если список пуст или состоит из одного элемента
	if (!list || !list->next)
		return NULL;
	LinkedList* node = list->next;
	LinkedList* tmp = list;
	tmp->next = NULL;
	HT_item* hashTableitem = NULL;
	memcpy(tmp->item, hashTableitem, sizeof(HT_item));
	
	// because of malloc
	free(tmp->item->key);
	free(tmp->item->value);
	free(tmp->item); //heap allocated??
	// because of new
	delete tmp;
}

void FreeLinkedList(LinkedList* list)
{
	while (list)
	{
		auto tmp = list;
		list = list->next;
		free(tmp->item->key);
		free(tmp->item->value);
		free(tmp->item);
		free(tmp);
	}
}

LinkedList** CreateOverflowBuckets(HashTable* table)
{
	LinkedList** buckets = new LinkedList * [table->size];
	for (int i = 0; i < table->size; i++)
	{
		buckets[i] = NULL;
	}
	return buckets;
}

void FreeOverflowBuckets(HashTable* table)
{
	auto buckets = table->overflowBucket;
	for (int i = 0; i < table->size; i++)
	{
		FreeLinkedList(buckets[i]);
	}
	delete[] buckets;
}

typedef struct HT_item HT_item;
struct HT_item
{
	char* key;
	char* value;
};

typedef struct HashTable HashTable;
struct HashTable
{
	HT_item** items;
	LinkedList** overflowBucket;
	int size;
	int count;
};

// вычисляем индекс
unsigned long HashFunction(char* str)
{
	unsigned long i = 0;
	for (int j = 0; str[j]; j++)
		i += str[j];
	return i % CAPACITY;
}

// create item with key-value pair in HT
HT_item* CreateItem(char* key, char* value)
{
	HT_item* item = new HT_item;

	// TODO: стоит ли менять на 'new'?
	item->key = (char*)malloc(strlen(key) + 1);
	item->value = (char*)malloc(strlen(value) + 1);

	strcpy(item->key, key);
	strcpy(item->value, value);

	return item;
}

HashTable* CreateTable(int size)
{
	HashTable* table = new HashTable;
	table->size = size;
	table->count = 0;
	table->items = new HT_item*[table->size] {0};

	for (int i = 0; i < table->size; i++)
		table->items[i] = NULL;

	table->overflowBucket = CreateOverflowBuckets(table);

	return table;
}

void FreeItem(HT_item* item)
{
	// потому что malloc
	free(item->key);
	free(item->value);

	// потому что new
	delete item;
}

void FreeTable(HashTable* table)
{
	for (int i = 0; i < table->size; i++)
	{
		HT_item* item = table->items[i];
		if (item != NULL)
			FreeItem(item);
	}

	FreeOverflowBuckets(table);
	delete table->items;
	delete table;
}

void HandleCollision(HashTable* table, unsigned long index, HT_item* item)
{
	LinkedList* head = table->overflowBucket[index];

	// if list doesn't exist yet
	if (head = NULL)
	{
		head = CreateListElem();
		head->item = item;
		table->overflowBucket[index] = head;
		return;
	}
	// if list already exists
	else
	{
		table->overflowBucket[index] = LinkedList_insert(head, item);
		return;
	}
}

void HT_insert(HashTable* table, char* key, char* value)
{
	if (table->count == table->size)
	{
		std::cout << "Insert error: Hash Table is full" << std::endl;
		return;
	}

	HT_item* item = CreateItem(key, value);
	int index = HashFunction(key);

	HT_item* current_item = table->items[index];
	if (current_item == NULL)
	{
		table->items[index] = item;
		table->count++;
	}
	else
	{
		// case 1: update existing value
		// TODO: нужно ли использовать СИ-шный strcmp для сравнения значения ключей и значений?
		if (strcmp(current_item->key, key))
		{
			strcpy(table->items[index]->value, value);
			return;
		}

		// case 2: COLLISION!!!
		else
		{
			HandleCollision(table, current_item);
			// например, создать массив и записывать в него пары ключ-значение(ключи потом можно отсортировать)
			return;
		}
	}
}

void HandleCollision(HashTable* table, HT_item* item)
{
	return;
}

char* HT_search(HashTable* table, char* key)
{
	int index = HashFunction(key);
	HT_item* item = table->items[index];
	if (item != NULL)
	{
		if (strcmp(item->key, key) == 0)
			return item->value;
	}
	return NULL;
}

void printSearch(HashTable* table, char* key)
{
	char* value;
	if ((value = HT_search(table, key)) == NULL)
	{
		std::cout << "Key '" << key <<"' doesn't exist" << std::endl;
		return;
	}
	std::cout << "Key: " << key << "\t Value: " << value << std::endl;
	return;
}

void printTable(HashTable* table)
{
	std::cout << "\t-------- Hash Table --------" << std::endl;
	for (int i = 0; i < table->size; i++)
	{
		if (table->items[i])
			std::cout << "Index: " << i << "\tKey: " << table->items[i]->key << "\t\tValue: " << table->items[i]->value << std::endl;
	}
	std::cout << "\t-------- End of Table --------" << std::endl;
}

int main()
{
	HashTable* ht = CreateTable(CAPACITY);
	HT_insert(ht, const_cast < char*>("1"), const_cast < char*>("First address"));
	HT_insert(ht, const_cast < char*>("2"), const_cast < char*>("Second address"));
	printSearch(ht, const_cast < char*>("1"));
	printSearch(ht, const_cast < char*>("2"));
	printSearch(ht, const_cast < char*>("3"));
	printTable(ht);
	FreeTable(ht);
	return 0;
}
