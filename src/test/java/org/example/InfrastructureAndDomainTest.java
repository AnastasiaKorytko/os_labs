package org.example;

import jakarta.servlet.FilterChain;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.Signature;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import java.io.PrintWriter;
import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class InfrastructureAndDomainTest {

    private static final String TEST_IP = "192.168.0.1";
    private static final String TEST_METHOD_NAME = "testMethod";
    private static final String TEST_MESSAGE = "Test task update";
    private static final String STATUS_DONE = "done";

    private static final int HTTP_STATUS_TOO_MANY_REQUESTS = 429;

    @Test
    @DisplayName("RateLimitFilter: return 429 when limit is exceeded")
    void rateLimit_Exceeded_Returns429() throws Exception {
        RateLimitFilter filter = new RateLimitFilter();
        HttpServletRequest req = mock(HttpServletRequest.class);
        HttpServletResponse resp = mock(HttpServletResponse.class);
        FilterChain chain = mock(FilterChain.class);

        when(req.getRemoteAddr()).thenReturn(TEST_IP);
        when(resp.getWriter()).thenReturn(mock(PrintWriter.class));

        for (int i = 0; i < 20; i++) {
            filter.doFilter(req, resp, chain);
        }
        filter.doFilter(req, resp, chain);

        verify(resp).setStatus(HTTP_STATUS_TOO_MANY_REQUESTS);
    }

    @Test
    @DisplayName("LoggingAspect: should execute without errors")
    void loggingAspect_ShouldExecuteWithoutError() {
        LoggingAspect aspect = new LoggingAspect();
        JoinPoint jp = mock(JoinPoint.class);
        Signature sig = mock(Signature.class);

        when(jp.getSignature()).thenReturn(sig);
        when(sig.getName()).thenReturn(TEST_METHOD_NAME);

        assertDoesNotThrow(() -> {
            aspect.logBefore(jp);
            aspect.logAfter(jp, null);
        });
    }

    @Test
    @DisplayName("MessageListener: successfully receive string message")
    void messageListener_ShouldReceiveMessage() {
        MessageListener listener = new MessageListener();
        assertDoesNotThrow(() -> listener.listen(TEST_MESSAGE));
    }

    @Test
    @DisplayName("Task Entity: verify getters and setters")
    void taskEntity_SettersAndGetters() {
        Task task = new Task();
        task.setStatus(STATUS_DONE);
        assertEquals(STATUS_DONE, task.getStatus());
    }

    @Test
    @DisplayName("RabbitConfig: verify queue name via constant")
    void rabbitConfig_QueueName() {
        RabbitConfig config = new RabbitConfig();
        assertEquals(TaskService.TASK_QUEUE, config.taskQueue().getName());
    }
}