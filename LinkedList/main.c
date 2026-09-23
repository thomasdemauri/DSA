#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct Node {
    char *url;
    struct Node *previous;
    struct Node *next;
} Node;

typedef struct LinkedList {
    Node *head;
    Node *tail;
    Node *currentPage;
} LinkedList;

Node* createNode(Node *tail, char *value) {
    Node *newNode = calloc(1, sizeof(Node));
    
    newNode->previous = tail;
    newNode->next = NULL;
    newNode->url = strdup(value);

    return newNode;
}

void AddItem(LinkedList *list, char *value) {
    Node *newNode;

    if (list->head == NULL) {
        newNode = createNode(NULL, value);
        list->head = newNode;
        list->tail = newNode;
        list->currentPage = list->tail;
        return;
    }

    // check if current page is not the same as tail, 
    //if so it means we need to delete the subsquent pages
    // before add a new one
    if (list->tail != list->currentPage) {

        Node *tmp = list->tail;
        while (tmp != list->currentPage) {
            Node *toDelete = tmp;
            tmp = tmp->previous;

            free(toDelete->url);
            free(toDelete);
        }

        list->tail = list->currentPage;
        list->tail->next = NULL;
    }

    newNode = createNode(list->tail, value);

    list->tail->next = newNode;
    list->tail = newNode;
    list->currentPage = newNode;
}

void NavigateToPreviousItem(LinkedList *list) {
    
    if (list->currentPage == NULL || list->currentPage->previous == NULL) {
        printf("No pages to navigate. \n");
        return;
    }

    list->currentPage = list->currentPage->previous;
}


void NaviteFoward(LinkedList *list) {

    if (list->currentPage == NULL || list->currentPage->next == NULL) {
        printf("No pages to navigate. \n");
        return;
    }

    list->currentPage = list->currentPage->next;
}

void PrintStructure(LinkedList *list) {

    for (Node *tmp = list->head; tmp != NULL; tmp = tmp->next) {

        if (list->currentPage == tmp) {
            printf("[%s] ", tmp->url);
        } else {
            printf("%s ", tmp->url);
        }
    

        if (tmp->next != NULL) {
            printf("-> ");
        }
    }

    printf("\n");
}

void FreeList(LinkedList *list) {
    Node *tmp = list->head;
    while (tmp != NULL) {
        Node *toDelete = tmp;
        tmp = tmp->next;

        free(toDelete->url);
        free(toDelete);
    }

    free(list);
}

// =============== Draw Menu =====================

void PrintMenu() {
    printf("[1] Navigate to a location \n");
    printf("[2] Go back\n");
    printf("[3] Go foward\n");
    printf("[4] Exit\n");
    printf("Option: \n");
}

void clearConsole() {
    printf("\033[H\033[J");
}

int main() {

    LinkedList *list = calloc(1, sizeof(LinkedList));

    char urlBuffer[64];
    char optionBuffer[10];
    int option = -1;

    while (option != 4) {
        clearConsole();

        if (list->currentPage != NULL) {
            PrintStructure(list);
        }

        PrintMenu();

        fgets(optionBuffer, sizeof(optionBuffer), stdin);
        option = atoi(optionBuffer);

        if (option == 0) {
            printf("Invalid option\n");
            break;
        }

        switch (option)
        {
        case 1:
            fgets(urlBuffer, sizeof(urlBuffer), stdin);
            urlBuffer[strcspn(urlBuffer, "\n")] = '\0';
            AddItem(list, urlBuffer);
            break;
        case 2:
            NavigateToPreviousItem(list);
            break;
        case 3:
            NaviteFoward(list);
            break;
        default:
            break;
        }
        
        memset(urlBuffer,0,sizeof(urlBuffer));
        memset(optionBuffer,0,sizeof(optionBuffer));
    }

    PrintStructure(list);
    FreeList(list);

    return 0;
}