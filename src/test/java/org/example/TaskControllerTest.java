package org.example;

import com.fasterxml.jackson.databind.ObjectMapper;
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

    @Test @WithMockUser
    void getAll_Returns200() throws Exception {
        mockMvc.perform(get("/tasks")).andExpect(status().isOk());
    }

    @Test @WithMockUser
    void getById_ReturnsTask() throws Exception {
        when(service.getById(1L)).thenReturn(Optional.of(new Task("T", "D", "todo")));
        mockMvc.perform(get("/tasks/1")).andExpect(status().isOk());
    }

    @Test @WithMockUser
    void create_Returns201() throws Exception {
        Task task = new Task("T", "D", "todo");
        when(service.create(any())).thenReturn(task);
        mockMvc.perform(post("/tasks").with(csrf())
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(mapper.writeValueAsString(task)))
                .andExpect(status().isCreated());
    }

    @Test @WithMockUser
    void delete_NotFound_Returns404() throws Exception {
        when(service.delete(1L)).thenReturn(false);
        mockMvc.perform(delete("/tasks/1").with(csrf()))
                .andExpect(status().isNotFound());
    }

    @Test @WithMockUser
    void update_NotFound_Returns404() throws Exception {
        when(service.getById(1L)).thenReturn(Optional.empty());
        mockMvc.perform(put("/tasks/1").with(csrf())
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(mapper.writeValueAsString(new Task())))
                .andExpect(status().isNotFound());
    }

    @Test
    void unauthorized_Returns401() throws Exception {
        mockMvc.perform(get("/tasks")).andExpect(status().isUnauthorized());
    }
}
