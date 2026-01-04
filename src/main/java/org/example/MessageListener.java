package org.example;

import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.stereotype.Component;

@Component
public class MessageListener {

    @RabbitListener(queues = TaskService.TASK_QUEUE)
    public void listen(String message) {
        System.out.println(" [RabbitMQ] Message received: " + message);
    }
}