---
# C3PO: PCRF Build and Run Instructions
---
Perform the following procedures in order.
1.  Follow the instructions located in the **_"Build and Installation Instructions for External Modules"_** provided in _{installation_root}/c3po/README.md_. Make sure these steps are complete.
2.  Build the **PCRF**.

        $ cd {installation_root}/c3po/pcrf
        $ make
3. Update the following files with any configuration changes you need:
   + {installation_root}/c3po/pcrf/conf/pcrf.conf
   + {installation_root}/c3po/pcrf/conf/pcrf.json
4.  If this is the first time you are running the application, create the freeDiameter certificates using the following steps. **make_certs.sh** takes two parameters, supply the diameter host name without realm and then the diameter realm.
**NOTE - the diameter host and realm names must match the names set in step 3**.

        $ cd {installation_root}/c3po/pcrf/conf
        $ ../bin/make_certs.sh pcrf test3gpp.net
5. To run the application:
    > cd {installation_root}/c3po/pcrf
    > bin/pcrf -j conf/pcrf.json

## Chronos Installation
1. Under sudo, create `/etc/apt/sources.list.d/clearwater.list` with the following contents:

        deb http://repo.cw-ngv.com/stable binary/
2. Install the signing key used by the Clearwater server with:

        curl -L http://repo.cw-ngv.com/repo_key | sudo apt-key add -
3. Re-index the package manager:

        sudo apt-get update
4. Install the chronos package:

        sudo apt-get install chronos
## Important Pistache Note
The **PCRF** C3PO application uses the _**pistache**_ Client for sending rule timer configurations to _**Chronos**_.  The _**pistache**_ Client contains several bugs that have been resolved, but **NOT** merged into the master branch.  For the **PCRF** to function properly, the pistache code changes in https://github.com/oktal/pistache/pull/154 need to be integrated into the local _**pistache**_ installation. 