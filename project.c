#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define constants
#define MAX_USERS 100
#define MAX_TESTS 5
#define MAX_APPOINTMENTS 50
#define MAX_SYMPTOMS 6
#define MAX_TEST_PER_APPOINTMENT 5
#define THRESHOLD 3
#define MAX_STRING_LENGTH 50
#define MAX_DISTANCE 999

// User structure
typedef struct {
    char username[MAX_STRING_LENGTH];
    char password[MAX_STRING_LENGTH];
    char role[10];
    char name[MAX_STRING_LENGTH];
    char contact[15];
} User;

// Test structure
typedef struct {
    char testName[MAX_STRING_LENGTH];
    float testFee;
} Test;

// Appointment structure
typedef struct {
    char patientName[MAX_STRING_LENGTH];
    char doctorName[MAX_STRING_LENGTH];
    char date[20];
    char testNames[MAX_TEST_PER_APPOINTMENT][MAX_STRING_LENGTH];
    float testFees[MAX_TEST_PER_APPOINTMENT];
    int testCount;
    int isPaid;
} Appointment;

// Prescription structure
typedef struct {
    char patientName[MAX_STRING_LENGTH];
    char doctorName[MAX_STRING_LENGTH];
    char date[20];
    char medicines[200];
} Prescription;

// Global variables
User users[MAX_USERS];
int userCount = 0;
User currentUser;
Test tests[MAX_TESTS] = {
    {"Blood Test", 200},
    {"X-ray", 500},
    {"CT Scan", 1500},
    {"MRI", 2500},
    {"ECG", 300}
};
Appointment testAppointments[MAX_APPOINTMENTS];
Appointment doctorAppointments[MAX_APPOINTMENTS];
Prescription prescriptions[MAX_APPOINTMENTS];
int testAppointmentCount = 0;
int doctorAppointmentCount = 0;
int prescriptionCount = 0;
float totalIncome = 0.0;

// Function prototypes
void trim_whitespace(char *str);
int min(int a, int b);
void generate_prescription();
void userRegistration();
void userAuthentication();
void profileUpdate();
void testSelection();
void doctorAppointmentBooking();
void testAppointmentBooking();
void paymentSystem();
void doctorAppointmentList();
void testPriceUpdate();
void userManagement();
void reportGeneration();
void showMenuBasedOnRole();
void viewTotalIncome();
int countPaidTestAppointments();
int countPaidDoctorAppointments();
void addPrescription();

// Function implementations
void trim_whitespace(char *str) {
    int start = 0, end = strlen(str) - 1;
    while (isspace(str[start]))
        start++;
    while (end > start && isspace(str[end]))
        end--;

    str[end + 1] = '\0';
    memmove(str, str + start, end - start + 2);
}

int min(int a, int b) { 
    return (a < b) ? a : b; 
}

int countPaidTestAppointments() {
    int count = 0;
    for (int i = 0; i < testAppointmentCount; i++) {
        if (testAppointments[i].isPaid) {
            count++;
        }
    }
    return count;
}

int countPaidDoctorAppointments() {
    int count = 0;
    for (int i = 0; i < doctorAppointmentCount; i++) {
        if (doctorAppointments[i].isPaid) {
            count++;
        }
    }
    return count;
}

void userRegistration() {
    printf("\n[User Registration]\n");
    User newUser;
    
    printf("Enter username: ");
    fgets(newUser.username, MAX_STRING_LENGTH, stdin);
    newUser.username[strcspn(newUser.username, "\n")] = '\0';
    
    printf("Enter password: ");
    fgets(newUser.password, MAX_STRING_LENGTH, stdin);
    newUser.password[strcspn(newUser.password, "\n")] = '\0';
    
    printf("Enter role (admin/doctor/patient): ");
    fgets(newUser.role, 10, stdin);
    newUser.role[strcspn(newUser.role, "\n")] = '\0';
    
    printf("Enter full name: ");
    fgets(newUser.name, MAX_STRING_LENGTH, stdin);
    newUser.name[strcspn(newUser.name, "\n")] = '\0';
    
    printf("Enter contact: ");
    fgets(newUser.contact, 15, stdin);
    newUser.contact[strcspn(newUser.contact, "\n")] = '\0';

    users[userCount++] = newUser;
    printf("Registration successful!\n");
}

