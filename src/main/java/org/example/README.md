Advanced To-Do List API
This project is a high-performance RESTful API for managing tasks, built with Spring Boot. It implements a complete microservices-ready architecture including database persistence, caching, messaging, and security.

Core Features
RESTful CRUD: Supports GET, POST, PUT, PATCH, and DELETE operations for task resources.

Database: Uses PostgreSQL for persistent storage and Hibernate/JPA for ORM.

Caching: Implements Redis caching to speed up data retrieval and reduce database load.

Messaging: Integrated RabbitMQ broker to handle asynchronous task creation events.

Rate Limiting: Built-in traffic shaping using a Token Bucket filter (20 requests per minute per IP).

Security: Protected with Spring Security Basic Authentication and CSRF protection.

Logging: Aspect-Oriented Programming (AOP) for automatic method execution logging.

Monitoring: Spring Boot Actuator enabled for health checks and metrics.

Project Structure
TaskController: Handles HTTP requests and returns JSON responses.

TaskService: Contains business logic, manages cache, and sends messages to RabbitMQ.

TaskRepository: Interface for PostgreSQL database interactions.

LoggingAspect: Intercepts service calls for automated logging.

RateLimitFilter: Protects the API from traffic spikes.

MessageListener: Asynchronously processes messages from the queue.

Authentication
Access to the API requires Basic Auth. Username: admin Password: banan52

API Endpoints
GET /tasks: Retrieve all tasks.

GET /tasks/{id}: Retrieve a specific task (cached).

POST /tasks: Create a new task and trigger a RabbitMQ event.

PUT /tasks/{id}: Perform a full update of a task.

PATCH /tasks/{id}: Update specific fields like title or status.

DELETE /tasks/{id}: Remove a task and clear associated cache.

Deployment and Testing
The application is fully containerized. Using the provided Dockerfile and docker-compose.yml, the entire stack (App, PostgreSQL, Redis, RabbitMQ) can be launched with a single command. The project also includes a suite of unit and integration tests covering service logic, web controllers, and infrastructure components.