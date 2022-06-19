/* Emircan KIREZ - Ch3rry */
/* Last Update: 02/06/2022 */
/* 
--------------- Bilgilendirme -----------------------
• Projeye geçmeden önce README.md dosyasını okumanız tavsiye edilmektedir.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX 255

/* Kullanilan Matris TEK Boslukludur
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|     |     |                             |
+ +-+ + +-+-+ +-+-+-+-+-+-+-+-+-+ +-+-+-+ +
|   |   |     |   |   |         |   |     |
+-+ + +-+ +-+-+ + + + + +-+-+-+ +-+ + +-+-+
|   | |   |     |   | |     | | |   |   | |
+ +-+ + +-+ +-+-+-+-+-+-+ +-+-+ +-+-+-+ +-+
| |   | | | |         | | |   |     | |   |
+ +-+-+ +-+ + +-+-+-+ + + + + +-+ + +-+-+ +
|     | |   |   |   | | |   |   | |       |
+ +-+ + + +-+-+ +-+ + +-+-+-+-+ + +-+-+-+-+
| |   | |     |   | |         | |       | |
+ + +-+ +-+-+-+-+ + +-+-+-+-+ + +-+ +-+ +-+
| | |             |         | |   | | |   |
+ + + +-+-+-+-+-+-+-+ +-+-+ + +-+ + + +-+ +
| |     |   |             | |   | |   |   |
+ +-+-+ + + + +-+ +-+-+ +-+-+ + + +-+ + +-+
| |     | | |   |     | |     | | | | |   |
+ + +-+-+ + +-+ + +-+-+-+ +-+-+-+ + + +-+ +
| | |     |   | |   |   |       | |   | | |
+ + + +-+-+-+-+ +-+ + +-+-+-+-+ + +-+-+-+ +
| |               | |   | | |   |     | | |
+ + +-+-+-+-+-+-+-+-+-+ +-+-+ +-+-+-+ +-+ +
| |     |   | |     | | | |         | |   |
+-+-+-+ + + + + +-+ +-+ +-+ +-+ +-+-+ + +-+
|   |   | | | | | |     | | |   |   | |   |
+ +-+ +-+ + + + +-+-+-+-+-+ + +-+ + + +-+-+
|       | | | |           | | |   | |     |
+-+-+-+ +-+ + +-+-+-+-+-+ + + +-+-+-+-+-+ +
|b          |           |   |         |  c|
+---+-+-+-+-+---+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

//Dosyadan okudugum satırları sakladıgım linkli liste
typedef struct lineNode{
	char line[100];
	struct lineNode *next;
}LINES; 

//x ve y indislerini birlikte tutmak için olusturdugum struct
typedef struct indices{
	int x, y;
}INDICES;

//Ziyaret ettigim dügümleri tuttugum linkli liste 
typedef struct visitedIndices{
	INDICES* indices;
	struct visitedIndices *next;
}VISITED;

//DFS için kullandıgım stack yapısı - Üstteki linkli listede INDICES* tuttugum için kolaylık olsun diye burada da öyle tutuyorum
typedef struct {
	INDICES* item[MAX]; //0 .. MAX-1
	int top;
}STACK;

//Function Prototypes (18)
void initialize(STACK *s);
void push(STACK *s, INDICES* indices);
int isEmpty(STACK *s);
INDICES* pop(STACK *s);
INDICES* top(STACK *s);
int readFromFile(LINES **head, char *fileName, int *mazeHeight);
LINES *createLineNode(char *line);
void addLineNode(LINES **head, char *line);
int findWidth(char *line);
char **createMaze(int mazeHeight, int mazeWidth);
void initializeMaze(LINES *head, char **maze, int mazeHeight, int mazeWidth, int *startingX, int *startingY);
void findMazeExit(VISITED **visitedhead, int startingX, int startingY, STACK *s, char **maze, int mazeHeight, int mazeWidth);
void printMaze(char **maze, int mazeHeight, int mazeWidth, int score);
INDICES* createIndices(int x, int y);
VISITED* createVisitedIndices(INDICES *indices);
void addVisitedNode(VISITED **head, VISITED* newVisited);
void deleteVisitedNode(VISITED **head);
void deallocateMaze(char **maze, int mazeHeight, int mazeWidth);

int main(){
	srand(time(NULL));
	LINES *lineHead = NULL;
	VISITED *visitedHead = NULL;
	char fileName[20];
	char **maze; //labirenti sakladýðým matris yapým
	STACK s;
	int startingX, startingY, mazeWidth, mazeHeight;
	initialize(&s); //Stack'i initialize eder. Stack boþ ise top deðerimiz -1'i gösterir


	printf("Labirenti okumak istediginiz dosyayi uzantisiyla beraber yaziniz: ");
	scanf("%s", fileName);
	
	if(!readFromFile(&lineHead, fileName, &mazeHeight)){
		printf("Dosya okunamadigi icin program kapatilmistir.");
		return -1;
	}
	
	mazeWidth = findWidth(lineHead->line);
	maze = createMaze(mazeHeight, mazeWidth);
	initializeMaze(lineHead, maze, mazeHeight, mazeWidth, &startingX, &startingY);
	findMazeExit(&visitedHead, startingX, startingY, &s, maze, mazeHeight, mazeWidth);
	
	/* Programý kapatmadan önce silme iþlemleri yapar */
	
	//Stack'i boþaltýr
	while(!isEmpty(&s)){
		INDICES *tmp = pop(&s);
	}
	
	//Linkli listelerin head'lerini sýfýrlar
	visitedHead = NULL;
	lineHead = NULL;
	
	//Oluþturulan labirenti deallocate eder
	deallocateMaze(maze, mazeHeight, mazeWidth);
	return 0;
}

