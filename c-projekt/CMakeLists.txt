cmake_minimum_required(VERSION 3.10)
project("Hoehenmessung")

add_executable(mini-test src/ReadComPort.c src/schreibliste.c)
add_executable(ohne-test src/SImuliert.c src/schreibliste.c)