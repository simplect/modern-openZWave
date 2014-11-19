import zmq

if __name__ == "__main__":
    context = zmq.Context()
    socket = context.socket(zmq.SUB)
    socket.setsockopt_string(zmq.SUBSCRIBE, '')
    socket.connect("tcp://127.0.0.1:5556")

    while True:
        json_data = socket.recv_json()
        print("{0}".format(json_data))
