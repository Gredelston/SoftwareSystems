#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

// Below are several reasons that test.txt is a suitable test file:
// Several words appear multiple times
// Some words appear mutliple times with different capitalization
// Some words have punctuation before, after, or in between
// Some words are separated by multiple spaces
// There are linebreaks and tabs
// The file inds in whitespace
// There is an entirely-punctuation word, which should not get histogrammed
// There are numbers, which should get histogrammed (for the sake of, say, years)
#define FILE_LOCATION "test.txt"

// Prints out an array of gchars.
// INPUTS:  gc -- a pointer to a 0-terminated array of gchars
// OUTPUTS: none
void gchar_arr_print(gchar *gc) {
	int i;
	for(i = 0; gc[i]!=0; i++)
		printf("%c", gc[i]);
}

// Prints a 2D array of gchars;
// i.e., an array of words.
// INPUTS: gcs -- a pointer to a 0-terminated array of 
//				  0-terminated arrays of gchars
// OUTPUTS: none
void gchar_arr2_print(gchar **gcs) {
	int i;
	for (i=0; gcs[i]!=0; i++) {
		gchar_arr_print(gcs[i]);
		printf("");
	}
	printf("\n\n");
}

// Ok, this does a liiiittle more than remove punctuation.
// It also turns all the text into lower-case,
// and turns all whitespace into spaces.
// This makes the words more consistent for processing.
// INPUTS: oldstr -- a pointer to a 0-terminated array of gchars
// OUTPUTS: a pointer to a 0-terminated array of gchars, but cleaner
gchar *gchar_remove_punct(gchar *oldstr) {
	gchar *newstr;
	gchar c;
	int new_index = 0;
	int old_index = 0;
	int spaceBefore = 0; // two spaces? redundant!!
	while(1) {
		c = oldstr[old_index];
		if (g_ascii_isalnum(c)) {
			newstr[new_index] = c;
			spaceBefore = 0;
			new_index++;
		} else if ( g_ascii_isspace(c) &&
				    !spaceBefore 	   &&
				    oldstr[old_index+1]!=0) {
			newstr[new_index] = ' ';
			spaceBefore = 1;
			new_index++;
		} else if (c == 0) { // i.e.,  punctuation.
			newstr[new_index] = 0;
			return newstr;
		}
		old_index++;
	}
}

// If the key in question already exists in the hashtable,
// then increment it by 1.
// Otherwise, add it to the hashtable, with a value of 1.
// INPUTS: hash -- a pointer to a GHashTable
//		   key  -- a pointer to an array of gchars
// OUTPUTS: none; modifies hash in place
void g_hash_increment(GHashTable *hash, gchar *key) {
	int *one = 1;
	if (g_hash_table_contains(hash, key))
		g_hash_table_replace(hash, key, g_hash_table_lookup(hash,key) + 1);
	else
		g_hash_table_insert(hash, key, one);
}

// Does the actual assignment!
// Goes through the list of words, and increments the table.
// INPUTS: words -- a pointer to a 2-day array of gchars;
//					typically, output from g_strsplit()
// 		   hash -- a pointer to a GHashTable
// OUTPUTS: none; modifies hash in place
void think(gchar **words, GHashTable *hash) {
	int i = 0;
	while(1) {
		if (words[i] == 0)
			return;
		else
			g_hash_increment(hash, words[i]);
		i++;
	}
}

// Prints out a hash table, in the format "key: value"
// INPUTS: keys -- the output of g_hash_table_get_keys(hash)
//		   hash -- the hashtable which generated keys
// OUTPUTS: none
void g_hash_print(GList *keys, GHashTable *hash) {
	gchar *key = keys -> data;
	int data = g_hash_table_lookup(hash, key);
	gchar_arr_print(key);
	printf(": %i\n", data);
	if (keys->next != NULL)
		g_hash_print(keys->next, hash);
}

int main (void)
{
	// Read that file!
	gchar *filename = FILE_LOCATION;
	gchar *contents;
	gssize *length = malloc(sizeof(gssize));
	g_file_get_contents(filename, &contents, length, NULL);

	// Format that string!
	contents = g_ascii_strdown(contents, *length);
	gchar_arr_print(contents);
	contents = gchar_remove_punct(contents);

	// Split those words!
	gchar **split = g_strsplit(contents, " ", 0);

	// Populate that histogram!
	GHashTable *hash = g_hash_table_new( g_str_hash, g_str_equal );
	think(split, hash);

	// Show that correctness!
	puts("\n");
	g_hash_print(g_hash_table_get_keys(hash), hash);

	// Return that 0!
    return 0;
}