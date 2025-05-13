#include "../headers/order_book.h"
#include <cstdlib>
#include <iostream>

const double ASSET_PRICE = 100;
const int MAX_SPREAD = 100; // IN BP

int main() {

    std::cout << "------------\n ORDER BOOK\n------------\n\n";

	srand(time(0));

	order_book order_book;

	for (int i = 0; i < 10; i++) {
		double price_bid = ASSET_PRICE - ((rand() % MAX_SPREAD + 1) / 100.0);
		double price_ask = ASSET_PRICE + ((rand() % MAX_SPREAD + 1) / 100.0);

		unsigned int size_bid = rand() % 101 + 1;
		unsigned int size_ask = rand() % 101 + 1;

		order_book.insert_limit(price_bid, size_bid, BUY);
		order_book.insert_limit(price_ask, size_ask, SELL);
	}

	order_book.print();
    
    order_book.insert_market(32, BUY);

    std::cout << "--------------------------------------------------------------------------------\n";

	order_book.print();
	return 0;
}
