FROM node:8-alpine

# Copy code & configuration
COPY . /app
COPY docker-nginx.conf /etc/nginx/nginx.conf

# Install packages and build program then remove building toolchain
RUN apk --no-cache add nginx alpine-sdk cmake bash && \
    mkdir /app/build && \
    cd /app/build && \
    cmake .. && \
    make install && \
    apk del --no-cache alpine-sdk cmake

# Install node.js dependencies
RUN cd /app/gui/server && npm install

# nginx listens on 8000
EXPOSE 8000

# Set command
CMD ["/app/docker-start.sh"]
