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
- for testing make/build:
    ```sh
    docker run -it -v $(pwd):/home/snake ubuntu:focal
    ```
- for playing game:
    ```sh
    docker run -it -v $HOME/.kube/config:/home/snake/.kube/config snake 
    ```