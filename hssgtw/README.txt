C3PO: HSSGTW Build and Run Instructions

Perform the following procedures in order.

  1. Follow the instructions located in the "Build and Installation
     Instructions for External Modules" provided in
     {installation_root}/c3po/README.md_. Make sure these steps are complete.

  2.  Build HSSGTW.

        $ cd {isntallation_root}/c3po/hssgtw
        $ make
  
  3. Update the following files with any configuration changes:

       {installation_root}/c3po/hssgtw/conf/hssgtw.json
       
  4. To run the application:

       cd {installation_root}/c3po/hssgtw
       bin/hssgtw -j conf/hssgtw.json

