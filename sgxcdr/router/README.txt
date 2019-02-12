Router System Dependency Install and Run guide

ZeroMQ based store and forward asynchronous queue. Router system assigns
input stream from DealerIn to output stream for DealerOut.

About this guide

This document provides instructions on "How to build and run KMS".

===================
Supported Platforms
===================

1. OS: Ubuntu:16.04, Arch: x64

============
Dependencies
============

Following is the list of dependency.

1. Python 2.7
2. PyZMQ - Python bindings for zeromq.

====================
Install Dependencies
====================

* Installer Script
  Execute install.sh script to install dependencies.

  #./install.sh

* Manual Installation
  1. Install Python 2.7

     #sudo apt update
     #sudo apt install python2.7 python-pip

  2. Install PyZMQ

     #pip install pyzmq==17.0.0

==================
Run Router System
==================
1. Start Monitor

   #python start_and__monitor.py

2. Start InQueue Router

   #python in_queue_router.py

3. Start OutQueue Router

   #python out_queue_router.py

