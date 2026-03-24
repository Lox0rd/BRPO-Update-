#include <stdio.h>
#include <stdlib.h>

int isPrime(int num) {
   if (num < 2) return 0;
   if (num == 2) return 1;
   if (num % 2 == 0) return 0;
   
   for (int i = 3; i * i <= num; i += 2) {
       if (num % i == 0) {
           return 0;
       }
   }
   return 1;
}

int sumOfDigitsInBase7(int num) {
   int sum = 0;
   while (num > 0) {
       sum += num % 7; 
       num /= 7;
   }
   return sum;
}

int main() {
   const int MAX_N = 10000;
   
   int* primes = (int*)malloc((MAX_N / 2 + 2) * sizeof(int));
   int primeCount = 0;
   
   struct Exception {
       int prime;
       int digitSum;
   };
   struct Exception* exceptions = (struct Exception*)malloc((MAX_N / 2 + 2) * sizeof(struct Exception));
   int exceptionCount = 0;
   
   primes[primeCount++] = 2;
   
   for (int num = 3; num <= MAX_N; num += 2) {
       int isPrimeNum = 1;
       
       for (int i = 0; i < primeCount; i++) {
           if (primes[i] * primes[i] > num) {
               break;
           }
           if (num % primes[i] == 0) {
               isPrimeNum = 0;
               break;
           }
       }
       
       if (isPrimeNum) {
           primes[primeCount++] = num;
           
           int digitSum = sumOfDigitsInBase7(num);
           if (!isPrime(digitSum)) {
               exceptions[exceptionCount].prime = num;
               exceptions[exceptionCount].digitSum = digitSum;
               exceptionCount++;
           }
       }
   }
   
   printf("\nИсключения (простые числа, у которых сумма цифр в 7-ричной системе НЕ является простым):\n");
   if (exceptionCount == 0) {
       printf("Исключений не найдено\n");
   } else {
       printf("Число  иСумма цифр (7-ричн.)\n");
       for (int i = 0; i < exceptionCount; i++) {
           printf("%d %d\n", exceptions[i].prime, exceptions[i].digitSum);
       }
   }
   
   printf("\nСтатистика:\n");
   printf("Всего простых чисел до %d: %d\n", MAX_N, primeCount);
   printf("Найдено исключений: %d\n", exceptionCount);
   
   free(primes);
   free(exceptions);
   
   return 0;
}