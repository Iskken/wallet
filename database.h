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
    printf("1. Add new entry\n2. Create new category\n3. Display total statistics\n4. Display statistics for a specific time period\n5. Save database\n 0. Exit\n\n");
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

void SaveDatabase(Database* db, const char* filename, int *saved)
{
    if (*saved == 1) return;
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
    *saved = 1;
}

void SaveToCategoryDatabase(char *new_category, long budget_limit, const char *filename)
{
    FILE *fp = fopen(filename, "a");
    fprintf(fp, "%s", new_category);
    fprintf(fp, "- %d", budget_limit);
    fprintf(fp, "\n");
    fclose(fp);
}

void AddEntry(Database *db,  int *saved, Hashmap *cat) //add the new entry into "txt" file
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
            strcpy(input.category, try_category);

            printf("Would you like to set up a budget limit for this particular category?");
            scanf("%d", &select);
            if (select)
            {
                scanf("%d", &cat->all_entries[cat->num_of_entries].budget_limit);
                SaveToCategoryDatabase(input.category,cat->all_entries[cat->num_of_entries].budget_limit, "category.txt");
            }
            SaveToCategoryDatabase(input.category,cat->all_entries[cat->num_of_entries].budget_limit, "category.txt");
            cat->num_of_entries++;
        }
        else
        {
            printf("Entry not added. Please try again with a valid category.\n");
            return; // Exit the function as the user selected "No"
        }
    }
    strcpy(input.category, try_category);

    printf("\nThe amount of money:");
    scanf("%lf", &input.amount);

    printf("\nThe currency:");
    scanf("%s", input.currency);

   AddEntryToDb(db, &input); //add the new entry into the database

   saved = 0;
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
            return i;
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

void DisplayTotalStatistics(Hashmap *map_category, const Database *db)
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
    printf("Statistics for each category in all currencies:\n");
    for (int i = 0; i < map_category->num_of_entries;i++)
    {
        printf("%s - %s - %d\n", map_category->all_entries[i].key, map_category->all_entries[i].currency, map_category->all_entries[i].value);
    }
    printf("The total income is: %d\nThe total expenses are: %d", total_income, total_expense);
    printf("\nThe overall balance is: %d\n", total_income-total_expense);
}

void InitializeHashMap(Hashmap *map_category)
{
    map_category->num_of_entries = 0;
    map_category->capacity = 5;
    map_category->all_entries = (KeyValueStructure*)malloc(map_category->capacity*sizeof(KeyValueStructure));
    if (map_category->all_entries == NULL)
    {
        printf("Failed to initialize data structure for categories");
        exit(1);
    }
}

int IndexKeySearch(char *key, Hashmap *map_category)
{
    for (int i = 0; i < map_category->num_of_entries;i++)
    {
        if (strcmp(key, map_category->all_entries[i].key) == 0)
        {
            return i;
        }
    }
    return -1;
}

void LoadHashMap(Hashmap *map_category, Database *db)
{
    int j = 0;
    for (int i = 0; i < db->num_of_entries;i++)
    {
        if (map_category->num_of_entries == map_category->capacity)
        {
            size_t new_capacity = map_category->num_of_entries > 0 ? map_category->capacity * 2 : 1;
            KeyValueStructure *new_allEntries = realloc(map_category->all_entries, new_capacity * sizeof(KeyValueStructure));
            if (new_allEntries == NULL) {
                printf("Failed to reallocate memory for new entries of capacity Hashmap");
                return;
            }
            map_category->all_entries = new_allEntries;
            map_category->capacity = new_capacity;
        }
        else if (map_category->num_of_entries != map_category->capacity && db->all_entries[i].type == EXPENSE)
        {
            int index = IndexKeySearch(db->all_entries[i].category, map_category);
            if (index != -1 && strcmp(map_category->all_entries[index].currency, db->all_entries[i].currency) == 0)
            {
                map_category->all_entries[index].value += db->all_entries[i].amount;
            }
            else
            {
                strcpy(map_category->all_entries[j].key, db->all_entries[i].category);
                strcpy(map_category->all_entries[j].currency, db->all_entries[i].currency);
                map_category->all_entries[j].value = db->all_entries[i].amount;
                j++;
                map_category->num_of_entries++;
            }
        }
    }
}

