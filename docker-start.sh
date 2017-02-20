#!/bin/sh
set -euo pipefail

# Replace server uri and path
sed -i "s|var api_base_url.*|var api_base_url = '$SERVER_URI';|" /app/gui/client/index.html

nginx
cd /app/gui/server
npm start
