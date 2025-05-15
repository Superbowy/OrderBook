#pragma once
#include "order.h"
#include <functional>
#include <map>
#include <queue>
#include <unordered_set>

class order_book {

  private:
	std::map<float, std::queue<Order *>> bids;
	std::map<float, std::queue<Order *>> asks;
	std::unordered_map<std::string, Order *> orders;

  public:
    float mid() const;
	float best_bid() const;
	float best_ask() const;
    float spread() const;

	std::string insert_limit(float price_level, unsigned int quantity, SIDE side);
	bool insert_market(unsigned int quantity, SIDE side);
	bool cancel_order(std::string id);
    bool match();

    bool random_cancel();
	void print() const;
};
