#include "../headers/order_book.h"
#include "../headers/utils.h"
#include <iomanip>
#include <iostream>

double order_book::mid() const { return (best_bid() + best_ask()) / 2; }

double order_book::best_bid() const {
	if (bids.empty())
		return 0.0;
	return bids.begin()->first;
}

double order_book::best_ask() const {
	if (asks.empty())
		return 0.0;
	return asks.begin()->first;
}

void order_book::print() const {
	std::cout << "------------\n ORDER BOOK\n------------\n\n";
	for (const auto &pair : bids) {
		std::cout << std::fixed << std::setprecision(3) << "Price level: " << pair.first;
		std::cout << "         ";
		std::cout << "Quantity: " << queue_sum(pair.second) << "\n";
	}

	std::cout << "=========== MID : " << mid() << " ===============\n";

	for (const auto &pair : asks) {
		std::cout << std::fixed << std::setprecision(3) << "Price level: " << pair.first;
		std::cout << "        ";
		std::cout << "Quantity: " << queue_sum(pair.second) << "\n";
	}
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

std::string order_book::insert_limit(double price_level, unsigned int quantity, SIDE side) {

	std::string id = generate_random_id();

	Order *order1 = new Order{id, side, quantity, price_level};

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
			unsigned int total_position = queue_sum(price_level);

			if (total_position >= quantity) {
				while (quantity > 0 && !price_level.empty()) {
					if (price_level.front()->quantity > quantity) {
						price_level.front()->quantity -= quantity;
						quantity = 0;
					} else {
						quantity -= price_level.front()->quantity;
						price_level.pop();
					}
				}
				return true;
			} else {
				quantity -= total_position;
				it = asks.erase(it);
			}
		}
	} else if (side == SELL) {
		for (auto it = bids.begin(); it != bids.end() && quantity > 0;) {
			auto &price_level = it->second;
			unsigned int total_position = queue_sum(price_level);

			if (total_position >= quantity) {
				while (quantity > 0 && !price_level.empty()) {
					if (price_level.front()->quantity > quantity) {
						price_level.front()->quantity -= quantity;
						quantity = 0;
					} else {
						quantity -= price_level.front()->quantity;
						price_level.pop();
					}
				}
				return true;
			} else {
				quantity -= total_position;
				it = bids.erase(it);
			}
		}
	}
	return true;
}
