#!/bin/bash

if [ $# -eq 98988 ]
then
    kill -3 $1
else
    echo "Usage: $0 <pid>"
    exit 1
fi
