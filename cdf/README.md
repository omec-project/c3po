---
C3PO: CDF Build and Run Instructions
---
Perform the following procedures in order.
1.  Follow the instructions located in the **_"Build and Installation Instructions for External Modules"_** provided in _{installation_root}/c3po/README.md_. Make sure these steps are complete.
2.  Build the **CDF**.

        $ cd {installation_root}/c3po/cdf
        $ make
3. Update the following files with any configuration changes:
    + {installation_root}/c3po/cdf/conf/cdf.conf
    + {installation_root}/c3po/cdf/conf/cdf.json
4.  If this is the first time you are running the application, create the freeDiameter certificates using the following steps. **make_certs.sh** takes two parameters, supply the diameter host name without realm and then the diameter realm.
**NOTE - the diameter host and realm names must match the names set in step 3**.

        $ cd {installation_root}/c3po/cdf/conf
        $ ../bin/make_certs.sh cdf test3gpp.net
5. To run the application:

        $ cd ${installation_root}/c3po/cdf
        $ bin/cdf -j conf/cdf.json

