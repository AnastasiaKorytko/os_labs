package org.example;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
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

    private static final Long TEST_ID = 1L;
    private static final Long NOT_FOUND_ID = 2L;
    private static final String TEST_TITLE = "Test Title";
    private static final String TEST_DESC = "Description";
    private static final String STATUS_TODO = "todo";
    private static final String STATUS_DONE = "done";
    private static final String NEW_TITLE = "New Title";

    private Task testTask;

    @BeforeEach
    void setUp() {
        testTask = new Task(TEST_TITLE, TEST_DESC, STATUS_TODO);
        testTask.setId(TEST_ID);
    }

    @Test
    @DisplayName("Create task: save to DB and send to queue")
    void create_ShouldSaveAndSendMessage() {
        when(repository.save(any())).thenReturn(testTask);
        Task result = taskService.create(new Task());
        assertNotNull(result);
        verify(rabbitTemplate).convertAndSend(eq(TaskService.TASK_QUEUE), anyString());
    }

    @Test
    @DisplayName("Get all tasks: return list")
    void getAll_ShouldReturnTasks() {
        when(repository.findAll()).thenReturn(List.of(testTask));
        List<Task> result = taskService.getAll();
        assertEquals(1, result.size());
        assertEquals(TEST_TITLE, result.get(0).getTitle());
    }

    @Test
    @DisplayName("Get by ID: task found")
    void getById_Found_ShouldReturnTask() {
        when(repository.findById(TEST_ID)).thenReturn(Optional.of(testTask));
        Optional<Task> result = taskService.getById(TEST_ID);
        assertTrue(result.isPresent());
        assertEquals(TEST_ID, result.get().getId());
    }

    @Test
    @DisplayName("Get by ID: task not found")
    void getById_NotFound_ShouldReturnEmpty() {
        when(repository.findById(NOT_FOUND_ID)).thenReturn(Optional.empty());
        Optional<Task> result = taskService.getById(NOT_FOUND_ID);
        assertFalse(result.isPresent());
    }

    @Test
    @DisplayName("Full task update: save with given ID")
    void update_ShouldSaveTaskWithGivenId() {
        when(repository.save(any())).thenReturn(testTask);
        Task result = taskService.update(TEST_ID, testTask);
        assertEquals(TEST_ID, result.getId());
        verify(repository).save(testTask);
    }

    @Test
    @DisplayName("Partial update (patch): update provided fields")
    void patch_ShouldUpdateOnlyProvidedFields() {
        when(repository.findById(TEST_ID)).thenReturn(Optional.of(testTask));
        when(repository.save(any())).thenAnswer(i -> i.getArguments()[0]);
        Map<String, String> updates = Map.of(
                "status", STATUS_DONE,
                "title", NEW_TITLE
        );

        Task patched = taskService.patch(TEST_ID, updates);
        assertNotNull(patched);
        assertEquals(STATUS_DONE, patched.getStatus());
        assertEquals(NEW_TITLE, patched.getTitle());
    }

    @Test
    @DisplayName("Partial update (patch): return null if ID not found")
    void patch_NotFound_ShouldReturnNull() {
        when(repository.findById(TEST_ID)).thenReturn(Optional.empty());
        Task result = taskService.patch(TEST_ID, Map.of("status", STATUS_DONE));
        assertNull(result);
    }

    @Test
    @DisplayName("Delete: return true if found and deleted")
    void delete_Found_ShouldReturnTrue() {
        when(repository.existsById(TEST_ID)).thenReturn(true);
        boolean result = taskService.delete(TEST_ID);
        assertTrue(result);
        verify(repository).deleteById(TEST_ID);
    }

    @Test
    @DisplayName("Delete: return false if ID not found")
    void delete_NotFound_ShouldReturnFalse() {
        when(repository.existsById(TEST_ID)).thenReturn(false);
        boolean result = taskService.delete(TEST_ID);
        assertFalse(result);
        verify(repository, never()).deleteById(anyLong());
    }
}