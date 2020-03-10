FROM ubuntu:18.04
COPY server.js .
COPY package.json .
RUN apt-get update &&\
    apt-get -y install ca-certificates npm python build-essential nodejs &&\
    npm config set strict-ssl false &&\
    npm install ws
RUN apt-get install -y  wget dialog net-tools
RUN apt-get install -y nginx
COPY html /var/www/html/
EXPOSE  8080
EXPOSE  80
CMD service nginx start && /usr/bin/node server.js
