# snake
ncurses + kubernetes + snake
## Usage Instructions
1. This game needs a kubeconfig file and deploys kubernetes resources everytime the snake collects food
    - This can be optionally disabled when the game starts
2. Before playing make sure your current kubectl context is a cluster that you are allowed to make changes to
3. Before starting the game load kubernetes yaml files into a bundle/ directory
    - yaml files in this directory will be deployed one by one as you collect food for the snake

## Build Instructions
1. requires:
    - make
    - clang
    - python3-dev
    - python3-pip

1. build with: `make`
2. run with: `./snake`
3. kubernetes mode requires: `pip3 install kubernetes`
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
## play the game 
*NOTE:* any valid kubeconfig can be substituted (or omitted) here

- using local kubeconfig
```sh
docker run -it \
    -e TERM \
    -e COLUMNS="$(tput cols)" \
    -e LINES="$(tput lines)" \
    -v $HOME/.kube/config:/home/snake/.kube/config \
    mcmull27/snake
```

- using custom bundle (directory of kube manifests to deploy)
```sh
docker run -it \
    -e TERM \
    -e COLUMNS="$(tput cols)" \
    -e LINES="$(tput lines)" \
    -v $HOME/.kube/config:/home/snake/.kube/config \
    -v $(pwd)/bundle:/home/snake/bundle
    mcmull27/snake
```