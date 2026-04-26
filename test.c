#include <stdio.h>

int main() {
    int a, b, c;
    scanf("%d %d %d", &a, &b, &c);

    c += a+b;
    for (int i = 0; i < c; i++) printf("%d", i);

    return 0;
}