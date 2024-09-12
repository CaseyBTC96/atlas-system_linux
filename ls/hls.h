#ifndef HLS_H
#define HLS_H
#include <sys/types.h>

/* Structure to represent an entry (file or directory) */
struct Entry
{
	char name[256]; /* Name of the entry */
	mode_t st_mode; /* Mode of the entry (permissions, type, etc.) */
	int is_hidden; /* Flag to indicate if the entry is hidden (1) or not (0) */
	struct Entry *next; /* Pointer to the next node in the list */
};

/* Structure to represent the linked list of entries */
struct EntryList
{
	struct Entry *head; /* Pointer to the head (first node) of the list */
	int count; /* Counter to keep track of the number of elements in the list */
};

/* Function prototypes */

/**
 * list_files - Function that lists the files in a directory excluding hidden
 * ones (those starting with '.').
 * @path: The path of the directory to list.
 * @program_name: The name of the program (argv[0]).
 * @num_args: The number of arguments to the program.
 * @display_one_per_line: Whether to display one entry per line.
 * @list: Pointer to the EntryList to store the entries.
 * @show_hidden: Whether to display hidden
 */
void list_files(const char *path, const char *program_name, int num_args, int display_one_per_line, int show_hidden, int show_almost_all, int detailed_listing, struct EntryList *list);

/**
 * add_entry_to_list - Function to add a new entry to the linked list.
 * @list: Pointer to the EntryList where the entry will be added.
 * @name: Name of the entry to be added.
 * @st_mode: Mode of the entry (permissions, type, etc.).
 * @is_hidden: Whether to display
 */
void add_entry_to_list(struct EntryList *list, const char *name, mode_t st_mode, int is_hidden);

/**
 * free_entry_list - Function to free the memory used by the linked list.
 * @list: Pointer to the EntryList to be freed.
 */
void free_entry_list(struct EntryList *list);

#endif /* HLS_H */
