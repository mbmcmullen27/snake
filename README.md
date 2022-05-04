# snake

## Usage Instructions
1. This game reads your local kubeconfig file and deploys kubernetes resources everytime the snake collects food
    - This can be optionally disabled when the game starts
2. Before playing make sure your current kubectl context is a cluster that you are allowed to make changes to
3. Before starting the game load kubernetes manifest yaml files into the bundle/ directory
    - yaml files in this directory will be deployed one by one as you collect food for the snake

## Compilation Instructions
1. build with: `make`
2. run with: `./snake`
3. requires: `pip3 install kubernetes`
4. test: `make test && ./test`
