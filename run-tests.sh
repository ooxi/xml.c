#!/bin/bash
./test-xml-c
./test-xml-cpp
valgrind --tool=memcheck --leak-check=full --track-origins=yes -v ./test-xml-c
valgrind --tool=memcheck --leak-check=full --track-origins=yes -v ./test-xml-cpp
