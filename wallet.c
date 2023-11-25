#include <D:/projects/wallet/database.h>

int main()
{
    Database db;
    KeyValueStructure kd;
    InitializeDatabase(&db);
    LoadDatabase(&db,"check.txt");
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
    return 0;
}