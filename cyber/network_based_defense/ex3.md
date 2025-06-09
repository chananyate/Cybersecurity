# Reverse Proxy Docker Setup

## Overview
In this exercise, I set up a reverse proxy using nginx within a docker container. The proxy channels all incoming requests to a primary web server and diverts specific requests (like requests to `/special`) to a different web server.

## Project Structure
```
.
├── docker-compose.yml
├── nginx.conf
├── webserver1
│   └── index.html
└── webserver2
    ├── index.html
    └── special
        └── index.html
```

## Steps Taken

### 1. Create Website Content

#### Webserver 1 Content
`webserver1/index.html`:
```html
<html>
  <body>
    <h1>Welcome to Web Server 1</h1>
  </body>
</html>
```

#### Webserver 2 Content
`webserver2/index.html`:
```html
<html>
  <body>
    <h1>Welcome to Web Server 2</h1>
  </body>
</html>
```

`webserver2/special/index.html`:
```html
<html>
  <body>
    <h1>Welcome to Special Page</h1>
  </body>
</html>
```

### 2. Create Docker Compose Configuration
I defined the services in a `docker-compose.yml` file to include:
* A **reverse proxy** using Nginx
* Two web servers using Nginx Alpine
* A shared network for communication

`docker-compose.yml`:
```yaml
version: "3"
services:
  webserver1:
    image: nginx:alpine <!-- nginx alpine is a lightweight, small size and efficient version of the Nginx web server -->
    container_name: webserver1
    volumes:
      - ./webserver1:/usr/share/nginx/html:ro
    networks:
      - webnet

  webserver2:
    image: nginx:alpine
    container_name: webserver2
    volumes:
      - ./webserver2:/usr/share/nginx/html:ro
    networks:
      - webnet

  reverse-proxy:
    image: nginx:alpine
    container_name: reverse-proxy
    volumes:
      - ./nginx.conf:/etc/nginx/nginx.conf:ro
    ports:
      - "80:80"
    networks:
      - webnet
    depends_on:
      - webserver1
      - webserver2

networks:
  webnet:
    driver: bridge
```

### 3. Configure Nginx as a Reverse Proxy
I created a custom `nginx.conf` file to:
* Forward all traffic to `webserver1`
* Redirect requests to `/special` to `webserver2`

`nginx.conf`:
```nginx

events {}

http {
    upstream webserver1 {
        server webserver1:80;
    }

    upstream webserver2 {
        server webserver2:80;
    }

    server {
        listen 80;
        
        location / {
            proxy_pass http://webserver1;
        }
        
        location /special {
            proxy_pass http://webserver2;
        }
    }
}
```

### 4. Build and Run the Containers
I used the following commands to build and run the setup:

```sh
docker-compose up -d
```

### 5. Testing the Setup

Default Web Server Response:
```bash
curl http://localhost
```

Output:
```html
<html>
  <body>
    <h1>Welcome to Web Server 1</h1>
  </body>
</html>
```

Special Route Handling:
```bash
curl http://localhost/special
```

Output:
```html
<html>
  <body>
    <h1>Welcome to Special Page</h1>
  </body>
</html>
```

