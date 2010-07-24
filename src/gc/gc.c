#include <gc/gc.h>
#include <util/util.h>
#include <stdio.h>

typedef struct
{
	int type;
	size_t size;
	IteratePointers ip;
	Free fr;
	char* name;
} type_definition;


//10 megs
#define MAX_MEM_USE (1024*1024*100)
#define MAX_SKIPS (1000)

typedef struct mem_list_s
{
	void* location;
	struct mem_list_s* next;
} mem_list;

typedef struct root_list_s
{
	void** root;
	struct root_list_s* next;
} root_list;

struct garbage_collector_s
{
	arraylist* type_list;
	int current_mark;
	mem_list* active_list;
	root_list* roots;
	size_t total_alloced_mem;
	int allocs_since_last_collect;
	unsigned int num_roots;
	unsigned int num_active_objs;
};

static inline char is_marked(garbage_collector* gc, gc_header* entry)
{
	return entry->_mark == gc->current_mark;
}
static inline void mark_entry(garbage_collector* gc, gc_header* entry)
{
	entry->_mark = gc->current_mark ;
}
static inline void unmark_entry(garbage_collector* gc, gc_header* entry)
{
	entry->_mark = (gc->current_mark == 0) ? 1 : 0;
}
static inline void swap_marks(garbage_collector* gc)
{
	gc->current_mark = (gc->current_mark == 1) ? 0 : 1;
}
static garbage_collector* gc_get()
{
	static garbage_collector* gc = NULL;
	if(gc == NULL)
	{
		gc = (garbage_collector*) checked_malloc(sizeof(garbage_collector));
		gc->total_alloced_mem = 0;
		gc->current_mark = 0;
		gc->active_list = NULL;
		gc->roots = NULL;
		gc->num_active_objs = 0;
		gc->num_roots = 0;
		gc->type_list = arraylist_create();
		gc->allocs_since_last_collect = 0;
	}
	return gc;
}

unsigned int gc_register_type(char* name, size_t sz, IteratePointers ip, Free fr)
{
	garbage_collector* gc =  gc_get();
	type_definition* td = checked_malloc(sizeof(type_definition));
	td->type = arraylist_size(gc->type_list);
	td->ip = ip;
	td->fr = fr;
	td->size = sz;
	td->name = name;
	arraylist_add(gc->type_list, td);
	return td->type;
}

static inline type_definition* get_type(garbage_collector* gc, unsigned int type)
{
	return (type_definition*) (arraylist_get(gc->type_list, type));
}

void gc_info()
{
	garbage_collector* gc =  gc_get();
	printf("GC INFO:\n");
	printf("\tRoots: %d\n", gc->num_roots);
	root_list* cur = gc->roots;
	while(cur != NULL)
	{
		gc_header* header = (gc_header*) *(cur->root);
		if(header == NULL)
		{
			printf("\t\tNULL\n");
		}
		else
		{
			type_definition *td = get_type(gc, header->_type);
			printf("\t\t%s\n", td->name);
		}
		cur = cur->next;
	}
	printf("\tActive objects: %d\n", gc->num_active_objs);
	printf("\tMemory use: %d\n", gc->total_alloced_mem);
}

static void collect(garbage_collector* gc);

void* gc_alloc(unsigned int type)
{
	garbage_collector* gc =  gc_get();
	size_t sz = get_type(gc, type)->size;
	
	if(gc->total_alloced_mem + sz > MAX_MEM_USE || gc->allocs_since_last_collect > MAX_SKIPS)
	{
		gc->allocs_since_last_collect = 0;
		collect(gc);
	}
	gc->allocs_since_last_collect++;
	gc_header* entry = (gc_header*) checked_malloc(sz);
	gc->total_alloced_mem += sz;
	unmark_entry(gc, entry);
	entry->_type = type;

	mem_list* mem_entry = (mem_list*) checked_malloc(sizeof(mem_list));
	mem_entry->location = entry;
	mem_entry->next = gc->active_list;
	gc->active_list = mem_entry;

	gc->num_active_objs++;
	
	return entry;
}

void gc_push_root(void** root)
{
	garbage_collector* gc =  gc_get();
	root_list* rl = (root_list*) checked_malloc(sizeof(root_list));
	rl->root = root;
	rl->next = gc->roots;
	gc->roots = rl;
	gc->num_roots++;
}
void gc_pop_root()
{
	garbage_collector* gc =  gc_get();
	root_list* top = gc->roots;
	gc->roots = top->next;
	checked_free(top);
	gc->num_roots--;
}
static void mark(garbage_collector* gc, void* root);
static void mark_helper(void* ptr, void* state)
{
	garbage_collector* gc = state;
	mark(gc, ptr);
}

static void mark(garbage_collector* gc, void* root)
{
// 	debug(GC, "Marking...\n");
	gc_header* entry = (gc_header*) root;
	if(root == NULL) return;
	if(is_marked(gc, entry)) return;

	mark_entry(gc, entry);
	type_definition* td = get_type(gc, entry->_type);
	IteratePointers ip = td->ip;
	if(ip != NULL)
	{
		(*ip)(root, &mark_helper, gc);
	}
}

static void sweep(garbage_collector* gc)
{
	debug(GC, "Sweep Started\n");
	size_t start_mem = gc->total_alloced_mem;
	unsigned int start_objs = gc->num_active_objs;
	mem_list* le = gc->active_list;
	mem_list* new_list = NULL;
	while(le!=NULL)
	{
		gc_header* entry = (gc_header*) le->location;
		
		mem_list* next = le->next;
		if(!is_marked(gc, entry))
		{
			type_definition* td = get_type(gc, entry->_type);
			Free fr = td->fr;
			if(fr != NULL)
			{
				(*fr)(le->location);//run the finalizer
			}
// 			debug(GC, "Freeing mem\n");
			checked_free(le->location);
			checked_free(le);
			gc->total_alloced_mem -= td->size;
			gc->num_active_objs--;
		}
		else
		{
			le->next = new_list;
			new_list = le;
		}
		
		le = next;
	}
	gc->active_list = new_list;
	swap_marks(gc);
	debug(GC, "memory freed:( %d bytes, %d objs)\n", (start_mem - gc->total_alloced_mem), (start_objs - gc->num_active_objs));
}

static void collect(garbage_collector* gc)
{
	debug(GC, "Collect Started: (%d roots, %d objs)\n", gc->num_roots, gc->num_active_objs);
	root_list* cur = gc->roots;
	while(cur != NULL)
	{
		mark(gc, *(cur->root));
		cur = cur->next;
	}
	sweep(gc);
}
