#include <stdio.h>
#include <math.h>
#include <locale.h>

const double TARGET_ANGLE = 45.0; // Целевой угол в градусах
const int MINUTES_IN_HOUR = 60;
const int HOURS_ON_CLOCK = 12;
const double DEGREES_PER_MINUTE = 6.0; // 360/60
const double DEGREES_PER_HOUR = 30.0; // 360/12
const double DEGREES_PER_MINUTE_FOR_HOUR_HAND = 0.5; // 30/60

// Функция для вычисления угла между стрелками в заданное время
double calculateAngle(int hours, int minutes) {
    // Нормализуем часы (12-часовой формат)
    hours = hours % HOURS_ON_CLOCK;

    // Вычисляем углы стрелок от 12 часов
    double minuteAngle = minutes * DEGREES_PER_MINUTE;
    double hourAngle = hours * DEGREES_PER_HOUR + minutes * DEGREES_PER_MINUTE_FOR_HOUR_HAND;

    // Вычисляем абсолютную разницу углов
    double diff = fabs(hourAngle - minuteAngle);

    // Возвращаем минимальный угол (если разница > 180, берём дополнение до 360)
    return (diff < (360.0 - diff)) ? diff : (360.0 - diff);
}

// Функция для поиска ближайшего времени через перебор
int findTimeToAngleByBruteForce(int currentHours, int currentMinutes, double targetAngle) {
    currentHours = currentHours % HOURS_ON_CLOCK;
    if (currentHours == 0) currentHours = 12;

    // Проверяем ближайшие 12 часов (720 минут)
    for (int minutesToAdd = 0; minutesToAdd <= 720; ++minutesToAdd) {
        int totalMinutes = currentMinutes + minutesToAdd;
        int addHours = totalMinutes / 60;
        int newMinutes = totalMinutes % 60;
        int newHours = (currentHours + addHours) % 12;
        if (newHours == 0) newHours = 12;

        double angle = calculateAngle(newHours, newMinutes);

        if (fabs(angle - targetAngle) < 0.1) {
            return minutesToAdd;
        }
    }

    return -1; // не найдено
}

// Функция для точного аналитического решения
double findTimeToAngleAnalytical(int hours, int minutes, double targetAngle) {
    // Переводим всё в минуты от 00:00
    double totalMinutesFromStart = hours * 60.0 + minutes;

    // Относительная скорость между стрелками: 5.5 градусов в минуту
    // (минутная стрелка движется на 6°/мин, часовая на 0.5°/мин,
    //  разница = 6 - 0.5 = 5.5°/мин)
    double relativeSpeed = 5.5; // градусов в минуту

    // Вычисляем текущий угол между стрелками
    double currentAngle = calculateAngle(hours % 12, minutes);

    // Разница между текущим углом и целевым
    double angleDiff = targetAngle - currentAngle;

    // Есть два случая: можем достичь угла 45° или угла 315° (который тоже равен 45°)
    // Так как угол всегда берётся минимальный между 0 и 180 градусами

    // Случай 1: стрелки сближаются
    double time1 = angleDiff / relativeSpeed;

    // Случай 2: стрелки удаляются (но могут снова сблизиться до 45° с другой стороны)
    // Для этого нужно пройти дополнение: (360 - 2*targetAngle) градусов
    double time2 = (360.0 - 2.0 * targetAngle - angleDiff) / relativeSpeed;

    // Случай 3: ещё один полный оборот
    double time3 = (360.0 + angleDiff) / relativeSpeed;

    // Ищем минимальное положительное время
    double minTime = -1;

    if (time1 > 1e-9) {
        minTime = time1;
    }

    if (time2 > 1e-9 && (minTime < 0 || time2 < minTime)) {
        minTime = time2;
    }

    if (time3 > 1e-9 && (minTime < 0 || time3 < minTime)) {
        minTime = time3;
    }

    // Проверяем, что время меньше 12 часов (720 минут)
    if (minTime > 0 && minTime < 720) {
        // Проверяем результат
        int checkMinutes = (int)round(minTime);
        int newTotalMinutes = minutes + checkMinutes;
        int addHours = newTotalMinutes / 60;
        int newMinutes = newTotalMinutes % 60;
        int newHours = (hours % 12 + addHours) % 12;
        if (newHours == 0) newHours = 12;

        double verifyAngle = calculateAngle(newHours, newMinutes);

        if (fabs(verifyAngle - targetAngle) < 0.1) {
            return minTime;
        }
    }

    return -1;
}

