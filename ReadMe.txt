git clone https://github.com/chenxiaoqino/udp-image-streaming.git
cd udp-image-streaming/
cmake . && make
./server 10000 &
./client 127.0.0.1 10000