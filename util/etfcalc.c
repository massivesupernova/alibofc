#include <stdio.h>

int main() {
  double gridRate = 0;
  double startPrice = 0;
  printf("input startPrice and gridRate:");
  if (scanf("%lf %lf", &startPrice, &gridRate) != 2) {
    printf("error occured when parsing input value\n");
    return 1;
  }
  if (gridRate <=0 || gridRate >= 1 || startPrice <= 0) {
    printf("input values are invalid\n");
    return 1;
  }
  double gridPrice = startPrice * gridRate;
  double curPrice = startPrice;
  double nextPrice = curPrice - gridPrice;
  while (nextPrice > 0) {
    printf("%8lf %8lf\n", curPrice, gridPrice/nextPrice);
    curPrice = nextPrice;
    nextPrice = curPrice - gridPrice;
  }
  return 0;
}
