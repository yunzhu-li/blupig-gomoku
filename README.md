renju-parallel
------

master [![Build Status](https://travis-ci.org/yunzhu-li/renju-parallel.svg?branch=master)](https://travis-ci.org/yunzhu-li/renju-parallel) &nbsp; dev [![Build Status](https://travis-ci.org/yunzhu-li/renju-parallel.svg?branch=dev)](https://travis-ci.org/yunzhu-li/renju-parallel)


A Renju (連珠, 五子棋, Gomoku, Five in a Row) AI with a customized `heuristic negamax` algorithm, written in `C++`.

Currently runs single-threaded, I'm working on the parallelization, pattern analysis and more to increase depth.

A live version is hosted on: [https://yunzhu.li/apps/renju](https://yunzhu.li/apps/renju)

![Alt text](gui/screenshots/00.png?raw=true "Screenshot")

Build & Run Your Own Copy
-----

This game works in a C/S archticture, an HTML client communicates with the C++ game on the server via a Web server and a simple PHP script.

Some configuration work is required.

The following steps are based on Ubuntu Linux (16.04 LTS).

##### 1. Clone the repo
```
git clone https://github.com/yunzhu-li/renju-parallel.git
```

##### 2. Install required software

```
sudo apt update
sudo apt install -y make cmake clang nginx php7.0-fpm
```

##### 3. Build and install the game
```
mkdir renju-parallel/build
cd renju-parallel/build
cmake .. -DENABLE_TESTING=YES
sudo make install
```

##### 4. Test the binary
```
./renju_test

...

[  PASSED  ] 5 tests.
```

##### 5. Copy files
Copy game files to default nginx html file location, if you have a different one, change accordingly.
```
cd ..
sudo mkdir -p /var/www/html/renju/
sudo chmod -R 777 /var/www/html/renju/
cp gui/server/renju.php /var/www/html/renju/index.php
cp -r gui/client/* /var/www/html/renju/
mv /var/www/html/renju/index.html /var/www/html/renju/renju.html
```

##### 6. Configure HTML client
Open `/var/www/html/renju/index.html`, search for this line:
```
var api_base_url = 'http://127.0.0.1';
```

Replace the `IP address` (and port number if needed) to the server's public IP, or leave as default if running locally.


##### 7. Configure nginx and PHP
Open the nginx default configuration file `/etc/nginx/sites-available/default`

Delete all content, and paste:
```
server {
    listen 80;
    root /var/www/html;
    index index.html index.php;

    location ~ \.php$ {
        include snippets/fastcgi-php.conf;
        fastcgi_pass unix:/run/php/php7.0-fpm.sock;
    }
}
```

##### 8. Start server
```
sudo systemctl restart nginx
sudo systemctl restart php7.0-fpm
```

##### 9. Test setup
Open `http://<server-ip>/renju/renju.html` in your browser, you should be able to play the game.
