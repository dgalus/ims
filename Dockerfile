FROM debian:stable-slim

RUN apt update
RUN apt -y upgrade
RUN apt -y install gcc make libcurl4-gnutls-dev libconfig-dev

COPY . /app
WORKDIR /app
RUN make

CMD ["./bin/ims", "--config", "/data/config.ini"]
