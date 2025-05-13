#pragma once
#include <functional>
#include <map>
#include <queue>
#include <unordered_set>
#include "order.h"

class order_book {

  private:
	std::map<double, std::queue<Order>> bids;
	std::map<double, std::queue<Order>> asks;
	std::unordered_set<std::string> ids;

  public:
	void print();
	void insert_limit(double price_level, unsigned int quantity, SIDE side);
	void insert_market(unsigned int quantity, SIDE side);
};
