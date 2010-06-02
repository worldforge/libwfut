import os
from libwfut import WFUT

#### Select a mirror
##wfut.getMirrorList(server_root + mirror_file, mirrors);
##### TODO: Select mirror
##if mirrors.empty() == False:
##	print mirrors.front().url;

## Set up event handlers
def onDownloadComplete(url, filename, local, updates, tmpfile):
	print "Downloaded", filename;
	fileobject = updates.getFiles()[filename];
	## Log to local list
	local.addFile(fileobject);
	## Record Update
	recordUpdate(fileobject, tmpfile);

def onDownloadFailure(url, filename, reason):
	print "Downloaded Failed", filename, "Reason:", reason;

def onUpdateReason(filename, reason):
	#print filename, reason, WFUT.WFUT_UPDATE_NO_LOCAL;
	pass;

def recordUpdate(fileobject, tmpfile):
	"""Records a file update in the temp file."""
	print(tmpfile)
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
		print(fname)
		## TODO: Make sure crc is printed as unsigned.
		entry = "".join(["<file filename=\"", fname, "\" version=\"", str(fileobject.version), "\" crc32=\"",  str(fileobject.crc32), "\" size=\"", str(fileobject.size), "\" execute=\"", str(fileobject.execute), "\"/>\n"]);
		fp.write(entry);

		fp.close();
	except Exception, e:
		print e;

class Client:
	def __init__(self):
		self.mirror_file = "mirrors.xml";
		self.channel_file = "wfut.xml";
		self.tmpfile = "tempwfut.xml";

		self.wfut = WFUT.WFUTClient();
		self.wfut.init();

		self.channel = ".";
		self.local_path = "./";
		self.system_path = "";
		self.server_root = "";

	def setChannelName(self, channel):
		self.channel = channel;

	def setSystemPath(self, system_path):
		self.system_path = system_path;

	def setLocalPath(self, local_path):
		self.local_path = local_path;

	def setServerRoot(self, server_root):
		self.server_root = server_root;

	def startUpdates(self):
		self.mirrors = WFUT.MirrorList();
		self.updates = WFUT.ChannelFileList();
		self.local = WFUT.ChannelFileList();
		self.tmplist = WFUT.ChannelFileList();
		self.system = WFUT.ChannelFileList();
		self.server = WFUT.ChannelFileList();

		local_root = self.local_path + "/" + self.channel + "/";
		local_wfut = self.local_path + "/" + self.channel + "/" + self.channel_file;
		system_wfut = self.system_path + "/" + self.channel + "/" + self.channel_file;
		server_wfut = self.server_root + "/" + self.channel + "/" + self.channel_file;

		self.tmp_wfut = self.local_path + "/" + self.tmpfile;

		## Hook up callbacks
		#  Create a callback binding the updates list and tmp file loations.
		def dc(u,f): return onDownloadComplete(u, f, self.local, self.updates, self.tmp_wfut)
		self.dc = dc;
		self.wfut.DownloadCompleteCB(self.dc);

		self.wfut.DownloadFailedCB(onDownloadFailure);
		self.wfut.UpdateReasonCB(onUpdateReason);

		
		## Read local updates location
		if (os.path.exists(local_wfut)):
			if (self.wfut.getLocalList(local_wfut, self.local) > 0):
				print "Error reading local wfut.xml";
			else:
				if (self.channel == "."):
					 self.channel = self.local.getName();

		## If channel name is still '.', then we cannot proceed.
		if (self.channel == "."):
			raise "Unable to determine local channel name";

		## Look for tmpwfut.xml file and pull in extra updates.
		if (os.path.exists(self.tmp_wfut)):
			if (self.wfut.getLocalList(self.tmp_wfut, self.tmplist) > 0):
				print "Error reading tempwfut.xml";
			else:
				## Copy tmp list into local list
				files = self.tmplist.getFiles();
				for filename, fileobject in files.items():
					self.local.addFile(fileobject)

		## Read system location
		if (os.path.exists(system_wfut)):
			if (self.wfut.getLocalList(system_wfut, self.system) > 0):
				print "Error reading system wfut.xml";
			else:
				pass	

		## Get server list
		if (self.wfut.getFileList(server_wfut, self.server) > 0):
			print "Error reading or downloading server wfut.xml";
		else:
			pass

		## Calculate updates
		self.wfut.calculateUpdates(self.server, self.system, self.local, self.updates, local_root);

		# Set channel name
		self.local.setName(self.server.getName());

		## Download updates
		self.wfut.updateChannel(self.updates, self.server_root + "/" + self.channel, local_root);

	def poll(self):
		return self.wfut.poll();
			
	def finishUpdates(self):	
		## Save download list
		self.wfut.saveLocalList(self.local, local_wfut);
		## Delete temp files.
		os.remove(self.tmp_wfut);

	def __deinit__(self):
		self.wfut.shutdown();

