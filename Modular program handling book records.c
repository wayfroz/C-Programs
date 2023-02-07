
//Modular program handling book records
#include <stdio.h>
#include <stdlib.h> //contains malloc prototypу
#define MaxSize 50

//structure specifying information in list
typedef struct book{
    char title[MaxSize], author[MaxSize];
    char year[10];
} Book;

//creating a structure specifying content of the list nodes
typedef struct node{
    Book data;
    struct node *link;
} Node;

typedef Node *pNode; //defining list as a name for a pointer to Node type

//declaring the functions
void clean_buffer(void);
int Menu(void);
void InitializeList(pNode *pList);
int CheckFor_NewLine(char *pstr);
void AppendData(pNode *pList);
void WriteBinary(pNode *pList, char *pFile);
void Print_theRecord(pNode *pList);
void EmptyList(pNode *pList);
void DeleteLast(pNode *pList);

int main(void){
    char file[500] = "xxx.txt";
    int choice;
    //creating a head to point to the first node
    pNode head = NULL;
    while ((choice = Menu()) != 5){
        if (choice == 1){
            InitializeList(&head);//initializing the list
        }
        else if(choice == 2){
            AppendData(&head);
        }
        else if(choice == 3){
            WriteBinary(&head, file);
        }
        else
            Print_theRecord(&head);
    }
    //free all memory allocated
    EmptyList(&head);
    return 0;
}

//function menu for using
int Menu(void){
    int choice;
    //printing out the options for
    printf("\nChoose the option: \n");
    printf("1) Initialize   2) Append data\n3) Write        4) Print data    5) Quit\nEnter your choice: ");
    while((scanf("%d", &choice) != 1) || (choice < 1 || choice > 5)){
        printf("Invalid Choice.\nChoose the option: \n");
        printf("1) Initialize   2) Append data\n3) Write 4) Print data\n");
    }
    clean_buffer();
    return choice;
}

void InitializeList(pNode *pList){
    //checking if the list exists
    if (*pList != NULL){
        EmptyList(pList);
        printf("The list is empty.\n");
    }
    printf("The list was initialized.\n");
}

void EmptyList(pNode *pList){
    pNode current = *pList, temp;
    while(current!= NULL){
        temp = current -> link;
        free(current);
        current = temp;
    }
}

void AppendData(pNode *pList){
    FILE *fp = fopen("HW15Data.txt","r");
    //getting the information from the file
    if (fp != NULL){
        //creating a temporary pointer
        pNode TempPt = *pList;
        if (*pList == NULL){
            if ((TempPt = *pList = malloc(sizeof(Node))) != NULL) //allocating memory
                TempPt -> link = NULL; //if only node would be created
        }
        else if (*pList != NULL){
            TempPt = *pList;
            while (TempPt -> link != NULL)//interating till the last node to create a new one
                TempPt = TempPt -> link;
            if ((TempPt = malloc(sizeof(Node))) != NULL)//allocating memory for a new node
                TempPt -> link = NULL;
        }
        int check = 1, check_memory = 1;
        while(check != 0 && check_memory != 0){
            //printf("top of the while loop\n");
            if ((check = ((fgets(TempPt->data.title, MaxSize, fp) != NULL) && CheckFor_NewLine(TempPt -> data.title)))){ //quits as it hits \n)
                //printf("%s\n", TempPt -> data.title);
                if((check = ((fgets(TempPt->data.author, MaxSize, fp) != NULL) && CheckFor_NewLine(TempPt -> data.author)))){
                    //printf("Successfully read the author!\n");
                    //printf("%s\n", TempPt -> data.author);
                    check = ((fgets(TempPt->data.year, 10, fp) != NULL) && CheckFor_NewLine (TempPt -> data.year));
                    //printf("check is %d\n", check);
                    //printf("%s\n", TempPt -> data.year);
                }
            }
            if(check == 0 && ferror(fp))
                printf("Failed to read the data!\n");
            else{
                if ((check_memory = ((TempPt -> link = malloc(sizeof(Node))) != NULL))){ //allocating memory for the next node + store the address into the link
                    TempPt = TempPt -> link; //assigning TempPt to the address of the new node
                    TempPt -> link = NULL;
                } //if exiting while loop -> while link == NULL (last node)
            }
        }
    }
    DeleteLast(pList);
    printf("The data was appended.\n");
}


void WriteBinary(pNode *pList, char *pFile){
    FILE *file;
    pNode TempPt = *pList;
    if((file = fopen(pFile, "wb")) == NULL){
        printf("Error opening the file");
    }
    else{
        while(TempPt){
            fwrite(&TempPt -> data, 1, sizeof(Book), file);
            TempPt = TempPt -> link;
        }
    fclose(file);
    }
    printf("The data was written.\n");
}

void Print_theRecord(pNode *pList){

    printf("\n");
    int i = 0, choice;
    pNode temp = *pList;
    while(temp-> link != NULL){
        printf("%d: %s", ++i, temp -> data.title);
        temp = temp -> link;
    }
    printf("Select the title number: ");
    scanf("%d", &choice);
    clean_buffer();
    for(i = 1, temp = *pList; i < choice; i++, temp = temp -> link);
    printf("\nThe title: %s\nAuthor: %s\nYear: %s", temp -> data.title, temp -> data.author, temp -> data.year);
}

void clean_buffer(void){
    while(getchar() != '\n');
}

int CheckFor_NewLine(char *pstr){
    while(*pstr != '\0' && *pstr != '\n')
        pstr++;
    return (*pstr == '\n');
}

void DeleteLast(pNode *pList) {
if(*pList != NULL) {
    if((*pList)->link == NULL) {
        free(pList);
        *pList = NULL; //in case the first node contains no info
    }
    else {
        pNode second_to_last = *pList;
        pNode delete = *pList;
        while(delete -> link != NULL){ //searching for the second to last node that has data
            second_to_last = delete;
            delete = delete -> link;
        }
        second_to_last -> link = NULL;//disconnect link of second last node with last node
        free(delete);//deleting lastNode
    }
  }
}

