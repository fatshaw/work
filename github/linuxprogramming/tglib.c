/*
 * Some examples of using glib.
 *
Makefile:
tglib:tglib.c
	gcc -o tglib -g tglib.c -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lrt -lglib-2.0
 */

#include <glib.h>
#include <glib-object.h>
#include <gobject/gvaluecollector.h>
#include <string.h>

typedef struct {
        gchar *name;
} ShareInfo_t;

//---------Only for debug-----------------
// The following definition is copied from glib internal code.
typedef struct _GHashNode GHashNode;

struct _GHashTable
{
  gint             size;
  gint             mod;
  guint            mask;
  gint             nnodes;
  gint             noccupied;  /* nnodes + tombstones */
  GHashNode       *nodes;
  GHashFunc        hash_func;
  GEqualFunc       key_equal_func;
  volatile gint    ref_count;
#ifndef G_DISABLE_ASSERT
  /*
   * Tracks the structure of the hash table, not its contents: is only
   * incremented when a node is added or removed (is not incremented
   * when the key or data of a node is modified).
   */
  int              version;
#endif
  GDestroyNotify   key_destroy_func;
  GDestroyNotify   value_destroy_func;
};
//---------Only for debug-----------------

//---------For simplicity: don't have to integrate libsoup--------------------
// the following macros and function definitions are copied from libsoup.
// So you don't have to copy them when you already have libsoup.
#define SOUP_VALUE_SETV(val, type, args)                                \
G_STMT_START {                                                          \
        char *error = NULL;                                             \
                                                                        \
        memset (val, 0, sizeof (GValue));                               \
        g_value_init (val, type);                                       \
        G_VALUE_COLLECT (val, args, G_VALUE_NOCOPY_CONTENTS, &error);   \
        if (error)                                                      \
                g_free (error);                                         \
} G_STMT_END

#define SOUP_VALUE_GETV(val, type, args)                                \
G_STMT_START {                                                          \
        char *error = NULL;                                             \
                                                                        \
        G_VALUE_LCOPY (val, args, G_VALUE_NOCOPY_CONTENTS, &error);     \
        if (error)                                                      \
                g_free (error);                                         \
} G_STMT_END

static void
soup_value_hash_value_free(gpointer val)
{
	g_print("To deallocate %p\n", val);
	g_value_unset(val);
	g_free(val);
}

GHashTable *
soup_value_hash_new(void)
{
	return (g_hash_table_new_full(g_str_hash, g_str_equal, g_free, soup_value_hash_value_free));
}

void
soup_value_hash_insert_value (GHashTable *hash, const char *key, GValue *value)
{
        GValue *copy = g_new0 (GValue, 1);
	gchar *gkey = g_strdup(key);

	g_print("allocated copy=%p, gkey=%p\n", copy, gkey);
        g_value_init (copy, G_VALUE_TYPE (value));
        g_value_copy (value, copy);
        g_hash_table_insert (hash, gkey, copy);
}

void
soup_value_hash_insert (GHashTable *hash, const char *key, GType type, ...)
{
        va_list args;
        GValue val;

        va_start (args, type);
        SOUP_VALUE_SETV (&val, type, args);
        va_end (args);
        soup_value_hash_insert_value (hash, key, &val);
}

void
soup_value_array_append (GValueArray *array, GType type, ...)
{
        va_list args;
        GValue val;

        va_start (args, type);
        SOUP_VALUE_SETV (&val, type, args);
        va_end (args);
        g_value_array_append (array, &val);
}

void
soup_value_array_insert (GValueArray *array, guint index_, GType type, ...)
{
        va_list args;
        GValue val;

        va_start (args, type);
        SOUP_VALUE_SETV (&val, type, args);
        va_end (args);
        g_value_array_insert (array, index_, &val);
}

gboolean
soup_value_array_get_nth (GValueArray *array, guint index_, GType type, ...)
{
        GValue *value;
        va_list args;

        value = g_value_array_get_nth (array, index_);
        if (!value || !G_VALUE_HOLDS (value, type))
                return FALSE;

        va_start (args, type);
        SOUP_VALUE_GETV (value, type, args);
        va_end (args);
        return TRUE;
}