/* STACK Fonksiyonlarý */
void initialize(STACK *s){
	s->top = -1;
}

//stack'i yeteri kadar büyük ayarlayýp dolu olup olmadýðýna bakmadým
void push(STACK *s, INDICES* indices){ 
	s->item[++s->top] = indices; //önce arttýr yeni yeri göstersin, ondan sonra ekle 
}

int isEmpty(STACK *s){
	return s->top == -1 ? 1 : 0;
}

INDICES* pop(STACK *s){ //Boþ olup olmadýðýna bakmadým çünkü DFS'te stack boþ olmadýðý sürece pop yapýyoruz
	return s->item[s->top--]; //toptaki deðeri al, sonra bir azalt
}

INDICES* top(STACK *s){
	return s->item[s->top];
}

//Dosyadan satýr satýr okuyup, onlarý LINES linkli listesine kaydeder ve labirentin yüksekliðini bulur.
int readFromFile(LINES **head, char *fileName, int *mazeHeight){
	FILE *fPtr;
	char line[100];
	
	if((fPtr = fopen(fileName, "r")) == NULL){
		printf("Dosya acilamadi!!\n");
		return 0;
	}
	

	while(fgets(line, 100, fPtr) != NULL){
		addLineNode(head, line);
		(*mazeHeight)++;
	}

	fclose(fPtr);
	return 1;
}

//Dosyadan satýr satýr okurken elde ettiðimiz line'larý saklamak için node oluþturur
LINES *createLineNode(char *line){
	LINES *newLineNode = (LINES *)malloc(sizeof(LINES));
	strcpy(newLineNode->line, line);
	newLineNode->next = NULL;
	return newLineNode;
}

//Satýrlarý içinde tutan bir node oluþturur ve linkli listeye sondan ekler
void addLineNode(LINES **head, char *line){
	LINES *newLineNode = createLineNode(line);
	if(*head == NULL){
		*head = newLineNode;
	}else{
		LINES *tmp = *head;
		while(tmp->next != NULL){
			tmp = tmp->next;
		}
		tmp->next = newLineNode;
	}
}

//Labirentin geniþliði bir satýrýn uzunluðuna bakarak belirler
int findWidth(char *line){
	int i = 0;
	while(i < 100 && line[i] != '\n'){
		i++;
	}
	return i;
}

//Labirentin yüksekliði ve geniþliðine göre bir matris oluþturup onu geri döndürür
char **createMaze(int mazeHeight, int mazeWidth){
	char **maze;
	int i;
	maze = (char **)malloc(mazeHeight * sizeof(char *));
	for(i = 0; i < mazeHeight; i++){
		maze[i] = (char *)malloc(mazeWidth * sizeof(char));
	}
	return maze;
}

//Daha önceden dosyadan okuduðum ve LINES linkli listesinden satýrlarý teker teker alarak matrisi initialize eder. Ayný zamanda baþlangýç noktasýnýn indislerini bulur
void initializeMaze(LINES *head, char **maze, int mazeHeight, int mazeWidth, int *startingX, int *startingY){
	int i, j, randomX, randomY;
	LINES *tmp = head;
	for(i = 0; i < mazeHeight; i++){
		for(j = 0; j < mazeWidth; j++){
			maze[i][j] = tmp->line[j];
			if(maze[i][j] == 'b'){
				*startingX = i;
				*startingY = j;
			}
		}
		tmp = tmp->next;
	}
	
	//10 adet elma eklemek istedim
	i = 0;
	while(i < 10){
		randomX = rand() % mazeHeight;
		randomY = rand() % mazeWidth;
		if(maze[randomX][randomY] == ' '){
			maze[randomX][randomY] = 'O';
			i++;
		} 
	}
}

