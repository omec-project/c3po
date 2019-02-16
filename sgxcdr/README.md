# Quick Guide for SGX CDR System Setup

````

DPN ---> SGX Dealer-In ----> Router-IN ------> Router-Out -----> Dealer-Out ----> CTF
                                 Router Monitoring

````


Git clone the C3PO repository

A. To setup dealer-in and dealer-out go inside dealer folder

Go inside cloned path cd $(installation path)/c3po/sgxcdr/dealer/ and run the following command (as root / sudo):

````
./install.sh
````

After the above command is executed, all the dependent libraries will be downloaded and built.

Set the desired configuration in the config file in conf/dealer.json.
Update the "kmsserver", "kmsport" accordingly.
dealer->"runmode" as “IN”.
"cdrport" (use by default:6789) and "port" refers to the dealer-in port (use by default: 443)


Set the desired Public verification X.509 certificate and CA X.509 Certificate inside Enclave/ca_bundle.h

B. Build the dealer-in using the command:

````
make SGX_MODE=HW SGX_DEBUG=1
````

C. Run this command to know about the dealer's MRENCALVE and MRSIGNER. This binary is the dealer-in (you can optionally rename the binary or the folder to dealer-in)

````
./dealer -j conf/dealer.json -x
````

Also, update the #define KMS_MRSIGNER as the dealer-in’s MRSIGNER in Enclave/ca_bundle.h

We need to come out of the dealer directory and make another directory called "dealer-out"
and copy all the contents of dealer.

````
#cd ..
#mkdir dealer-out
#cp -R dealer dealer-out
````

D. Go to dealer-out directory. Set the configuration in conf/dealer.json.
Update the "kmsserver", "kmsport" accordingly.
dealer->"runmode" as “OUT”.
"cdrport" (use by default:6790) and "port" refers to the dealer-out port (use by default: 444)

And also create a directory "cdr" in dealer-out directory.

````
# cd dealer-out
# mkdir cdr
````

E. Run this command again to check for the MRENCALVE and MRSIGNER for dealer-out. It should be same as dealer-in

````
# ./dealer -j conf/dealer.json -x
````

F. KMS Set up
Go to clone path cd c3po/sgxcdr/kms

Set the MRENCLAVE and MRSIGNER of dealer-in (or dealer-out) in Enclave/ca_bundle.h file.
Here, the structure array “const EnclaveMeasurements dealerMeasurements[]” should consist of MRENCLAVE and MRSIGNER similar to the following:

````

{
        "9525fc227fde387fxxxxxx...xxxx
        "63ef969cbc34ee46xxxxxx...xxxx
        0,
        0
},
{
        "",
        "",
        0,
        0
},
````

Set the desired configuration in conf/kms.json (MRENCLAVE and MRSIGNER)

G. Install the dependencies by running:

````
./install.sh
````

H. Build the kms using the command

````
# make SGX_MODE=HW SGX_DEBUG=1
````

I. Router setup
Go to clone path cd c3po/sgxcdr/router and install the dependencies by running:

````
./install.sh
````

J. DataPlane node set up to use SGX CDR system

On the DP node, we have to set the IP, PORT, MRENCLAVE and MRSIGNER of dealer-in in the "interface.cfg" file at location ngic-rtc/config/interface.cfg

Place X509 Certificate PEM file issued by CA and X509 Private key PEM file inside conf/ folder of DP.

Build the DP application with SGX_BUILD option enabled in DP install.sh
Go to DP folder (ngic-rtc/dp) and run the following:

````
make clean; make
````

K. Charge Trigger Function (CTF) setup to use SGX CDR system
Go to the clone path: cd c3po/ctf

In the conf/ctf.json file under CTF, here we have to set the "sgxserverip", "sgxserverport", MRENCLAVE and MRSIGNER of dealer-in.

L. Perform the steps as mentioned in README.txt to build the CTF and make sure the generated ctf.cert.pem  and ctf.key.pem certificates are placed in conf/

M. Charge data Function (CDF) setup to use SGX CDR system
Go to the clone path: cd c3po/cdf

N. Perform the steps as mentioned in README.txt to build the CDF and make sure the generated cdf.cert.pem  and cdf.key.pem certificates are placed in conf/

O.How to start the set up.

Follow the below sequence to the executables:

cdf: The command for running cdf is

````
# ./bin/cdf -f conf/cdf.conf
````

ctf : The command for running ctf is

````
# ./bin/ctf -j conf/ctf.json
````

Router Monitoring Script:

````
# python start_and__monitor.py
````

Router In :

````
# python in_queue_router.py
````

Router Out:

````
#	python out_queue_router.py
````

KMS:

````
# ./kms -j conf/kms.json
````

Dealer In:

````
# ./dealer -j conf/dealer.json
````

Dealer Out: Here Go to the dealer-out folder and then execute this command.

````
# ./dealer -j conf/dealer.json
````

DP: Go to ngic-rtc/dp directory.

````
# ./run.sh
````

Run a traffic generator of choice to test the setup

============================
KNOWN ISSUES and LIMITATIONS
============================
a. CDR encryption keys obtained from KMS are not sealed/saved
   by dealers
b. Only one instance of KMS is supported, however multiple KMS
   instances (with a cluster configuration) required for high
   availability
c. CDR records sent to SGX Dealer-in is buffered in SGX Dealer-in if ZMQ
   Router-in is not available or does not accept any more messages.
   Adjust ZMQ_SNDHWM value in SGX Dealer-in to control this behavior of buffering.
d. The signatures play a very important role in establishing secure connection between
   KMS and dealers. Any change in these signatures after connection establishment
   between KMS and dealers will prevent access to previously protected data.