gboolean
soup_value_hash_lookup (GHashTable *hash, const char *key, GType type, ...)
{
        va_list args;
        GValue *value;

        value = g_hash_table_lookup (hash, key);
        if (!value || !G_VALUE_HOLDS (value, type))
                return FALSE;

        va_start (args, type);
        SOUP_VALUE_GETV (value, type, args);
        va_end (args);

        return TRUE;
}
//---------For simplicity: don't have to integrate libsoup--------------------

//------ some test functions for glib --------------
void test_g_file_test()
{
	/* g_file_test */ 
	/* Test if a file exists. */ 
	if (!g_file_test("/home/soho/test/tglib.c", G_FILE_TEST_EXISTS)) { 
		g_print ("File not found. \n"); 
	} else { 
		g_print ("File found. \n");    
	} 
}

void test_GArray1()
{
	GArray *garray;
	gint i;

	/* We create a new array to store gint values.
	   We don't want it zero-terminated or cleared to 0's. */
	garray = g_array_new (FALSE, FALSE, sizeof (gint));
	for (i = 0; i < 10000; i++)
		g_array_append_val (garray, i);

	for (i = 0; i < 10000; i++) {
		if (g_array_index (garray, gint, i) != i) {
			g_print ("ERROR: got %d instead of %d\n", g_array_index (garray, gint, i), i);
		}
	}

	g_array_free (garray, TRUE);
}

void test_GArray2()
{
	GArray *garray = NULL;
	ShareInfo_t *shareInfo = NULL;
	int i = 0;

	garray = g_array_new(FALSE, FALSE, sizeof (ShareInfo_t));
	g_print("garray=%p, garray->data=%p, garray->len=%d\n", garray, garray->data, garray->len);
	
	/* file the array. */
	shareInfo = g_try_new0(ShareInfo_t, 1);
	shareInfo->name = g_strdup("share1");
	g_print("shareInfo=%p, name=%s %p\n", shareInfo, shareInfo->name, shareInfo->name);
	g_array_append_vals(garray, (gconstpointer)shareInfo, 1);
	/* only the value/content of shareInfo copied to garray, including shareInfo->name. 
	   So we can deallocate shareInfo now. */
	g_free(shareInfo);
	shareInfo = NULL;

	shareInfo = g_try_new0(ShareInfo_t, 1);
	shareInfo->name = g_strdup("share2");
	g_print("shareInfo=%p, name=%s %p\n", shareInfo, shareInfo->name, shareInfo->name);
	g_array_append_vals(garray, (gconstpointer)shareInfo, 1);
	g_free(shareInfo);
	shareInfo = NULL;

	g_print("garray=%p, garray->data=%p, garray->len=%d\n", garray, garray->data, garray->len);

	for (i = 0; i < garray->len; ++i) {
		shareInfo = &g_array_index(garray, ShareInfo_t, i);
		g_print("shareInfo=%p, name=%s %p\n", shareInfo, shareInfo->name, shareInfo->name);
		g_free(shareInfo->name);
	}
	g_array_free(garray, TRUE);

}

void print_key_value(gpointer key, gpointer value, gpointer user_data)
{
	g_print("%s ---> %s\n", (const char *)key, (const char *)value);
}

void display_hash_table(GHashTable *table)
{
	g_hash_table_foreach(table, print_key_value, NULL);
}

void test_GHashTable1()
{
	GHashTable *table = NULL;

	table = g_hash_table_new(g_str_hash, g_str_equal);

	g_hash_table_insert(table, "1", "one");
	g_hash_table_insert(table, "2", "two");
	g_hash_table_insert(table, "3", "three");
	g_hash_table_insert(table, "4", "four");
	g_hash_table_insert(table, "5", "five");

	display_hash_table(table);
	g_print("Size of hash table: %d \n", g_hash_table_size(table));

	g_print("Before replace: 3 ---> %s \n", (const char *)g_hash_table_lookup(table, "3"));
	g_hash_table_replace(table, "3", "third");
	g_print("After replace: 3 ---> %s \n", (const char *)g_hash_table_lookup(table, "3"));

	g_hash_table_remove(table, "2");
	display_hash_table(table);
	g_print("Now size of hash table: %d \n", g_hash_table_size(table));

	g_hash_table_destroy(table);
}

