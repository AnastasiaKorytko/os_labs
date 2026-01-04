package org.example;

import com.fasterxml.jackson.databind.ObjectMapper;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.WebMvcTest;
import org.springframework.boot.test.mock.mockito.MockBean;
import org.springframework.http.MediaType;
import org.springframework.security.test.context.support.WithMockUser;
import org.springframework.test.web.servlet.MockMvc;

import java.util.Optional;

import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.when;
import static org.springframework.security.test.web.servlet.request.SecurityMockMvcRequestPostProcessors.csrf;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.*;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.*;

@WebMvcTest(TaskController.class)
class TaskControllerTest {

    @Autowired private MockMvc mockMvc;
    @MockBean private TaskService service;
    @Autowired private ObjectMapper mapper;

    private static final String TASKS_PATH = "/tasks";
    private static final String TASK_BY_ID_PATH = "/tasks/{id}";

    private static final Long TEST_ID = 1L;
    private static final String TEST_TITLE = "T";
    private static final String TEST_DESC = "D";
    private static final String TEST_STATUS = "todo";

    @Test
    @WithMockUser
    @DisplayName("GET /tasks: return 200 OK")
    void getAll_Returns200() throws Exception {
        mockMvc.perform(get(TASKS_PATH))
                .andExpect(status().isOk());
    }

    @Test
    @WithMockUser
    @DisplayName("GET /tasks/{id}: return task when found")
    void getById_ReturnsTask() throws Exception {
        Task task = new Task(TEST_TITLE, TEST_DESC, TEST_STATUS);
        when(service.getById(TEST_ID)).thenReturn(Optional.of(task));

        mockMvc.perform(get(TASK_BY_ID_PATH, TEST_ID))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.title").value(TEST_TITLE));
    }

    @Test
    @WithMockUser
    @DisplayName("POST /tasks: return 201 Created")
    void create_Returns201() throws Exception {
        Task task = new Task(TEST_TITLE, TEST_DESC, TEST_STATUS);
        when(service.create(any(Task.class))).thenReturn(task);

        mockMvc.perform(post(TASKS_PATH).with(csrf())
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(mapper.writeValueAsString(task)))
                .andExpect(status().isCreated())
                .andExpect(jsonPath("$.title").value(TEST_TITLE));
    }

    @Test
    @WithMockUser
    @DisplayName("DELETE /tasks/{id}: return 404 Not Found")
    void delete_NotFound_Returns404() throws Exception {
        when(service.delete(TEST_ID)).thenReturn(false);

        mockMvc.perform(delete(TASK_BY_ID_PATH, TEST_ID).with(csrf()))
                .andExpect(status().isNotFound());
    }

    @Test
    @WithMockUser
    @DisplayName("PUT /tasks/{id}: return 404 if task does not exist")
    void update_NotFound_Returns404() throws Exception {
        when(service.getById(TEST_ID)).thenReturn(Optional.empty());

        mockMvc.perform(put(TASK_BY_ID_PATH, TEST_ID).with(csrf())
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(mapper.writeValueAsString(new Task())))
                .andExpect(status().isNotFound());
    }

    @Test
    @DisplayName("Security: return 401 Unauthorized for anonymous user")
    void unauthorized_Returns401() throws Exception {
        mockMvc.perform(get(TASKS_PATH))
                .andExpect(status().isUnauthorized());
    }
}