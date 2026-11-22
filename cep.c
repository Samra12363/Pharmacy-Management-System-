/*
 *  PHARMACY MANAGEMENT SYSTEM
 * --------------------------------------------------------------------
 *  A menu-driven program to manage a pharmacy's medicine stock.
 *
 *  Features:
 *    - Add, display, search (name / category / ID), update, delete
 *    - Sell medicine and print a receipt
 *    - Low stock alerts
 *    - Sales report (daily, weekly, total)
 *    - Save / load records using a text file (pharmacy.txt)
 *  Note: Enter -1 at most prompts to go back / cancel.
 */

#include <stdio.h>    /* printf, scanf, file handling (FILE, fopen...) */
#include <string.h>   /* strcpy, strcmp */

#define MAX 100       /* Maximum number of medicines the system can store */
#define LOW_STOCK 10  /* A medicine is "low on stock" at or below this quantity */

/* Structure that holds the information of one medicine */
struct Medicine
{
    int id;             /* Unique medicine ID */
    char name[50];      /* Medicine name (one word) */
    char category[30];  /* Medicine category, e.g. Antibiotic */
    float price;        /* Price per unit */
    int quantity;       /* Units currently in stock */
    int sold;           /* Total units sold of this medicine */
};

/* Global array that stores all medicines */
struct Medicine medicines[MAX];

/* Global counters */
int medicineCount = 0;  /* How many medicines are currently stored */
int dailySales = 0;     /* Units sold today */
int weeklySales = 0;    /* Units sold this week */
int totalSales = 0;     /* Units sold overall */

/* ---------------------- Function prototypes ---------------------- */
int findMedicine(int id);
void chooseCategory(char category[]);
void addMedicine();
void displayMedicines();
void searchMedicineByName();
void searchMedicineByCategory();
void searchMedicineById();
void searchMedicine();
void updateMedicine();
void sellMedicine();
void lowStockAlert();
void salesReport();
void saveRecords();
void loadRecords();
void deleteMedicine();
void resetSales();

/*
 * findMedicine
 * Searches the array for a medicine with the given ID.
 * Returns its index in the array, or -1 if it is not found.
 */
int findMedicine(int id)
{
    int i;

    for (i = 0; i < medicineCount; i++)
    {
        if (medicines[i].id == id)
        {
            return i;   /* Found: return the position */
        }
    }

    return -1;          /* Not found */
}

/*
 * chooseCategory
 * Shows the category menu and copies the chosen category name into
 * the 'category' string. If the user cancels or enters an invalid
 * number, 'category' is set to an empty string "".
 */
void chooseCategory(char category[])
{
    int choice;

    printf("\n------ MEDICINE CATEGORIES ------\n");
    printf("1. Antibiotic\n");
    printf("2. Painkiller\n");
    printf("3. Vitamin\n");
    printf("4. Antacid\n");
    printf("5. Antiallergic\n");
    printf("6. Antiseptic\n");
    printf("7. Cough and Cold\n");
    printf("8. Diabetes\n");
    printf("-1. Back to Main Menu\n");

    printf("Enter category number: ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            strcpy(category, "Antibiotic");
            break;

        case 2:
            strcpy(category, "Painkiller");
            break;

        case 3:
            strcpy(category, "Vitamin");
            break;

        case 4:
            strcpy(category, "Antacid");
            break;

        case 5:
            strcpy(category, "Antiallergic");
            break;

        case 6:
            strcpy(category, "Antiseptic");
            break;

        case 7:
            /* Stored without spaces so it is safe to save/load with scanf */
            strcpy(category, "CoughCold");
            break;

        case 8:
            strcpy(category, "Diabetes");
            break;

        default:
            /* -1 or any invalid number: return an empty category */
            strcpy(category, "");
            printf("Going back to main menu.\n");
    }
}

/*
 * addMedicine
 * Reads a new medicine from the user, validates it, and stores it.
 * Validation: ID must be positive and unique, price must be > 0,
 * quantity must not be negative. Entering -1 cancels at any step.
 */