int clock_main() {
    int hours, minutes;
    setlocale(LC_ALL, "Russian");
    printf("Введите время (часы и минуты через пробел): ");
    scanf("%d %d", &hours, &minutes);

    // Проверка корректности ввода
    if (hours < 0 || hours >= 24 || minutes < 0 || minutes >= 60) {
        printf("Некорректное время!\n");
        return 1;
    }

    // Конвертируем в 12-часовой формат
    int hours12 = hours % 12;
    if (hours12 == 0) hours12 = 12;

    printf("\nТекущее время: %02d:%02d (%d:%02d в 12-часовом формате)\n",
           hours, minutes, hours12, minutes);

    // Вычисляем текущий угол
    double currentAngle = calculateAngle(hours12, minutes);
    printf("Текущий угол между стрелками: %.2f градусов\n", currentAngle);

    // Проверяем, не равен ли текущий угол уже 45°
    if (fabs(currentAngle - TARGET_ANGLE) < 0.1) {
        printf("\nТекущий угол уже равен 45 градусам!\n");
        return 0;
    }

    // Используем аналитический метод
    double timeToTarget = findTimeToAngleAnalytical(hours12, minutes, TARGET_ANGLE);

    if (timeToTarget > 0) {
        int minutesToWait = (int)round(timeToTarget);

        // Вычисляем будущее время
        int totalMinutes = minutes + minutesToWait;
        int addHours = totalMinutes / 60;
        int futureMinutes = totalMinutes % 60;
        int futureHours = (hours12 + addHours) % 12;
        if (futureHours == 0) futureHours = 12;

        printf("\nАналитический метод:\n");
        printf("Через %d минут(ы) угол будет 45 градусов\n", minutesToWait);
        printf("Будущее время: %d:%02d\n", futureHours, futureMinutes);

        // Проверка
        double verifyAngle = calculateAngle(futureHours, futureMinutes);
        printf("Проверка: угол в это время = %.2f градусов\n", verifyAngle);
    }

    // Используем метод перебора для проверки
    int minutesToWaitBrute = findTimeToAngleByBruteForce(hours12, minutes, TARGET_ANGLE);

    if (minutesToWaitBrute >= 0) {
        // Вычисляем будущее время для перебора
        int totalMinutes = minutes + minutesToWaitBrute;
        int addHours = totalMinutes / 60;
        int futureMinutes = totalMinutes % 60;
        int futureHours = (hours12 + addHours) % 12;
        if (futureHours == 0) futureHours = 12;

        printf("\nМетод перебора:\n");
                printf("Через %d минут(ы) угол будет 45 градусов\n", minutesToWaitBrute);
        printf("Будущее время: %d:%02d\n", futureHours, futureMinutes);
    } else {
        printf("\nМетод перебора: угол 45 градусов не будет достигнут в ближайшие 12 часов\n");
    }

    // Пример расчёта для времени 16:25 (4:25)
    printf("\n--- Пример расчёта для 16:25 (4:25) ---\n");

    // Время 4:25
    int h = 4;
    int m = 25;

    // Вычисляем углы стрелок
    double hourAngle = h * 30 + m * 0.5; // 4*30 + 25*0.5 = 120 + 12.5 = 132.5°
    double minuteAngle = m * 6; // 25*6 = 150°

    // Разность углов
    double diff = fabs(hourAngle - minuteAngle); // |132.5 - 150| = 17.5°
    double currentAngle2 = (diff < (360 - diff)) ? diff : (360 - diff); // min(17.5, 342.5) = 17.5°

    printf("Время 4:25:\n");
    printf("  Часовая стрелка: %.1f°\n", hourAngle);
    printf("  Минутная стрелка: %.0f°\n", minuteAngle);
    printf("  Текущий угол: %.1f°\n", currentAngle2);

    // Разница до 45°
    double angleDiff = 45.0 - currentAngle2; // 45 - 17.5 = 27.5°

    // Относительная скорость: 5.5°/мин
    double timeNeeded = angleDiff / 5.5; // 27.5 / 5.5 = 5 минут

    printf("  Разница до 45°: %.1f°\n", angleDiff);
    printf("  Относительная скорость: 5.5°/мин\n");
    printf("  Время до 45°: %.0f минут\n", timeNeeded);

    return 0;
}
