import zmq
import time
if __name__ == "__main__":
    context = zmq.Context()
    socket = context.socket(zmq.PUB)
    socket.connect("tcp://127.0.0.1:5555")
    flip = False
while True:
    time.sleep(3)
    socket.send_json({'symbol':'zwave', 'homeid':23194408, 'nodeid' : 8, 'value': flip})
    if flip:
        flip = False
    else:
        flip = True