void addMedicine()
{
    int id, quantity;
    float price;
    char name[50];
    char category[30];

    /* Stop if the array is already full */
    if (medicineCount >= MAX)
    {
        printf("\nMedicine storage is full!\n");
        return;
    }

    printf("\n========== ADD MEDICINE ==========\n");

    /* ---- Medicine ID ---- */
    printf("Enter Medicine ID (-1 to exit): ");
    scanf("%d", &id);

    if (id == -1)
        return;

    if (id <= 0)
    {
        printf("ID must be greater than zero.\n");
        return;
    }

    /* IDs must be unique */
    if (findMedicine(id) != -1)
    {
        printf("This ID already exists.\n");
        return;
    }

    /* ---- Medicine name (%49s leaves room for '\0' in name[50]) ---- */
    printf("Enter Medicine Name (-1 to exit): ");
    scanf("%49s", name);

    if (strcmp(name, "-1") == 0)
        return;

    /* ---- Category ---- */
    chooseCategory(category);

    if (strcmp(category, "") == 0)   /* Empty means user cancelled */
        return;

    /* ---- Price ---- */
    printf("Enter Medicine Price (-1 to exit): ");
    scanf("%f", &price);

    if (price == -1)
        return;

    if (price <= 0)
    {
        printf("Price must be greater than zero.\n");
        return;
    }

    /* ---- Quantity ---- */
    printf("Enter Available Quantity (-1 to exit): ");
    scanf("%d", &quantity);

    if (quantity == -1)
        return;

    if (quantity < 0)
    {
        printf("Quantity cannot be negative.\n");
        return;
    }

    /* All inputs are valid: store the medicine in the next free slot */
    medicines[medicineCount].id = id;
    strcpy(medicines[medicineCount].name, name);
    strcpy(medicines[medicineCount].category, category);
    medicines[medicineCount].price = price;
    medicines[medicineCount].quantity = quantity;
    medicines[medicineCount].sold = 0;   /* New medicine: nothing sold yet */

    medicineCount++;

    printf("\nMedicine added successfully!\n");
}

/*
 * displayMedicines
 * Prints all stored medicines in a table.
 */
void displayMedicines()
{
    int i;

    printf("\n========== ALL MEDICINES ==========\n");

    if (medicineCount == 0)
    {
        printf("No medicine records available.\n");
        return;
    }

    /* Table header */
    printf("%-8s %-15s %-15s %-10s %-10s %-8s\n",
           "ID", "Name", "Category", "Price", "Quantity", "Sold");

    printf("--------------------------------------------------------------\n");

    /* One row per medicine */
    for (i = 0; i < medicineCount; i++)
    {
        printf("%-8d %-15s %-15s %-10.2f %-10d %-8d\n",
               medicines[i].id,
               medicines[i].name,
               medicines[i].category,
               medicines[i].price,
               medicines[i].quantity,
               medicines[i].sold);
    }
}

/*
 * searchMedicineByName
 * Finds and prints every medicine whose name exactly matches the
 * entered name (case-sensitive).
 */
void searchMedicineByName()
{
    char searchName[50];
    int i, found = 0;   /* found: 1 once at least one match is printed */

    if (medicineCount == 0)
    {
        printf("\nNo medicine records available.\n");
        return;
    }

    printf("\n========== SEARCH BY NAME ==========\n");
    printf("Enter Medicine Name (-1 to exit): ");
    scanf("%49s", searchName);

    if (strcmp(searchName, "-1") == 0)
        return;

    /* Check every medicine (names are not guaranteed unique) */
    for (i = 0; i < medicineCount; i++)
    {
        if (strcmp(medicines[i].name, searchName) == 0)
        {
            printf("\nMedicine Found!\n");
            printf("Medicine ID: %d\n", medicines[i].id);
            printf("Medicine Name: %s\n", medicines[i].name);
            printf("Category: %s\n", medicines[i].category);
            printf("Price: %.2f\n", medicines[i].price);
            printf("Available Quantity: %d\n", medicines[i].quantity);
            printf("Total Sold: %d\n", medicines[i].sold);

            found = 1;
        }
    }

    if (found == 0)
    {
        printf("Medicine not found.\n");
    }
}

