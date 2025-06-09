# Load Balancer Setup with Docker Compose

This document explains the process of setting up a simple load balancing environment using Docker Compose, consisting of two web server containers and one load balancer (NGINX). The goal is to balance incoming HTTP traffic between two web servers running on different containers.

## High-Level Purpose

In this setup, we have three main components:

- **Server 1** (`webserver1`): A container running a web server that serves a simple HTML page.
- **Server 2** (`webserver2`): Another container running a web server similar to Server 1 but can serve a different HTML page or content.
- **Load Balancer** (`loadbalancer`): An NGINX container configured to load balance the incoming traffic between the two servers. This ensures that HTTP requests are distributed evenly between the two servers, improving availability and scalability.

### Architecture Overview

- The **load balancer** (NGINX) acts as a reverse proxy, forwarding requests to either **server 1** or **server 2** based on the load balancing configuration.
- Both **server 1** and **server 2** are accessible via ports `8081` and `8082` respectively, but users will only interact with the **load balancer** via port `80`.

## Components and Configuration

### 1. `docker-compose.yml`

The `docker-compose.yml` file defines the services (containers), networks, and volumes needed for our setup. Here's the configuration:

```yaml
version: "3"
services:
  server1:
    image: webserver1
    container_name: server1
    ports:
      - "8081:80"
    networks:
      - mynetwork

  server2:
    image: webserver2
    container_name: server2
    ports:
      - "8082:80"
    networks:
      - mynetwork

  loadbalancer:
    image: nginx
    container_name: loadbalancer
    ports:
      - "80:80"
    volumes:
      - ./loadbalancer/nginx.conf:/etc/nginx/nginx.conf:ro
    networks:
      - mynetwork

networks:
  mynetwork:
    driver: bridge
```

### 2. Explanation of docker-compose.yml

- **Server 1** (`server1`):
  - Uses the `webserver1` image, which could be any web server image (like NGINX or Apache) with a simple HTML page (an image is essentially a blueprint or template for creating Docker containers)
  - Container listens on port `8081` and is attached to the `mynetwork` network

- **Server 2** (`server2`):
  - Similar to Server 1, uses the `webserver2` image
  - Container listens on port `8082` and is attached to the `mynetwork` network

- **Load Balancer** (`loadbalancer`):
  - Uses the official NGINX image
  - Listens on port `80` and acts as a reverse proxy
  - Loads the `nginx.conf` file from the local system for routing configuration

### 3. NGINX Configuration (`nginx.conf`)

Here's the configuration that determines how NGINX balances traffic:

```nginx
http {
    upstream backend {
        # Load balancing between server1 and server2
        server server1:80;
        server server2:80;
    }

    server {
        listen 80;
        
        location / {
            proxy_pass http://backend;
            proxy_set_header X-Real-IP $remote_addr;
            proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        }
    }
}
```

### 4. Explanation of nginx.conf

- `upstream backend`: Defines the backend servers (server1 and server2)
- `server block`: Listens on port 80 and proxies requests to the backend
- `proxy_set_header`: Sets headers for preserving client information

### 5. Networking

- **Network** (`mynetwork`):
  - All three services connected to `mynetwork`
  - Uses bridge driver (Docker default)
  - Enables container communication via container names
- **Ports**:
  - Server 1: Port 8081
  - Server 2: Port 8082
  - Load Balancer: Port 80

## Getting Started

1. I downloaded Docker 
    ```bash
        https://github.com/docker/compose/releases/download/v2.17.3/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose
    ```
    ```bash
    sudo chmod +x /usr/local/bin/docer-compose
    sudo ln -s /usr/local/bin/docker-compose /usr/bin/docker-compose
    ```
2. I built and start the containers:
   ```bash
   sudo docker-compose up -d
   ```

3. I verified the containers are running:
   ```bash
   sudo docker ps
   ```

## Testing Load Balancing

I tested the load balancing using HTTP requests with different `X-Forwarded-For` headers:

```bash
# Should return Server 1
curl -H "X-Forwarded-For: 1.2.3.4" http://localhost

# Should return Server 2
curl -H "X-Forwarded-For: 5.6.7.8" http://localhost
```

Output:
```html
<html><body><h1>Server 1</h1></body></html>
```
```html
<html><body><h1>Server 2</h1></body></html>
```

This simulates requests from two different clients by using the `X-Forwarded-For` header. This allows us to test how the load balancer routes the traffic based on the source IP.

By setting the `X-Forwarded-For` header to `1.2.3.4`, we simulate a request coming from a client with this IP address. The load balancer will route the request to **Server 1** based on its configuration (like round-robin or IP hashing).

