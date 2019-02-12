import time
import zmq
import socket
import os
import sys

def create_slave_streamer_queue(dp_ip):

	try:
		context = zmq.Context(1)

		frontend = context.socket(zmq.PULL) 
		frontend.setsockopt(zmq.RCVBUF,32768)
		frontend.setsockopt(zmq.RCVHWM,999999)
		frontend.bind("ipc://ipc/slaves/pull/"+dp_ip)

        	backend = context.socket(zmq.PUSH)
		backend.setsockopt(zmq.SNDBUF,32768)
		backend.setsockopt(zmq.SNDHWM,999999)
		backend.bind("ipc://ipc/slaves/push/"+dp_ip)

		zmq.device(zmq.STREAMER, frontend, backend) 
	except:
		print "bringing down zmq device" 
	finally:
		pass
        	frontend.close()
        	backend.close()
        	context.term()	


os.system("mkdir -p ./ipc/slaves/pull")
os.system("mkdir -p ./ipc/slaves/push")

print len(sys.argv[1])
create_slave_streamer_queue(sys.argv[1])

