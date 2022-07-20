/* Emircan K�REZ - Ch3rry */
/* Last Update - 20/07/2022 */
/*
----------------------- B�LG�LEND�RME --------------------------
� Projeye ge�meden �nce README.md dosyas�n� okuman�z tavsiye edilmektedir.
� Projeyle alakal� herhangi bir soru ve �neri i�in "emircan200123@hotmail.com" mail adresi �zerinden, konu ba�l���na "Github | C-TR / Linkli Listeyi Ters �evir"
yazarak benimle ileti�ime ge�ebilirsiniz.
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct node{
	int value;
	struct node *next;
}NODE;

void insert(NODE **head, int value);
NODE *createNode(int value);
void printList(NODE *head);
void reverse(NODE **head);

int main(){
	NODE *head = NULL;
	insert(&head, 9);
	insert(&head, 7);
	insert(&head, 5);
	insert(&head, 2);
	printList(head);
	reverse(&head);
	printList(head);
	reverse(&head);
	printList(head);
	return 0;
}

void insert(NODE **head, int value){
	NODE *newNode = createNode(value);
	newNode->next = *head;
	*head = newNode;
}

NODE *createNode(int value){
	NODE *tmp = (NODE *)malloc(sizeof(NODE));
	tmp->value = value;
	tmp->next = NULL;
	return tmp;
}

void printList(NODE *head){
	while(head != NULL){
		printf("%d ", head->value);
		head = head->next;
	}
	printf("\n");
}

void reverse(NODE **head){
	NODE *prev = NULL, *current = *head, *next;
	while(current != NULL){
		next = current->next; //next'i sakla
		current->next = prev; //cuurent node bir �ncesini g�stersin
		prev = current; //prev'i g�ncelle
		current = next; //current'� g�ncelle
	}
	*head = prev;
}
