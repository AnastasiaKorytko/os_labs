package org.example;

import org.springframework.cache.annotation.CacheEvict;
import org.springframework.cache.annotation.Cacheable;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.Map;
import java.util.Optional;

@Service
public class TaskService {

    private final TaskRepository repository;

    public TaskService(TaskRepository repository) {
        this.repository = repository;
    }

    public List<Task> getAll() {
        return repository.findAll();
    }

    @Cacheable(value = "tasks", key = "#id")
    public Optional<Task> getById(Long id) {
        System.out.println(" Database query for ID: " + id);
        return repository.findById(id);
    }

    public Task create(Task task) {
        return repository.save(task);
    }

    @CacheEvict(value = "tasks", key = "#id")
    public Task update(Long id, Task task) {
        task.setId(id);
        return repository.save(task);
    }

    @CacheEvict(value = "tasks", key = "#id")
    public Task patch(Long id, Map<String, String> updates) {
        Task task = repository.findById(id).orElse(null);
        if (task == null)
            return null;
        if (updates.containsKey("title")) task.setTitle(updates.get("title"));
        if (updates.containsKey("description")) task.setDescription(updates.get("description"));
        if (updates.containsKey("status")) task.setStatus(updates.get("status"));

        return repository.save(task);
    }

    @CacheEvict(value = "tasks", key = "#id")
    public boolean delete(Long id) {
        if (!repository.existsById(id)) return false;
        repository.deleteById(id);
        return true;
    }
}