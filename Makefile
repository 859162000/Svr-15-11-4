# $Id: Makefile 6736 2006-06-05 06:53:00Z yyz $ -D_TEST_DATA_LOG dddd

BUILD_DIR_ROOT = $(PWD)
BUILD_STRING = `date +%Y%m%d`" build"`cat $(BUILD_DIR_ROOT)/.build`
VERSIONSTRING = `if [ $(VERSION_STRING) ]; then echo $(VERSION_STRING); else echo "0.0.0"; fi`
ZEBRA_DEBUG_OPT = -std=gnu++0x -g -D_USE_GCC_4 -D_USE_EPOLL_ -D_USE_CMD_NAMESPACE -D_ENCDEC_MSG  -D_ALL_SUPER_GM  $(ZEBRA_OPTIONS) -DVS=$(VERSIONSTRING) -I /usr/local/mysql/include/mysql -L/usr/local/mysql/lib/mysql -lmysqlclient
ZEBRA_DIST_OPT = -g -O2 -D_USE_EPOLL_ -D_USE_CMD_NAMESPACE -D_ENCDEC_MSG  -DVS=$(VERSIONSTRING)
SUB_DIRS = base/EncDec base ScenesServer SessionServer BillServer GatewayServer SuperServer RecordServer FLServer InfoServer 
TEST_SUB_DIRS = base test
ALL_SUB_DIRS = $(SUB_DIRS) $(TEST_SUB_DIRS)
NJOBS = `if [ "" == "$(NCPUS)" ]; then echo ""; else echo "-j$(NCPUS)"; fi`

.PHONY : all debug debug_nogm ctags doc distclean clean sql svn build release_cp dist_tar dist map distall dist_tar_gm dist_gm map_gm distall_gm $(ALL_SUB_DIRS) ChangeLog

all: debug

cmd:
	./lua rcmd.lua base/Command.h

cmd_clean:
	rm -rf base/Command.h
	mv base/Command.h.bak base/Command.h
		 
debug:
	@for dir in $(SUB_DIRS); \
		do \
		ZEBRA_COMPILE_OPT='$(ZEBRA_DEBUG_OPT) -DBUILD_STRING=$(BUILD_STRING)' $(MAKE) $(NJOBS) -C $$dir; \
		done

debug_nogm:
	@for dir in $(SUB_DIRS); \
		do \
		ZEBRA_COMPILE_OPT='$(ZEBRA_DIST_OPT) -DBUILD_STRING=$(BUILD_STRING)' $(MAKE) $(NJOBS) -C $$dir; \
		done

test:
	@for dir in $(TEST_SUB_DIRS); \
		do \
		ZEBRA_COMPILE_OPT='$(ZEBRA_DEBUG_OPT) -DBUILD_STRING=$(BUILD_STRING)' $(MAKE) $(NJOBS) -C $$dir; \
		done

sql:
	@$(MAKE) -C sql

