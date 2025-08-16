# Multi-stage build for Vue.js frontend
FROM node:20-alpine AS build

# Set working directory
WORKDIR /app

# Install dependencies first (better Docker layer caching)
COPY package*.json ./
RUN npm ci

# Copy source code and build Vue.js app
COPY . .
RUN npm run build

# Production image with Nginx
FROM nginx:alpine AS runtime

# Copy built Vue.js app to Nginx html directory
COPY --from=build /app/dist /usr/share/nginx/html

# Copy custom Nginx configuration (includes API proxy)
COPY nginx.conf /etc/nginx/conf.d/default.conf

# Expose port 80 for web traffic
EXPOSE 80
