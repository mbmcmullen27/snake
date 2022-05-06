FROM ubuntu:focal as base

WORKDIR /home/snake/
COPY . /home/snake/

RUN apt update && \
    apt upgrade -y && \
    apt install -y make clang python3-dev python3-pip && \
    pip3 install kubernetes

RUN make && \
    touch std-err.log std-out.log

RUN useradd -ms /bin/bash snake && \
    chown -R snake:snake /home/snake

USER snake:snake

CMD ["sh","-c","./snake"]
