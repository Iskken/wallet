#ifndef DATABASE_H
#define DATABASE_H

#include <D:/projects/wallet/data_structures.h>

void InitializeDatabase(Database* db)
{
    db->num_of_entries = 0;
    db->capacity = 10;
    db->all_entries = (Entry*)malloc(db->capacity*sizeof(Entry));
    if (db->all_entries == NULL)
    {
        printf("Failed to initialize");
        exit(1);
    }
}

void InitializeKeyValueStructure(KeyValueStructure *kd)
{
    
}

void AddEntryToDb(Database *db, const Entry *new_entry);
int SearchCategory(const char* category, Database *db);

void LoadDatabase(Database* db, const char* filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("The load of database is unsuccesful\n");
        exit(2);
    }

    //load database into database structure here
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) //the condition is that it works until a new line is encountered or the buffer is full
    {
        Entry new_entr;
        char type[8]; //variable to hold the type (INCOME or EXPENSE)

        if (sscanf(buffer, "%d-%d-%d,%7[^,],%19[^,],%lf,%3s",  //[^,] - indicates that we should read until a coma
                   &new_entr.date.year, &new_entr.date.month, &new_entr.date.day,
                   type, new_entr.category, &new_entr.amount, new_entr.currency) == 7)
            {
                if (strcmp(type, "Income") == 0) //if the string between coma's is "Income"
                {
                    new_entr.type = INCOME;
                }
                else
                {
                    new_entr.type = EXPENSE;
                }
                //printf("%d %d %d %d %s %lf %s", new_entr.date.year, new_entr.date.month,new_entr.date.day,new_entr.type, new_entr.category, new_entr.amount,new_entr.currency);
                AddEntryToDb(db, &new_entr);
            }
        else
        {
            printf("Error loading the database");
            exit(3);
        }
    }
    db->old_num_of_entries = db->num_of_entries;
    fclose(fp);
}

void DisplayMenu()
{
    printf("1. Add new entry\n2. Create new category\n3. Display total statistics\n4. Display statistics for a specific time period\n5. Save database\n 6. Exit\n\n");
}

void AddEntryToDb(Database *db, const Entry *new_entry) {
    if (db->num_of_entries == db->capacity) {
        size_t new_capacity = db->capacity > 0 ? db->capacity * 2 : 1;
        Entry *new_allEntries = realloc(db->all_entries, new_capacity * sizeof(Entry));
        if (new_allEntries == NULL) {
            printf("Failed to reallocate memory for new entries");
            return;
        }
        db->all_entries = new_allEntries;
        db->capacity = new_capacity;
    }

    db->all_entries[db->num_of_entries] = *new_entry; // add new entry
    db->num_of_entries++; // increment as we add a new entry
}

void SaveDatabase(Database* db, const char* filename)
{
    FILE *fp = fopen(filename, "a");
    int old = db->old_num_of_entries;
    int new = db->num_of_entries;

    while (old < new)
    {
        fprintf(fp, "%d-%d-%d", db->all_entries[old].date.year, db->all_entries[old].date.month,db->all_entries[old].date.day);
        
        if (db->all_entries[old].type == INCOME)
            fprintf(fp, "%s", ",Income");
        else
            fprintf(fp, "%s", ",Expense");

        fprintf(fp, ",%s", db->all_entries[old].category);

        fprintf(fp, ",%.2lf", db->all_entries[old].amount);
        fprintf(fp, ",%s\n", db->all_entries[old].currency);
        old++;
    }
}

void SaveToCategoryDatabase(char *new_category, const char *filename)
{
    FILE *fp = fopen(filename, "a");
    fprintf(fp, "%s", new_category);
    fprintf(fp, "\n");
    fclose(fp);
}

void AddEntry(Database *db, const char *filename) //add the new entry into "txt" file
{
    Entry input;
    type_t option;

    printf("New Entry\nDate:");
    scanf("%d %d %d", &input.date.year, &input.date.month, &input.date.day);

    printf("\nType:0.Expense 1.Income\nEnter the type: ");
    scanf("%d", &input.type);
    
    printf("\nCategory(no more than 20 characters):");
    char try_category[20];
    scanf("%19s", try_category);
    if (!SearchCategory(try_category, db))
    {
        printf("The category %s was not found.\nWould you like to create a category with this name?\n0.No 1.Yes\n", try_category);
        int select;
        scanf("%d", &select);
        if (select)
        {
            strcpy(input.category, try_category); //DO NOT FORGET to add the else statement to handle the user's "NO" case
            SaveToCategoryDatabase(input.category, "category.txt");
        }
    }
    strcpy(input.category, try_category);

    printf("\nThe amount of money:");
    scanf("%lf", &input.amount);

    printf("\nThe currency:");
    scanf("%s", input.currency);

   AddEntryToDb(db, &input); //add the new entry into the database
}

int SearchCategory(const char* category, Database *db)
{
    //I have to check every category variable in database strcuture instead of a file
    //if not found -> update category, if found do nothing
    for (int i = 0; i < db->num_of_entries; i++)
    { 
        if (strcmp(db->all_entries[i].category, category) == 0)
        {
            printf("\nFOUND");
            return 1;
        }
    }
    return 0;
}

void CreateCategory()
{
    FILE *fp = fopen("category.txt", "a");
    printf("Insert the name of new category(no more than 20 characters):");
    char cat[20];
    scanf("%s", cat);
    fprintf(fp, "%s", cat);
    fprintf(fp, "\n");
    fclose(fp);
}

void DisplayTotalStatistics(const Database *db)
{
    int total_income = 0;
    int total_expense = 0;

    for (int i = 0; i < db->num_of_entries; i++)
    {
        if (db->all_entries[i].type == INCOME)
            total_income += db->all_entries[i].amount;
        else
            total_expense += db->all_entries[i].amount;
    }

    printf("The total income is: %d\nThe total expenses are: %d", total_income, total_expense);
    printf("\nThe overall balance is: %d", total_income-total_expense);
}

void DisplayMostExpensiveCategory(const Database* db)
{
    
}

void DisplayStatisticsForPeriod(const Database* db, int startYear, int startMonth, int startDay, int endYear, int endMonth, int endDay);
void FreeDatabase(Database* db);

#endif