/*
 * searchMedicineByCategory
 * Lets the user pick a category and lists all medicines in it.
 */
void searchMedicineByCategory()
{
    char category[30];
    int i, found = 0;

    if (medicineCount == 0)
    {
        printf("\nNo medicine records available.\n");
        return;
    }

    printf("\n========== SEARCH BY CATEGORY ==========\n");

    chooseCategory(category);

    if (strcmp(category, "") == 0)   /* User cancelled */
        return;

    printf("\nMedicines in category: %s\n", category);

    printf("%-8s %-15s %-10s %-10s %-8s\n",
           "ID", "Name", "Price", "Quantity", "Sold");

    for (i = 0; i < medicineCount; i++)
    {
        if (strcmp(medicines[i].category, category) == 0)
        {
            printf("%-8d %-15s %-10.2f %-10d %-8d\n",
                   medicines[i].id,
                   medicines[i].name,
                   medicines[i].price,
                   medicines[i].quantity,
                   medicines[i].sold);

            found = 1;
        }
    }

    if (found == 0)
    {
        printf("No medicines found in this category.\n");
    }
}

/*
 * searchMedicineById
 * Finds one medicine by its unique ID and prints its details.
 */
void searchMedicineById()
{
    int id, index;

    if (medicineCount == 0)
    {
        printf("\nNo medicine records available.\n");
        return;
    }

    printf("\n========== SEARCH BY ID ==========\n");
    printf("Enter Medicine ID (-1 to exit): ");
    scanf("%d", &id);

    if (id == -1)
        return;

    index = findMedicine(id);

    if (index == -1)
    {
        printf("Medicine not found.\n");
        return;
    }

    printf("\nMedicine Found!\n");
    printf("Medicine ID: %d\n", medicines[index].id);
    printf("Medicine Name: %s\n", medicines[index].name);
    printf("Category: %s\n", medicines[index].category);
    printf("Price: %.2f\n", medicines[index].price);
    printf("Available Quantity: %d\n", medicines[index].quantity);
    printf("Total Sold: %d\n", medicines[index].sold);
}

/*
 * searchMedicine
 * Search menu: asks how the user wants to search, then calls the
 * matching search function.
 */
void searchMedicine()
{
    int choice;

    printf("\n========== SEARCH MEDICINE ==========\n");
    printf("1. Search by Name\n");
    printf("2. Search by Category\n");
    printf("3. Search by ID\n");
    printf("-1. Back to Main Menu\n");

    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            searchMedicineByName();
            break;

        case 2:
            searchMedicineByCategory();
            break;

        case 3:
            searchMedicineById();
            break;

        case -1:
            printf("Going back to main menu.\n");
            break;

        default:
            printf("Invalid choice.\n");
    }
}

/*
 * updateMedicine
 * Finds a medicine by ID, then lets the user change its name, price,
 * quantity or category. New values are validated like in addMedicine.
 */
