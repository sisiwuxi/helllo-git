#include <stdio.h>
#include <math.h>
int main(void)
{
    double result;
    double x = 4.0;
    result = exp(x);
    printf("'e'raised to the power\\of%lf(e^%lf)=%lf\n", x, x, result);
    return 0;
}
