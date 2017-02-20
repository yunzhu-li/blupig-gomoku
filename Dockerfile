FROM node:7-alpine

# Install packages for building and debugging
RUN apk --no-cache add nginx alpine-sdk cmake bash

# Copy code & configuration
COPY . /app
COPY nginx.conf /etc/nginx/nginx.conf

# Build program
RUN mkdir /app/build && \
    cd /app/build && \
    cmake .. && \
    make install

# Install node.js dependencies
RUN cd /app/gui/server && npm install

# Remove building toolchain
RUN apk del --no-cache alpine-sdk cmake

# nginx listens on 8000
EXPOSE 8000

# Set command
CMD ["/app/start.sh"]
