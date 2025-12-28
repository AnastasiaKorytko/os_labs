package org.example;

import org.springframework.stereotype.Service;

import java.util.List;
import java.util.Map;
import java.util.Optional;

@Service
public class TaskService {

    private final TaskRepository repos;

    public TaskService(TaskRepository repository) {
        this.repos = repository;
    }

    public List<Task> getAll() {
        return repos.findAll();
    }

    public Optional<Task> getById(Long id) {
        return repos.findById(id);
    }

    public Task create(Task task) {
        return repos.save(task);
    }

    public Task update(Long id, Task task) {
        task.setId(id);
        return repos.save(task);
    }

    public Task patch(Long id, Map<String, String> updates) {
        Task task = repos.findById(id).orElse(null);
        if (task == null) return null;

        if (updates.containsKey("title"))
            task.setTitle(updates.get("title"));

        if (updates.containsKey("description"))
            task.setDescription(updates.get("description"));

        if (updates.containsKey("status"))
            task.setStatus(updates.get("status"));

        return repos.save(task);
    }

    public boolean delete(Long id) {
        if (!repos.existsById(id)) return false;
        repos.deleteById(id);
        return true;
    }
}
