renju-parallel
------

master [![Build Status](https://travis-ci.org/yunzhu-li/renju-parallel.svg?branch=master)](https://travis-ci.org/yunzhu-li/renju-parallel) &nbsp; dev [![Build Status](https://travis-ci.org/yunzhu-li/renju-parallel.svg?branch=dev)](https://travis-ci.org/yunzhu-li/renju-parallel)


A Renju (連珠, 五子棋, Gomoku, Five in a Row) AI with a customized `heuristic negamax` algorithm, written in `C++`.

Currently runs single-threaded, I'm working on the parallelization, pattern analysis and more to increase depth.

A live demo is hosted on: https://apps.yunzhu.li/renju

![Alt text](gui/screenshots/00.png?raw=true "Screenshot")

Build & Run Your Own Copy
-----
It is very easy to run this containerized app.

- Make sure you have `docker` and `docker-compose` installed on your local environment.

- Clone this repo, in the repo root, run:
```
docker-compose up -d
```

- Access `http://localhost:8000` in your browser.

- Play!
