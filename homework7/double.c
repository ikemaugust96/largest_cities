/*
 * Program: Doubly Linked List of Top 20 Largest Cities
 * Description: This program reads data from "uscities.csv" and processes the top 20 largest cities by population.
 * It calculates the total population, identifies the northernmost city, and provides linked list operations using a doubly linked list.
 *
 * Author: wenxuan yang(ikem)
 * Date: 02/27/2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 4096  
#define MAX_CITY_NAME 200     
#define NUM_CITIES 20
#define MAX_CITIES 1000       

// Structure for City Data
typedef struct {
    char city[MAX_CITY_NAME];
    char state[10];
    int population;
} City;

// Doubly Linked List Node
typedef struct doubleNode {
    City *data;
    struct doubleNode *next;
    struct doubleNode *prev;
} dNode;

dNode *list = NULL; // Head of the linked list

// Function to clean quotes and handle special characters
void clean_string(char *str) {
    char *src = str;
    char *dst = str;

    if (*src == '"') {
        src++;
    }

    while (*src) {
        if (*src == '"' && *(src + 1) == '"') {
            *dst++ = '\\';
            *dst++ = 'u';
            src += 2;
        } else if (*src == '"') {
            src++;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
}

// Function to remove commas from population strings
void remove_commas(char *str) {
    char *src = str;
    char *dst = str;
    while (*src) {
        if (*src != ',') {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}

// Function to compare cities by population for sorting
int compare_population(const void *a, const void *b) {
    City *cityA = (City *)a;
    City *cityB = (City *)b;
    return cityB->population - cityA->population;
}

// Function to add a node to the end of the doubly linked list
void add_to_end(dNode **head, City *data) {
    dNode *new_node = malloc(sizeof(dNode));
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;

    if (*head == NULL) {
        *head = new_node;
    } else {
        dNode *current = *head;
        while (current->next) {
            current = current->next;
        }
        current->next = new_node;
        new_node->prev = current;
    }
}

// Function to print the doubly linked list
void print_list(dNode *head, int n) {
    dNode *current = head;
    int count = 0;
    while (current && count < n) {
        City *city = (City *)current->data;
        printf("%s %s, population %d\n", city->city, city->state, city->population);
        current = current->next;
        count++;
    }
}

// Function to get the size of the linked list
int get_size(dNode *head) {
    int size = 0;
    dNode *current = head;
    while (current) {
        size++;
        current = current->next;
    }
    return size;
}

// Function to delete the n-th node from the list
void delete_node(dNode **head, int n) {
    if (*head == NULL) return;

    dNode *temp = *head;

    if (n == 1) {
        *head = temp->next;
        if (*head) (*head)->prev = NULL;
        free(temp);
        return;
    }

    for (int i = 1; temp != NULL && i < n; i++) {
        temp = temp->next;
    }

    if (temp == NULL) return;

    if (temp->next) temp->next->prev = temp->prev;
    if (temp->prev) temp->prev->next = temp->next;

    free(temp);
}

// Function to reverse the doubly linked list
void reverse_list(dNode **head) {
    dNode *current = *head;
    dNode *temp = NULL;

    while (current) {
        temp = current->prev;
        current->prev = current->next;
        current->next = temp;
        current = current->prev;
    }

    if (temp) *head = temp->prev;
}

// Function to move n-th node to front
void move_to_front(dNode **head, int n) {
    if (n <= 1 || *head == NULL) return;

    dNode *current = *head;

    for (int i = 1; current != NULL && i < n; i++) {
        current = current->next;
    }

    if (current == NULL) return;

    if (current->prev) current->prev->next = current->next;
    if (current->next) current->next->prev = current->prev;

    current->next = *head;
    current->prev = NULL;
    (*head)->prev = current;
    *head = current;
}

int main() {
    FILE *file = fopen("uscities.csv", "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    char *line = malloc(MAX_LINE_LENGTH * sizeof(char));
    fgets(line, MAX_LINE_LENGTH, file);

    City cities[MAX_CITIES];
    int city_count = 0;

    while (fgets(line, MAX_LINE_LENGTH, file) && city_count < MAX_CITIES) {
        char city[MAX_CITY_NAME], state[10], population_str[20];
        int field_count = 0;
        char *field = strtok(line, ",");

        while (field && field_count < 9) {
            clean_string(field);
            if (field_count == 0) {
                strncpy(city, field, MAX_CITY_NAME - 1);
            } else if (field_count == 2) {
                strncpy(state, field, 9);
            } else if (field_count == 8) {
                strncpy(population_str, field, 19);
                remove_commas(population_str);
            }
            field = strtok(NULL, ",");
            field_count++;
        }

        cities[city_count].population = atoi(population_str);
        strncpy(cities[city_count].city, city, MAX_CITY_NAME - 1);
        strncpy(cities[city_count].state, state, 9);

        city_count++;
    }

    fclose(file);

    qsort(cities, city_count, sizeof(City), compare_population);

    for (int i = 0; i < NUM_CITIES; i++) {
        add_to_end(&list, &cities[i]);
    }

    char command[10];
    while (1) {
        printf("\nsize, delete, reverse, get, or print: ");
        scanf("%s", command);

        if (strcmp(command, "size") == 0) {
            printf("Size is %d\n", get_size(list));
        } else if (strcmp(command, "delete") == 0) {
            int position;
            printf("Enter a number to delete: ");
            scanf("%d", &position);
            delete_node(&list, position);
        } else if (strcmp(command, "reverse") == 0) {
            reverse_list(&list);
        } else if (strcmp(command, "get") == 0) {
            int position;
            printf("Enter a number: ");
            scanf("%d", &position);
            move_to_front(&list, position);
        } else if (strcmp(command, "print") == 0) {
            int n;
            printf("Enter a number: ");
            scanf("%d", &n);
            print_list(list, n);
        } else {
            break;
        }
    }

    free(line);
    return 0;
}

