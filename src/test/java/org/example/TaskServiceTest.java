package org.example;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;
import org.springframework.amqp.rabbit.core.RabbitTemplate;

import java.util.*;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.*;
import static org.mockito.Mockito.*;

@ExtendWith(MockitoExtension.class)
class TaskServiceTest {

    @Mock private TaskRepository repository;
    @Mock private RabbitTemplate rabbitTemplate;
    @InjectMocks private TaskService taskService;

    private Task testTask;

    @BeforeEach
    void setUp() {
        testTask = new Task("Test Title", "Desc", "todo");
        testTask.setId(1L);
    }

    @Test
    void create_ShouldSaveAndSendMessage() {
        when(repository.save(any())).thenReturn(testTask);
        Task result = taskService.create(new Task());
        assertNotNull(result);
        verify(rabbitTemplate).convertAndSend(eq("taskQueue"), anyString());
    }

    @Test
    void getAll_ShouldReturnTasks() {
        when(repository.findAll()).thenReturn(List.of(testTask));
        List<Task> result = taskService.getAll();
        assertEquals(1, result.size());
    }

    @Test
    void getById_Found_ShouldReturnTask() {
        when(repository.findById(1L)).thenReturn(Optional.of(testTask));
        assertTrue(taskService.getById(1L).isPresent());
    }

    @Test
    void getById_NotFound_ShouldReturnEmpty() {
        when(repository.findById(2L)).thenReturn(Optional.empty());
        assertFalse(taskService.getById(2L).isPresent());
    }

    @Test
    void update_ShouldSaveTaskWithGivenId() {
        when(repository.save(any())).thenReturn(testTask);
        Task result = taskService.update(1L, testTask);
        assertEquals(1L, result.getId());
        verify(repository).save(testTask);
    }

    @Test
    void patch_ShouldUpdateOnlyProvidedFields() {
        when(repository.findById(1L)).thenReturn(Optional.of(testTask));
        when(repository.save(any())).thenAnswer(i -> i.getArguments()[0]);

        Task patched = taskService.patch(1L, Map.of("status", "done", "title", "New"));

        assertEquals("done", patched.getStatus());
        assertEquals("New", patched.getTitle());
    }

    @Test
    void patch_NotFound_ShouldReturnNull() {
        when(repository.findById(1L)).thenReturn(Optional.empty());
        assertNull(taskService.patch(1L, Map.of("status", "done")));
    }

    @Test
    void delete_Found_ShouldReturnTrue() {
        when(repository.existsById(1L)).thenReturn(true);
        assertTrue(taskService.delete(1L));
        verify(repository).deleteById(1L);
    }

    @Test
    void delete_NotFound_ShouldReturnFalse() {
        when(repository.existsById(1L)).thenReturn(false);
        assertFalse(taskService.delete(1L));
        verify(repository, never()).deleteById(anyLong());
    }
}