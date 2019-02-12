Run this script if Policy MemberShip Changes:

./PM_MV_Changes.sh <cassandra_ip>

THis script will get all the Subscriber policy values which are having same domains as Policy Membership values.

For each Subscriber policy value it checks the below case

  If  ( PM.MV PM.Matchtype SP.MV) == TRUE then rulename will be added in computed reule names of Subscriber policy table

 Else it will ensure that rulename from PM table is not present in computed reule names of Subscriber policy table



PM.Matchtype -- operator

PM - Policy MemberShip
MV - MemberShip Value
SP - Subscriber Policyi
