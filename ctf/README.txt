C3PO: CTF Build and Run Instructions

Perform the following procedures in order.

  1. Follow the instructions located in the "Build and Installation
     Instructions for External Modules" provided in
     {installation_root}/c3po/README.txt. Make sure these steps are complete.

  2. Build CTF.

       $ cd {isntallation_root}/c3po/ctf
       $ make

  3. Update the following files with any configuration changes:

       {installation_root}/c3po/ctf/conf/ctf.conf
       {installation_root}/c3po/ctf/conf/ctf.json

  4. If this is the first time you are running the application, create the
     freeDiameter certificates using the following steps. make_certs.sh takes
     two parameters, supply the diameter host name without realm and then the
     diameter realm.

       NOTE - the diameter host and realm names must match the names set in step 3

       $ cd {installation_root}/c3po/ctf/conf
       $ ../bin/make_certs.sh ctf test3gpp.net

  5. To run the application:

      $ cd ${installation_root}/c3po/ctf
      $ bin/ctf -j conf/ctf.json

Configuration Instructions
--------------------------
1. Mount a network share that corresponds to the network share where the Data
   Plane (DP) writes the CSV usage files. For example, the development share
   is mounted on /dpcsv. Update the datapath, archivepath, and trackpath
   accordingly.
