#!/usr/bin/python
import os, sys
from ConfigParser import ConfigParser
from libwfut import WFUTCore

## Read global/local config files
def loadConfig():
	## Construct filename for user config file
	filename = os.path.join(os.environ['HOME'], '.libwfutrc');

	config = ConfigParser();

	## TODO: Move into a setDefaults() method?

	## Set some default values
	config.add_section("general");
	config.set("general", "server_root", "http://white.worldforgedev.org/WFUT/");

	## Read user config file.	
	config.read([filename]);

	## TODO: We do not need to do this all the time...
	## Save config file
	fp = open(filename, "w");
	config.write(fp);
	fp.close();

	return config;

##def printUsage(invoked):
##	print invoked." -u channel_name OPTIONS";
##	print "-u channel name\t (Required) May be '.' to determine from current dir";
##	print "-s system_path\t Path containing system/read-only channel data"
##	print "-S server_root\t Url to remote server root";
##	print "-p local_path\t Path to local updates dir. Current dir is used by default.";

## Load user config
config = loadConfig();

## Setup default values
server_root = config.get("general", "server_root");
channel = ".";
local_path = "./";
system_path = "";
use_avahi = False

## Arguments parsing function
import getopt
optlist, args = getopt.getopt(sys.argv[1:], "u:s:p:vS:hL")

for (arg, opt) in optlist:
	if arg == '-u': channel = opt;
	if arg == '-s': system_path = opt;
	if arg == '-S': server_root = opt;
	if arg == '-p': local_path = opt;	
	if arg == '-v': pass
	if arg == '-h': pass
	if arg == '-L': use_avahi = True

## If scan local network;
if use_avahi:
	try:
		## Use wfut-avahi-client to return a sever root url
		import WFUTAvahiClient;
		server_root = WFUTAvahiClient.go();
	except Exception as e:
		print e;

	

## Check we have required data input
if channel == ".":
	print "No channel specified, will try using current directory";

client = WFUTCore.Client();

client.setChannelName(channel);
client.setLocalPath(local_path);
client.setSystemPath(system_path);
client.setServerRoot(server_root);

client.startUpdates();

while (client.poll()):
	pass

client.__deinit__();
