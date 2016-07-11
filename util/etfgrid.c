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
  double prevPrice = startPrice;
  double price = prevPrice - gridPrice;
  double prevPriceInt = 0;
  double priceInt = 0;
  while (price > 0 && gridPrice/price <= 1.0) {
    prevPriceInt = ((int)(prevPrice * 1000.0)) / 1000.0;
    priceInt = ((int)(price * 1000.0)) / 1000.0;
    printf("%5.3lf %6.3lf %8lf %8lf\n", priceInt,
        (prevPriceInt-priceInt)*100/priceInt, price, gridPrice/price);
    prevPrice = price;
    price = prevPrice - gridPrice;
  }
  return 0;
}
