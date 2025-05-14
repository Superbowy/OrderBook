#pragma once
#include <string>

enum SIDE { BUY, SELL };

struct Order {
	std::string id;
	SIDE side;
	unsigned int quantity;
    double price;
};