void userAuthentication() {
    printf("\n[User Authentication]\n");
    char username[MAX_STRING_LENGTH], password[MAX_STRING_LENGTH];
    
    printf("Enter username: ");
    fgets(username, MAX_STRING_LENGTH, stdin);
    username[strcspn(username, "\n")] = '\0';
    
    printf("Enter password: ");
    fgets(password, MAX_STRING_LENGTH, stdin);
    password[strcspn(password, "\n")] = '\0';

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            currentUser = users[i];
            printf("Login successful! Welcome, %s\n", currentUser.name);
            return;
        }
    }
    printf("Invalid username or password!\n");
}

void profileUpdate() {
    printf("\n[Profile Update]\n");
    printf("Enter new contact: ");
    char newContact[15];
    fgets(newContact, 15, stdin);
    newContact[strcspn(newContact, "\n")] = '\0';
    strcpy(currentUser.contact, newContact);
    printf("Profile updated!\n");
}

void testSelection() {
    printf("\n[Available Tests]\n");
    for (int i = 0; i < MAX_TESTS; i++) {
        printf("%d. %s - ৳%.2f\n", i + 1, tests[i].testName, tests[i].testFee);
    }
}

void doctorAppointmentBooking() {
    printf("\n[Doctor Appointment Booking]\n");
    if (doctorAppointmentCount >= MAX_APPOINTMENTS) {
        printf("Appointment list is full!\n");
        return;
    }

    Appointment newAppointment;
    
    printf("Enter doctor's name: ");
    fgets(newAppointment.doctorName, MAX_STRING_LENGTH, stdin);
    newAppointment.doctorName[strcspn(newAppointment.doctorName, "\n")] = '\0';
    
    printf("Enter date (dd-mm-yyyy): ");
    fgets(newAppointment.date, 20, stdin);
    newAppointment.date[strcspn(newAppointment.date, "\n")] = '\0';

    strcpy(newAppointment.patientName, currentUser.name);
    newAppointment.isPaid = 0;
    newAppointment.testCount = 0;

    doctorAppointments[doctorAppointmentCount++] = newAppointment;
    printf("Doctor appointment booked!\n");
}

void testAppointmentBooking() {
    printf("\n[Test Appointment Booking]\n");
    if (testAppointmentCount >= MAX_APPOINTMENTS) {
        printf("Appointment list is full!\n");
        return;
    }

    Appointment newAppointment;
    printf("Enter date (dd-mm-yyyy): ");
    fgets(newAppointment.date, 20, stdin);
    newAppointment.date[strcspn(newAppointment.date, "\n")] = '\0';

    strcpy(newAppointment.patientName, currentUser.name);
    newAppointment.isPaid = 0;
    newAppointment.testCount = 0;

    // Show available tests
    printf("\nAvailable Tests:\n");
    for (int i = 0; i < MAX_TESTS; i++) {
        printf("%d. %s - ৳%.2f\n", i + 1, tests[i].testName, tests[i].testFee);
    }

    // Get multiple test selections
    printf("\nEnter test numbers to book (space separated, e.g. '1 3 5'): ");
    char input[100];
    fgets(input, sizeof(input), stdin);

    // Process selected tests
    char *token = strtok(input, " ");
    while (token != NULL && newAppointment.testCount < MAX_TEST_PER_APPOINTMENT) {
        int testIndex = atoi(token);
        if (testIndex >= 1 && testIndex <= MAX_TESTS) {
            strcpy(newAppointment.testNames[newAppointment.testCount], tests[testIndex - 1].testName);
            newAppointment.testFees[newAppointment.testCount] = tests[testIndex - 1].testFee;
            newAppointment.testCount++;
        }
        token = strtok(NULL, " ");
    }

    if (newAppointment.testCount > 0) {
        testAppointments[testAppointmentCount++] = newAppointment;
        printf("\nTest appointment booked with %d tests:\n", newAppointment.testCount);
        for (int i = 0; i < newAppointment.testCount; i++) {
            printf("- %s (৳%.2f)\n", newAppointment.testNames[i], newAppointment.testFees[i]);
        }
    } else {
        printf("No valid tests selected. Appointment not booked.\n");
    }
}

