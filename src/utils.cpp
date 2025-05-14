#include "../headers/order.h"
#include <queue>
#include <random>
#include <string>

struct Order;

std::string generate_random_id() {
	const std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::string id;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, chars.size() - 1);

	for (int i = 0; i < 12; ++i) {
		id += chars[dis(gen)];
	}
	return id;
}

unsigned int queue_sum(std::queue<Order*> q) {
	unsigned int result{0};
	while (!q.empty()) {
		result += q.front()->quantity;
		q.pop();
	}
	return result;
}
