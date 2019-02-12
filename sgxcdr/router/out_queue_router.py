import time
import random
import os
import zmq
import threading
import sys
import signal

context = zmq.Context.instance()
client = context.socket(zmq.ROUTER)
client.bind("tcp://0.0.0.0:6790")
client.setsockopt(zmq.LINGER,0)
client.setsockopt(zmq.ROUTER_MANDATORY,1)

context = zmq.Context.instance()
monitor = context.socket(zmq.PUB)
monitor.bind("ipc://ipc/monitor/out_router")

slave_queue_sockets = {} 

def connect_to_queue(connection_string, type):
        queue_type = zmq.PULL if type == "PULL" else zmq.PUSH
        zmq_context = zmq.Context()
        zmq_socket = zmq_context.socket(queue_type)
        zmq_socket.connect(connection_string)
        return zmq_socket

poller = zmq.Poller()
poller.register(client, zmq.POLLIN)

slave_queue_ids = {}
slave_queue_stats = {}


def scan_for_new_slave_queue():
	print "Scanning for new live slave queues"
	for dp_id in os.listdir("./ipc/slaves/push/"):
		if not dp_id in slave_queue_ids:
        		print "...Found live slave socket : ",dp_id, ". Connecting...."
			socket = connect_to_queue("ipc://ipc/slaves/push/"+dp_id,"PULL") 
        		slave_queue_sockets[socket] = [0,""] 
			slave_queue_ids[dp_id] = socket 
        		print "Connected"
       			print ""


while True:
	ready_sockets = dict(poller.poll())
	for slave_socket in ready_sockets:
		if slave_socket == client:
		   	new_dealer = client.recv_multipart()[0]
		   	print "Dealer Connected : ", new_dealer
			scan_for_new_slave_queue()
			if new_dealer[0] == '2':
				for socket, [identity, last_message] in slave_queue_sockets.iteritems():
					if identity == 0:
						slave_queue_sockets[socket] = [new_dealer,""]
	    					poller.register(socket, zmq.POLLIN)
						dp_id = [k for k, v in slave_queue_ids.iteritems() if v == socket][0]
						slave_queue_stats[new_dealer] = [dp_id, 0]
						client.send_multipart([new_dealer,dp_id])
						print "Found unserved slave queue ", dp_id," connected to dealer :", new_dealer
						break
			elif new_dealer[0] == '0':
				for socket, [identity, last_message] in slave_queue_sockets.iteritems():
					if identity == 0:
						slave_queue_sockets[socket] = [new_dealer, ""]
						poller.register(socket, zmq.POLLIN)
						result = [k for k,v in slave_queue_ids.iteritems() if k == new_dealer]
						if result:
							dp_id = result[0]
							slave_queue_stats[new_dealer] = [dp_id, 0]
							client.send_multipart([new_dealer, dp_id])
							print("stream matched for new dealer %s" % new_dealer)
						break
		else:
			[identity, last_message]  = slave_queue_sockets[slave_socket] 
			assert(identity != 0)	
			if (last_message == ""):
            			message = slave_socket.recv()
				slave_queue_sockets[slave_socket][1] = message	
			else:
				message = last_message
    			try:
    				client.send_multipart([identity,message],zmq.NOBLOCK)
				slave_queue_stats[identity][1] += 1
				slave_queue_sockets[slave_socket][1] = ""
				monitor.send_json({"out_router" : slave_queue_stats})
			except zmq.ZMQError as e:
				if e.errno == zmq.EAGAIN:
					print "Socket busy..Trying Again..."
					time.sleep(0.1)
				else:
					print "Error in sending message : ",e
					print "Slave socket : ", slave_socket
					print "DEALER DISCONNECTED : ",identity
					slave_queue_sockets[slave_socket] = [0,""]
					del slave_queue_stats[identity]
					poller.unregister(slave_socket)

