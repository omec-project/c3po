import time
import random
import os
import zmq
import sys
import signal

context = zmq.Context.instance()
client = context.socket(zmq.ROUTER)
client.bind("tcp://0.0.0.0:6789")
client.setsockopt(zmq.LINGER,0)
client.setsockopt(zmq.ROUTER_MANDATORY,1)

context = zmq.Context.instance()
monitor = context.socket(zmq.PUB)
monitor.bind("ipc://ipc/monitor/in_router")

slave_queue_sockets = {} 
slave_queue_stats = {}

def connect_to_queue(connection_string, type):
        queue_type = zmq.PULL if type == "PULL" else zmq.PUSH
        zmq_context = zmq.Context()
        zmq_socket = zmq_context.socket(queue_type)
        zmq_socket.connect(connection_string)
        return zmq_socket


def create_slave_streamer_queue(queue_name):
	print len(queue_name)
	push_queue_path = "./ipc/slaves/pull/"+queue_name 
	if not os.path.exists(push_queue_path):
		print "Slave queue : ",queue_name," not found...starting new..."
		os.system("nohup python cdr_slave_streamer_device.py "+queue_name+" >/dev/null 2>&1 &")
		time.sleep(0.5)
	return connect_to_queue("ipc://ipc/slaves/pull/"+queue_name,"PUSH")

poller = zmq.Poller()
message_ctr = 0
while True:
		try:
			dp_message = client.recv_multipart()
			identity = dp_message[0]
			if not identity in slave_queue_sockets: 
				print "New identity connection..", identity
				slave_queue_sockets[identity] = create_slave_streamer_queue(identity)	  
				poller.register(slave_queue_sockets[identity],zmq.POLLOUT)
				slave = slave_queue_sockets[identity]
				slave_queue_stats[identity] = [identity, 0]
			else:
				ready_slave_sockets = dict(poller.poll())
				slave = slave_queue_sockets[identity]
				if slave in ready_slave_sockets and ready_slave_sockets[slave] == zmq.POLLOUT:
					slave.send(dp_message[1],zmq.NOBLOCK)
					slave_queue_stats[identity][1] += 1
					monitor.send_json({"in_router" : slave_queue_stats})	
	        except KeyboardInterrupt:
			print "Exiting program..."
			client.close()
		except :
			e = sys.exc_info()[0]	
			print "Error in sending message : ",e
			print "For identity :",identity if 'identity' in vars() else "NULL"
			print "For Slave :", slave if 'slave' in vars() else "NULL"
			os.kill(os.getpid(), signal.SIGKILL)	
			#client.send_multipart([identity,"CLOSE"])
			#del slave_queue_sockets[identity]
			
