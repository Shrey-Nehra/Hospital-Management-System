#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Record
{
    int PatientID;
    char name[50];
    int age;
    char disease[50];
    int doctorID;
    int priority; // 1-Emergency, 2-Normal
    float bill;
    int bill_status; // 1-Paid, 2-Have To Pay
    int status;      // 1-Admitted, 2-Discharged
    struct Record *next;
};

struct Record *head = NULL;
struct Record *tail = NULL; 
struct Record *lastEmergency = NULL;
int lastPatientID;
char ADMIN_CODE[13] = "dElEtErEcOrD";
int serial;
int total, totalActive;

struct Record *createNode()
{
    struct Record *newPatient = (struct Record *)malloc(sizeof(struct Record));
    if (!newPatient)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newPatient->next = NULL;
    return newPatient;
}

int generatePatientID()
{
    int maxSerial = 0;

    time_t t;
    struct tm *date;
    time(&t);
    date = localtime(&t);

    int day = date->tm_mday;
    int month = date->tm_mon + 1;
    int year = (date->tm_year + 1900) % 100;

    int today = day * 10000000 + month * 100000 + year * 1000;

    struct Record *ptr = head;

    while (ptr != NULL)
    {
        if (ptr->PatientID / 1000 == today / 1000)
        {
            int serial = ptr->PatientID % 1000;

            if (serial > maxSerial)
                maxSerial = serial;
        }
        ptr = ptr->next;
    }

    return today + maxSerial + 1;
}
void readFileData()
{
    FILE *fp;
    fp = fopen("records.txt", "r");

    if (fp == NULL)
    {
        printf("FILE NOT FOUND!\n");
        return;
    }
    struct Record *newPatient, *ptr;
    while (1)
    {
        newPatient = (struct Record *)malloc(sizeof(struct Record));
        if (!newPatient)
        {
            printf("Memory allocation failed\n");
            exit(1);
        }

        if (fscanf(fp, "%d|%49[^|]|%d|%49[^|]|%d|%d|%f|%d|%d", &newPatient->PatientID, newPatient->name, &newPatient->age,
             newPatient->disease, &newPatient->doctorID, &newPatient->priority,
             &newPatient->bill, &newPatient->bill_status, &newPatient->status) != 9)
        {
            free(newPatient);
            break;
        }
        newPatient->next = NULL;

        if (head == NULL)
        {
            head = newPatient;
            tail = newPatient;
        }
        else
        {
            tail->next = newPatient;
            tail = newPatient;
        }
        if (newPatient->priority == 1)
        {
            lastEmergency = newPatient;
        }
    }
    fclose(fp);
    printf("RECORDS FETCHED SUCCESSFULLY.\n");
}

void addEmergency(struct Record *newPatient);
void addNormal(struct Record *newPatient);

void addNewPatient()
{
    struct Record *newPatient = createNode();
    newPatient->PatientID = generatePatientID();

    printf("Your ID is: %d\n", newPatient->PatientID);

    printf("Enter the NAME of the patient: ");
    scanf(" %[^\n]", newPatient->name);

    printf("Enter the AGE of the patient: ");
    scanf("%d", &newPatient->age);

    printf("Enter the DISEASE the patient is suffering from: ");
    scanf(" %[^\n]", newPatient->disease);

    printf("Enter the ID of the doctor assigned: ");
    scanf("%d", &newPatient->doctorID);

    printf("Enter the priority of the patient(1-Emergency and 2-Normal): ");
    scanf("%d", &newPatient->priority);
    if (newPatient->priority != 1 && newPatient->priority != 2)
    {
        printf("INVALID INPUT, TRY AGAIN!\n");
        free(newPatient);
        return;
    }

    printf("Enter the amount to be paid by the Patient: ");
    scanf("%f", &newPatient->bill);

    printf("Enter the bill status(1-Paid & 2-Have to pay): ");
    scanf("%d", &newPatient->bill_status);
    if (newPatient->bill_status != 1 && newPatient->bill_status != 2)
    {
        printf("INVALID INPUT, TRY AGAIN!\n");
        free(newPatient);
        return;
    }

    newPatient->status = 1;

    if (newPatient->priority == 1)
    {
        addEmergency(newPatient);
    }
    else if (newPatient->priority == 2)
    {
        addNormal(newPatient);
    }
    total++;
    totalActive++;
}

