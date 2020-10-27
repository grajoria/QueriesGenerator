#!/bin/sh
set -x
set -e

g++ -o queries_generator queries_generator.cpp
g++ -o generate_aws_cmds generate_aws_cmds.cpp

./queries_generator
./generate_aws_cmds

chmod +x aws_cmds.sh

./aws_cmds.sh
