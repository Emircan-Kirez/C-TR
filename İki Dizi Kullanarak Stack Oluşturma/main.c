/* Emircan K�REZ - Ch3rry */
/* Son G�ncelleme: 06/07/2022 */
/*
----------------------- B�LG�LEND�RME --------------------------
� Projeye ge�meden �nce README.md dosyas�n� okuman�z tavsiye edilmektedir.
� Projeyle alakal� herhangi bir soru ve �neri i�in "emircan200123@hotmail.com" mail adresi �zerinden, konu ba�l���na "Github | C-TR / �ki Dizi Kullanarak Stack Olu�turma"
yazarak benimle ileti�ime ge�ebilirsiniz.
*/
#include <stdio.h>                                                                                                                                                                  
#define MAX 1 //Fonksiyonlar� denemek i�in MAX 1 olarak ayarlanm��t�r. Toplam stack boyutumu 2 MAX't�r.

//Function Prototypes
int isFull(int );
void push(int );
int isEmpty(int );
void pop();

//Global variables - s�rekli parametre olarak atmayal�m diye
int stackA[MAX], stackB[MAX];
int aMax = -1;
int bMax = -1;

int main(){
    push(1);
    push(2);
    push(3);
    pop();
    push(4);

    return 0;
}


int isFull(int max){
    if(max == MAX - 1){
        return 1;
    }
    return 0;
}

//push ederken �nce stackA, sonra stackB kontrol edilmeli
void push(int value){
    if(isFull(aMax)){
        if(isFull(bMax)){
            printf("%d degeri eklenemedi cunku stack full dolu !!\n", value);
        }else{
            stackB[++bMax] = value;
            printf("%d degeri stack'e eklendi. Toplam eleman sayisi: %d\n", value, aMax + bMax + 2);
        }
    }else{
        stackA[++aMax] = value;
        printf("%d degeri stack'e eklendi. Toplam eleman sayisi: %d\n", value, aMax + bMax + 2);
    }
}

int isEmpty(int max){
    if(max == -1){
        return 1;
    }
    return 0;
}

//pop ederken �nce stackB, sonra stackA kontrol edilmeli
void pop(){
	int poppedValue;
    if(isEmpty(bMax)){
        if(isEmpty(aMax)){
            printf("Stack'ten hicbir deger pop edilemedi cunku stack bos !!\n");
        }else{
        	poppedValue = stackA[--aMax];
            printf("%d degeri stack'ten pop edildi. Toplam eleman sayisi: %d\n", poppedValue, aMax + bMax + 2);
        }
    }else{
    	poppedValue = stackB[--bMax];
        printf("%d degeri stack'ten pop edildi. Toplam eleman sayisi: %d\n", poppedValue, aMax + bMax + 2);
    }
}