void addEmergency(struct Record *newPatient)
{
    if (head == NULL)
    {
        head = newPatient;
        tail = newPatient;   
        lastEmergency = newPatient;
    }

    else if (lastEmergency == NULL) 
// It means that this is the first emergency patient, there are other patients but not in emergency.
    // And thus this patient's record will be inserted at the start of the linked list.
    {
        newPatient->next = head;     
        head = newPatient;
        lastEmergency = newPatient;
    }
    else
    {
        newPatient->next = lastEmergency->next;
        lastEmergency->next = newPatient;
        lastEmergency = newPatient;
    }

    if (lastEmergency == tail)
    {
        tail = newPatient; // If the list is only of emergency patients, tail should also move.
    }

    printf("PATIENT RECORD ADDED TO EMERGENCY SUCCESSFULLY!\n");
}

void addNormal(struct Record *newPatient)
{
    if (head == NULL)
    {
        head = newPatient;
        tail = newPatient;
        printf("PATIENT RECORD ADDED SUCCESSFULLY!\n");
        return;
    }
    tail->next = newPatient;
    tail = tail->next;
    printf("PATIENT RECORD ADDED SUCCESSFULLY!\n");
}

void displayRecord(struct Record *ptr)
{
    if (ptr == NULL)
    {
        printf("Pointer is empty!\n");
        return;
    }
    else
    {
        printf("PatientID: %9d  ", ptr->PatientID);
        printf("Name: %s  ", ptr->name);
        printf("Age: %d  ", ptr->age);
        printf("Disease: %s  ", ptr->disease);
        printf("DoctorID: %03d  ", ptr->doctorID);
        if (ptr->priority == 1)
        {
            printf("Emergency  ");
        }
        else
        {
            printf("Normal  ");
        }
        printf("Bill: %f", ptr->bill);
        if (ptr->bill_status == 1)
        {
            printf("-Paid  ");
        }
        else
        {
            printf("-Have to Pay  ");
        }

        if (ptr->status == 1)
        {
            printf("Admitted\n");
        }
        else
        {
            printf("Discharged\n");
        }
    }
}

void displayAll()
{
    printf("\n");
    struct Record *ptr = head;
    while (ptr != NULL)
    {
        displayRecord(ptr);

        ptr = ptr->next;
    }
    printf("\n");
}

void displayActive()
{
    printf("\n");
    int found = 0;
    struct Record *ptr = head;
    if (head == NULL)
    {
        printf("NO RECORDS TO DISPLAY.\n");
    }
    while (ptr != NULL)
    {
        if (ptr->status == 1)
        {
            displayRecord(ptr);
            found = 1;
        }
        ptr = ptr->next;
    }

    if (found == 0)
    {
        printf("NO ACTIVE PATIENT.\n");
    }
    printf("\n");
}

struct Record *searchPatient(int key)
{
    struct Record *ptr = head;
    while (ptr != NULL)
    {
        if (ptr->PatientID == key)
        {
            displayRecord(ptr);
            return ptr;
        }
        ptr = ptr->next;
    }
    printf("Patient not Found!\n");
    ptr = NULL;
    return ptr;
}