svn:
	@svn ps svn:ignore -F ./.svnignore ./;
	@for dir in sql $(ALL_SUB_DIRS); \
	do \
	svn ps svn:ignore -F $$dir/.svnignore $$dir/; \
	done
	@find . -iname \*.[ch] -exec svn ps svn:keywords "Id" \{\} \;
	@find . -iname \*.cpp -exec svn ps svn:keywords "Id" \{\} \;
	@find . -iname Makefile -exec svn ps svn:keywords "Id" \{\} \;
	@find . -iname \*.[ch] -exec svn ps svn:eol-style native \{\} \;
	@find . -iname \*.cpp -exec svn ps svn:eol-style native \{\} \;
	@find . -iname Makefile -exec svn ps svn:eol-style native \{\} \;
	@find . -iname \*.sql -exec svn ps svn:eol-style native \{\} \;
	@find . -iname \*.xml.example -exec svn ps svn:eol-style native \{\} \;
	@svn ps svn:eol-style native map/*.xml;
	@svn ps svn:eol-style native Doxyfile.pub;
	@svn ps svn:eol-style native Doxyfile;
	@svn ps svn:eol-style native ChangeLog;
	@svn ps svn:eol-style native gnuify-changelog.pl;
	@svn ps svn:executable yes .version;
	@svn ps svn:executable yes findexp;
	@svn ps svn:executable yes go;
	@svn ps svn:executable yes gnuify-changelog.pl;

build:
	@./.version

ChangeLog:
	@svn log -v | ./gnuify-changelog.pl > ChangeLog	

release_cp:
	@mkdir -pv release/
	@cp -avf config.xml.example release/
	@cp -avf miniConfig.xml release/
	@find $(SUB_DIRS) -iname \*.xml -exec cp -avf \{\} release/ \;
	@cp -avf SuperServer/SuperServer release/
	@objcopy --only-keep-debug release/SuperServer release/SuperServer.symbol
	@objcopy --strip-debug release/SuperServer
	@cp -avf RecordServer/RecordServer release/
	@objcopy --only-keep-debug release/RecordServer release/RecordServer.symbol
	@objcopy --strip-debug release/RecordServer
	@cp -avf BillServer/BillServer release/
	@objcopy --only-keep-debug release/BillServer release/BillServer.symbol
	@objcopy --strip-debug release/BillServer
	@cp -avf MiniServer/MiniServer release/
	@objcopy --only-keep-debug release/MiniServer release/MiniServer.symbol
	@objcopy --strip-debug release/MiniServer
	@cp -avf SessionServer/SessionServer release/
	@objcopy --only-keep-debug release/SessionServer release/SessionServer.symbol
	@objcopy --strip-debug release/SessionServer
	@cp -avf ScenesServer/ScenesServer release/
	@objcopy --only-keep-debug release/ScenesServer release/ScenesServer.symbol
	@objcopy --strip-debug release/ScenesServer
	@cp -avf GatewayServer/GatewayServer release/
	@objcopy --only-keep-debug release/GatewayServer release/GatewayServer.symbol
	@objcopy --strip-debug release/GatewayServer
	@cp -avf tools/zntop release/
	#@cp -avf tools/zebraclient release/
	#@cp -avf tools/base64 release/
	@cp -avf sql/*.sql release/
	@cp -avf sql/*.txt release/
	@cp -avf forbidWords release/
	@cp -avf gm_conf.xml release/
	@cp -avf process.dat release/
	@find quest -type f -iname \*xml -exec cp -v \{\} release/ --parents \;
	@find newquest -type f -iname \*lua -exec cp -v \{\} release/ --parents \;

dist_tar:
	@mkdir -pv dist
	@svn info>release/ver.txt
	@tar cvfj dist/Zebra-release-`date +%Y%m%d%H%M`-`cat .build`.`uname -m`.tar.bz2 release/

dist: build distclean clean debug_nogm release_cp dist_tar

map:
	@mkdir -pv dist
	@tar cvfj dist/Zebra-map-`date +%Y%m%d%H%M`-`cat .build`.tar.bz2 map/*

distall: dist map

dist_tar_gm:
	@mkdir -pv dist
	@svn info>release/ver.txt
	@tar cvfj dist/Zebra-release-gm-`date +%Y%m%d%H%M`-`cat .build`.`uname -m`.tar.bz2 release/

dist_gm: build distclean clean debug release_cp dist_tar_gm

map_gm:
	@mkdir -pv dist
	@tar cvfj dist/Zebra-map-`date +%Y%m%d%H%M`-`cat .build`.tar.bz2 map/*

distall_gm: dist_gm map_gm

wc:
	@find . -iname \*.[ch] -exec cat \{\} \; | wc -l
	@find . -iname \*.cpp -exec cat \{\} \; | wc -l

wc_blank:
	@find . -iname \*.[ch] -exec cat \{\} \; | grep -c '^ *$$'
	@find . -iname \*.cpp -exec cat \{\} \; | grep -c '^ *$$'

ctags:
	@ctags -R -h ".c.cpp.h" -o tags

doc:
	@doxygen Doxyfile.pub

alldoc:
	@doxygen

distclean:
	@find . -iname .\*.d -exec rm \{\} \;

clean:
	@for dir in $(ALL_SUB_DIRS); \
	do \
	$(MAKE) -C $$dir clean; \
	done
