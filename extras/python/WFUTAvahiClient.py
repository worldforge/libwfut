#!/usr/bin/python

def go():
	## I think we need an object here which will poll until told to stop.
	## Register some callbacks to spit out servers

        print "Here"
	import sys
	import time
	import dbus, gobject, avahi
	from dbus.mainloop.glib import DBusGMainLoop

	import string

	serviceType = "_wfut._tcp"
	servicePort = 0;
	serviceTXT = ""

	domain = ""
	host = ""

	## Map of discovered servers
	servers = {}

	def service_add(interface, protocol, name, type, domain, flags):


		print "found server"

	        ## Find out more info
	        obj_path = server.ServiceResolverNew(interface, protocol, name, type, domain, avahi.PROTO_UNSPEC, dbus.UInt32(0))

	        resolver = dbus.Interface(bus.get_object(avahi.DBUS_NAME, obj_path),
	                   avahi.DBUS_INTERFACE_SERVICE_RESOLVER)

	        ## Hook up callbacks to 'Found'
	        resolver.connect_to_signal('Found', found)

	def found(interface, protcol, name, type, domain, host, aprotocol, address, port, txt, flags):

	        ## Construct a server url from address and port number.
	        ## Uses '[' ']' incase of IPv6 addresses.
	        servers[name] = "".join([ "http://", "[" , address, "]", ":", str(port)])
	        ## Look for a "url" txt field to override the one constructed above.
	        txt_arr = avahi.txt_array_to_string_array(txt)
	        for rec in txt_arr:
	                (key, value) = rec.split("=")
	                if key == "url":
	                        servers[name] = value
	        print "Found server: " + servers[name]



	if True:
            print "Here"

	    DBusGMainLoop( set_as_default=True )

	    main_loop = gobject.MainLoop()
	    bus = dbus.SystemBus()

	    ## TODO: Hook up service added / removed

	    global server
	    server = dbus.Interface(
	            bus.get_object( avahi.DBUS_NAME, avahi.DBUS_PATH_SERVER ),
	            avahi.DBUS_INTERFACE_SERVER )

	    ## Find services from any interface and protocol
	    interface =  avahi.IF_UNSPEC
	    protocol = avahi.PROTO_UNSPEC

	    ## Use local domain.
	    domain = "local"

	    ## Call method to create new browser, and get back an object
	    ## path for it.
	    obj_path = server.ServiceBrowserNew(interface, protocol, serviceType, domain, dbus.UInt32(0))

	    ## Create browser interface for the new object
	    browser = dbus.Interface(bus.get_object(avahi.DBUS_NAME, obj_path), avahi.DBUS_INTERFACE_SERVICE_BROWSER)

	    ## Hook up signal for new found services
	    browser.connect_to_signal('ItemNew', service_add)

	    try:

		context = main_loop.get_context()
	        ## TODO: loop for short time period
	        while (context.pending()):
			print "tick"
	                time.sleep(0.5)

			# Handle commands here
			context.iteration(True)


		## TODO: Turn off avahi

		## Offer choices to user
		index = 0;
		options = [];
	        for (key, value) in servers.iteritems():
			index = index + 1;
			print(string.join(["Found (", str(index), ") ", key, " -> ", value]))
			options.append(key)


		while True:
			try:
				print "Pick Server: ",
				opt = sys.stdin.readline();
	       			opt = int(opt) - 1;
				break;

			except Exception as e:
				print e

		## get selected server option
		choice = options[opt]
		server = servers[choice];


		return server
		## Fall back to original code with new server choice

	    except KeyboardInterrupt:
	        pass