void DisplayMostExpensiveCategory(Hashmap *map_category)
{
    if (map_category->num_of_entries == 0) {
        printf("Hashmap is empty\n");
        return;
    }

    int *check = (int *)malloc(map_category->num_of_entries * sizeof(int));
    if (check == NULL) {
        printf("Memory allocation failed\n");
        exit(-1);
    }

    for (int i = 0; i < map_category->num_of_entries; i++) {
        check[i] = 0;
    }

    for (int i = 0; i < map_category->num_of_entries; i++) {
        if (!check[i]) {
            int max = map_category->all_entries[i].value;
            int max_index = i;

            for (int j = i + 1; j < map_category->num_of_entries; j++) {
                if (!check[j] && strcmp(map_category->all_entries[i].currency, map_category->all_entries[j].currency) == 0) {
                    check[j] = 1;

                    if (max < map_category->all_entries[j].value) {
                        max = map_category->all_entries[j].value;
                        max_index = j;
                    }
                }
            }

            printf("The most expensive category for currency %s is %s with expenses counting up to %d\n", 
            map_category->all_entries[i].currency, map_category->all_entries[max_index].key, map_category->all_entries[max_index].value);
        }
    }

    free(check);
}

int checkDate(Date *startDate, Date *endDate, Date *check) {
    if (check->year < startDate->year || (check->year == startDate->year && check->month < startDate->month) || 
        (check->year == startDate->year && check->month == startDate->month && check->day < startDate->day)) {
        return -1;  // check date is before the start date
    } else if (check->year > endDate->year || (check->year == endDate->year && check->month > endDate->month) || 
               (check->year == endDate->year && check->month == endDate->month && check->day > endDate->day)) {
        return 1;   // check date is after the end date
    } else {
        return 0;   // check date is within the specified period
    }
}

void DisplayStatisticsForPeriod(const Database* db)
{
    printf("Please enter the start date in format YYYY-MM-DD: ");
    Date start_date;
    if (scanf("%d-%d-%d", &start_date.year, &start_date.month, &start_date.day) != 3) {
        printf("Invalid input");
        return;
    }

    printf("Please enter the end date in format YYYY-MM-DD: ");
    Date end_date;
    if (scanf("%d-%d-%d", &end_date.year, &end_date.month, &end_date.day) != 3) {
        printf("Invalid input");
        return;
    }

    printf("Statistics for each category within %d-%d-%d to %d-%d-%d\n",
        start_date.year, start_date.month, start_date.day,
        end_date.year, end_date.month, end_date.day);

    int *check = (int *)malloc(db->num_of_entries * sizeof(int));
    if (check == NULL) {
        printf("Memory allocation failed");
        return;
    }

    for (int i = 0; i < db->num_of_entries; i++) {
        check[i] = 0;
    }

    for (int i = 0; i < db->num_of_entries; i++) {
        if (check[i] || checkDate(&start_date, &end_date, &db->all_entries[i].date) != 0) continue;

        int total_spent = 0;
        int max_spent = 0;
        int total_income = 0;
        char currency[4]; 
        char category[20]; 
        strncpy(currency, db->all_entries[i].currency, sizeof(currency) - 1);
        currency[sizeof(currency) - 1] = '\0'; 
        strncpy(category, db->all_entries[i].category, sizeof(category) - 1);
        category[sizeof(category) - 1] = '\0';

        for (int j = i; j < db->num_of_entries; j++) {
            if (check[j]) continue;

            if (strcmp(currency, db->all_entries[j].currency) == 0 &&
                strcmp(category, db->all_entries[j].category) == 0 &&
                checkDate(&start_date, &end_date, &db->all_entries[j].date) == 0) {

                if (db->all_entries[j].type == INCOME) {
                    total_income += db->all_entries[j].amount;
                } else {
                    total_spent += db->all_entries[j].amount;
                    if (db->all_entries[j].amount > max_spent) {
                        max_spent = db->all_entries[j].amount;
                    }
                }
                check[j] = 1;
            }
        }

        printf("\nThe statistics for category %s in currency %s are:\nTotal Income:%d\nTotal Expense:%d\nMax Expense:%d\n",
            category, currency, total_income, total_spent, max_spent);
    }

    free(check);
}

#endif