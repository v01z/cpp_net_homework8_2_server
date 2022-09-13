Homework N 8, part 2 (server)

Work based on libhttpserver C++ library: https://github.com/etr/libhttpserver

SSL certifcate and key had been done by issuing next command:
openssl req -x509 -newkey rsa:2048 -nodes -sha256 -keyout key.pem -out cert.pem -days 365 \
-subj "/C=RU/ST=Moscow region/L=Moscow/O=GeekBrains/OU=C++/CN=localhost"