void updateMedicine()
{
    int id, index, choice;
    int newQuantity;
    float newPrice;
    char newName[50];
    char newCategory[30];

    printf("\n========== UPDATE MEDICINE ==========\n");
    printf("Enter Medicine ID (-1 to exit): ");
    scanf("%d", &id);

    if (id == -1)
        return;

    index = findMedicine(id);

    if (index == -1)
    {
        printf("Medicine not found.\n");
        return;
    }

    printf("\nMedicine Found: %s\n", medicines[index].name);

    printf("1. Update Name\n");
    printf("2. Update Price\n");
    printf("3. Update Quantity\n");
    printf("4. Update Category\n");
    printf("-1. Back to Main Menu\n");

    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:   /* Change name */
            printf("Enter New Name (-1 to exit): ");
            scanf("%49s", newName);

            if (strcmp(newName, "-1") == 0)
                return;

            strcpy(medicines[index].name, newName);

            printf("Name updated successfully.\n");
            break;

        case 2:   /* Change price */
            printf("Enter New Price (-1 to exit): ");
            scanf("%f", &newPrice);

            if (newPrice == -1)
                return;

            if (newPrice <= 0)
            {
                printf("Price must be greater than zero.\n");
                return;
            }

            medicines[index].price = newPrice;

            printf("Price updated successfully.\n");
            break;

        case 3:   /* Change quantity */
            printf("Enter New Quantity (-1 to exit): ");
            scanf("%d", &newQuantity);

            if (newQuantity == -1)
                return;

            if (newQuantity < 0)
            {
                printf("Quantity cannot be negative.\n");
                return;
            }

            medicines[index].quantity = newQuantity;

            printf("Quantity updated successfully.\n");
            break;

        case 4:   /* Change category */
            chooseCategory(newCategory);

            if (strcmp(newCategory, "") == 0)   /* User cancelled */
                return;

            strcpy(medicines[index].category, newCategory);

            printf("Category updated successfully.\n");
            break;

        case -1:
            printf("Going back to main menu.\n");
            break;

        default:
            printf("Invalid choice.\n");
    }
}

/*
 * sellMedicine
 * Sells a given quantity of a medicine: reduces stock, increases the
 * sold counters, and prints a receipt with the total bill.
 */
void sellMedicine()
{
    int id, index, quantity;
    float bill;

    printf("\n========== SELL MEDICINE ==========\n");
    printf("Enter Medicine ID (-1 to exit): ");
    scanf("%d", &id);

    if (id == -1)
        return;

    index = findMedicine(id);

    if (index == -1)
    {
        printf("Medicine not found.\n");
        return;
    }

    /* Show current details so the user knows what is available */
    printf("\nMedicine Name: %s\n", medicines[index].name);
    printf("Price: %.2f\n", medicines[index].price);
    printf("Available Quantity: %d\n", medicines[index].quantity);

    printf("Enter Quantity to Sell (-1 to exit): ");
    scanf("%d", &quantity);

    if (quantity == -1)
        return;

    if (quantity <= 0)
    {
        printf("Quantity must be greater than zero.\n");
        return;
    }

    /* Cannot sell more than what is in stock */
    if (quantity > medicines[index].quantity)
    {
        printf("Not enough stock available.\n");
        return;
    }

    /* Update stock and sales figures */
    medicines[index].quantity -= quantity;
    medicines[index].sold += quantity;

    dailySales += quantity;
    weeklySales += quantity;
    totalSales += quantity;

    /* Total bill = units sold x price per unit */
    bill = quantity * medicines[index].price;

    printf("\n========== SALE RECEIPT ==========\n");
    printf("Medicine Name: %s\n", medicines[index].name);
    printf("Quantity Sold: %d\n", quantity);
    printf("Price Per Unit: %.2f\n", medicines[index].price);
    printf("Total Bill: %.2f\n", bill);

    printf("\nSale completed successfully!\n");
}

/*
 * lowStockAlert
 * Warns about every medicine whose quantity is at or below LOW_STOCK.
 */
void lowStockAlert()
{
    int i, found = 0;

    printf("\n========== LOW STOCK ALERT ==========\n");

    for (i = 0; i < medicineCount; i++)
    {
        if (medicines[i].quantity <= LOW_STOCK)
        {
            printf("WARNING: Low stock!\n");
            printf("Medicine ID: %d\n", medicines[i].id);
            printf("Medicine Name: %s\n", medicines[i].name);
            printf("Available Quantity: %d\n", medicines[i].quantity);
            printf("-----------------------------\n");

            found = 1;
        }
    }

    if (found == 0)
    {
        printf("All medicines have enough stock.\n");
    }
}

