#include <D:/projects/wallet/database.h>

int main()
{
    Database db;
    Hashmap map_category;
    InitializeDatabase(&db);
    InitializeHashMap(&map_category);
    LoadDatabase(&db,"check.txt");
    LoadHashMap(&map_category, &db);

    int option = 1;
    int saved = 1;
    while (option)
    {
        DisplayMenu();
        scanf("%d", &option);
        switch(option)
        {
            case 1: AddEntry(&db, &saved, &map_category); break;
            case 2: CreateCategory(); break;
            case 3: DisplayTotalStatistics(&map_category, &db); DisplayMostExpensiveCategory(&map_category);break;
            case 4: DisplayStatisticsForPeriod(&db); break;
            case 5: SaveDatabase(&db, "check.txt", &saved); break;
            case 0:
            if (saved == 0) 
            {
                printf("Do you want to exit the application without saving?\n1.Yes\n0.No");
                int option2;
                scanf("%d", &option2);
                if (!option2)
                {
                    SaveDatabase(&db, "check.txt", &saved);
                }
            }
            break;
            default:printf("Invalid operation, please retype it");break;
        }
    }
    free(db.all_entries);
    free(map_category.all_entries);
    return 0;
}