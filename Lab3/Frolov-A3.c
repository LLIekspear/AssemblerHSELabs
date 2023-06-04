#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

int main() {
    setlocale(LC_ALL, "rus");
    
    char str[3];
    int start = 1, flag=0;
    //далее, если проблема с проверками, то это мой косяк.
    //я сначала сделал проверку через условия, по кодам ASCII, а потом прочитал не до конца условия к выполнению задачи
    //и приделал isdigit, для которого адаптировал программу. Достаточно или isdigit и без проверок кодов ASCII
    //или проверки по кодам ASCII
    while (start == 1) {
        printf("\n\nInput a number (3 numbers in one): ");
        scanf("%c%c%c", &str[0], &str[1], &str[2]);
        getchar();
        for (int i = 0; i < 3; ++i) {
            if (!isdigit(str[i])) {
                flag = 1;
            }
        }
        str[0] -= '0';
        str[1] -= '0';
        str[2] -= '0';
        while ((str[0] < 0 || str[0]>9) || (str[1] < 0 || str[1]>9) || (str[2] < 0 || str[2]>9)||flag==1) {
            printf("\nNumbers must be 0-9 and not letter!\n");
            scanf("%c%c%c", &str[0], &str[1], &str[2]);
            getchar();
            flag = 0;
            for (int i = 0; i < 3; ++i) {
                if (!isdigit(str[i])) {
                    flag = 1;
                }
            }
            str[0] -= '0';
            str[1] -= '0';
            str[2] -= '0';
        }
        __asm {
            mov al, str[1]
            mov bl, str[2]
            cmp al, bl
            jne stepfirst //уйти на stepfirst, если не равны 2-й и 3-й символы, флаг ZF на 0 для перехода
            mov bl, str[0]
            cmp al, bl
            jle stepfirst //уйти на stepfirst, если вторая цифра не превышает первую, флаг CF на 1 и ZF на 1 для перехода
            mov al, str[2]
            cmp al, bl
            jle stepfirst //уйти на stepfirs, если третья цифра не превышает первую, флаг CF на 1 и ZF на 1 для перехода 
            mov str[1], 0 //заменяем вторую и цифру на ноль
            mov str[2], 0
            jmp end
            
            stepfirst:
                mov al, str[0]
                cmp al, '\t' //т.к. у \t будет номер 9, поэтому если у нас номер al совпадает с \t => там лежит девятка
                je end //в конец, если первая цифра равна 9, если ZF на 1
                inc al
                mov str[0], al
            end:
                nop
        }
        str[0] += '0';
        str[1] += '0';
        str[2] += '0';
        printf("\nResult:\n %c%c%c", str[0], str[1], str[2]);
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
