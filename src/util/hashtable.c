#include <util/hashtable.h>
#include <util/util.h>
#include <stdio.h>
#include <string.h>
#define NUM_SIZES 13
#define HIGH_WATER_MARK 0.75
#define LOW_WATER_MARK 0.25

static unsigned int sizes[] =
{
	11,
	23,
	53,
	101,
	211,
	433,
	863,
	1723,
	3463,
	6947,
	14831,
	29663,
	59447
};

typedef struct list_s
{
	void* key;
	void* value;
	struct list_s* next;
} list_t;

struct hashtable_s
{
	list_t** table;
	int table_size_index;
	int num_keys;
	int (*cmp_func)(void*, void*);
	unsigned int (*hash_func)(void*);
};

hashtable* hashtable_create(int (*cmp)(void*, void*), unsigned int (*hash)(void*))
{
	hashtable* ht = (hashtable*) checked_malloc(sizeof(hashtable));
	ht->cmp_func = cmp;
	ht->hash_func = hash;
	ht->table = (list_t**) checked_malloc(sizes[0] * sizeof(list_t*));

	ht->table_size_index = 0;

	ht->num_keys = 0;
	return ht;
}

static list_t* make_entry(void* key,  void* value, list_t* next)
{
	list_t* entry;
	entry = (list_t*) checked_malloc(sizeof(list_t));
	entry->key = key;
	entry->value = value;
	entry->next = next;
	return entry;
}

static unsigned int hash(hashtable* ht, void* str)
{
	unsigned int hashval = (*(ht->hash_func))(str);
	return hashval % sizes[ht->table_size_index];
}

static list_t* find_key(hashtable* ht, void* key)
{
	unsigned int hv = hash(ht, key);
	list_t* entry = ht->table[hv];

	while (entry!= NULL)
	{
		if ((*(ht->cmp_func))(entry->key, key) == 0)
		{
			return entry;
		}
		entry = entry->next;
	}

	return NULL;
}

static void add_pair(hashtable* ht, void* key, void* new_val)
{
	unsigned int hv = hash(ht, key);
	
	list_t* head = ht->table[hv];
	ht->table[hv] = make_entry(key, new_val, head);
}

static void* insert_key(hashtable* ht,  void* key,  void* new_val)
{
	list_t* entry = find_key(ht, key);

	if (entry != NULL)
	{
		void* old_val = entry->value;
		entry->value = new_val;
		return old_val;
	}
	add_pair(ht, key, new_val);
	return NULL;
}

static void change_table_size(hashtable* ht, int new_size_index)
{
	if (new_size_index == NUM_SIZES - 1)
	{
		fprintf(stderr, "hash table is too big.\n");
		exit(1);
	}

	list_t** old_tab = ht->table;

	int old_size = sizes[ht->table_size_index];

	ht->table_size_index = new_size_index;
	int new_size = sizes[ht->table_size_index];
	list_t** new_tab = (list_t**) checked_malloc(new_size * sizeof(list_t*));
	
	ht->table = new_tab;
	
	//copy everything over
	for (int i = 0; i < old_size; i++)
	{
		list_t* entry = old_tab[i];

		while(entry != NULL)
		{
			list_t* next = entry->next;
			int hv = hash(ht, entry->key);
			list_t* e_old = new_tab[hv];
			entry->next = e_old;
			new_tab[hv] = entry;
			entry = next;
		}
	}

	checked_free(old_tab);
}

void* hashtable_put(hashtable* ht,  void* key,  void* value)
{
	void* old = insert_key(ht, key, value);

	if (old == NULL)
	{
		ht->num_keys++;

		if ((double) ht->num_keys / (double) sizes[ht->table_size_index] > HIGH_WATER_MARK)
		{
			change_table_size(ht, ht->table_size_index + 1);
		}
	}

	return old;
}

void* hashtable_get(hashtable* ht,  void* key)
{
	list_t* entry = find_key(ht, key);

	if (entry != NULL) return entry->value;

	return NULL;
}

int hashtable_contains_key(hashtable* ht,  void* key)
{
	return hashtable_get(ht, key) != NULL;
}

void* hashtable_remove(hashtable* ht,  void* key)
{
	unsigned int hv = hash(ht, key);
	list_t* entry = ht->table[hv];
	void* rval = NULL;

	while (entry != NULL)
	{
		list_t* prev = NULL;
		
		if ((*(ht->cmp_func))(entry->key, key) == 0)
		{
			rval = entry->value;

			if (prev == NULL)    //this is the only thing in the list so just free it
			{
				ht->table[hv] = NULL;
			}
			else
			{
				//delete in the middle of the list
				prev->next = entry->next;
			}
			checked_free(entry);
			ht->num_keys--;
			if ((double) ht->num_keys / (double) sizes[ht->table_size_index] < LOW_WATER_MARK)
			{
				change_table_size(ht, ht->table_size_index - 1);
			}
			break;
		}

		prev = entry;
		entry = entry->next;
	}

	return rval;
}
void hashtable_destroy(hashtable* ht)
{
	int size = sizes[ht->table_size_index];
	
	for (int i = 0; i < size; i++)
	{
		list_t* entry = ht->table[i];
		
		while(entry != NULL)
		{
			list_t* next = entry->next;
			checked_free(entry);
			entry = next;
		}
	}
	checked_free(ht->table);
	checked_free(ht);
}
void hashtable_foreach(hashtable* ht, void (*cb) (key_value_pair*, void*), void* state)
{
	int size = sizes[ht->table_size_index];
	key_value_pair kvp;
	for (int i = 0; i < size; i++)
	{
		list_t* entry = ht->table[i];
		
		while(entry != NULL)
		{
			list_t* next = entry->next;
			
			kvp.key = entry->key;
			kvp.value = entry->value;
			(*cb)(&kvp, state);
			entry = next;
		}
	}
}

unsigned int string_hash(void* data)
{
	char* str = (char*)data;
	unsigned int hashval = 0;
	for (; *str != '\0'; str++)
	{
		hashval = *str + 31 * hashval;
	}
	return hashval;
}

int string_cmp(void* d1, void* d2)
{
	char* s1 = (char*) d1;
	char* s2 = (char*) d2;
	return strcmp(s1, s2);
}