void updatePatient(int key)
{
    printf("PRESENT DETAILS OF THE PATIENT: \n");
    struct Record *ptr = searchPatient(key);
    if (ptr == NULL)
    {
        printf("PATIENT NOT FOUND!");
        return;
    }

    else
    {
        int choice2, priority_earlier;
        while (1)
        {
            printf("\n-------UPDATE DETAILS-------\n");
            printf("1. Priority     2. Disease     3. Doctor\n");
            printf("4. Bill Amount     5. Bill Status     6. EXIT\n");
            printf("Select the data you want to change: ");
            scanf("%d", &choice2);
            switch (choice2)
            {
            case 1:
                priority_earlier = ptr->priority;
                printf("Enter the updated priority of the patient(1-Emergency, 2-Normal): ");
                scanf("%d", &ptr->priority);
// There can be two cases: Either patient was in Emergency earlier and now in normal condition
            // OR the patient was in normal condition earlier but now, has an emergency.
                if (priority_earlier == 1 && ptr->priority == 2)
                {
            // It means the patient was in Emergency and now in normal condition..
            // Detatch the patient record from the start of the linked list and add it to the end
                    if (head->PatientID == key)
                    {
                        struct Record *node = head;
                        node->next = NULL;
                        if (head == lastEmergency)
                        {
                            lastEmergency = NULL;
                        }
                        head = head->next;
                        addNormal(node);
                        break;
                    }
                    struct Record *temp = head;
                    while (temp->next != NULL)
                    {

                        if ((temp->next)->PatientID == key)
                        {
                            if ((temp->next == lastEmergency))
                            {
                                if (head == lastEmergency)
                                    lastEmergency = NULL;
                                else
                                    lastEmergency = temp;
                            }

                            struct Record *node = temp->next;
                            node->next = NULL;
                            temp->next = (temp->next)->next;
                            addNormal(node);
                            break;
                        }
                        temp = temp->next;
                    }
                }
                else if (priority_earlier == 2 && ptr->priority == 1)
                {
// It means that the patient was normal before but now has an emergency
// Move the patient record from the end of the linked list
//(end refers to the last few nodes which are for normal patients), 
//to the start of the linked list.
                    if (head->PatientID == key)
                    {
                        if (head == tail)
                        {
                            tail = NULL;
                        }
                        struct Record *node = head;
                        node->next = NULL;
                        head = head->next;
                        addEmergency(node);
                        break;
                    }
                    struct Record *temp = head;
                    while (temp->next != NULL)
                    {
                        if ((temp->next)->PatientID == key)
                        {
                            if (temp->next == tail)
                            {
                                tail = temp;
                            }
                            struct Record *node = temp->next;
                            temp->next = (temp->next)->next;
                            node->next = NULL;
                            addEmergency(node);
                            break;
                        }
                        temp = temp->next;
                    }
                }

                break;
            case 2:
                printf("Enter the updated disease of the patient: ");
                scanf(" %[^\n]", ptr->disease);
                break;
            case 3:
                printf("Enter the ID of the new doctor assigned to the patient: ");
                scanf("%d", &ptr->doctorID);
                break;
            case 4:
                printf("Enter the final bill amount: ");
                scanf("%f", &ptr->bill);
                break;
            case 5:
                printf("Enter the bill status at the time(1-Paid, 2-Have to Pay): ");
                scanf("%d", &ptr->bill_status);
                break;
            case 6:
                return;
            default:
                printf("Invalid Choice!\n");
                return;
            }
        }
        printf("Patient Record updated successfully.\n");
    }
}

void dischargePatient(int key)
{
    struct Record *ptr = searchPatient(key);
    if (ptr == NULL)
    {
        return;
    }

    ptr->status = 2;
    totalActive--;
    printf("Patient Discharged successfully.\n");
}

void displayActivePatientOfDoctor(int key)
{
    struct Record *ptr = head;
    int found = 0;
    while (ptr != NULL)
    {
        if (ptr->doctorID == key && ptr->status == 1)
        {
            displayRecord(ptr);
            found = 1;
        }
        ptr = ptr->next;
    }
    if (found == 0)
    {
        printf("NO PATIENT UNDER THIS DOC.\n");
    }
}

void toLowerCase(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
        {
            str[i] = str[i] + 32;
        }
    }
}

