#define CAPACITY 20000
#include <stdio.h>
#include <string>

typedef struct HT_item HT_item;

struct HT_item
{
	char* key;
	char* value;
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
	HT_item* item = (HT_item*)malloc(sizeof(HT_item));
	item->key = (char*)malloc(strlen(key) + 1);
	item->value = (char*)malloc(strlen(value) + 1);

	strcpy(item->key, key);
	strcpy(item->value, value);

	return item;
}



int main()
{
	std::string str = "booba";
	return 0;
}
