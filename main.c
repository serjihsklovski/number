#include <stdio.h>
#include "number.h"


int main() {
    Number a = new_Number("7");
    Number b = new_Number("06");
    Number c = new_Number("+94");
    Number d = new_Number("-534");
    Number e = new_Number("79432");
    Number f = new_Number("~4532");
    Number g = new_Number("-4124532521");
    Number h = new_Number("~-000000_0");
    Number i = new_Number("~-003443563.1");
    Number j = new_Number("~00000000000");

    printf("a = ");
    print_number(a);
    puts("");

    printf("b = ");
    print_number(b);
    puts("");

    printf("c = ");
    print_number(c);
    puts("");

    printf("d = ");
    print_number(d);
    puts("");

    printf("e = ");
    print_number(e);
    puts("");

    printf("f = ");
    print_number(f);
    puts("");

    printf("g = ");
    print_number(g);
    puts("");

    printf("h = ");
    print_number(h);
    puts("");

    printf("i = ");
    print_number(i);
    puts("");

    printf("j = ");
    print_number(j);
    puts("");

    delete_Number(a);
    delete_Number(b);
    delete_Number(c);
    delete_Number(d);
    delete_Number(e);
    delete_Number(f);
    delete_Number(g);
    delete_Number(h);
    delete_Number(i);
    delete_Number(j);

    return 0;
}
