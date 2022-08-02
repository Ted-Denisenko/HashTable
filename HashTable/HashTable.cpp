#define CAPACITY 20000
#include <stdio.h>
#include <string>

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
	int size;
	int count;
};

unsigned long hashFunction(char* str)
{
	unsigned long i = 0;
	for (int j = 0; str[j]; j++)
		i += str[j];
	return i % CAPACITY;
}

// create item with key-value pair in HT
HT_item* createItem(char* key, char* value)
{
	HT_item* item = new HT_item;

	// TODO: стоит ли менять на 'new'?
	item->key = (char*)malloc(strlen(key) + 1);
	item->value = (char*)malloc(strlen(value) + 1);

	strcpy(item->key, key);
	strcpy(item->value, value);

	return item;
}

HashTable* create_table(int size)
{
	HashTable* table = new HashTable;
	table->size = size;
	table->count = 0;
	table->items = new HT_item*;

	for (int i; i < table->size; i++)
		table->items[i] = NULL;

	return table;
}

void freeItem(HT_item* item)
{
	// потому что malloc
	free(item->key);
	free(item->value);

	// потому что new
	delete item;
}

void freeTable(HashTable* table)
{
	for (int i; i < table->size; i++)
	{
		HT_item* item = table->items[i];
		if (item != NULL)
			freeItem(item);
	}

	delete table->items;
	delete table;
}

int main()
{
	std::string str = "booba";
	return 0;
}
