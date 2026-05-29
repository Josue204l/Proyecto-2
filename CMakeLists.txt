cmake_minimum_required(VERSION 4.1)
project(Proyecto_2)

set(CMAKE_CXX_STANDARD 20)

add_executable(Proyecto_2 main.cpp
        World.cpp
        World.h
        space.cpp
        space.h
        Objective.cpp
        Objective.h
        Logger.cpp
        Logger.h
        Character.cpp
        Character.h
        Element.h
        Item.cpp
        Item.h
        Obstacle.cpp
        Obstacle.h
        Event.cpp
        Event.h
        FileLoader.cpp
        FileLoader.h)
