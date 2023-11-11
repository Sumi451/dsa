#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Contact {
    char name[50];
    char number[15];
    struct Contact *next;
    struct Contact *prev;
};

struct Contact *head = NULL;

bool isAdmin = false;

bool adminLogin() {
    char adminUsername[] = "admin";
    char adminPassword[] = "admin123";
    char username[50], password[50];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    if (strcmp(username, adminUsername) == 0 && strcmp(password, adminPassword) == 0) {
        isAdmin = true;
        return true;
    } else {
        printf("Incorrect username or password\n");
        return false;
    }
}

bool userLogin() {
    printf("Logged in as a User.\n");
    return true;
}

void addContact() {
    struct Contact *newContact = (struct Contact *)malloc(sizeof(struct Contact));
    printf("Enter name: ");
    scanf("%s", newContact->name);
    printf("Enter number: ");
    scanf("%s", newContact->number);

    if (head == NULL) {
        newContact->next = newContact->prev = newContact;
        head = newContact;
    } else {
        struct Contact *last = head->prev;
        last->next = newContact;
        newContact->prev = last;
        newContact->next = head;
        head->prev = newContact;
    }
}

void displayContacts() {
    struct Contact *temp = head;
    if (temp == NULL) {
        printf("Phone book is empty!\n");
    } else {
        printf("Contacts:\n");
        do {
            printf("Name: %s, Number: %s\n", temp->name, temp->number);
            temp = temp->next;
        } while (temp != head);
    }
}

void saveContactsToFile() {
    FILE *fp = fopen("phonebook.txt", "w");
    if (fp == NULL) {
        printf("Error creating file.\n");
        return;
    }

    struct Contact *temp = head;
    do {
        fprintf(fp, "%s,%s\n", temp->name, temp->number);
        temp = temp->next;
    } while (temp != head);

    fclose(fp);
}

void readContactsFromFile() {
    FILE *fp = fopen("phonebook.txt", "r");
    if (fp == NULL) {
        printf("Error reading file.\n");
        return;
    }

    while (!feof(fp)) {
        struct Contact *newContact = (struct Contact *)malloc(sizeof(struct Contact));
        if (fscanf(fp, "%[^,],%s\n", newContact->name, newContact->number) != 2) {
            free(newContact);
            break;
        }

        if (head == NULL) {
            newContact->next = newContact->prev = newContact;
            head = newContact;
        } else {
            struct Contact *last = head->prev;
            last->next = newContact;
            newContact->prev = last;
            newContact->next = head;
            head->prev = newContact;
        }
    }

    fclose(fp);
}

void deleteContact(char *name) {
    if (head == NULL) {
        printf("Phone book is empty!\n");
    } else {
        struct Contact *current = head;
        struct Contact *prev = NULL;

        do {
            if (strcmp(current->name, name) == 0) {
                if (current == head) {
                    if (head->next == head) {
                        head = NULL;
                    } else {
                        head = head->next;
                        current->prev->next = current->next;
                        current->next->prev = current->prev;
                    }
                    free(current);
                    printf("Contact '%s' deleted.\n", name);
                    return;
                } else {
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                    free(current);
                    printf("Contact '%s' deleted.\n", name);
                    return;
                }
            }
            prev = current;
            current = current->next;
        } while (current != head);
        printf("Contact '%s' not found.\n", name);
    }
}
void searchContactByName(char searchName[]) {
    struct Contact *temp = head;
    int found = 0;
    if (temp != NULL) {
        do {
            if (strcmp(temp->name, searchName) == 0) {
                found = 1;
                printf("Contact Found!\nName: %s, Number: %s\n", temp->name, temp->number);
            }
            temp = temp->next;
        } while (temp != head && !found);
    }

    if (!found) {
        printf("Contact not found!\n");
    }
}

// Function to search for a contact by number
void searchContactByNumber(char searchNumber[]) {
    struct Contact *temp = head;
    int found = 0;
    if (temp != NULL) {
        do {
            if (strcmp(temp->number, searchNumber) == 0) {
                found = 1;
                printf("Contact Found!\nName: %s, Number: %s\n", temp->name, temp->number);
            }
            temp = temp->next;
        } while (temp != head && !found);
    }

    if (!found) {
        printf("Contact not found!\n");
    }
}

int main() {
    int choice;
    readContactsFromFile();

    do {
        printf("\n1. Admin Login\n2. User Login\n3. Exit\nEnter choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                if (adminLogin()) {
                    do {
                        printf("\n1. Add Contact\n2. Display Contacts\n3. Save Contacts to File\n4. Delete Contact\n5. Search by name\n6. Search by number\n7. Logout\nEnter choice: ");
                        scanf("%d", &choice);
                        switch (choice) {
                            case 1:
                                addContact();
                                break;
                            case 2:
                                displayContacts();
                                break;
                            case 3:
                                saveContactsToFile();
                                break;
                            case 4: {
                                char name[50];
                                printf("Enter contact name to delete: ");
                                scanf("%s", name);
                                deleteContact(name);
                                break;
                            }
                            case 5: {
                                char name[50];
                                printf("Enter contact name to search: ");
                                scanf("%s", name);
                                searchContactByName(name);
                                break;
                            }
                            case 6:{ char num[50];
                                printf("Enter contact number to search: ");
                                scanf("%s", num);
                                searchContactByNumber(num);
                                break;

                            }
                            case 7:
                                isAdmin = false;
                                break;
                            default:
                                printf("Invalid choice! Try again.\n");
                        }
                    } while (choice != 7);
                }
                break;
            case 2:
                if (userLogin()) {
                    int userChoice;
                    do{
                     printf("\n1. Display Contacts\n2. Search by name\n3. Search by number\n4. Exit2\nEnter choice: ");
        scanf("%d", &userChoice);
        switch (userChoice) {
            case 1:
                displayContacts();
                break;
            case 2: {
                char name[50];
                printf("Enter contact name to search: ");
                scanf("%s", name);
                searchContactByName(name);
                break;
            }
            case 3: {
                char num[50];
                printf("Enter contact number to search: ");
                scanf("%s", num);
                searchContactByNumber(num);
                break;
            }
            case 4:
                return true;
            default:
                printf("Invalid choice! Try again.\n");
        }
    } while (userChoice != 4);

    return true;
}
              
      }  } while (1);

    return 0;
    }

