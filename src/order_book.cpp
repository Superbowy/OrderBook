#include "../headers/order_book.h"
#include "../headers/utils.h"
#include <iomanip>
#include <iostream>

void order_book::print() {
	for (const auto &pair : bids) {
		std::cout << std::fixed << std::setprecision(3) << "Price level: " << pair.first;
		std::cout << "         ";
		std::cout << "Quantity: " << queue_sum(pair.second) << "\n";
	}

	std::cout << "=========================================\n";

	for (const auto &pair : asks) {
		std::cout << std::fixed << std::setprecision(3) << "Price level: " << pair.first;
		std::cout << "        ";
		std::cout << "Quantity: " << queue_sum(pair.second) << "\n";
	}
}

void order_book::insert_limit(double price_level, unsigned int quantity, SIDE side) {

	std::string id;
	do {
		id = generate_random_id();
	} while (ids.contains(id));

	Order order1 = {id, side, quantity};

	if (side == BUY) {
		bids[price_level].push(order1);
	} else {
		asks[price_level].push(order1);
	}

	ids.insert(id);
}

void order_book::insert_market(unsigned int quantity, SIDE side) {
    if (side == BUY) {
        // Process a buy market order, match against the sell side (asks)
        for (auto it = asks.begin(); it != asks.end() && quantity > 0; ) {
            auto &price_level = it->second; // Queue of orders at this price level
            unsigned int total_position = queue_sum(price_level);

            // If total position at this price level is more than quantity to be filled
            if (total_position >= quantity) {
                // Match remaining quantity
                while (quantity > 0 && !price_level.empty()) {
                    if (price_level.front().quantity > quantity) {
                        price_level.front().quantity -= quantity;
                        quantity = 0;
                    } else {
                        quantity -= price_level.front().quantity;
                        price_level.pop();
                    }
                }
                return;
            } else {
                // Fully consume this price level
                quantity -= total_position;
                it = asks.erase(it);  // Remove this price level from the book
            }
        }
    } else if (side == SELL) {
        // Process a sell market order, match against the buy side (bids)
        for (auto it = bids.begin(); it != bids.end() && quantity > 0; ) {
            auto &price_level = it->second; // Queue of orders at this price level
            unsigned int total_position = queue_sum(price_level);

            // If total position at this price level is more than quantity to be filled
            if (total_position >= quantity) {
                // Match remaining quantity
                while (quantity > 0 && !price_level.empty()) {
                    if (price_level.front().quantity > quantity) {
                        price_level.front().quantity -= quantity;
                        quantity = 0;
                    } else {
                        quantity -= price_level.front().quantity;
                        price_level.pop();
                    }
                }
                return;
            } else {
                // Fully consume this price level
                quantity -= total_position;
                it = bids.erase(it);  // Remove this price level from the book
            }
        }
    }
}

