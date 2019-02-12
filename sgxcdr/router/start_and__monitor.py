import zmq
import time
import threading
import sys
import signal
import os

old_stats = {"in_router" : {}, "out_router" : {}}  
new_stats = {"in_router" : {}, "out_router" : {}}  

def createDir(dir):
    try:
        if not os.path.exists(dir):
            os.makedirs(dir)
    except OSError:
        print ('Error: Creating directory. ' +  dir)

def monitor_router(name):
	global new_stats 
	path = 'ipc://ipc/monitor/'+name
	context = zmq.Context()
	socket = context.socket(zmq.SUB)
	socket.connect(path)
	socket.setsockopt(zmq.SUBSCRIBE, '')
	while True:
		try:
			message = socket.recv_json()
			new_stats[name] = message[name]
		except:
			print "Thread will exit now..."
			socket.close()
			break

createDir('./ipc/monitor')
createDir('./ipc/slaves/push')

t1 = threading.Thread(target=monitor_router,args=("in_router",))
t2 = threading.Thread(target=monitor_router,args=("out_router",))
t1.start()
t2.start()
os.system("clear")
try:
	while True:
		#print in_string
		#print out_string
		#print new_stats
		#print old_stats
		print "**************** Ingress Statistics *******************\n" 
		print "DP IP     |     Total Messages    |   Msgs/Sec"  
		for dp_ip, [queue_name, curr_ctr]  in new_stats['in_router'].iteritems():
			if not dp_ip in old_stats['in_router']:
				old_stats['in_router'][dp_ip] = 0
			print dp_ip,"  |   ",curr_ctr,"   |   ",(curr_ctr-old_stats["in_router"][dp_ip])/5 
			old_stats["in_router"][dp_ip]= curr_ctr
		print "\n\n\n**************** Egress Statistics *******************\n" 
		print "CTF IP          |           DP IP    |   Total Messages   |    Msgs/Sec"  
		for ident, [queue_name, curr_ctr]  in new_stats["out_router"].iteritems():
			if not ident in old_stats["out_router"]:
				old_stats["out_router"][ident] = 0
			print ident,"  |   ",queue_name,"   |   ",curr_ctr,"   |   ",(curr_ctr-old_stats["out_router"][ident])/5 
			old_stats["out_router"][ident]= curr_ctr
			
		time.sleep(5)
		os.system("clear")
except:
		print "Exception: Main thread will exit now"
		os.kill(os.getpid(), signal.SIGKILL)
