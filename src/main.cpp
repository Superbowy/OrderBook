#include "../headers/order_book.h"
#include <thread>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>

const double INIT_ASSET_PRICE = 38.9;
const int TICK_MS = 50;
const double DT = 1.0 / 252.0 / (1000.0 / TICK_MS);
const double VOLATILITY = 0.2;

int main() {
    srand(time(0));
    order_book book;

    double asset_price = INIT_ASSET_PRICE;

    for (int i = 0; i < 10; i++) {
        double spread = 0.1 + (rand() % 5) / 10.0;
        double bid = asset_price - spread / 2.0;
        double ask = asset_price + spread / 2.0;

        unsigned int bid_size = rand() % 50 + 10;
        unsigned int ask_size = rand() % 50 + 10;

        book.insert_limit(bid, bid_size, BUY);
        book.insert_limit(ask, ask_size, SELL);
    }

    while (true) {
        system("clear");
        book.print();
        std::this_thread::sleep_for(std::chrono::milliseconds(TICK_MS));

        double dW = sqrt(DT) * ((double)rand() / RAND_MAX * 2.0 - 1.0);
        asset_price *= exp(-0.5 * VOLATILITY * VOLATILITY * DT + VOLATILITY * dW);

        double dynamic_spread = 0.02 + VOLATILITY * asset_price * sqrt(DT);

        int r = rand() % 100;

        if (r < 30) {
            SIDE side = (rand() % 2 == 0) ? BUY : SELL;
            unsigned int size = rand() % 51 + 1;
            book.insert_market(size, side);
        } else if (r < 70) {
            double price_bid = asset_price - dynamic_spread / 2.0;
            double price_ask = asset_price + dynamic_spread / 2.0;

            unsigned int size_bid = rand() % 51 + 1;
            unsigned int size_ask = rand() % 51 + 1;

            book.insert_limit(price_bid, size_bid, BUY);
            book.insert_limit(price_ask, size_ask, SELL);
        } else {
            //book.random_cancel();
        }

        book.match();
    }

    return 0;
}

