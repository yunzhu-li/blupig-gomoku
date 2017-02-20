blupig
------

![blupig](gui/client/assets/blupig.jpg "blupig")

master [![Build Status](https://travis-ci.org/yunzhu-li/blupig.svg?branch=master)](https://travis-ci.org/yunzhu-li/blupig) &nbsp; dev [![Build Status](https://travis-ci.org/yunzhu-li/blupig.svg?branch=dev)](https://travis-ci.org/yunzhu-li/blupig)

A Renju (連珠, 五子棋, Gomoku, Five in a Row) AI with a custom `heuristic negamax` algorithm with `α-β pruning` and built-in rules and cut-offs, written in `C++`.

Currently runs single-threaded, supports only `Gomoku` rules, future plans:
- MCTS with parallelization
- Self-learning

A live demo is hosted on: https://apps.yunzhu.li/renju

![Alt text](gui/screenshots/00.png?raw=true "Screenshot")

Run Your Own Copy
-----
This application is available as a docker image.

- Make sure you have access to `docker`.

- Run:
  ```
  docker run -d --rm -p 8000:8000 -e SERVER_URI="http://<server-ip>:8000" yunzhu/blupig
  ```

- Access `http://<server-ip>:8000` in your browser.

- Play!
