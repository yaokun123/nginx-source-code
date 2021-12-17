
default:	build

clean:
	rm -rf Makefile objs

build:
	$(MAKE) -f objs/Makefile

install:
	$(MAKE) -f objs/Makefile install

modules:
	$(MAKE) -f objs/Makefile modules

upgrade:
	/Users/yaokun/nginx-private/output1.12/sbin/nginx -t

	kill -USR2 `cat /Users/yaokun/nginx-private/output1.12/logs/nginx.pid`
	sleep 1
	test -f /Users/yaokun/nginx-private/output1.12/logs/nginx.pid.oldbin

	kill -QUIT `cat /Users/yaokun/nginx-private/output1.12/logs/nginx.pid.oldbin`
