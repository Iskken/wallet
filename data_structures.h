#ifndef DATAST_H
#define DATAST_H

#include <stddef.h> //to use size_t
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {EXPENSE, INCOME} type_t;

typedef struct Date{
    int year;
    int month;
    int day;
} Date;

typedef struct Entry{
    Date date;
    type_t type;
    char category[20];
    double amount;
    char currency[4];
} Entry;

typedef struct Database{
    Entry *all_entries;
    size_t num_of_entries;
    size_t capacity;
    size_t old_num_of_entries;
} Database;

typedef struct{
    char *key;
    char *currency;
    int value;
}KeyValueStructure;

#endif