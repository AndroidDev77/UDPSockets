#include <arpa/inet.h>
#include <sys/types.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
class Order
{
public:
	struct Item {
		uint64_t time;
		uint32_t size;
		char assetName[32];
		float price;
		uint32_t quantity;
		uint16_t orderType;
	};
	Item item;
	Order();
	Order(char* data);

};
#pragma pack(pop)

Order::Order(char* data)
{

	//item = (Item*)data;
	memcpy(&item, data, sizeof(Item));
	item.time = be64toh(item.time);
	item.size = ntohl(item.size);
	//item.assetName = ntohll(item.assetName);
	item.price = ntohl(item.price);
	item.quantity = ntohl(item.quantity);
	item.orderType = ntohs(item.orderType);
}

Order::Order()
{}

std::ostream& operator<<(std::ostream &strm, const Order &order) {
  return strm << "Order(" << order.item.time << "," << order.item.size << ","
	      << order.item.assetName << "," << order.item.price << ","
	      << order.item.price << "," << order.item.quantity << ","
	      << order.item.orderType << ")\n";
}


