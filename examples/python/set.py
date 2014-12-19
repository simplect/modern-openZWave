import zmq
import time

def status_check(status):
    if 'status' in status and status['status'] == 'okay':
        return True
    else:
        return False

if __name__ == "__main__":
    context = zmq.Context()
    socket = context.socket(zmq.REQ)
    socket.connect("tcp://127.0.0.1:5555")
    print("connected")
    time.sleep(1)
    socket.send_json({'node_id' : 12, 'value': False})
    print("send")
    if not status_check(socket.recv_json()):
        print("failed to send message")
    print("status okay")
    time.sleep(1)
    socket.send_json({'node_id' : 12, 'value' : True})
    if not status_check(socket.recv_json()):
        print("failed to send message")

