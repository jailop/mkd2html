#include <stdlib.h>
#include "cstring.h"

struct string *string_new_len(const char *str, int len)
{
	/* A new object */
	struct string *s = malloc(sizeof(struct string));
	s->len = len;
	s->cap = _CSTRING_BUFSZ;
	while (s->cap < s->len + 1)
		s->cap *= 2;  /* Adjust size */
	s->str = malloc(sizeof(char) * s->cap);
	strncpy(s->str, str, s->len);
	s->str[s->len] = 0; /* Fix end of string */
	return s;
}

char *string_free_opt(struct string *s, int all)
{
    char *ret = s->str;
    if (all) {
        free(s->str);
        ret = NULL;
    }
    free(s);
    return ret;
}

struct string *string_erase_len(struct string *s, int pos, int len) {
    int i = pos;
    if (pos + 1 >= s->len)
        return s;
    while (s->str[i] && ((i + len) < s->len)) {
        s->str[i] = s->str[i + len];
        i++;
    }
    s->str[i] = 0;
    return s;
}

struct string *string_insert_len(struct string *s, int pos, char *str, int len)
{
    int i, modmem = 0;
    s->len += len;
    while (s->cap < (s->len + 1))
    {
        s->cap *= 2;
        modmem = 1;
    }
    if (modmem) {
        char *aux;
        aux = realloc(s->str, sizeof(char) * s->cap);
        s->str = aux;
    }
    for (i = s->len - len; i >= pos; i--)
        s->str[i + len] = s->str[i];
    strncpy(&(s->str[pos]), str, len);
    return s;
}

char **cstrsplit(char *st, const char *del) {
    char **ret = malloc(sizeof(char *) * _CSTRING_BUFSZ);
    char *ini, *end, **aux;
    int n, len, cap = _CSTRING_BUFSZ;
    for (n = 0, ini = st; 
        (end = strstr(ini, del)); 
        ini = end + strlen(del), n++) {
        if (n + 2 == cap) {
            cap *= 2;
            aux = realloc(ret, sizeof(char *) * cap);
            ret = aux;
        }
        len = end - ini;
        ret[n] = malloc(sizeof(char) * (len + 1));
        strncpy(ret[n], ini, len);
        ret[n][len] = 0;
    }
    ret[n] = malloc(sizeof(char) * (strlen(ini) + 1));
    strcpy(ret[n], ini);
    ret[n + 1] = NULL;
    return ret;
}

/* charv_len cuenta el número de elementos que
 * conforman un vector de strings.
 */
int ccharv_len(char **lst) {
    char **t; /* Iterador del vector de string */
    int n;    /* Variable para contar el número de elementos */
    for (t = lst, n = 0; *t; n++, t++);
    return n;
}

/* ccharv_free libera la memoria asignada para un
 * arreglo de strings. Primero se librera la memoria
 * de cada strings y luego la del vector
 */
void ccharv_free(char **lst) {
    char **t;
    for (t = lst; *t; t++) free(*t);
    free(lst);
}
