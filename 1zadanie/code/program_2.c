#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Функция для чтения строки из бинарного файла
char* read_string(FILE *file) {
    int len;
    size_t read_len;

    read_len = fread(&len, sizeof(int), 1, file);
    if (read_len != 1) {
        return NULL;
    }

    char* str = (char*)malloc(len + 1);
    if (str == NULL) {
        return NULL;
    }

    read_len = fread(str, sizeof(char), len, file);
    if (read_len != len) {
        free(str);
        return NULL;
    }

    str[len] = '\0';
    return str;
}

// Функция для записи строки в бинарный файл
void write_string(FILE* file, const char* str) {
    int len = (int)strlen(str);
    fwrite(&len, sizeof(int), 1, file);
    fwrite(str, sizeof(char), len, file);
}

int dvachisla() {
    FILE* file;

    // Создаём файл для записи
    file = fopen("input.bin", "wb");
    if (file == NULL) {
        printf("Ошибка создания файла input.bin\n");
        return 1;
    }
    
    // Берём 2 числа
    char number1[] = "123456789";
    char number2[] = "987654321";

    printf("Записываем числа в input.bin:\n");
    printf("  Первое число: %s\n", number1);
    printf("  Второе число: %s\n", number2);

    write_string(file, number1);
    write_string(file, number2);

    fclose(file);
    printf("Файл input.bin создан\n\n");

    // Читаем числа
    file = fopen("input.bin", "rb");
    if (file == NULL) {
        printf("Ошибка открытия файла input.bin\n");
        return 1;
    }

    char* num1_str = read_string(file);
    char* num2_str = read_string(file);

    fclose(file);

    if (num1_str == NULL || num2_str == NULL) {
        printf("Ошибка чтения данных из файла\n");
        if (num1_str)
            free(num1_str);
        if (num2_str)
            free(num2_str);
        return 1;
    }

    printf("Прочитаны числа:\n");
    printf("  Первое число: %s\n", num1_str);
    printf("  Второе число: %s\n\n", num2_str);

    // Вычисления
    printf("Вычисления\n");

    long long num1 = strtoll(num1_str, NULL, 10);
    long long num2 = strtoll(num2_str, NULL, 10);

    long long sum = num1 + num2;
    long long umn = num1 * num2;

    printf("\nСложение столбиком:\n");
    printf("  %s\n", num1_str);
    printf("+ %s\n", num2_str);
    printf("= %lld\n\n", sum);

    printf("Умножение столбиком:\n");
    printf("  %s\n", num1_str);
    printf("* %s\n", num2_str);
    printf("= %lld\n\n", umn);

    // Промежуточные результаты
    long long temp_num2 = llabs(num2);
    int digits[20];
    int digit_count = 0;

    if (temp_num2 == 0) {
        digits[0] = 0;
        digit_count = 1;
    } else {
        while (temp_num2 > 0) {
            digits[digit_count] = (int)(temp_num2 % 10);
            temp_num2 = temp_num2 / 10;
            digit_count++;
        }
    }

    printf("Промежуточные результаты умножения:\n");
    for (int j = 0; j < digit_count; j++) {
        int digit = digits[j];
        long long intermediate = num1 * digit;

        if (num2 < 0 && j == digit_count - 1) {
            intermediate = -intermediate;
        }

        printf("  %s * %d = %lld", num1_str, digit, intermediate);
        printf("\n");
    }

    // Сохраняем результаты
    printf("\nСохраняем результаты в output.bin\n");
    file = fopen("output.bin", "wb");
    if (file == NULL) {
        printf("Ошибка открытия файла для записи\n");
        free(num1_str);
        free(num2_str);
        return 1;
    }

    // Записываем исходные числа
    write_string(file, num1_str);
    write_string(file, num2_str);

    char result_str[500];

    // Записываем сумму
    sprintf(result_str, "Сумма: %lld\n", sum);
    write_string(file, result_str);

    // Записываем произведение
    sprintf(result_str, "Произведение: %lld\n", umn);
    write_string(file, result_str);

    // Записываем промежуточные результаты
    write_string(file, "Промежуточные результаты умножения:\n");

    for (int j = 0; j < digit_count; j++) {
        int digit = digits[j];
        long long intermediate = num1 * digit;

        if (num2 < 0 && j == digit_count - 1) {
            intermediate = -intermediate;
        }

        sprintf(result_str, "%s * %d = %lld\n", num1_str, digit, intermediate);
        write_string(file, result_str);
    }

    fclose(file);
    printf("Результаты сохранены в файл output.bin\n");

    // Смотрим содержимое
    printf("\nСмотрим содержимое output.bin\n");

    file = fopen("output.bin", "rb");
    if (file != NULL) {
        printf("\nСодержимое:\n");

        char* read_str;

        // Читаем и выводим все строки из output.bin
        while (1) {
            read_str = read_string(file);
            if (read_str == NULL || feof(file)) {
                if (read_str) free(read_str);
                break;
            }
            printf("%s", read_str);
            free(read_str);
        }

        fclose(file);
    }

    free(num1_str);
    free(num2_str);

    // Пауза (опционально): ждём нажатия Enter
    printf("Нажмите Enter для завершения...\n");
    getchar();

    return 0;
}
