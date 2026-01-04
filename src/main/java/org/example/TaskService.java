package org.example;

import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.cache.annotation.CacheEvict;
import org.springframework.cache.annotation.Cacheable;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.Map;
import java.util.Optional;

@Service
public class TaskService {

    public static final String TASK_QUEUE = "taskQueue";
    public static final String CACHE_NAME = "tasks";

    private final TaskRepository repository;
    private final RabbitTemplate rabbitTemplate;

    public TaskService(TaskRepository repository, RabbitTemplate rabbitTemplate) {
        this.repository = repository;
        this.rabbitTemplate = rabbitTemplate;
    }

    public List<Task> getAll() {
        return repository.findAll();
    }

    @Cacheable(value = CACHE_NAME, key = "#id")
    public Optional<Task> getById(Long id) {
        System.out.println(" [Log] Database query for ID: " + id);
        return repository.findById(id);
    }

    public Task create(Task task) {
        Task savedTask = repository.save(task);
        String msg = "The task has been created: " + savedTask.getTitle() + " (ID: " + savedTask.getId() + ")";

        rabbitTemplate.convertAndSend(TASK_QUEUE, msg);

        return savedTask;
    }

    @CacheEvict(value = CACHE_NAME, key = "#id")
    public Task update(Long id, Task task) {
        task.setId(id);
        return repository.save(task);
    }

    @CacheEvict(value = CACHE_NAME, key = "#id")
    public Task patch(Long id, Map<String, String> updates) {
        Task task = repository.findById(id).orElse(null);
        if (task == null) return null;

        if (updates.containsKey("title")) task.setTitle(updates.get("title"));
        if (updates.containsKey("description")) task.setDescription(updates.get("description"));
        if (updates.containsKey("status")) task.setStatus(updates.get("status"));

        return repository.save(task);
    }

    @CacheEvict(value = CACHE_NAME, key = "#id")
    public boolean delete(Long id) {
        if (repository.existsById(id)) {
            repository.deleteById(id);
            return true;
        }
        return false;
    }
}