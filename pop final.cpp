 #include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DAYS 31
#define MAX_NOTE_LENGTH 200
#define CREDENTIALS_FILE "credentials.txt"

typedef struct {
    char date[11]; // Format: YYYY-MM-DD
    int calories;
    int steps;
    float water; // in liters
    char note[MAX_NOTE_LENGTH]; // Note as a paragraph
} DailyRecord;

DailyRecord records[MAX_DAYS];
int recordCount = 0;

void registerUser() {
    FILE *file = fopen(CREDENTIALS_FILE, "a");
    if (file == NULL) {
        printf("Error opening file for registration.\n");
        return;
    }
    
    char username[50], password[50];
    printf("Enter username for registration: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);
    
    fprintf(file, "%s %s\n", username, password);
    fclose(file);
    printf("Registration successful! You can now log in.\n");
}

int loginUser() {
    FILE *file = fopen(CREDENTIALS_FILE, "r");
    if (file == NULL) {
        printf("Error opening file for login. Make sure you register first.\n");
        return 0;
    }
    
    char username[50], password[50], fileUser[50], filePass[50];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);
    
    while (fscanf(file, "%s %s", fileUser, filePass) != EOF) {
        if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
            printf("Login successful!\n");
            fclose(file);
            return 1;
        }
    }
    
    fclose(file);
    printf("Invalid username or password.\n");
    return 0;
}

void addDailyRecord() {
    if (recordCount >= MAX_DAYS) {
        printf("Record list is full for the month!\n");
        return;
    }
    
    DailyRecord newRecord;
    printf("Enter date (YYYY-MM-DD): ");
    scanf("%s", newRecord.date);
    printf("Enter calories burnt: ");
    scanf("%d", &newRecord.calories);
    printf("Enter steps count: ");
    scanf("%d", &newRecord.steps);
    printf("Enter water intake (in liters): ");
    scanf("%f", &newRecord.water);
    getchar(); // To consume the newline character left by scanf
    printf("Enter a note for the day: ");
    fgets(newRecord.note, MAX_NOTE_LENGTH, stdin);
    newRecord.note[strcspn(newRecord.note, "\n")] = '\0'; // Remove trailing newline
    
    records[recordCount++] = newRecord;
    printf("Daily record added successfully!\n");
}

void viewRecords() {
    if (recordCount == 0) {
        printf("No records found.\n");
        return;
    }

    // Table Header
    printf("\n+------------+----------+-------+----------+------------------------------+\n");
    printf("| Date       | Calories | Steps | Water (L)| Note                         |\n");
    printf("+------------+----------+-------+----------+------------------------------+\n");

    // Table Rows
    for (int i = 0; i < recordCount; i++) {
        printf("| %-10s | %-8d | %-5d | %-8.2f | %-28s |\n",
               records[i].date,
               records[i].calories,
               records[i].steps,
               records[i].water,
               records[i].note);
    }

    // Table Footer
    printf("+------------+----------+-------+----------+------------------------------+\n");
}
void calculateMonthlyAverage() {
    if (recordCount == 0) {
        printf("No records available to calculate averages.\n");
        return;
    }
    
    int totalCalories = 0, totalSteps = 0;
    float totalWater = 0.0;
    
    for (int i = 0; i < recordCount; i++) {
        totalCalories += records[i].calories;
        totalSteps += records[i].steps;
        totalWater += records[i].water;
    }
    
    printf("\nMonthly Averages:\n");
    printf("Calories: %.2f\n", (float)totalCalories / recordCount);
    printf("Steps: %.2f\n", (float)totalSteps / recordCount);
    printf("Water: %.2f L\n", totalWater / recordCount);
}

void exportToFile() {
    FILE *file = fopen("activity_records.txt", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    
    fprintf(file, "Date\t\tCalories\tSteps\tWater (L)\tNote\n");
    fprintf(file, "---------------------------------------------------------------\n");
    for (int i = 0; i < recordCount; i++) {
        fprintf(file, "%s\t%d\t\t%d\t%.2f\t%s\n",
                records[i].date,
                records[i].calories,
                records[i].steps,
                records[i].water,
                records[i].note);
    }
    
    fclose(file);
    printf("Records exported to 'activity_records.txt' successfully.\n");
}

int main() {
    int choice;
    int loggedIn = 0;
    
    printf("Welcome to the Personal Activity Tracker!\n");
    printf("1. Register\n");
    printf("2. Login\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1: 
            registerUser();
            return 0;
        case 2: 
            loggedIn = loginUser();
            break;
        default: 
            printf("Invalid choice. Exiting.\n");
            return 0;
    }
    
    if (!loggedIn) {
        return 0; // Exit if login fails
    }
    
    do {
        printf("\nPersonal Activity Tracker\n");
        printf("1. Add Daily Record\n");
        printf("2. View Records\n");
        printf("3. Calculate Monthly Averages\n");
        printf("4. Export Records to File\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: addDailyRecord(); break;
            case 2: viewRecords(); break;
            case 3: calculateMonthlyAverage(); break;
            case 4: exportToFile(); break;
            case 5: printf("Exiting program.\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 5);
    
    return 0;
}