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
sudo apt install -y make clang nginx php7.0-fpm
```

##### 3. Build the game
```
cd renju-parallel
make
```

##### 4. Test the binary
```
bin/renju test
```

After a few seconds, you should see an output like this:
```
{"message":"ok","result":{"ai_player":"1","cpu_time":"1833","eval_count":"4804587","move_c":"9","move_r":"8","num_threads":"1","pm_count":"144137610","winning_player":"0"}}
```

##### 5. Copy files
Copy game files to default nginx html file location, if you have a different one, change accordingly.
```
mkdir -p /var/www/html/renju/bin/
cp bin/renju /var/www/html/renju/bin/
cp gui/server/renju.php /var/www/html/renju/index.php
cp -r gui/client/* /var/www/html/renju/
```

##### 6. Configure HTML client
Open `/var/www/html/renju/renju.html`, search for this line:
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
systemctl restart nginx
systemctl restart php7.0-fpm
```

##### 9. Test setup
Open `http://<server-ip>/renju/renju.html` in your browser, you should be able to play the game.

