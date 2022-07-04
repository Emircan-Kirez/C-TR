#include <stdio.h>																											                                                        //E.K.
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

//Function Prototypes
char **createBoard();
void printBoard(char **board);
void resetBoard(char **board);
void chooseYourSign(char *userSign, char *computerSign);
char checkWhetherThereIsAWinner(char **board);
void userMove(char **board, char userSign, int *freeSpaces);
void computerMove(char **board, char computerSign, int *freeSpaces);

int main(){
	char **board = createBoard();
	char userSign, computerSign, winner, response;
	int freeSpaces, i;
	
	do{
		winner = ' ';
		freeSpaces = 9;
		resetBoard(board);
		chooseYourSign(&userSign, &computerSign);
		
		printf("ILK DURUM: \n");
		printBoard(board);
		
		//ilk 4 hareket kazanan olamayacaðý için kontrole gerek yok
		for(i = 0; i < 2; i++){
			userMove(board, userSign, &freeSpaces);
			printBoard(board);
			computerMove(board, computerSign, &freeSpaces);
			printBoard(board);
		}
		
		while(winner == ' ' && freeSpaces != 0){
			userMove(board, userSign, &freeSpaces);
			printBoard(board);
			winner = checkWhetherThereIsAWinner(board);
			if(winner == ' ' && freeSpaces != 0){
				computerMove(board, computerSign, &freeSpaces);
				printBoard(board);
				winner = checkWhetherThereIsAWinner(board);
			}
		} 
		
		if(winner == userSign){
			printf("KAZANDIN\n");
		}else if(winner == computerSign){
			printf("KAYBETTIN\n");
		}else{
			printf("Oynanabilecek yer kalmadi. Bu yuzden BERABERE :)\n");
		}
		

		printf("\nTekrar oynamak ister misin? (E/H): ");
		scanf(" %c", &response);
		response = toupper(response);
	}while(response == 'E');
	
	printf("Oynadigin icin tesekkurler :)");
	return 0;
}

char **createBoard(){
	char **tmp;
	int i;
	tmp = (char **)calloc(3, sizeof(char *));
	for(i = 0; i < 3; i++){
		tmp[i] = (char *)calloc(3, sizeof(char));
	}
	return tmp;
}

void printBoard(char **board){
	int i, j;
	printf("---------------\n");
	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++){
			printf("%3c |", board[i][j]);
		}
		printf("\n---------------\n");
	}
	printf("\n");
}

void resetBoard(char **board){
	int i, j;
	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++){
			board[i][j] = ' ';
		}
	}
}

void chooseYourSign(char *userSign, char *computerSign){
	do{
		printf("X olmak icin X'e, O' olmak icin O'ya basiniz: ");
		scanf(" %c", userSign);
		
		*userSign = toupper(*userSign);
		if(*userSign != 'X' && *userSign != 'O'){
			printf("Hatali bir giris yaptiniz. Tekrar deneyiniz...\n");
		}
	}while(*userSign != 'X' && *userSign != 'O');
	
	if(*userSign == 'X'){
		*computerSign = 'O';
	}else{
		*computerSign = 'X';
	}
}

char checkWhetherThereIsAWinner(char **board){
	int i;
	
	//satýr kontrolü
	for(i = 0; i < 3; i++){
		if(board[i][0] == board[i][1] && board[i][0] == board[i][2]){
			return board[i][0];
		}
	}
	
	//sütun kontrolü
	for(i = 0; i < 3; i++){
		if(board[0][i] == board[1][i] && board[0][i] == board[2][i]){
			return board[0][i];
		}
	}
	
	//sol üstten sað alta çarpraz kontrol
	if(board[0][0] == board[1][1] && board[0][0] == board[2][2]){
		return board[0][0];
	}
	
	//sað üstten sol altta çarpraz kontrol
	if(board[0][2] == board[1][1] && board[0][2] == board[2][0]){
		return board[0][2];
	}
	
	return ' ';
}

void userMove(char **board, char userSign, int *freeSpaces){
	int x, y;

	printf("Sira sizde.\n");
	do{
		printf("Oynamak istediginiz yerin satir ve sutun numaralarini yaziniz: ");
		scanf("%d %d", &x, &y);
		
		if(board[x-1][y-1] != ' '){
			printf("Oynamak istediginiz yer bos degil. Lutfen bos olan bir yer seciniz..\n");
		}
	}while(board[x-1][y-1] != ' ');
	
	board[x-1][y-1] = userSign;
	(*freeSpaces)--;
}

void computerMove(char **board, char computerSign, int *freeSpaces){
	srand(time(NULL));
	int x, y;
	
	do{
		x = rand() % 3;
		y = rand() % 3;
	}while(board[x][y] != ' ');
	
	board[x][y] = computerSign;
	printf("Bilgisayar oynadi.\n");
	(*freeSpaces)--;
}

