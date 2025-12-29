package org.example;

import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.annotation.AfterReturning;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Component;

@Aspect
@Component
public class LoggingAspect {

    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    @Before("execution(* org.example.TaskService.*(..))")
    public void logBefore(JoinPoint joinPoint) {
        logger.info("LOG: Method call: {}", joinPoint.getSignature().getName());
    }

    @AfterReturning(pointcut = "execution(* org.example.TaskService.*(..))", returning = "result")
    public void logAfter(JoinPoint joinPoint, Object result) {
        logger.info("LOG: Method {} executed successfully", joinPoint.getSignature().getName());
    }
}
