cmake_minimum_required(VERSION 3.10)
project(LaborProgramm VERSION 1.0)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(src)
find_package(SQLite3 REQUIRED)

add_executable(LaborProgramm 
    src/main.c 
    src/SImuliert.c 
    src/ReadComPort.c 
    src/schreibliste.c
    src/datenbank.c
    src/lookup.c
    src/fun.c
)

target_link_libraries(LaborProgramm PRIVATE m SQLite::SQLite3)