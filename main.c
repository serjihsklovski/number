#include <stdio.h>
#include "number.h"


int main() {
    Number a = new_Number("7");
    Number b = new_Number("06");
    Number c = new_Number("+9");
    Number d = new_Number("-5");
    Number e = new_Number("79");
    Number f = new_Number("~4");
//    Number c = new_Number("-4124532521");           // integer negative
//    Number d = new_Number("-235246.132467545");     // fraction negative
//    Number e = new_Number("1_000_000.432");         // '_' separator
//    Number f = new_Number("-1`000`000.999`999");    // '`' separator
//    Number g = new_Number("~3.14");                 // approximate value
//    Number h = new_Number("~-1_000_000`000.999");   // all inclusive

//    print_number(a);
//    print_number(b);
//    print_number(c);
//    print_number(d);
//    print_number(e);
//    print_number(f);
//    print_number(g);
//    print_number(h);

    delete_Number(a);
    delete_Number(b);
    delete_Number(c);
    delete_Number(d);
    delete_Number(e);
    delete_Number(f);

    return 0;
}
