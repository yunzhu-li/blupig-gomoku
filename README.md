blupig-gomoku
------

![blupig](gui/client/assets/blupig.jpg "blupig")

[![Build Status](https://travis-ci.org/yunzhu-li/blupig-gomoku.svg?branch=master)](https://travis-ci.org/yunzhu-li/blupig-gomoku)

A Gomoku (五子棋, Five in a Row) AI with a custom `heuristic negamax` algorithm with `alpha-beta pruning` and built-in rules and cut-offs, written in `C++`.

It provides:
  - A REST API (used by the [HTML client](gui))
  - A CLI interface
  - The stdin / stdout based [protocol](http://petr.lastovicka.sweb.cz/protocl2en.htm) used in Gomocup

Currently runs single-threaded, supports only `Gomoku` rules, future plans:
- MCTS with parallelization
- Self-learning

A live demo is hosted on:

[![Run on Ainize](https://ainize.ai/static/images/run_on_ainize_button.svg)](https://gomoku-yhm7035.endpoint.ainize.ai/)

![Alt text](gui/screenshots/00.png?raw=true "Screenshot")

Run Your Own Copy
-----
This application is available as a docker image `yunzhu/blupig-gomoku`.

- Make sure you have access to `docker`.

- Run:
  ```
  docker run -d --rm -p 8000:8000 -e SERVER_URI="http://<server-ip>:8000" yunzhu/gomoku
  ```
  Access `http://<server-ip>:8000` in your browser.

  or
  ```
  docker build -t gomoku .
  docker run -d --rm -p 8000:8000" gomoku
  ```
  Access `http://127.0.0.1:8000` in your browser.

- Play!
