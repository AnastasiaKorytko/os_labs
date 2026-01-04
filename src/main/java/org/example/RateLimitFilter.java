package org.example;

import io.github.bucket4j.Bandwidth;
import io.github.bucket4j.Bucket;
import io.github.bucket4j.Refill;
import jakarta.servlet.*;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.stereotype.Component;

import java.io.IOException;
import java.time.Duration;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

@Component
public class RateLimitFilter implements Filter {

    private static final int MAX_REQUESTS_PER_MINUTE = 20;
    private static final int REFILL_TOKENS = 20;
    private static final int REFILL_DURATION_MINUTES = 1;
    private static final int HTTP_STATUS_TOO_MANY_REQUESTS = 429;

    private final Map<String, Bucket> cache = new ConcurrentHashMap<>();

    private Bucket createNewBucket() {
        return Bucket.builder()
                .addLimit(Bandwidth.classic(MAX_REQUESTS_PER_MINUTE,
                        Refill.greedy(REFILL_TOKENS, Duration.ofMinutes(REFILL_DURATION_MINUTES))))
                .build();
    }

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain)
            throws IOException, ServletException {

        HttpServletRequest httpRequest = (HttpServletRequest) request;
        String ip = httpRequest.getRemoteAddr();

        Bucket bucket = cache.computeIfAbsent(ip, k -> createNewBucket());

        if (bucket.tryConsume(1)) {
            chain.doFilter(request, response);
        } else {
            HttpServletResponse httpResponse = (HttpServletResponse) response;
            httpResponse.setStatus(HTTP_STATUS_TOO_MANY_REQUESTS);
            httpResponse.setContentType("text/plain;charset=UTF-8");
            httpResponse.getWriter().write("Too Many Requests: The request limit has been reached.");
        }
    }
}