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

//TODO: ��������� �������� �� ���� ������, ��� �������� �����, ����� ����� ������ � ����������� �����. ����� �������� �� ����� ��� �� �������� ����
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
                    mov cx, sie; //��������� ���-�� ��������� � cx (5 ��� 6)
                    mov ax, mode; //��������� ��������� ����� (0 ��� ���������, 1 ��� ����)
                    lea esi, wordArr; //��������� ����� ������� wordArray � esi, ���� ������� ������� ������
                    cmp ax, 0h; //�������� ax � 1h � 16-���� �������������
                    jne WORDMODE; //���� ax �� ����� 1h, �� �������� � �������� ����
                    lea esi, byteArr; //��������� ����� ������� byteArray � esi
                    jmp L;
                L: //����� ��� ����� �� ��������� ������� ����
                    mov al, [esi]; //�������� �����, ����������� �� ������ esi, �� ������ ��������� � ������� al
                    //mov tmp1, al;
                    mov bl, al; //��������� �������� al � bl
                    mov dl, al; //��������� �������� al � dl
                    and al, 01100000b; //�������� ������� ���� 6:5 � al
                    shr al, 5; //���������� ����� ������ �� 5 ��������, ������� � ������� ����
                    and bl, 00000110b; //�������� ������� ���� 2:1 � bl
                    shr bl, 1; //���������� ����� ������ �� 1 ������, ������� � ������� ����
                    xor al, bl; //��������� �������� � ��������� � al
                    //�������� �� ��������� 0, ���� �� ���, �� 6:5 ���� �������� ��������� ����� 2:1, ����� ��� | ���� ������ ����� ���, ����� xor ����� � ����� 11
                    //cmp al, 0;
                    cmp al, 00000011b;
                    //jne INV; // ������� � �������� � ���������
                    je INV;
                    inc esi; //������� � ���������� ������
                    dec cx; //��������� �������
                    cmp cx, 0; //��������� �������
                    jne L; //��������� � ������ �����
                    jmp FINAL; //���� � ����� ����� �����

                INV:
                    or dl, 10000001b; //���������� ������� � ������� ����
                    mov [esi], dl; //������� ���������� ������� �������
                    inc esi; //������� � ���������� ������
                    dec cx; //��������� �������
                    cmp cx, 0; //��������� �������
                    jne L; //��������� � ������ �����
                    jmp FINAL; //���� � ����� ����� �����

                WORDMODE:
                    mov ax, [esi]; //�������� �����, ����������� �� ������ esi, �� ������ ��������� � ������� al
                    mov bx, ax; //��������� �������� ax � bx
                    mov dx, ax; //��������� �������� ax � dx
                    and ax, 0000000001100000b; //�������� ���� 6:5
                    shr ax, 5; //���������� ����� ������ �� 5 ��������, ������� � ������� ����
                    and bx, 0000000000000110b; // �������� ���� 2:1
                    shr bx, 1; //���������� ����� ������ �� 1 ������, ������� � ������� ����
                    xor ax, bx; //��������� �������������� ����� ax ������������ bx � ��������� ��������� � ax
                    //cmp ax, 0; //���� �� ����� 0, ����� ���� 6:5 �������� ��������� ����� 2:1
                    //jne INVW;
                    cmp ax, 0000000000000011b;
                    je INVW;
                    inc esi; //������� � ���������� ������
                    inc esi;
                    dec cx; //��������� �������
                    cmp cx, 0; //��������� �������
                    jne WORDMODE; //��������� � ������ �����
                    jmp FINAL; //���� � ����� ����� �����
                INVW:
                    or dx, 1000000000000001b; //���������� ������� � ������� ����
                    mov [esi], dx; //������� ���������� ������� �������
                    inc esi; //������� � ���������� ������
                    inc esi;
                    dec cx; //��������� �������
                    cmp cx, 0; //��������� �������
                    jne WORDMODE; //��������� � ������ �����
                    jmp FINAL; //���� � ����� ����� �����
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