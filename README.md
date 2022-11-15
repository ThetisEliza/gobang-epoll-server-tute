<!--
 * @Date: 2022-11-14 19:37:53
 * @LastEditors: ThetisEliza wxf199601@gmail.com
 * @LastEditTime: 2022-11-15 16:40:18
 * @FilePath: /gobang-epoll-server-tute.hub/README.md
-->
# gobang-epoll-server-tute

## Instuctions

tutorial for epoll and built with an interactive gobang competition

## To start the project, what needs settling down

1. g++ 12.2.0
2. cmake 3.24.2
3. sh build.sh
4. ./release/ges

## Here is all the dev history

### init 11.9

1. initialize the project
2. add basic configurations: gitignore readme project-structures

### prepare basic server loop 11.10

1. add inet & socket struct
2. I found the previous tute too complicated, I guess I should rewrite a clear new one.

### prepare basic tutes on individual functions 11.11

1. how to use unix kernel to build a dump server
2. how to use epoll to callback a interrupted operation
3. how to use pipeline to send data between pids

### finish serveral tutes of functions 11.15

1. how to manupulate threads in linux
2. how to create a threadpool to consume concurrent tasks