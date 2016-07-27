#include <stdio.h>
#include "number.h"


int main() {
    Number a = new_Number("7");
    Number b = new_Number("06");
    Number c = new_Number("+9");
    Number d = new_Number("-5");
    Number e = new_Number("79");
    Number f = new_Number("~4");
    Number g = new_Number("-4124532521");           // integer negative
    Number h = new_Number("~-0000003242");
    Number i = new_Number("~-003443563.1");
    Number j = new_Number("~00000000000");
//    Number d = new_Number("-235246.132467545");     // fraction negative
//    Number e = new_Number("1_000_000.432");         // '_' separator
//    Number f = new_Number("-1`000`000.999`999");    // '`' separator
//    Number g = new_Number("~3.14");                 // approximate value
//    Number h = new_Number("~-1_000_000`000.999");   // all inclusive

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

    printf("i = ");
    print_number(i);
    puts("");

    printf("j = ");
    print_number(j);
    puts("");

//    print_number(g);
//    print_number(h);

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
