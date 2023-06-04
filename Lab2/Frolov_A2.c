#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>


//переделать чуток (в отчет?) 
// дописать в отчете причину ffff (доп код) и отправить. срок 1 марта
void assm(short int v_c, short int y, char x, char z) {
    short int v_as;
    __asm {
        // chislitel
        mov al, x //pomechaem x v AL (AL=13)
        //add al, 2
        cbw //expand [byte]AL do [word]AX (AX=0013)
        add ax, 2
                    //inc ax //x+1
                    //inc ax //x+2
        imul y //y*(x+2) (DX:AX)
        sub ax, 1 //vishislyaem mladshee slovo chislitelya (AX)
        sbb dx, 0 //vichislyaem starshee slovo chislitelya (DX)
        //znamenatel
        mov bx, ax //sochranyaem mladshee slovo chislitelya v BX
        mov al, z //pomechaem z v AL
        cbw //expand [byte]AL do [word]AX 

        cmp ax, 0
        jge cont
        not ax
        inc ax

        cont:
        //cbw

        add ax, 2 //vichislyaem znam v AX
        xchg ax, bx //pomechaem mladshee slovo chislitelya v AX,
        //znam - v BX
        idiv bx //delim chisl na znam (AX - chastnoe, DX - ostatok)
        mov cx, 3 //pomechaem 3 v CX
        sub cx, ax //vichli is CX soderzhimoe AX, t.e. znachenie drobi
        mov v_as, cx


        //mov bx, cx
        //xor cx, cx
        //sub cx, bx
        //mov v_as, cx
        //----
        //mov ax, 0
        //sub ax, cx
        //mov v_as, ax//cx //resultat pomestili v v_as 
    }
    printf("Resultat na Assembler'e: \n %d (10-system) ili %x (16-system)\n", v_as, v_as/*(v_as<0) ? '-' : ' ', (v_as<0) ? -v_as : v_as%c%X*/);
    printf("%d", v_as);
    printf("Resultat na C: \n %d (10-system) ili %X (16-system)\n", v_c, /*(v_c < 0) ? '-' : ' ', (v_c < 0) ? -v_c : v_c %c%X*/v_c);
}

int main() {
    //connect russian localization
    setlocale(LC_ALL, "rus");
    //variables
    int i, own_input=0;
    short int y, v_c;
    char x, z;
    int start = 1;
    while (start == 1) {
        printf("\n\nWould you like to input your own values? (0/1): ");
        if (scanf("%d", &own_input)) {
            if (own_input == 1) {
                printf("\n\nInput values of X, Y, Z: ");
                scanf("%d %d %d", &x, &y, &z);
                while (z==-0x2) {
                    printf("\n\nZ can't be equal -0x2!");
                    printf("\nInput values of X, Y, Z: ");
                    scanf("%hhx %hx %hhx", &x, &y, &z);
                }
            }
            if (own_input == 0) {
                //communication with user
                printf("Input number of test: \n");
                scanf("%d", &i);
                switch (i) {
                case 1:
                    //x = -0x3;
                    x = -3;
                    //y = 0x3;
                    y = 3;
                    //z = -0x3;
                    z = -3;
                    break;
                case 2:
                    x = 0x7E;
                    y = 0x4000;
                    z = -0x70;
                    break;
                }
            }
            v_c = 3 - ((y * (x + 2) - 1) / (z + 2));
            assm(v_c, y, x, z);
            //cycle of checking
            printf("\n\nAgain? (0/1): ");
            scanf("%d", &start);
        }
        else {
            printf("\n\nIncorrect input!");
        }
    }
    return 0;
}
