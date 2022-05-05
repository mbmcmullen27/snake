# snake
ncurses + kubernetes + snake
## Usage Instructions
1. This game needs a kubeconfig file and deploys kubernetes resources everytime the snake collects food
    - This can be optionally disabled when the game starts
2. Before playing make sure your current kubectl context is a cluster that you are allowed to make changes to
3. Before starting the game load kubernetes manifest yaml files into the bundle/ directory
    - yaml files in this directory will be deployed one by one as you collect food for the snake

## Build Instructions
1. build with: `make`
2. run with: `./snake`
3. requires: `pip3 install kubernetes`
4. test: `make test && ./test`

## Docker instructions
- test make/build in a base ubuntu 20 image:
    ```sh
    docker run -it -v $(pwd):/home/snake ubuntu:focal
    ```
- build from source:
    ```sh
    docker build . -t snake
    ```
- pull the image:
    ```sh
    docker pull mcmull27/snake:latest
    ```
- play the game:
    ```sh
    docker run -it -e TERM -e COLUMNS="$(tput cols)" -e LINES="$(tput lines)" -v $HOME/.kube/config:/home/snake/.kube/config snake
    ```