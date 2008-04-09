#!/usr/bin/python
import os, sys
from ConfigParser import ConfigParser
from libwfut import WFUT
import WFUTCore

## Read global/local config files
def loadConfig():
	filename = os.path.join(os.environ['HOME'], '.libwfutrc');

	config = ConfigParser();
	## Set some default values
	config.add_section("general");
	
	config.set("general", "server_root", "http://white.worldforgedev.org/WFUT/");
	
	config.read([filename]);

	## Save config file
	fp = open(filename, "w");
	config.write(fp);
	fp.close();

	return config;

config = loadConfig();
## Setup default values
server_root = config.get("general", "server_root");
channel = ".";
local_path = "./";
system_path = "";

## Arguments parsing function
import getopt
optlist, args = getopt.getopt(sys.argv[1:], "u:s:p:vS:h")

for (arg, opt) in optlist:
	if arg == '-u': channel = opt;
	if arg == '-s': system_path = opt;
	if arg == '-S': server_root = opt;
	if arg == '-p': local_path = opt;	
	if arg == '-v': pass
	if arg == '-h': pass

client = WFUTCore.Client();

client.setChannelName(channel);
client.setLocalPath(local_path);
client.setSystemPath(system_path);
client.setServerRoot(server_root);

client.startUpdates();

while (client.poll()):
	pass

client.__deinit__();
