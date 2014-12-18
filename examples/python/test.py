import zmq
import time
if __name__ == "__main__":
    context = zmq.Context()
    socket = context.socket(zmq.PUB)
    socket.connect("tcp://127.0.0.1:5555")
    time.sleep(1)
    socket.send_json({'node_id' : 8, 'value': 0})
    time.sleep(1)
    socket.send_json({'node_id' : 8, 'value' : 255})