void paymentSystem() {
    printf("\n[Payment System]\n");
    float totalDue = 0.0;
    int unpaidCount = 0;

    for (int i = 0; i < testAppointmentCount; i++) {
        if (strcmp(testAppointments[i].patientName, currentUser.name) == 0 && !testAppointments[i].isPaid) {
            printf("Appointment on %s with tests:\n", testAppointments[i].date);
            float appointmentTotal = 0.0;
            for (int j = 0; j < testAppointments[i].testCount; j++) {
                printf("- %s: ৳%.2f\n", testAppointments[i].testNames[j], testAppointments[i].testFees[j]);
                appointmentTotal += testAppointments[i].testFees[j];
            }
            printf("Total for this appointment: ৳%.2f\n", appointmentTotal);
            totalDue += appointmentTotal;
            unpaidCount++;
        }
    }

    if (unpaidCount == 0) {
        printf("No unpaid test appointments found.\n");
        return;
    }

    printf("\nTotal amount due: ৳%.2f\n", totalDue);
    printf("Pay now? (1 for Yes, 0 for No): ");
    int choice;
    scanf("%d", &choice);
    getchar();

    if (choice == 1) {
        for (int i = 0; i < testAppointmentCount; i++) {
            if (strcmp(testAppointments[i].patientName, currentUser.name) == 0 && !testAppointments[i].isPaid) {
                testAppointments[i].isPaid = 1;
                for (int j = 0; j < testAppointments[i].testCount; j++) {
                    totalIncome += testAppointments[i].testFees[j];
                }
            }
        }
        printf("Payment of ৳%.2f successful!\n", totalDue);
    } else {
        printf("Payment cancelled.\n");
    }
}

void doctorAppointmentList() {
    printf("\n[Doctor Appointments List]\n");

    if (doctorAppointmentCount == 0) {
        printf("No appointments found.\n");
        return;
    }

    for (int i = 0; i < doctorAppointmentCount; i++) {
        if (strcmp(currentUser.role, "admin") == 0 || strcmp(doctorAppointments[i].doctorName, currentUser.name) == 0) {
            printf("Appointment #%d:\n", i + 1);
            printf("- Patient: %s\n", doctorAppointments[i].patientName);
            printf("- Doctor: %s\n", doctorAppointments[i].doctorName);
            printf("- Date: %s\n", doctorAppointments[i].date);
        }
    }
}

void testPriceUpdate() {
    printf("\n[Test Price Update]\n");
    for (int i = 0; i < MAX_TESTS; i++) {
        printf("%d. %s - ৳%.2f\n", i + 1, tests[i].testName, tests[i].testFee);
    }
    printf("Select a test to update price: ");
    int testIndex;
    scanf("%d", &testIndex);
    getchar();
    
    if (testIndex < 1 || testIndex > MAX_TESTS) {
        printf("Invalid selection!\n");
        return;
    }
    
    printf("Enter new price: ");
    float newPrice;
    scanf("%f", &newPrice);
    getchar();
    
    tests[testIndex - 1].testFee = newPrice;
    printf("Test price updated!\n");
}

void userManagement() {
    printf("\n[User Management]\n");
    for (int i = 0; i < userCount; i++) {
        printf("%d. %s (%s)\n", i + 1, users[i].name, users[i].role);
    }
}

void reportGeneration() {
    printf("\n[Report Generation]\n");
    printf("Total Users: %d\n", userCount);
    printf("Total Doctor Appointments: %d\n", doctorAppointmentCount);
    printf("Total Test Appointments: %d\n", testAppointmentCount);
    printf("Total Completed Doctor Appointments: %d\n", countPaidDoctorAppointments());
    printf("Total Completed Test Appointments: %d\n", countPaidTestAppointments());
    printf("Total Income: ৳%.2f\n", totalIncome);
}

