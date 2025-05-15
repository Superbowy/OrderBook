#include "../headers/order_book.h"
#include "../headers/utils.h"
#include <iomanip>
#include <iostream>

float order_book::mid() const { return (best_bid() + best_ask()) / 2; }

float order_book::best_bid() const {
	if (bids.empty())
		return 0.0;
	return std::prev(bids.end())->first;
}

float order_book::best_ask() const {
	if (asks.empty())
		return 0.0;
	return asks.begin()->first;
}

float order_book::spread() const{
    float ask = best_ask();
    return (ask - best_bid())/ask * 10000;
}

bool order_book::match(){
    while (!bids.empty() && !asks.empty()) {
        auto best_bid_price = std::prev(bids.end());
        auto best_ask_price = asks.begin();

        if (best_bid_price->first < best_ask_price->first)
            break;

        auto &bid_queue = best_bid_price->second;
        auto &ask_queue = best_ask_price->second;

        Order* buy_order = bid_queue.front();
        Order* sell_order = ask_queue.front();

        unsigned int traded_quantity = std::min(buy_order->quantity, sell_order->quantity);

        buy_order->quantity -= traded_quantity;
        sell_order->quantity -= traded_quantity;

        if (buy_order->quantity == 0) {
            delete buy_order;
            bid_queue.pop();
        }

        if (sell_order->quantity == 0) {
            delete sell_order;
            ask_queue.pop();
        }

        if (bid_queue.empty())
            bids.erase(best_bid_price);

        if (ask_queue.empty())
            asks.erase(best_ask_price);
    }
    return true;
}


bool order_book::cancel_order(std::string id) {
	double price_level = orders[id]->price;
	SIDE side = orders[id]->side;
	if (side == SELL && asks[price_level].size() == 1) {
		asks.erase(price_level);
	}
	if (side == BUY && bids[price_level].size() == 1) {
		bids.erase(price_level);
	}
	delete orders[id];
	return true;
}

std::string order_book::insert_limit(float price_level, unsigned int quantity, SIDE side) {

	std::string id = generate_random_id();

	Order *order1 = new Order{id, side, quantity, price_level};

    price_level = std::round(price_level * 1000.0) / 1000.0;

	if (side == BUY) {
		bids[price_level].push(order1);
	} else {
		asks[price_level].push(order1);
	}

	orders[id] = order1;
	return id;
}

bool order_book::insert_market(unsigned int quantity, SIDE side) {
	if (side == BUY) {
		for (auto it = asks.begin(); it != asks.end() && quantity > 0;) {
			auto &price_level = it->second;
			unsigned int available = queue_sum(price_level);

			if (available >= quantity) {
				while (quantity > 0 && !price_level.empty()) {
					auto &front_order = price_level.front();
					if (front_order->quantity > quantity) {
						front_order->quantity -= quantity;
						quantity = 0;
					} else {
						quantity -= front_order->quantity;
						price_level.pop();
					}
				}
				if (price_level.empty())
					it = asks.erase(it);
				return true;
			} else {
				quantity -= available;
				it = asks.erase(it);
			}
		}
	} else if (side == SELL) {
		for (auto it = bids.rbegin(); it != bids.rend() && quantity > 0;) {
			auto &price_level = it->second;
			unsigned int available = queue_sum(price_level);

			if (available >= quantity) {
				while (quantity > 0 && !price_level.empty()) {
					auto &front_order = price_level.front();
					if (front_order->quantity > quantity) {
						front_order->quantity -= quantity;
						quantity = 0;
					} else {
						quantity -= front_order->quantity;
						price_level.pop();
					}
				}
				if (price_level.empty()) {
					// Convert reverse_iterator to base iterator, then erase
					bids.erase(std::next(it).base());
				}
				return true;
			} else {
				quantity -= available;
				it = std::make_reverse_iterator(bids.erase(std::next(it).base()));
			}
		}
	}
	return true;
}

void order_book::print() const {
	std::cout << "------------\n ORDER BOOK\n------------\n\n";
	for (const auto &pair : bids) {
		std::cout << std::fixed << std::setprecision(3) << "Price level: " << pair.first;
		std::cout << "         ";
		std::cout << "Quantity: " << queue_sum(pair.second) << "\n";
	}

    
    std::cout << "\n========== " << mid() << " // " << spread() << "bps =========\n\n";

	for (const auto &pair : asks) {
		std::cout << std::fixed << std::setprecision(3) << "Price level: " << pair.first;
		std::cout << "        ";
		std::cout << "Quantity: " << queue_sum(pair.second) << "\n";
	}
}