void free_key(gpointer data)
{
	g_print("We free key: %s \n", (const char *)data);
	g_free(data);
}

void free_value(gpointer data)
{
	g_print("We free value: %s \n", (const char *)data);
	g_free(data);
}

void test_GHashTable2()
{
	GHashTable *table = NULL;

	table = g_hash_table_new_full(g_str_hash, g_str_equal, free_key, free_value);
	
	g_hash_table_insert(table, g_strdup("one"), g_strdup("first"));
	g_hash_table_insert(table, g_strdup("two"), g_strdup("second"));
	g_hash_table_insert(table, g_strdup("three"), g_strdup("third"));

	g_print("Remove an item from hash table: \n");
	g_hash_table_remove(table, "two");

	g_print("Destroy hash table: \n");
	g_hash_table_destroy(table);
}

void test_GValueArray_GHashTable()
{
	int i = 0;
	GValueArray *gvarray = g_value_array_new(1);
	GHashTable *htable = soup_value_hash_new();
	/* reference count is 1. */
	g_print("allocated htable=%pref_cnt=%d\n", htable, htable->ref_count);

	soup_value_hash_insert(htable, "id", G_TYPE_STRING, "id0");
	soup_value_hash_insert(htable, "name", G_TYPE_STRING, "name0");
	soup_value_array_append(gvarray, G_TYPE_HASH_TABLE, htable);
	/* reference count is 2 now. */
	g_print("allocated htable=%pref_cnt=%d\n", htable, htable->ref_count);

	g_hash_table_unref(htable);
	/* reference count is 1 now. */

	g_printf("len=%d\n", gvarray->n_values);
	//GValue *tmp = (GValue *)g_value_array_get_nth(gvarray, 0);
	GHashTable *tmp = NULL;
	char *id = NULL;
	soup_value_array_get_nth(gvarray, 0, G_TYPE_HASH_TABLE, &tmp);
	soup_value_hash_lookup(tmp, "id", G_TYPE_STRING, &id);
	g_print("tmp=%p,size=%d,id=%s\n", tmp, g_hash_table_size(tmp), id);

	g_value_array_free(gvarray);
}

/* It is not a recommendded usage!!! */
void test_stack_GValueArray()
{
	/* Please do not allocate GValueArray on a non-heap space. */
	GValueArray gvarray;

	memset(&gvarray, 0, sizeof (GValueArray));

	soup_value_array_insert(&gvarray, 0, G_TYPE_INT, 0);
	soup_value_array_insert(&gvarray, 1, G_TYPE_INT, 1);
	g_value_array_append(&gvarray, NULL);

	/* It is a workaround. But you should not know what members a GValueArray has. */
	g_free(gvarray.values);
}

void test_heap_GValueArray()
{
	GValueArray *gvarray = g_value_array_new(2);

	soup_value_array_insert(gvarray, 0, G_TYPE_INT, 0);
	soup_value_array_insert(gvarray, 1, G_TYPE_INT, 1);
	g_value_array_append(gvarray, NULL);

	g_value_array_free(gvarray);
}

static void
int2string (const GValue *src_value,
            GValue       *dest_value)
{
	if (g_value_get_int (src_value) == 88)
		g_value_set_static_string (dest_value, "An important number");
	else
		g_value_set_static_string (dest_value, "What's that?");
}

