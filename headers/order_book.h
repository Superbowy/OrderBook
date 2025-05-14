#pragma once
#include "order.h"
#include <functional>
#include <map>
#include <queue>
#include <unordered_set>

class order_book {

  private:
	std::map<double, std::queue<Order *>> bids;
	std::map<double, std::queue<Order *>> asks;
	std::unordered_map<std::string, Order *> orders;

  public:
    double mid() const;
	double best_bid() const;
	double best_ask() const;

	std::string insert_limit(double price_level, unsigned int quantity, SIDE side);
	bool insert_market(unsigned int quantity, SIDE side);
	bool cancel_order(std::string id);

    bool random_cancel();
	void print() const;
};