/*
 * salesReport
 * Prints daily/weekly/total sales, units sold per medicine, and the
 * total stock remaining across all medicines.
 */
void salesReport()
{
    int i;
    int totalRemaining = 0;   /* Sum of all medicines' quantities */

    printf("\n========== SALES REPORT ==========\n");

    printf("Daily Sales: %d units\n", dailySales);
    printf("Weekly Sales: %d units\n", weeklySales);
    printf("Total Sales: %d units\n", totalSales);

    printf("\n------ MEDICINE SALES DETAILS ------\n");

    for (i = 0; i < medicineCount; i++)
    {
        printf("%s : %d units sold\n",
               medicines[i].name, medicines[i].sold);

        totalRemaining += medicines[i].quantity;
    }

    printf("\nTotal Remaining Stock: %d units\n", totalRemaining);
}

/*
 * saveRecords
 * Writes all data to "pharmacy.txt" in a labelled, readable format.
 * File format:
 *   line 1      : Number of medicines:<count>
 *   next lines  : Id:.. Name:.. Category:.. Price:.. Quantity:.. Sold:..
 *                 (one line per medicine)
 *   last 3 lines: daily sales, weekly sales, total sales
 * loadRecords() must read this exact same format.
 */
void saveRecords()
{
    FILE *file;
    int i;

    file = fopen("pharmacy.txt", "w");   /* "w" creates/overwrites the file */

    if (file == NULL)
    {
        printf("Unable to save records.\n");
        return;
    }

    fprintf(file, "Number of medicines:%d\n", medicineCount);

    for (i = 0; i < medicineCount; i++)
    {
        fprintf(file, "Id:%d\t Name:%s\t Category:%s\t Price:%.2f\t Quantity:%d\t Sold:%d\n",
                medicines[i].id,
                medicines[i].name,
                medicines[i].category,
                medicines[i].price,
                medicines[i].quantity,
                medicines[i].sold);
    }

    fprintf(file, " daily sales:%d\n weekly sales:%d\n total sales:%d\n",
            dailySales, weeklySales, totalSales);

    fclose(file);

    printf("All records saved successfully.\n");
}

/*
 * loadRecords
 * Reads data back from "pharmacy.txt" (same labelled format that
 * saveRecords writes).
 * Checks that the file exists and is not damaged before using it.
 */
void loadRecords()
{
    FILE *file;
    int count;   /* Number of medicines stored in the file */
    int i;

    file = fopen("pharmacy.txt", "r");   /* "r" = read only */

    if (file == NULL)
    {
        printf("No previous records found.\n");
        return;
    }

    /* First line must be "Number of medicines:<count>" */
    if (fscanf(file, "Number of medicines:%d", &count) !=  1)
    {
        printf("File is empty or damaged.\n");
        fclose(file);
        return;
    }

    /* Reject counts that are negative or larger than the array */
    if (count < 0 || count > MAX)
    {
        printf("Invalid records in file.\n");
        fclose(file);
        return;
    }

    /* Read each medicine; the labels (Id:, Name:, ...) must match
       saveRecords exactly. A space in the format skips any spaces/tabs
       and newlines, and fscanf must successfully read all 6 fields. */
    for (i = 0; i < count; i++)
    {
        if (fscanf(file, " Id:%d Name:%49s Category:%29s Price:%f Quantity:%d Sold:%d",
                   &medicines[i].id,
                   medicines[i].name,
                   medicines[i].category,
                   &medicines[i].price,
                   &medicines[i].quantity,
                   &medicines[i].sold) != 6)
        {
            printf("Some medicine records are damaged.\n");
            fclose(file);
            return;
        }
    }

    /* Read the three sales counters from the last three lines */
    if (fscanf(file, " daily sales:%d weekly sales:%d total sales:%d",
               &dailySales, &weeklySales, &totalSales) != 3)
    {
        printf("Sales information is missing.\n");
        fclose(file);
        return;
    }

    /* Only now that everything loaded correctly, update the count */
    medicineCount = count;

    fclose(file);

    printf("All records loaded successfully.\n");
}

