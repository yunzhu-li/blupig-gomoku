FROM node:7-alpine

# Install nginx
RUN apk --no-cache add nginx alpine-sdk cmake

# Copy code & configuration
COPY . /app
COPY nginx.conf /etc/nginx/nginx.conf

# Build program
RUN mkdir /app/build && \
    cd /app/build && \
    cmake .. && \
    make install

# Change working directory
WORKDIR /app/gui/server

# Install dependencies
RUN npm install

# Start command
CMD ["sh", "-c", "nginx && npm start"]
