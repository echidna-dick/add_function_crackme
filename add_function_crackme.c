#include <stdio.h>

int add_to_nums(int a, int b){
    int result = a + b;
    return result;
}
int main(){
    int num1  = 10;
    int num2 = 20;
    int sum;

    sum = add_to_nums(num1, num2);
    printf("the sum is: %d", sum);
    return 0;
}
