Run this script if Subscriber Policy Changes:

./SP_MV_Changes.sh <cassandra_ip>

→ THis script will clear all the computed rulename from the Subscriber policy table
→ Will get all the Policy membership values with same domain.
→ For each Policy Membership value it check the below case


If  ( PM.MV PM.Matchtype SP.MV) == TRUE then rulename will be added in computed reule names of Subscriber policy table

PM.Matchtype -- operator

PM - Policy MemberShip
MV - MemberShip Value
SP - Subscriber Policy

