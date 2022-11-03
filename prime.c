#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

long unsigned int rand_num_gen() {
    long unsigned int rand_num;
    time_t t;
    long unsigned int rand_max = 9999999999;
    long unsigned int rand_min = 1000000000;

    srand((unsigned) time(&t));
    rand_num = rand_min + rand() % (rand_max + 1 - rand_min);
    printf("%lu\n", rand_num);
    return rand_num;
}

bool prime_check(long unsigned int num) {
    long i;
    long unsigned int m;
    bool check = true;

    m = num / 2;
    for(i = 2; i <= m; i++) {
        if(num % i == 0) {
            //printf("Entered number is not prime");
            check = false;
            break;
        }
    }
    //1 not prime
    //0 is prime
    return check;
}

long unsigned int high_prime_find(long unsigned int num) {
    bool check = false;
    while(check == false) {
        num++;
        check = prime_check(num);
    }
    long unsigned int high_prime = num;
    printf("%s", "Prime found: ");
    printf("%lu\n", high_prime);
    return high_prime;
}

int main() {
    printf("%s\n", "Prime Finder has started");
    long unsigned int rand_num = rand_num_gen();
    long unsigned int curr_high_prime = high_prime_find(rand_num);
    while(true) {
        curr_high_prime = high_prime_find(curr_high_prime);
    }
}