void viewTotalIncome() {
    printf("\n[Total Income Report]\n");
    printf("Total Income from Patient Bills: ৳%.2f\n", totalIncome);
    
    float testIncome[MAX_TESTS] = {0};
    for (int i = 0; i < testAppointmentCount; i++) {
        if (testAppointments[i].isPaid) {
            for (int j = 0; j < testAppointments[i].testCount; j++) {
                for (int k = 0; k < MAX_TESTS; k++) {
                    if (strcmp(testAppointments[i].testNames[j], tests[k].testName) == 0) {
                        testIncome[k] += testAppointments[i].testFees[j];
                        break;
                    }
                }
            }
        }
    }

    for (int i = 0; i < MAX_TESTS; i++) {
        if (testIncome[i] > 0) {
            printf("- %s: ৳%.2f\n", tests[i].testName, testIncome[i]);
        }
    }
}

void addPrescription() {
    printf("\n[Add Prescription]\n");

    if (prescriptionCount >= MAX_APPOINTMENTS) {
        printf("Cannot add more prescriptions. Database full!\n");
        return;
    }
    
    Prescription newPres;
    
    printf("Enter patient name: ");
    fgets(newPres.patientName, MAX_STRING_LENGTH, stdin);
    newPres.patientName[strcspn(newPres.patientName, "\n")] = '\0';
    
    strcpy(newPres.doctorName, currentUser.name);

    printf("Enter date (dd-mm-yyyy): ");
    fgets(newPres.date, 20, stdin);
    newPres.date[strcspn(newPres.date, "\n")] = '\0';

    printf("Enter medicines: ");
    fgets(newPres.medicines, 200, stdin);
    newPres.medicines[strcspn(newPres.medicines, "\n")] = '\0';

    prescriptions[prescriptionCount++] = newPres;
    printf("\nPrescription added successfully!\n");
}

void showMenuBasedOnRole() {
    int choice;
    while (1) {
        printf("\n==================================\n");
        printf(" Healthcare System Management\n");
        printf("==================================\n");

        if (strcmp(currentUser.role, "patient") == 0) {
            printf("Patient Portal - Welcome %s\n", currentUser.name);
            printf("1. Profile Update\n");
            printf("2. View Available Tests\n");
            printf("3. Book Doctor Appointment\n");
            printf("4. Book Test Appointment\n");
            printf("5. Payment System\n");
            printf("6. Logout\n");
        }
        else if (strcmp(currentUser.role, "doctor") == 0) {
            printf("Doctor Portal - Welcome, Dr. %s\n", currentUser.name);
            printf("1. Appointment List\n");
            printf("2. Add Prescription\n");
            printf("3. Logout\n");
        }
        else if (strcmp(currentUser.role, "admin") == 0) {
            printf("Admin Portal - Welcome %s\n", currentUser.name);
            printf("1. Test Price Update\n");
            printf("2. User Management\n");
            printf("3. Report Generation\n");
            printf("4. View Total Income\n");
            printf("5. Appointment List\n");
            printf("6. Logout\n");
        }

        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        if (strcmp(currentUser.role, "patient") == 0) {
            switch (choice) {
                case 1: profileUpdate(); break;
                case 2: testSelection(); break;
                case 3: doctorAppointmentBooking(); break;
                case 4: testAppointmentBooking(); break;
                case 5: paymentSystem(); break;
                case 6: return;
                default: printf("Invalid choice!\n");
            }
        }
        else if (strcmp(currentUser.role, "doctor") == 0) {
            switch (choice) {
                case 1: doctorAppointmentList(); break;
                case 2: addPrescription(); break;
                case 3: return;
                default: printf("Invalid choice!\n");
            }
        }
        else if (strcmp(currentUser.role, "admin") == 0) {
            switch (choice) {
                case 1: testPriceUpdate(); break;
                case 2: userManagement(); break;
                case 3: reportGeneration(); break;
                case 4: viewTotalIncome(); break;
                case 5: doctorAppointmentList(); break;
                case 6: return;
                default: printf("Invalid choice!\n");
            }
        }
    }
}

int main() {
    int choice;
    while (1) {
        printf("\n==================================\n");
        printf(" Healthcare System Management\n");
        printf("==================================\n\n");
        printf("We have 3 different Portal for (Patient, Doctor, Admin). Reg who you are.\n\n");
        printf("1. User Registration\n");
        printf("2. User Authentication(Login)\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: userRegistration(); break;
            case 2: 
                userAuthentication(); 
                if (strlen(currentUser.username) > 0) {
                    showMenuBasedOnRole();
                }
                break;
            case 3: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}