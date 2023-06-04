#include <iostream>
#include <bitset>
#include <string>

bool checkString(std::string str) {
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] != '0' && str[i] != '1') {
            return false;
        }
    }
    return true;
}

//TODO: приделать проверку на ввод строки, где вводится число, иначе прога виснет в бесконечном цикле. Можно проверку на число или на успешный ввод
int main() {
    setlocale(LC_ALL, "rus");

    short mode;
    int size;
    short sie;
    char byteArr[6];
    unsigned short wordArr[6];
    char tmp, tmp1=0, tmp2;
    while (true) {
        std::cout << "Select mode 0 or 1:\n";
        std::cin >> mode;
        if (mode == 0 || mode == 1) {
            std::cout << "Input the amount of elements (5 or 6):\n";
            std::cin >> size;
            if (size == 5 || size == 6) {
                if (size == 5)
                    sie = 5;
                else if (size == 6)
                    sie = 6;
                std::string str = "";
                if (mode == 0) {
                    bool flag = false;
                    for (int i = 0; i < sie; ++i) {
                        std::cout << "Input element (binary):\n";
                        std::cin >> str;
                        flag = !(checkString(str) && (str.size() <= 8));
                        if (flag)
                            break;
                        byteArr[i] = std::strtol(str.c_str(), NULL, 2);
                    }
                    if (flag)
                        continue;
                    std::cout << "Inputed array:\n";
                    for (int i = 0; i < sie; ++i) {
                        std::cout << "byteArray[" << i << "] in binary view: " << std::bitset<8>(byteArr[i]) << " and in hex view: 0x" << std::hex << (unsigned int)byteArr[i] << " and in dec: " << std::dec << (unsigned int)byteArr[i] << "\n";
                    }
                }
                else {
                    bool flag = false;
                    for (int i = 0; i < sie; ++i) {
                        std::cout << "Input element (binary):\n";
                        std::cin >> str;
                        flag = !(checkString(str) && (str.size() <= 16));
                        if (flag) {
                            break;
                        }
                        wordArr[i] = std::strtol(str.c_str(), NULL, 2);
                    }
                    if (flag)
                        continue;
                    std::cout << "Inputed array:\n";
                    for (int i = 0; i < sie; ++i) {
                        std::cout << "wordArray[" << i << "] in binary view: " << std::bitset<16>(wordArr[i]) << " and in hex: 0x" << std::hex << (unsigned int)wordArr[i] << " and in dec: " << std::dec << (unsigned int)wordArr[i] << "\n";
                    }
                }
                __asm {
                    mov cx, sie; //поместили кол-во элементов в cx (5 или 6)
                    mov ax, mode; //поместили выбранный режим (0 для байтового, 1 для слов)
                    lea esi, wordArr; //поместили адрес массива wordArray в esi, если условие пройдет нижнее
                    cmp ax, 0h; //сравниои ax с 1h в 16-чном представлении
                    jne WORDMODE; //если ax не равен 1h, то работаем с массивом слов
                    lea esi, byteArr; //поместили адрес массива byteArray в esi
                    jmp L;
                L: //метка для цикла по элементам массива байт
                    mov al, [esi]; //значение байта, хранящегося по адресу esi, из памяти поместили в регистр al
                    //mov tmp1, al;
                    mov bl, al; //поместили значение al в bl
                    mov dl, al; //поместили значение al в dl
                    and al, 01100000b; //выделили битовое поле 6:5 в al
                    shr al, 5; //логический сдвиг вправо на 5 разрядов, прижали к правому краю
                    and bl, 00000110b; //выделили битовое поле 2:1 в bl
                    shr bl, 1; //логический сдвиг вправо на 1 разряд, прижали к правому краю
                    xor al, bl; //применили инверсию и сохранили в al
                    //проверим на равенство 0, если не так, то 6:5 биты являются инверсией битов 2:1, иначе нет | даже должно выйти так, чтобы xor выдал в конце 11
                    //cmp al, 0;
                    cmp al, 00000011b;
                    //jne INV; // перешли к сценарию с инверсией
                    je INV;
                    inc esi; //перешли к следующему адресу
                    dec cx; //уменьшили счетчик
                    cmp cx, 0; //проверили счетчик
                    jne L; //вернулись в начало цикла
                    jmp FINAL; //ушли в самый конец иначе

                INV:
                    or dl, 10000001b; //установили старший и младший биты
                    mov [esi], dl; //вернули измененный элемент массива
                    inc esi; //перешли к следующему адресу
                    dec cx; //уменьшили счетчик
                    cmp cx, 0; //проверили счетчик
                    jne L; //вернулись в начало цикла
                    jmp FINAL; //ушли в самый конец иначе

                WORDMODE:
                    mov ax, [esi]; //значение слова, хранящегося по адресу esi, из памяти поместили в регистр al
                    mov bx, ax; //поместили значение ax в bx
                    mov dx, ax; //поместили значение ax в dx
                    and ax, 0000000001100000b; //выделили поле 6:5
                    shr ax, 5; //логический сдвиг вправо на 5 разрядов, прижали к правому краю
                    and bx, 0000000000000110b; // выделили поле 2:1
                    shr bx, 1; //логический сдвиг вправо на 1 разряд, прижали к правому краю
                    xor ax, bx; //применили инвертирование битов ax относительно bx и сохранили результат в ax
                    //cmp ax, 0; //если не равно 0, тогда биты 6:5 являются инверсией битов 2:1
                    //jne INVW;
                    cmp ax, 0000000000000011b;
                    je INVW;
                    inc esi; //перешли к следующему адресу
                    inc esi;
                    dec cx; //уменьшили счетчик
                    cmp cx, 0; //проверили счетчик
                    jne WORDMODE; //вернулись в начало цикла
                    jmp FINAL; //ушли в самый конец иначе
                INVW:
                    or dx, 1000000000000001b; //установили старший и младший биты
                    mov [esi], dx; //вернули измененный элемент массива
                    inc esi; //перешли к следующему адресу
                    inc esi;
                    dec cx; //уменьшили счетчик
                    cmp cx, 0; //проверили счетчик
                    jne WORDMODE; //вернулись в начало цикла
                    jmp FINAL; //ушли в самый конец иначе
                FINAL:
                    nop;
                }
                std::cout << std::bitset<8>(tmp)<<" " << std::bitset<8>(tmp1)<<" " << std::bitset<8>(tmp2)<<"\n";
                std::cout << "Output array:\n";
                if (mode == 0) {
                    for (int i = 0; i < sie; ++i) {
                        std::cout << "byteArray[" << i << "] in binary view: " << std::bitset<8>(byteArr[i]) << " and in hex: 0x" << std::hex << (unsigned int)byteArr[i] << " and in dec: " << std::dec << (unsigned int)byteArr[i] << "\n";
                    }
                }
                else if (mode == 1) {
                    for (int i = 0; i < sie; ++i) {
                        std::cout << "wordArray[" << i << "] in binary view: " << std::bitset<16>(wordArr[i]) << " and in hex: 0x" << std::hex << (unsigned int)wordArr[i] << " and in dec: " << std::dec << (unsigned int)wordArr[i] << "\n";
                    }
                }
                std::cout << "Again? (0 or 1)\n";
                std::cin >> str;
                if (str != "1")
                    break;
            }
            else {
                std::cout << "Invalid values!\n";
            }
        }
        else {
            std::cout << "Invalid value!\n";
        }
    }
    exit(0);
}