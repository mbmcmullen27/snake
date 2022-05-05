FROM ubuntu:focal as base

RUN apt update && \
    apt upgrade -y && \
    apt install -y make clang python3-dev python3-pip && \
    pip3 install kubernetes

RUN useradd -ms /bin/bash snake
USER snake:snake

COPY . /home/snake/
WORKDIR /home/snake/

RUN make
CMD ["sh","-c","stty size && ./snake"]
