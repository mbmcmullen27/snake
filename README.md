# Snake
ncurses + kubernetes + snake
## Usage Instructions
1. The snake accepts a kubeconfig file and deploys kubernetes resources everytime the snake collects food
    - This can be optionally disabled when the game starts
    - The config can also be omitted entirely 
2. Before playing make sure your current kubectl context is a cluster that you are allowed to make changes to
    - the only scenario where kube-snake will clean up after itself is if you provide a kubeconfig file and omit a bundle directory (see commands below)
    - the python client doesn't have a mechanism yet for `kubectl delete -f` until this [pr](https://github.com/kubernetes-client/python/pull/1392) gets merged
3. Before starting the game load kubernetes yaml files into a bundle/ directory
    - yaml files in this directory will be deployed one by one as you collect food for the snake

## Play the Game! 

### Docker instructions
- pull the image:
```sh
docker pull mcmull27/snake:latest
```
- only snake
```sh
docker run -it \
    -e COLUMNS="$(tput cols)" \
    -e LINES="$(tput lines)" \
    mcmull27/snake

```

- kube mode using local kubeconfig
```sh
docker run -it \
    -e COLUMNS="$(tput cols)" \
    -e LINES="$(tput lines)" \
    -v $HOME/.kube/config:/home/snake/.kube/config \
    mcmull27/snake
```

- using custom bundle (directory of kube manifests to deploy)
```sh
docker run -it \
    -e COLUMNS="$(tput cols)" \
    -e LINES="$(tput lines)" \
    -v $HOME/.kube/config:/home/snake/.kube/config \
    -v $(pwd)/bundle:/home/snake/bundle
    mcmull27/snake
```

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

### Docker commands

- test make/build in a base ubuntu 20 image:
```sh
docker run -it -v $(pwd):/home/snake ubuntu:focal
```
- build from source:
```sh
docker build . -t snake
```