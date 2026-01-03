package org.example;

import jakarta.servlet.FilterChain;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.Signature;
import org.junit.jupiter.api.Test;
import java.io.PrintWriter;
import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class InfrastructureAndDomainTest {

    @Test
    void rateLimit_Exceeded_Returns429() throws Exception {
        RateLimitFilter filter = new RateLimitFilter();
        HttpServletRequest req = mock(HttpServletRequest.class);
        HttpServletResponse resp = mock(HttpServletResponse.class);
        FilterChain chain = mock(FilterChain.class);
        when(req.getRemoteAddr()).thenReturn("192.168.0.1");
        when(resp.getWriter()).thenReturn(mock(PrintWriter.class));

        for (int i = 0; i < 20; i++) filter.doFilter(req, resp, chain);
        filter.doFilter(req, resp, chain);

        verify(resp).setStatus(429);
    }

    @Test
    void loggingAspect_ShouldExecuteWithoutError() {
        LoggingAspect aspect = new LoggingAspect();
        JoinPoint jp = mock(JoinPoint.class);
        Signature sig = mock(Signature.class);
        when(jp.getSignature()).thenReturn(sig);
        when(sig.getName()).thenReturn("testMethod");

        assertDoesNotThrow(() -> {
            aspect.logBefore(jp);
            aspect.logAfter(jp, null);
        });
    }

    @Test
    void messageListener_ShouldReceiveMessage() {
        MessageListener listener = new MessageListener();
        assertDoesNotThrow(() -> listener.listen("Test task update"));
    }

    @Test
    void taskEntity_SettersAndGetters() {
        Task task = new Task();
        task.setStatus("done");
        assertEquals("done", task.getStatus());
    }

    @Test
    void rabbitConfig_QueueName() {
        RabbitConfig config = new RabbitConfig();
        assertEquals("taskQueue", config.taskQueue().getName());
    }
}