void displayPatientsSufferingFromSpecificDisease(char disease[50])
{
    struct Record *ptr = head;
    int found = 0;

    char input[50];
    strcpy(input, disease);
    toLowerCase(input);

    while (ptr != NULL)
    {
        char temp[50];
        strcpy(temp, ptr->disease);
        toLowerCase(temp);

        if (strcmp(temp, input) == 0)
        {
            displayRecord(ptr);
            found = 1;
        }
        ptr = ptr->next;
    }

    if (found == 0)
    {
        printf("NO PATIENT WITH THIS DISEASE.\n");
    }
}

void displayPatientsWhoseBillNotPaid()
{
    struct Record *ptr = head;
    int found = 0;
    while (ptr != NULL)
    {
        if (ptr->bill_status == 2)
        {
            displayRecord(ptr);
            found = 1;
        }
        ptr = ptr->next;
    }
    if (found == 0)
    {
        printf("NO SUCH PATIENT.\n");
    }
}

void deletePatient()
{
    if (head == NULL)
    {
        printf("NO RECORD FOUND.\n");
        return;
    }
    char code[13];
    int matched = 0;
    printf("Enter the ADMIN_CODE for deleting the record of a patient(YOU HAVE 3 CHANCES): ");
    for (int i = 0; i < 3; i++)
    {
        printf("Attempt %d: ", i + 1);
        scanf("%12s", code);
        if (strcmp(ADMIN_CODE, code) != 0)
        {
            printf("INVALID CODE ENTERED\n");
        }
        else
        {
            matched = 1;
            break;
        }
    }
    if (matched == 0)
    {
        printf("YOU DON'T HAVE THE ACCESS TO DELETE RECORD OF ANY PATIENT!\n");
        return;
    }
    else
    {
        printf("Correct Code!\n");
        int key;
        printf("Enter the patientID of the patient whose record you want to delete: ");
        scanf("%d", &key);
        if (head->PatientID == key)
        {
            int confirm;
            displayRecord(head);
            printf("ARE YOU SURE YOU WANT TO DELETE THIS RECORD??\n");
            printf("Enter 1-Yes and 2-No: ");
            scanf("%d", &confirm);
            if (confirm == 1)
            {
                struct Record *node;
                node = head;
                head = head->next;
                if (head == lastEmergency)
                {
                    lastEmergency = NULL;
                }
                if (head == tail)
                {
                    tail = head;
                }

                total--;
                if (node->status == 1)
                {
                    totalActive--;
                }
                free(node);
                return;
            }
            else if (confirm == 2)
            {
                printf("YOU CANCELLED THE DELETION.\n");
                return;
            }

            else
            {
                printf("INVALID ENTRY.\n");
                return;
            }
        }

        struct Record *ptr = head;
        while (ptr->next != NULL)
        {
            if ((ptr->next)->PatientID == key)
            {
                int confirm;
                displayRecord(ptr->next);
                printf("ARE YOU SURE YOU WANT TO DELETE THIS RECORD??\n");
                printf("Enter 1-Yes and 2-No: ");
                scanf("%d", &confirm);
                if (confirm == 1)
                {
                    struct Record *node;
                    node = ptr->next;
                    if (node == lastEmergency)
                    {
                        lastEmergency = ptr;
                    }
                    if (node == tail)
                    {
                        tail = ptr;
                    }
                    ptr->next = (ptr->next)->next;

                    total--;
                    if (node->status == 1)
                    {
                        totalActive--;
                    }
                    free(node);
                    printf("Patient Record Deleted Successfuly.\n");
                    return;
                }
                else if (confirm == 2)
                {
                    printf("YOU CANCELLED THE DELETION.\n");
                    return;
                }

                else
                {
                    printf("INVALID ENTRY.\n");
                    return;
                }
            }
            ptr = ptr->next;
        }
    }
}

int totalPatients()
{
    int count = 0;
    struct Record *ptr = head;
    while (ptr != NULL)
    {
        count++;
        ptr = ptr->next;
    }
    return count;
}