void test_GValue1()
{
	/* GValues must start zero-filled */
	GValue a = {0};
	GValue b = {0};
	const gchar *message;

	/* The GValue starts empty */
	g_assert (!G_VALUE_HOLDS_STRING (&a));

	/* Put a string in it */
	g_value_init (&a, G_TYPE_STRING);
	g_assert (G_VALUE_HOLDS_STRING (&a));
	g_value_set_static_string (&a, "Hello, world!");
	g_printf ("%s\n", g_value_get_string (&a));

	/* Reset it to its pristine state */
	g_value_unset (&a);

	/* It can then be reused for another type */
	g_value_init (&a, G_TYPE_INT);
	g_value_set_int (&a, 88);

	/* Attempt to transform it into a GValue of type STRING */
	g_value_init (&b, G_TYPE_STRING);

	/* An INT is transformable to a STRING */
	g_assert (g_value_type_transformable (G_TYPE_INT, G_TYPE_STRING));

	g_value_transform (&a, &b);
	g_printf ("%s\n", g_value_get_string (&b));

	/* Attempt to transform it again using a custom transform function */
	g_value_register_transform_func (G_TYPE_INT, G_TYPE_STRING, int2string);
	g_value_transform (&a, &b);
	g_printf ("%s\n", g_value_get_string (&b));

	g_value_unset(&a);
}

void test_GValue2()
{
	GValue a;

	memset(&a, 0, sizeof (GValue));
	g_value_init(&a, G_TYPE_STRING);
	g_value_set_string(&a, "Hello GValue"); /* it calls g_strdup(). */

	g_value_unset(&a);
}

void display_list1(GList *list)
{
	GList *it = NULL;

	for (it = list; it; it = it->next) {
		g_print("%s", (const char*)it->data);
	}
	g_print("\n");
}

void test_GList1()
{
	GList *list = NULL;

	/*
	 * insert at the tail
	 */
	list = g_list_append(list, "one ");
	list = g_list_append(list, "two ");
	list = g_list_append(list, "three ");
	list = g_list_append(list, "four ");
	list = g_list_append(list, "five ");
	display_list1(list);

	/* insert at the head */
	list = g_list_prepend(list, "zero ");
	display_list1(list);

	GList *it = NULL;

	/* find one node */
	it = g_list_find(list, "two ");
	g_print("Find data \"two \": %s\n", (const char*)it->data);

	int index = 0;

	/* find the position */
	index = g_list_position(list, it);
	g_print("index of \"two \" is: %d\n", index);

	it = g_list_nth(list, 1);
	g_print("%s\n", (const char*)it->data);

	/* remove one node */
	g_print("After remove data \"three \", the list:\n");
	list = g_list_remove(list, "three ");
	display_list1(list);

	/* insert one node in the middle */
	g_print("After insert string \"INSERT \" at index 3, the list:\n");
	list = g_list_insert(list, "INSERT ", 3);
	display_list1(list);

	g_list_foreach(list, (GFunc)g_print, NULL);
	g_print("\n");

	GList *last = NULL;
	last = g_list_last(list);
	for (it = last; it; it = it->prev) {
		g_print("%s", (const char*)it->data);
	}
	g_print("\n");

	g_list_free(list);
}

void test_GList2()
{
	GList *list = NULL;
	ShareInfo_t *shareInfo = NULL;

	shareInfo = g_try_new0(ShareInfo_t, 1);
	shareInfo->name = g_strdup("share1");
	g_print("shareInfo=%p, name=%s %p\n", shareInfo, shareInfo->name, shareInfo->name);
	list = g_list_append(list, shareInfo);

	shareInfo = g_try_new0(ShareInfo_t, 1);
	shareInfo->name = g_strdup("share2");
	g_print("shareInfo=%p, name=%s %p\n", shareInfo, shareInfo->name, shareInfo->name);
	list = g_list_append(list, shareInfo);

	GList *p = NULL;
	for (p = list; p != NULL; p = g_list_next(p)) {
		//g_print("%s\n", ((ShareInfo_t *)p->data)->name);
		ShareInfo_t *si = (ShareInfo_t *)p->data;
		if (si) {
			g_free(si->name);
			g_free(si);
		}
	}
	g_list_free(list);
}
int
main(int argc, char *argv[])
{
	g_type_init();

	//test_g_file_test();

	//test_GArray1();

	//test_GValueArray_GHashTable();

	//test_stack_GValueArray();

	//test_heap_GValueArray();

	//test_GHashTable2();

	//test_GValue1();
	//test_GValue2();

	//test_GList1();

	while (1) {
	test_GList2();
	usleep(10);
	break;
	}

	g_printf("bye~\n");
	return (0);
}
