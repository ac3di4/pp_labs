#!/bin/bash
python main.py ../data/static.csv static
python main.py ../data/static_1.csv static 1
python main.py ../data/static_100.csv static 100
python main.py ../data/static_10000.csv static 10000
python main.py ../data/static_100000.csv static 100000

python main.py ../data/dynamic.csv dynamic
python main.py ../data/dynamic_1.csv dynamic 1
python main.py ../data/dynamic_100.csv dynamic 100
python main.py ../data/dynamic_10000.csv dynamic 10000
python main.py ../data/dynamic_100000.csv dynamic 100000

python main.py ../data/guided.csv guided
python main.py ../data/guided_1.csv guided 1
python main.py ../data/guided_100.csv guided 100
python main.py ../data/guided_10000.csv guided 10000
python main.py ../data/guided_100000.csv guided 100000

python main.py ../data/auto.csv auto
python main.py ../data/runtime.csv runtime