//
void findMazeExit(VISITED **visitedHead, int startingX, int startingY, STACK *s, char **maze, int mazeHeight, int mazeWidth){
	//sol - aþaðý- sað - yukarý þeklinde kontrol eder. Bu þekilde stackte yukarý - sað - aþaðý - sol yapýsý kurulur
	int directions[][4] = {{0, 1, 0, -1}, {-1, 0, 1, 0}};  
	//int directions[][4] = {{-1, 0, 0, 1}, {0, -1, 1, 0}}; //aþaðý - sað - yukarý - sol pdf'teki örnek için
	int i, x, y, counter, score = 0;
	INDICES *tmp = createIndices(startingX, startingY);
	INDICES *current;
	
	//Baþlangýç noktasýný VISITED linkli listesine koyuyorum ve komþularýný stack'e atýyorum. Ondan sonra DFS uyguluyorum
	addVisitedNode(visitedHead, createVisitedIndices(tmp));
	for(i = 0; i < 4; i++){
		x = startingX + directions[0][i];
		y = startingY + directions[1][i];
		
		if(maze[x][y] == ' ' || maze[x][y] == 'O'){ //boþ yerse ya da elmaysa stack'e ekle
			tmp = createIndices(x, y);
			addVisitedNode(visitedHead, createVisitedIndices(tmp));
			push(s, tmp);
		}
	}
	
	printMaze(maze, mazeHeight, mazeWidth, score);
	while(!isEmpty(s)){
		tmp = pop(s);
		if(maze[tmp->x][tmp->y] == 'O'){ //elmayý yerse +10 puan
			score += 10;
		}
		maze[tmp->x][tmp->y] = '*';
		printMaze(maze, mazeHeight, mazeWidth, score);
		counter = 0;
		i = 0;
		while(i < 4){
			x = tmp->x + directions[0][i];
			y = tmp->y + directions[1][i];
			if(maze[x][y] == 'c'){
				printf("\nLabirentten cikis puaniniz: %d", score);
				current = createIndices(x, y);
				addVisitedNode(visitedHead, createVisitedIndices(current));
				return;
			}else if(maze[x][y] == ' ' || maze[x][y] == 'O'){ //eðer boþsa
				current = createIndices(x, y);
				addVisitedNode(visitedHead, createVisitedIndices(current));
				push(s, current);
				counter++;
			}
			i++;
		}
		

		if(counter == 0){ 
		//counter == 0 ise demekki hiç bir yere gidilememiþ. Bu da demek oluyor ki çýkmaz sokak. O yüzden skor'dan 5 çýkarýp, stack'teki son deðer'e kadar olan *'larý kaldýrýyorum.
		//Bu þekilde gittiðim yoldan geri dönmüþ oluyorum. Bunun için de VISITED linkli listesinden yararlanýyorum
			score -= 5;
			current = top(s);
			while((*visitedHead)->indices->x != current->x || (*visitedHead)->indices->y != current->y){
				maze[(*visitedHead)->indices->x][(*visitedHead)->indices->y] = ' '; //geri geliyorum
				printMaze(maze, mazeHeight, mazeWidth, score);
				deleteVisitedNode(visitedHead); //Baþtan eklediðim linkli listeden silerek ilgili noktaya gelmeye çalýþýyorum
			}
		}
	}
}

void printMaze(char **maze, int mazeHeight, int mazeWidth, int score){
	int i, j;
	printf("Anlik Puaniniz: %d\n", score);
	for(i = 0; i < mazeHeight; i++){
		for(j = 0; j < mazeWidth; j++){
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	usleep(500000); 
}

//x ve y deðerlerine göre INDICES oluþturur
INDICES* createIndices(int x, int y){
	INDICES* newIndices = (INDICES*)malloc(sizeof(INDICES));
	newIndices->x = x;
	newIndices->y = y;
	return newIndices;
}

//Ziyaret ettiðimiz noktalarýn kordinat bilgilerini içeren node oluþturur
VISITED* createVisitedIndices(INDICES *indices){
	VISITED* newVisited = (VISITED*)malloc(sizeof(VISITED));
	newVisited->indices = indices;
	newVisited->next = NULL;
	return newVisited;
}

//Ýlgili node'u linkli listeye baþtan ekler çünkü silme iþlemi yapacaksak en son eklediðimizi sileceðimiz için en sona kadar sürekli gitmektense baþa eklemek daha mantýklý
void addVisitedNode(VISITED **head, VISITED *newVisited){
	newVisited->next = *head;
	*head = newVisited;
}

//Baþtan silme yapar
void deleteVisitedNode(VISITED **head){
	VISITED *tmp = *head;
	*head = (*head)->next;
	free(tmp);
}

//Dynamic olarak oluþturulan matrisi deallocate eder
void deallocateMaze(char **maze, int mazeHeight, int mazeWidth){
	int i;
	for(i = 0; i < mazeHeight; i++){
		free(maze[i]);
	}
	free(maze);
}
