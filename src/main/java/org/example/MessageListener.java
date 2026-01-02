package org.example;

import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.stereotype.Component;

@Component
public class MessageListener {

    @RabbitListener(queues = "taskQueue")
    public void listen(String message) {
        System.out.println(" [RabbitMQ] Message received from the queue: " + message);
    }
}