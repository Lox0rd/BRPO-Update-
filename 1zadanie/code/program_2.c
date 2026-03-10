#include <stdio.h>
//#include <Windows.h>

int dvachisla() {
    //SetConsoleCP(1251);
    //SetConsoleOutputCP(1251);

    char num1_str[1000];  
    char num2_str[1000];

    long long num1 = 0;
    long long num2 = 0;

    unsigned long long sum;
    unsigned long long umn;

    int i;  

    printf("Введите первое число: ");
    scanf("%s1000", num1_str); //изменения

    printf("Введите второе число: ");
    scanf("%s1000", num2_str); //изменения

    i = 0;
    int sign1 = 1;
    if (num1_str[0] == '-') {
        sign1 = -1;
        i = 1;
    }

    num1 = 0;
    while (num1_str[i] != '\0') {
        num1 = num1 * 10 + (num1_str[i] - '0');
        i++;
    }
    num1 = num1 * sign1;

    i = 0;
    int sign2 = 1;
    if (num2_str[0] == '-') {
        sign2 = -1;
        i = 1;
    }

    num2 = 0;
    while (num2_str[i] != '\0') {
        num2 = num2 * 10 + (num2_str[i] - '0');
        i++;
    }
    num2 = num2 * sign2;

    printf("\nсложение\n");
    printf("  %s\n", num1_str);
    printf("+ %s\n", num2_str);

    sum = (unsigned long long)num1 + num2;
    printf("= %llu\n", sum);

    printf("\nумножение\n");
    printf("  %s\n", num1_str);
    printf("* %s\n", num2_str);

    umn = (unsigned long long)num1 * num2;
    printf("= %llu\n", umn);

    printf("\nПромежуточные результаты умножения:\n");

    long long temp_num2 = num2;
    if (temp_num2 < 0) {
        temp_num2 = -temp_num2;
    }

    int czifra[20]; 
    int c = 0;

    if (temp_num2 == 0) {
        czifra[0] = 0;
        c = 1;
    }
    else {
        while (temp_num2 > 0) {
            czifra[c] = temp_num2 % 10;
            temp_num2 = temp_num2 / 10;
            c++;
        }
    }

    for (int j = 0; j < c; j++) {
        int znach = czifra[j];
        long long promezh = num1 * znach; 
        printf("  %s * %d = %lld\n", num1_str, znach, promezh);
    }

    return 0;
}
