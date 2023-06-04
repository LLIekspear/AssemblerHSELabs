#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

int main() {
    setlocale(LC_ALL, "rus");
    
    int start = 1, flag=0;
    while (start == 1) {
        short int a[16], b[14] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 }; //14, а не 16, т.к. средних элементов может быть максимум 14 при 16 исходных
        int c[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }; //под адреса
        int res = 0;

        input:
        printf("\n\nInput numbers (16 numbers): ");
        scanf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", &a[0], &a[1], &a[2], &a[3], &a[4], &a[5], &a[6], &a[7], &a[8], &a[9], &a[10], &a[11], &a[12], &a[13], &a[14], &a[15]);
        getchar();

        __asm {
            lea esi, a //адрес начала массива a поместили в esi
            lea edi, b //адрес начала массива b поместили в edi
            lea edx, c //адрес начала массива c поместили в edx
            mov cx, 14 //макс. число повторений в цикле поместили в cx, 14 т.к. последние два элемента не образуют новых последовательностей из трех элементов 

            first:
                mov al, [esi] //элемент a[i] поместили в регистр al (как байт, поэтому +1 дальше идет для перехода к след. элементу)
                mov bl, [esi + 2] //элемент a[i+1] поместили в регистр bl(как байт)

                cmp al, bl //сравнили a[i] и a[i+1]
                jl second //переходим на second, если первый элемент < второго

                mov al, [esi+4] //элемент a[i+2] поместили в регистр al
                cmp bl,al //сравнили a[i+1] и a[i+2]
                jl second //переходим на second, если второй элемент < третьего

                //скопировать средний элемент (т.е. bl) в массив B, а адрес (смещение) этого элемента в C
                mov [edi], bl //поместили средний элемент a[i+1] в b[i]
                add esi, 4 //изменили адрес на адрес следующего элемента
                mov [edx], esi //поместили адрес (смещение) a[i+1] в c[i]
                sub esi, 4 //вернули прежний адрес
                add edi, 2 //увеличили i для b[i]
                add edx, 4 //увеличили i на 1 для c[i]
                add res, 1 //увеличили кол-во таких элементов
            second:
                add esi, 2 //увеличили i для a[i]
                dec cx //cx=cx-1
                cmp cx,0
                jne first //уйти на first, если cx не равно 0
                nop
        }
        printf("Input massive: ");
        for (int i = 0; i < 16; ++i) {
            printf(" %d ", a[i]);
        }
        printf("\nResult massive: ");
        for (int i = 0; i < 16; ++i) {
            printf(" %x ", c[i]);
        }
        printf("\nResult is %d", res);
        printf("\n\nAgain(0/1)?: ");
        if (scanf("%d", &start)) {
            getchar();
            if (start == 1)
                start = 1;
            else
                start = 0;
        }
    }
    exit(0);
}