int totalActivePatients()
{
    int count = 0;
    struct Record *ptr = head;
    while (ptr != NULL)
    {
        if (ptr->status == 1)
        {
            count++;
        }
        ptr = ptr->next;
    }
    return count;
}

void saveAllPatients()
{
    FILE *fp;
    fp = fopen("records.txt", "w");

    if (fp == NULL)
    {
        printf("Failed To open File!\n");
    }
    struct Record *ptr = head;
    while (ptr != NULL)
    {
        fprintf(fp, "%d|%s|%d|%s|%03d|%d|%f|%d|%d\n", ptr->PatientID, 
             ptr->name, ptr->age, ptr->disease, ptr->doctorID, ptr->priority,
             ptr->bill, ptr->bill_status, ptr->status);

        ptr = ptr->next;
    }
    fclose(fp);

    printf("RECORDS SAVED SUCCESSFULLY!\n");
}

int main()
{

    readFileData();
    total = totalPatients();
    totalActive = totalActivePatients();

    int choice, key;
    char disease[50];
    while (1)
    {
        printf("\n--------------------------------------MENU--------------------------------------\n");
        printf("1. Add Patient      2. Display Active Patient      3. Display All Patients\n");
        printf("4. Search Patient        5. Update Patient       6. Discharge Patient\n");
        printf("7. Show Active Patients of a Doctor        8. Display Patients with a specific disease     \n");
        printf("9. Display Patients who haven't paid the bill yet     10. Delete Patient     11. EXIT\n\n");

        printf("SELECT ONE OF THE FUNCTIONS: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addNewPatient();
            break;
        case 2:
            if (head == NULL)
            {
                printf("NO RECORD FOUND.\n");
                break;
            }

            displayActive();
            printf("The total number of active patients is %d.\n", totalActive);
            break;
        case 3:
            if (head == NULL)
            {
                printf("NO RECORD FOUND.\n");
                break;
            }
            displayAll();
            printf("The total number of patients(both active and inactive) are: %d.\n", total);
            break;
        case 4:
            if (head == NULL)
            {
                printf("NO RECORD FOUND.\n");
                break;
            }
            printf("Enter the PatientID of the patient you want to search: ");
            scanf("%d", &key);
            searchPatient(key);
            break;
        case 5:
            if (head == NULL)
            {
                printf("NO RECORD FOUND.\n");
                break;
            }
            printf("YOU CAN ONLY UPDATE: priority, disease, doctorID, bill, bill_status.\n");
            printf("Enter the PatientID of the patient whose details you want to update: ");
            scanf("%d", &key);
            updatePatient(key);
            break;
        case 6:
            if (head == NULL)
            {
                printf("NO RECORD FOUND.\n");
                break;
            }
            printf("Enter the patientID of the patient who is to be discharged: ");
            scanf("%d", &key);
            dischargePatient(key);
            break;
        case 7:
            if (head == NULL)
            {
                printf("NO RECORD FOUND.\n");
                break;
            }
            printf("Enter doctorID: ");
            scanf("%d", &key);
            displayActivePatientOfDoctor(key);
            break;
        case 8:
            if (head == NULL)
            {
                printf("NO RECORD FOUND.\n");
                break;
            }
            printf("Enter the disease: ");
            scanf(" %[^\n]", disease);
            displayPatientsSufferingFromSpecificDisease(disease);
            break;
        case 9:
            if (head == NULL)
            {
                printf("NO RECORD FOUND.\n");
                break;
            }
            displayPatientsWhoseBillNotPaid();
            break;
        case 10:
            if (head == NULL)
            {
                printf("NO RECORD FOUND.\n");
                break;
            }
            deletePatient();
            break;
        case 11:
            saveAllPatients();
            exit(0);
            break;

        default:
            printf("INVALID CHOICE!");
            break;
        }
    }

    return 0;
}