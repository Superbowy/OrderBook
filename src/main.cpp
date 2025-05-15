#include "../headers/order_book.h"
#include <chrono>
#include <cstdlib>
#include <thread>

const float ASSET_PRICE = 100;
const int MAX_SPREAD = 100; // IN BP

const int TICK = 100;

int main() {

	srand(time(0));

	order_book order_book;

	for (int i = 0; i < 10; i++) {
		float price_bid = ASSET_PRICE - ((rand() % MAX_SPREAD + 1) / 100.0);
		float price_ask = ASSET_PRICE + ((rand() % MAX_SPREAD + 1) / 100.0);

		unsigned int size_bid = rand() % 101 + 1;
		unsigned int size_ask = rand() % 101 + 1;

		order_book.insert_limit(price_bid, size_bid, BUY);
		order_book.insert_limit(price_ask, size_ask, SELL);
	}

	while (true) {
		system("clear");
		order_book.print();
		std::this_thread::sleep_for(std::chrono::milliseconds(TICK));

		int r = rand() % 100;

		if (r < 40) {
			SIDE side = (rand() % 2 == 0) ? BUY : SELL;
			unsigned int size = rand() % 51 + 1;
			order_book.insert_market(size, side);
		} else if (r < 70) {
			float price_bid = order_book.best_bid() - ((rand() % 3 + 1) / 100.0);
			float price_ask = order_book.best_ask() + ((rand() % 3 + 1) / 100.0);

			unsigned int size_bid = rand() % 51 + 1;
			unsigned int size_ask = rand() % 51 + 1;

			order_book.insert_limit(price_bid, size_bid, BUY);
			order_book.insert_limit(price_ask, size_ask, SELL);
		} else {
			//order_book.random_cancel();
		}
	}

	return 0;
}

