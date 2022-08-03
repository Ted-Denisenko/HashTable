#define CAPACITY 20000
#include <iostream>
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

	// TODO: ����� �� ������ �� 'new'?
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
	// ������ ��� malloc
	free(item->key);
	free(item->value);

	// ������ ��� new
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

void HT_insert(HashTable* table, char* key, char* value)
{
	if (table->count == table->size)
	{
		std::cout << "Insert error: Hash Table is full" << std::endl;
		return;
	}

	HT_item* item = createItem(key, value);
	int index = hashFunction(key);

	HT_item* current_item = table->items[index];
	if (current_item == NULL)
	{
		table->items[index] = item;
		table->count++;
	}
	else
	{
		// case 1: update existing value
		// TODO: ����� �� ������������ ��-���� strcmp ��� ��������� �������� ������ � ��������?
		if (strcmp(current_item->key, key))
		{
			strcpy(table->items[index]->value, value);
			return;
		}

		// case 2: COLLISION!!!
		else
		{
			handleCollision(table, current_item);
			// ��������, ������� ������ � ���������� � ���� ���� ����-��������(����� ����� ����� �������������)
			return;
		}
	}
}

void handleCollision(HashTable* table, HT_item* item) 
{

}

char* HT_search(HashTable* table, char* key)
{
	int index = hashFunction(key);
	HT_item* item = table->items[index];
	if (item != NULL)
	{
		if (strcmp(item->key, key) == 0)
			return item->value;
	}
	return NULL;
}

int main()
{
	std::string str = "booba";
	return 0;
}
