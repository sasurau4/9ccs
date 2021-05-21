#!/bin/bash
execute() {
    input="$1"

    ./9ccs "$input" > debug.s
    cc -o debug debug.s test_func.o
    ./debug
    echo "status: $?"
}

execute ./test/debug.c