#pragma once
#include "iostream"
#include "AMQPcpp.h"

namespace rabbitMQ
{
	class RabbitMQconsumer
	{
	public:
		AMQPQueue *rabbitMQqueue;
		bool consume = true;

		RabbitMQconsumer();
		~RabbitMQconsumer();
		void consumeMessages(std::string url, std::string queue);
		virtual void onMessage(std::string message);
		void stopConsume();
	};
}