#!/usr/bin/python
import os, sys
from ConfigParser import ConfigParser
from libwfut import WFUT


## TODO: Do not use lobals
## Location of tempwfut.xml
tmp_wfut = "";
## updates list
updates = None;

## Set up event handlers
def onDownloadComplete(url, filename):
	print "Downloaded", filename;
	fileobject = updates.getFiles()[filename];
	## Log to local list
	local.addFile(fileobject);
	## Record Update
	recordUpdate(fileobject, tmp_wfut);

def onDownloadFailure(url, filename, reason):
	print "Downloaded Failed", filename, "Reason:", reason;

def onUpdateReason(filename, reason):
	#print filename, reason, WFUT.WFUT_UPDATE_NO_LOCAL;
	pass;


def recordUpdate(fileobject, tmpfile):
	"""Records a file update in the temp file."""
	try:
		fp = None;
		if os.path.exists(tmpfile):
			## Append to existing file
			fp = open(tmpfile, "at");
		else:
			## Create a new file
			fp = open(tmp_wfut, "wt");
			fp.write("<?xml version=\"1.0\"?>\n");
			fp.write("<fileList dir=\"\">\n");

		fname = WFUT.Encoder_encodeString(fileobject.filename);
		## TODO: Make sure crc is printed as unsigned.
		entry = "".join(["<file filename=\"", fname, "\" version=\"", str(fileobject.version), "\" crc32=\"",  str(fileobject.crc32), "\" size=\"", str(fileobject.size), "\" execute=\"", str(fileobject.execute), "\"/>\n"]);
		fp.write(entry);

		fp.close();
	except Exception, e:
		print e;


## Read global/local config files

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


## Setup default values
server_root = config.get("general", "server_root");
mirror_file = "mirrors.xml";
channel_file = "wfut.xml";
tmpfile = "tempwfut.xml";

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

local_root = local_path + "/" + channel + "/";

## __Main__
wfut = WFUT.WFUTClient();
wfut.init();

## Hook up callbacks
wfut.DownloadCompleteCB(onDownloadComplete);
wfut.DownloadFailedCB(onDownloadFailure);
wfut.UpdateReasonCB(onUpdateReason);

## Select a mirror
mirrors = WFUT.MirrorList();
wfut.getMirrorList(server_root + mirror_file, mirrors);
### TODO: Select mirror
if mirrors.empty() == False:
	print mirrors.front().url;
		

## Read local updates location
local = WFUT.ChannelFileList();
local_wfut = local_path + "/" + channel + "/" + channel_file;
if (os.path.exists(local_wfut)):
	if (wfut.getLocalList(local_wfut, local) > 0):
		print "no!";
		## TODO Error!
		pass
	else:
		print "yes"
		if (channel == "."):
			 channel = local.getName();
		print local.getName();

## Look for tmpwfut.xml file and pull in extra updates.
tmplist = WFUT.ChannelFileList();
tmp_wfut = local_path + "/" + tmpfile;
if (os.path.exists(tmp_wfut)):
	if (wfut.getLocalList(tmp_wfut, tmplist) > 0):
		## Error
		pass
	else:
		## Copy tmp list into local list
		files = tmplist.getFiles();
		for filename, fileobject in files.items():
			local.addFile(fileobject)

## Read system location
system = WFUT.ChannelFileList();
system_wfut = system_path + "/" + channel + "/" + channel_file;
if (os.path.exists(system_wfut)):
	if (wfut.getLocalList(system_wfut, system) > 0):
		## Error
		pass
	else:
		pass	

## Get server list
server = WFUT.ChannelFileList();
server_wfut = server_root + "/" + channel + "/" + channel_file;
print server_wfut
if (wfut.getFileList(server_wfut, server) > 0):
	## Error!
	print "Eror"
	pass

## Calculate updates
updates = WFUT.ChannelFileList();
wfut.calculateUpdates(server, system, local, updates, local_root);

# Set channel name
local.setName(server.getName());

## Download updates
wfut.updateChannel(updates, server_root + "/" + channel, local_root);

## Keep polling until there are no more updates
while (wfut.poll()):
	pass

## Save download list
wfut.saveLocalList(local, local_wfut);


### Clean up

## Delete temp files.
os.remove(tmp_wfut);

wfut.shutdown();

