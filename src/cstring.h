#ifndef _CSTRING_H
#define _CSTRING_H

#include <string.h>

#define _CSTRING_BUFSZ 16 

/* string is a struct that holds autogrowing strings. It has three fields:
 * `str` allocates the string; `len` indicates the string length at current
 * moment; and `cap` is the current maximum capacity of the string.
 */
struct string
{
    char *str; /* String */
    int   len; /* Length - space used */
    int   cap; /* Capacity */
};

/* string_new_len creates a new string with an initial length.
 *
 * @params st : Source string. It can be empty.
 * @params len : Initial length
 *
 * @retun A new string.
 */
struct string *string_new_len(const char *st, int len);

/* string_new creates a new string with the length of the string given as
 * parameter. It is a macro to string_new_len. The initial length is calculated
 * with strlen.
 *
 * @params st : Source string. It can be empty.
 *
 * @return A new string
 */
#define string_new(st) string_new_len((st), strlen(st))

/* string_free_opt liberates space used in memory by a struct string structure. If
 * `all` parameter is set to 1 then memory allocate for `str` is free too, else
 * memory is not touched. This is useful when `str` has been allocated by others
 * functions and they manages its deallocation.
 *
 * @params s : string structure to be deallocated
 * @params all : if 1 then memory assigned to str field is deallocated
 *
 * @return NULL is all = 1, else a pointer to the str field.
 */
char *string_free_opt(struct string *s, int all);

#define string_free(s) string_free_opt(s, 1)

struct string *string_erase_len(struct string *s, int pos, int len);

#define string_erase(s, pos) string_erase_len((s), (pos), strlen(s->str) - (pos))

struct string *string_insert_len(struct string *s, int pos, char *st, int len);

#define string_insert(s, pos, st) string_insert_len((s), (pos), (st), strlen(st))

#define string_append(s, st) string_insert_len((s), strlen(s->str), (st), strlen(st))

#define string_prepend(s, st) string_insert_len((s), 0, (st), strlen(st))

char **cstrsplit(char *st, const char *del);

int ccharv_len(char **lst);

void ccharv_free(char **lst);

#endif /* _CSTRING_H */
