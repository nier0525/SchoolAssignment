#ifndef CGOLBAL_H
#define CGOLBAL_H

#include <iostream>
#include <time.h>
using std::cout;
using std::cin;
using std::endl;

#define PRODUCT_ID_SIZE 10
#define PRODUCT_NAME_SIZE 20
#define APPLIANCE_MODEL_NUMBER_SIZE 11
#define FOOD_WEIGHT_MAX 1000
#define WEAR_SIZE_MAX 105

#define PRODUCT_NAME_SIZE		20
#define PRICE_LEMIT				10000
#define PRODUCT_MAX_COUNT		10
#define	PRODUCT_CREATE			1
#define PRODUCT_SEARCH			2
#define PRODUCT_DELETE			3
#define	PROGRAM_EXIT			4

enum PRODUCT_CLASS { FOOD, WEAR, APPLIANCE };
enum FOOD_CLASS { VEGGIE, FRUIT, MEAT, GRAIN };
enum WEAR_CLASS { MAN, WOMAN };
enum APPLIANCE_CLASS { TV, FRIDGE, WASHER };
enum COUNT_CLASS { PRODUCT_COUNT, PRODUCT_FOOD_COUNT, PRODUCT_WEAR_COUNT, PRODUCT_APPLIANCE_COUNT };

#endif
