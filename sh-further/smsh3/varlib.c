/* varlib.c
 * a simple storage system to store name=value pairs
 * with facility to mark items as part of the environment
 * 
 * interface:
 *      VLstore(name, value) returns q for ok, 0 for no
 *      VLlookup(name) returns string or NULL if not there
 *      VList() prints out current table
 * environment -- ralated function
 *      VLexport(name) adds name to list of env vars
 *      VLtable2environ() copy from table to environ
 *      VLenviron2table() copy from environ to table
 * details:
 * the table is stored as an array of struct that
 * contain a flag for global and a single string of
 * the form name=value. This allows EZ addition to the
 * environment. It makes searching pretty easy, as 
 * long as you search for "name="
 */

#include <stdio.h>
#include <stdlib.h>
#include "varlib.h"
#include "string.h"

#define MAXVARS 200 // a linked list would be nicer

struct var {
    char* str; // name=val string
    int global; // a boolean
};

static struct var tab[MAXVARS]; // the table

static char* new_string(char*, char*); // private methods
static struct var* find_item(char*, int);

/*
 * traverse list, if found, replace it, else add at end 
 * since there is no delete, a blank one is a free one
 * return 1 if trouble, 0 if ok(like a command)
 */
int VLstore(char* name, char* val) {
    struct var* itemp;
    char* s;
    int rv = 1;

    // find spot to put it and make new string
    if ((itemp = find_item(name, 1)) != NULL && (s = new_string(name, val)) != NULL) {
        if (itemp->str) { // has a val?
            free(itemp->str); // y: remove it
        }
        itemp->str = s;
        rv = 0; // ok!
    }
    return rv;
}

// returns new string of form name=value or NULL on error
char* new_string(char* name, char* val) {
    char* retval;

    retval = malloc(strlen(name) + strlen(val) + 2);
    if (retval != NULL) {
        sprintf(retval, "%s=%s", name, val);
    }
    return retval;
}

// returns value of var or empty string if not there
char* VLlookup(char* name) {
    struct var* itemp;

    if ((itemp = find_item(name, 0)) != NULL) {
        return itemp->str+1+strlen(name);
    }
    return "";
}

/*
 * marks a var for export, adds it if not there
 * returns 1 for no, 0 for ok
 */
int VLexport(char* name) {
    struct var* itemp;
    int rv = 1;

    if ((itemp = find_item(name, 0)) != NULL) {
        itemp->global = 1;
        rv = 0;
    } else if (VLstore(name, "") == 1) {
        rv = VLexport(name);
    }
    return rv;
}

/*
 * searchs table for an item
 * returns ptr to struct or NULL if not found
 * OR if (first_blank) then ptr to first blank one
 */
static struct var* find_item(char* name, int first_blank) {
    int i;
    int len = strlen(name);
    char* s;

    for (i=0; i<MAXVARS && tab[i].str!=NULL; i++) {
        s = tab[i].str;
        if (strncmp(s, name, len) == 0 && s[len] == '=') {
            return &tab[i];
        }
    }
    if (i<MAXVARS && first_blank) {
        return &tab[i];
    }
    return NULL;
}

/*
 * performs the shells set command
 * Lists the contents of the variable table, marking each
 * exported variable with the symbol '*'
 */
void VLlist() {
    int i;
    for (i=0; i<MAXVARS && tab[i].str!=NULL; i++) {
        if (tab[i].global) {
            printf("*%s\n", tab[i].str);
        } else {
            printf("%s\n", tab[i].str);
        }
    }
}

/*
 * initialize the variable table by loading array of strings
 * return 1 for ok, 0 for not ok
 */
int VLenviron2table(char* env[]) {
    int i;
    char* newstring;

    for (i=0; env[i]!=NULL; i++) {
        if (i == MAXVARS) {
            return 0;
        }
        newstring = malloc(1+strlen(env[i]));
        if (newstring == NULL) {
            return 0;
        }
        strcpy(newstring, env[i]);
        tab[i].str = newstring;
        tab[i].global = 1;
    }
    while (i < MAXVARS) { // I know we don't need this
        tab[i].str = NULL; // static globals are nulled
        tab[i++].global = 0; // by default
    }
    return 1;
}

/*
 * build an array of pointers suitable for making a new environment
 * note, you need to free() this when done to aviod memory leaks
 */
char** VLtable2environ() {
    int i; // index
    int j; // another index
    int n = 0; // counter

    char** envtab; // array of pointers
    // first, count the number of global variables
    for (i = 0; i<MAXVARS && tab[i].str!=NULL; i++) {
        if (tab[i].global == 1) {
            n++;
        }
    }
    // then, allocate space for that many variables
    envtab = (char**)malloc((n+1) * sizeof(char*));
    if (envtab == NULL) {
        return NULL;
    }
    // then, load the array with pointers
    for (i=0, j=0; i<MAXVARS && tab[i].str!=NULL; i++) {
        if (tab[i].global == 1) {
            envtab[j++] = tab[i].str;
        }
    }
    envtab[j] = NULL;
    return envtab;
}