/*
 * deleteMedicine
 * Removes a medicine by ID. Later medicines are shifted one position
 * left to fill the gap, then the count is reduced by one.
 */
void deleteMedicine()
{
    int id, index, i;

    if (medicineCount == 0)
    {
        printf("\nNo medicines available to delete.\n");
        return;
    }

    printf("\n========== DELETE MEDICINE ==========\n");
    printf("Enter Medicine ID (-1 to exit): ");
    scanf("%d", &id);

    if (id == -1)
        return;

    index = findMedicine(id);

    if (index == -1)
    {
        printf("Medicine not found.\n");
        return;
    }

    printf("Medicine Found: %s\n", medicines[index].name);

    /* Shift every medicine after 'index' one place to the left */
    for (i = index; i < medicineCount - 1; i++)
    {
        medicines[i] = medicines[i + 1];
    }

    medicineCount--;

    if (medicineCount == 0)
    {
        printf("All medicines have been deleted.\n");
    }
    else
    {
        printf("Medicine deleted successfully.\n");
    }
}

/*
 * resetSales
 * Sets the daily or weekly sales counter back to zero
 * (e.g. at the end of a day or week). Total sales is never reset.
 */
void resetSales()
{
    int choice;

    printf("\n========== RESET SALES ==========\n");
    printf("1. Reset Daily Sales\n");
    printf("2. Reset Weekly Sales\n");
    printf("-1. Back to Main Menu\n");

    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        dailySales = 0;
        printf("Daily sales reset successfully.\n");
    }
    else if (choice == 2)
    {
        weeklySales = 0;
        printf("Weekly sales reset successfully.\n");
    }
    else if (choice == -1)
    {
        printf("Going back to main menu.\n");
    }
    else
    {
        printf("Invalid choice.\n");
    }
}

/*
 * main
 * Loads saved records at startup, then shows the main menu in a loop
 * until the user chooses -1. Records are saved automatically on exit.
 */
int main()
{
    int choice;

    loadRecords();   /* Restore data from the previous run (if any) */

    do
    {
        printf("\n\n");
        printf("========================================\n");
        printf("       PHARMACY MANAGEMENT SYSTEM       \n");
        printf("========================================\n");

        printf("1.  Add Medicine\n");
        printf("2.  Display All Medicines\n");
        printf("3.  Search Medicine\n");
        printf("4.  Update Medicine\n");
        printf("5.  Sell Medicine\n");
        printf("6.  Low Stock Alert\n");
        printf("7.  Sales Report\n");
        printf("8.  Save Records\n");
        printf("9.  Load Records\n");
        printf("10. Delete Medicine\n");
        printf("11. Reset Daily/Weekly Sales\n");

        printf("----------------------------------------\n");
        printf("-1. Exit Program\n");
        printf("========================================\n");

        printf("Enter Your Choice: ");
        scanf("%d", &choice);

        /* Call the function that matches the user's choice */
        switch (choice)
        {
            case 1:
                addMedicine();
                break;

            case 2:
                displayMedicines();
                break;

            case 3:
                searchMedicine();
                break;

            case 4:
                updateMedicine();
                break;

            case 5:
                sellMedicine();
                break;

            case 6:
                lowStockAlert();
                break;

            case 7:
                salesReport();
                break;

            case 8:
                saveRecords();
                break;

            case 9:
                loadRecords();
                break;

            case 10:
                deleteMedicine();
                break;

            case 11:
                resetSales();
                break;

            case -1:
                saveRecords();   /* Auto-save before quitting */
                printf("\nThank you for using the Pharmacy Management System!\n");
                break;

            default:
                printf("\nInvalid choice. Please try again.\n");
        }

    } while (choice != -1);   /* Repeat until the user exits */

    return 0;
}