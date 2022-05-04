FROM ubuntu:focal

RUN apt update && 
    apt upgrade && 
    apt install -y make clang python3-dev

COPY . /home/snake/
WORKDIR /home/snake/

RUN make

CMD ["./snake"]