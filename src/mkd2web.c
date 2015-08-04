#include <getopt.h>
#include <mkdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cstring.h"

#define BUFSZ 512
#define FRMSEP "%%%%\n" /* This separator is used in slideshows */
#define FRM_START "<div class=\"frame\">\n\n"
#define FRM_END   "\n</div>\n\n"

/* Web files attacked as binary objects */
extern char _binary_template_html_start; /* Web template */
extern char _binary_template_html_end;
extern char _binary_nav_html_start;      /* Navigation bar */
extern char _binary_nav_html_end;
extern char _binary_script_js_start;     /* Interaction script */
extern char _binary_script_js_end;
extern char _binary_style_css_start;     /* CSS style sheet */
extern char _binary_style_css_end;
extern char _binary_math_js_start;       /* Include Mathjax */
extern char _binary_math_js_end;

extern int optin;

/* str_replace replaces a label into the template by
 * string with limits in `start` and `end`
 */
void str_replace(struct string *tmpl, char *label, char *start, char *end) {
    int pos = strstr(tmpl->str, label) - tmpl->str;
    int len = end - start;
    string_erase_len(tmpl, pos, strlen(label));
    string_insert_len(tmpl, pos, start, len);
}

void print_help(const char *prgm) {
    printf("%s - A markdown to html converter\n"
           "Usage:\n"
           "  %s [OPTIONS] [input_file] [output_file]\n"
           "Options:\n"
           "  -h : Print this help\n"
           "  -m : Include Mathjax call\n"
           "  -s : Make slides (is a presentation)\n", prgm, prgm);
}
 
int main(int argc, char **argv) {
    FILE *fin = stdin, *fout = stdout;
    struct string *tmpl, *input;
    char *label, *html, buffer[BUFSZ];
    char **frames, **tkns;
    int len, pos;
    int opt, opt_math, opt_slides;
    MMIOT *mkd;

    /* Options processing */
    opt_math = 0;   /* Include mathjax call. Default: no */
    opt_slides = 0; /* Make slides. Default: no */
    while ((opt = getopt(argc, argv, "msh")) != -1) {
        switch (opt) {
            case 'm':
                opt_math = 1;
                break;
            case 's':
                opt_slides = 1;
                break;
            case 'h':
                print_help(argv[0]);
                exit(EXIT_SUCCESS);
        }
    }

    input = string_new("");
    if ((optind < argc) && (fin  = fopen(argv[optind++], "r")) == NULL) {
        fprintf(stderr, "Input file can not be read.\n");
        print_help(argv[0]);
        exit(EXIT_FAILURE);
    }
    while (fgets(buffer, BUFSZ, fin) != NULL)
        string_append(input, buffer);
    fclose(fin);
   
    frames = cstrsplit(input->str, FRMSEP);
    string_free(input);

    len = &_binary_template_html_end - &_binary_template_html_start;
    tmpl = string_new_len(&_binary_template_html_start, len);
  
    /* Conditional replacements */
    if (ccharv_len(frames) > 1)
        str_replace(tmpl, "{NAV}",
            &_binary_nav_html_start, &_binary_nav_html_end);
    else
        str_replace(tmpl, "{NAV}", NULL, NULL);
    if (opt_math)
        str_replace(tmpl, "{MATHJAX}",
            &_binary_math_js_start, &_binary_math_js_end);
    else
        str_replace(tmpl, "{MATHJAX}", NULL, NULL);

    /* Default replacements */
    str_replace(tmpl, "{STYLE}",
        &_binary_style_css_start, &_binary_style_css_end);
    str_replace(tmpl, "{SCRIPT}",
        &_binary_script_js_start, &_binary_script_js_end);
 
    /* Body processing */
    input = string_new("");
    tkns = frames;
    if (!opt_slides) /* No slides. Just one page */
        string_append(input, FRM_START);
    while (*tkns != NULL) {
        if (opt_slides) /* Make a div for each frame. Aka: make slides */
            string_append(input, FRM_START);
        len = strlen(*tkns);
        mkd = mkd_string(*tkns, len, 0);
        mkd_compile(mkd, MKD_EXTRA_FOOTNOTE);
        mkd_document(mkd, &html);
        string_append(input, html);
        mkd_cleanup(mkd);
        if (opt_slides)
            string_append(input, FRM_END);
        tkns++;
    }
    if (!opt_slides)
        string_append(input, FRM_END);
    label = "{BODY}";
    pos = strstr(tmpl->str, label) - tmpl->str;
    string_erase_len(tmpl, pos, strlen(label));
    string_insert(tmpl, pos, input->str);
    string_free(input);
    ccharv_free(frames);

    if (++optind == argc) 
        fout = fopen(argv[optind - 1], "w");
    fputs(tmpl->str, fout);
    fclose(fout);
    string_free(tmpl);

    return 0;
}

