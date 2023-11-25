#include <D:/projects/wallet/database.h>

int main()
{
    Database db;
    Hashmap map_category;
    InitializeDatabase(&db);
    InitializeHashMap(&map_category);
    LoadDatabase(&db,"check.txt");
    LoadHashMap(&map_category, &db);

    for (int i = 0; i < map_category.num_of_entries;i++)
    {
        printf("%s - %s - %d\n", map_category.all_entries[i].key, map_category.all_entries[i].currency, map_category.all_entries[i].value);
    }
    int option;
    DisplayMenu();
    scanf("%d", &option);
    switch(option)
    {
        case 1: AddEntry(&db, "check.txt"); break;
        case 2: CreateCategory(); break;
        case 3: DisplayTotalStatistics(&db); break;
    }
    SaveDatabase(&db, "check.txt");
    free(db.all_entries);
    free(map_category.all_entries);
    return 0;
}