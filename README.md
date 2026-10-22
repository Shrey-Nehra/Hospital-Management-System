# Hospital Management System in C

A menu-driven Hospital Management System developed in C to manage patient records using a singly linked list and file handling. The project supports patient registration, emergency prioritization, record management, billing status tracking, and persistent storage.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Data Structure and Implementation](#data-structure-and-implementation)
- [Record Storage Format](#record-storage-format)
- [Technologies Used](#technologies-used)
- [Compilation and Execution](#compilation-and-execution)
- [Limitations](#limitations)
- [Future Improvements](#future-improvements)

## Overview

The Hospital Management System is a console-based application designed to organize and manage patient information. It maintains patient records in a singly linked list and uses file handling to save records to a text file so they can be loaded again in a later execution.

Each patient record contains identification details, medical information, an assigned doctor, priority, bill amount, bill status, and admission status.

The program provides a menu through which users can perform different operations on patient records.

## Features

### Patient Management

- Register new patients and generate patient IDs using the system date.
- Display all patient records or only active patients.
- Search for a patient using their patient ID.
- Update priority, disease, assigned doctor, bill amount, and bill status.
- Discharge patients while retaining their records.
- Delete patient records after admin-code verification and confirmation.

### Emergency Prioritization

- Maintain emergency patients ahead of normal patients in the linked list.
- Insert new emergency patients after existing emergency patients.
- Add normal patients to the end of the list.
- Change a patient's position when their priority is changed from emergency to normal or normal to emergency.

### Search and Filtering

- Display active patients assigned to a particular doctor.
- Find patients by disease, without distinguishing uppercase and lowercase letters.
- Display patients whose bills are marked as unpaid.

### File Handling

- Load previously saved patient records from `records.txt` at startup and create a linked list of the previous records.
- Store patient information in a pipe-separated text format.
- Save the current patient records to records.txt when the user selects the exit option.

## Data Structure and Implementation

### Singly Linked List

Each patient is represented by a structure containing the patient's details and a pointer to the next record.

The program maintains three pointers:

- `head`: points to the first patient record.
- `tail`: points to the last patient record.
- `lastEmergency`: tracks the last emergency patient, helping the program insert new emergency patients in the correct position.

Dynamic memory allocation is used to create patient records. The linked list supports insertion, traversal, searching, updating, and deletion.

### Patient Priority

Priority is represented using integer values:

- `1` — Emergency
- `2` — Normal

Emergency patients are placed before normal patients. When a patient's priority changes, the program adjusts their position in the linked list.

### Admission and Billing Status

The program tracks whether a patient is admitted or discharged, along with whether their bill is paid or still pending. Discharged patients remain in the records unless explicitly deleted.

## Record Storage Format

Patient records are stored in `records.txt`. Each patient occupies one line, and the fields are separated by the pipe (`|`) character.

### Example

```text
180426001|Sample Patient|23|Fever|004|1|399.000000|2|1
```

This is an illustrative record showing the format; it is not a real patient record.

### Field Description

| Position | Field          | Description                                                         |
| -------- | -------------- | ------------------------------------------------------------------- |
| 1        | Patient ID     | Patient identifier generated using the system date.                 |
| 2        | Name           | Patient's name                                                      |
| 3        | Age            | Patient's age                                                       |
| 4        | Disease        | Recorded disease or condition                                       |
| 5        | Doctor ID      | ID of the assigned doctor                                           |
| 6        | Priority       | `1` = Emergency, `2` = Normal                                       |
| 7        | Bill amount    | Amount associated with the patient's bill                           |
| 8        | Bill status    | `1` = Paid, `2` = Have to Pay                                       |
| 9        | Patient status | `1` = Admitted, `2` = Discharged                                    |

The program uses formatted file input to read these fields in order and reconstruct the linked list. When saving, it writes the fields using the same separator-based format.

## Technologies Used

- **Language:** C
- **Data Structures:** Singly linked list
- **Memory Management:** Dynamic memory allocation using `malloc()` and `free()`
- **File Handling:** File reading and writing using standard C functions
- **Libraries:** `stdio.h`, `stdlib.h`, `string.h`, `time.h`

## Compilation and Execution

### Prerequisites

Install GCC or another compatible C compiler.

### Compile

```bash
gcc HMS.c -o HMS
```

### Run on Windows

```powershell
.\HMS.exe
```

### Run on Linux/macOS

```bash
./HMS
```

The program uses `records.txt` as its data file. Run the program from the directory where this file is located if you want it to load existing records.

## Limitations

- The application runs in the console and does not provide a graphical user interface.
- The program uses a basic admin-code check for record deletion; it is not a production-grade authentication system.
- Records are stored in a plain-text file rather than a database.
- Input validation and data-storage robustness could be improved for broader use.
- Inefficient File Updates: The program rewrites all patient records to records.txt whenever it saves, even if some records remain unchanged. This causes unnecessary file operations.

## Future Improvements

- **Graphical User Interface (GUI):** Develop a user-friendly graphical interface to replace the current console-based menu.
- **Role-Based Login and Access Control:** Introduce separate login pages and dashboards for patients, receptionists, doctors, and hospital administrators.
  - **Patients:** View their own medical details, prescribed medicines, doctor information, upcoming appointments, and billing details.
  - **Receptionists:** Register patients, manage appointments, and add or update patient records according to their permissions.
  - **Doctors:** Access relevant patient medical records, view assigned patients, and manage prescriptions and follow-up appointments.
  - **Hospital Administrators:** Manage hospital records, users, staff, billing information, and other administrative operations with appropriate permissions.
- **Database Integration:** Replace plain-text file storage with a database such as MySQL or SQLite for more structured data management.
- **Enhanced Security:** Implement secure authentication, role-based authorization, and appropriate protection for sensitive medical information.
- **Improved Input Validation and Error Handling:** Handle invalid inputs and file-operation failures more reliably.
- **Extended Billing and Reporting:** Add detailed invoices, payment history, and administrative reports.

## Disclaimer

This is an academic project intended to demonstrate C programming, linked-list operations, dynamic memory allocation, and file handling. It is not designed for managing real patient data.
