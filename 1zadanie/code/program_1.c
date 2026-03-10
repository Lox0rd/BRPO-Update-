#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int isRoman(char chislo[]) {
    char *valid_el = "IVXCDLM";
    for(int i = 0; i < strlen(chislo); i++) {
        if (strchr(valid_el, chislo[i]) == NULL) {
            return 1;
        }
    }
    return 0;
}

char *roman_to_units(const char *roman) {
    //Проверка входных данных
    if (!roman || *roman == '\0') return NULL;

    int len = strlen(roman);
    if (len >= 1000) return NULL; //Ограничение на длину входной строки

    //Буфер для нормализованной строки
    char normalized[4000] = {0};
    strcpy(normalized, roman);

    const char *patterns[] = {"CM", "CD", "XC", "XL", "IX", "IV"};
    const char *replacements[] = {"DCCCC", "CCCC", "LXXXX", "XXXX", "VIIII", "IIII"};
    int n_patterns = 6;

    for (int p = 0; p < n_patterns; p++) {
        const char *pattern = patterns[p];
        const char *replacement = replacements[p];
        int pat_len = strlen(pattern);
        int rep_len = strlen(replacement);

        char temp[4000] = {0};
        const char *start = normalized;
        char *out = temp;

        while (1) {
            const char *match = strstr(start, pattern);
            if (!match) {
                //Копируем остаток строки
                strcpy(out, start);
                break;
            }
            //Копируем часть до совпадения
            strncpy(out, start, match - start);
            out += (match - start);
            //Вставляем замену
            strcpy(out, replacement);
            out += rep_len;
            //Перемещаемся за совпадение
            start = match + pat_len;
        }
        // Обновляем normalized
        strcpy(normalized, temp);
    }

    //Подсчёт общего количества I
    int total_i = 0;
    for (int i = 0; normalized[i] != '\0'; i++) {
        char c = toupper(normalized[i]);
        switch (c) {
            case 'I': total_i += 1; break;
            case 'V': total_i += 5; break;
            case 'X': total_i += 10; break;
            case 'L': total_i += 50; break;
            case 'C': total_i += 100; break;
            case 'D': total_i += 500; break;
            case 'M': total_i += 1000; break;
            default: return NULL; // Недопустимый символ
        }
    }

    //Формирование результата
    char *result = (char *)malloc(total_i + 1);
    if (!result) return NULL;

    int pos = 0;
    for (int i = 0; normalized[i] != '\0'; i++) {
        char c = toupper(normalized[i]);
        int count = 0;
        switch (c) {
            case 'I': count = 1; break;
            case 'V': count = 5; break;
            case 'X': count = 10; break;
            case 'L': count = 50; break;
            case 'C': count = 100; break;
            case 'D': count = 500; break;
            case 'M': count = 1000; break;
        }
        for (int j = 0; j < count; j++) {
            result[pos++] = 'I';
        }
    }
    result[pos] = '\0';

    return result;
}

char *units_to_roman(const char *units) {
    //Проверка входных данных
    if (!units || *units == '\0') return NULL;

    int len = strlen(units);
    if (len <= 0 || len > 100) return NULL;  //Ограничение: 1–100

    //Буфер для результата
    char *result = (char *)calloc(10, sizeof(char));
    if (!result) return NULL;

    int pos = 0;      //Текущая позиция в result
    int value = len;  //Числовое значение = длина строки из I

    const int values[] = {
        100, 90, 50, 40,
        10,  9,  5,  4,
        1
    };
    const char *symbols[] = {
        "C", "XC", "L", "XL",
        "X", "IX", "V", "IV",
        "I"
    };
    int n = sizeof(values) / sizeof(values[0]);

    //Формируем римскую запись
    for (int i = 0; i < n; i++) {
        while (value >= values[i]) {
            strcpy(result + pos, symbols[i]);
            pos += strlen(symbols[i]);
            value -= values[i];
        }
    }

    return result;
}

char *roman_units_diff(const char *str1, const char *str2) {
    //Проверка входных данных
    if (!str1 || !str2) {
        return NULL;
    }

    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    //Вычисляем разницу длин
    size_t diff;
    if (len1 > len2) {
        diff = len1 - len2;
    } else {
        diff = len2 - len1;
    }

    //Если разница 0 — возвращаем пустую строку (только '\0')
    if (diff == 0) {
        char *result = calloc(1, sizeof(char));
        return result;
    }

    //Выделяем память под результат (diff символов 'I' + завершающий '\0')
    char *result = calloc(diff + 1, sizeof(char));
    if (!result) {
        return NULL; //Ошибка выделения памяти
    }

    //Заполняем строку символами 'I'
    memset(result, 'I', diff);
    result[diff] = '\0';

    return result;
}

int romantoint() {
    char roman1[100], roman2[100];
    char *units1, *units2, *diff_units, *result_roman;

    printf("Введите первое римское число: ");
    if (scanf("%99s", roman1) != 1 || isRoman(roman1)) {
        printf("Ошибка ввода первого числа.\n");
        return 1;
    }

    printf("Введите второе римское число: ");
    if (scanf("%99s", roman2) != 1 || isRoman(roman2)) {
        printf("Ошибка ввода Второго числа.\n");
        return 1;
    }

    //Преобразуем римские числа в строки из 'I'
    units1 = roman_to_units(roman1);
    units2 = roman_to_units(roman2);
    
    //Вычисляем разницу длин строк
    diff_units = roman_units_diff(units1, units2);

    //Если разница равна 0, результат — пустая строка
    if (strlen(diff_units) == 0) {
        printf("Результат:\n");
    }
    else {
        //Преобразуем разницу обратно в римскую запись
        result_roman = units_to_roman(diff_units);
        printf("Результат: %s\n", result_roman);
        free(result_roman);  //Освобождаем память
    }

    //Освобождаем всю выделенную память
    free(units1);
    free(units2);
    free(diff_units);

    return 0;
}