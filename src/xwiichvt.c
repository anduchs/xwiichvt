/*
 * xwiichvt
 * Copyright 2013, Andreas Fuchs
 * 
 */


#include <xwiimote.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> 
#include <sys/ioctl.h>
#include <linux/vt.h>


#include "config.h"


const int ledmap[11][4] = {
    {0, 0, 0, 0},
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1},
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 1},
    {1, 1, 1, 0},
    {0, 1, 1, 1},
    {1, 1, 1, 1}
};

int run_once(struct xwii_iface *iface)
{
	struct xwii_event event;
	int ret = 0;
	struct vt_stat vtstat;
	int vt = 0;
	int i;
	int console = 0;
	char *device = "/dev/console";
	//char *device = "/dev/tty0";
	
    console = open(device, O_RDWR);
	if (console < 0) {
        fprintf(stderr, "/dev/console problems. Do you have enough permission ?\n");
        exit(3);
    }
    if ((ioctl(console, VT_GETSTATE, &vtstat) < 0) || vt == -1) {
        fprintf(stderr, "/dev/console problems. Do you have enough permission ?\n");
        exit(3);
    }
    close(console);
    vt = vtstat.v_active;
    printf("Start vt %i.\n",vt);
    if (vt > 10)
        vt = 0;
    if (vt < 0) {
        printf("Error getting vt.\n");
        return -1;
    }
    for (i=0; i<4; i++) {
	    printf("Setting led %i to %i.\n", i+1, ledmap[vt][i]);
    	ret = xwii_iface_set_led(iface, i+1, ledmap[vt][i]);
    	if (ret) {
    	    printf("Error setting led %i to %i.\n", i+1, ledmap[vt][i]);
    	    return ret;
    	}
    }

	while (true) {
		ret = xwii_iface_poll(iface, &event);
		if (ret == -EAGAIN) {
			usleep(50000);
		} else if (ret) {
			printf("Error: Read failed with err:%d\n", ret);
			return ret;
		} else if (event.type == XWII_EVENT_KEY && event.v.key.state) {
            console = open(device, O_RDWR); //got some errors when leaving console open over switches...
	        if (console < 0) {
                fprintf(stderr, "/dev/console problems. Do you have enough permission ?\n");
                exit(3);
            }
            if ((ioctl(console, VT_GETSTATE, &vtstat) < 0) || vt == -1) {
                fprintf(stderr, "/dev/console problems. Do you have enough permission ?\n");
                exit(3);
            } else
                printf("old-vt %i.   ",vt);
            vt = vtstat.v_active;
            switch (event.v.key.code) {
		    case XWII_KEY_TWO:
		        vt += 2;
		    case XWII_KEY_ONE:
		        vt -= 1;
		        if (vt < 1)
		            vt = 1;
		        if (vt > 10)
		            vt = 10;
                if (ioctl(console, VT_ACTIVATE, vt) != 0) {
                    fprintf(stderr, "/dev/console problems. Do you have enough permission ?\n");
                    exit(3);
                } else
                    printf("Switched to vt %i.\n",vt);
                for (i=0; i<4; i++) {
                	ret = xwii_iface_set_led(iface, i+1, ledmap[vt][i]);
                	if (ret) {
                	    printf("Error setting led %i to %i.\n", i+1, ledmap[vt][i]);
                	    return ret;
                	}
                }
			    break;
			default:
			    continue;
			}
			close(console);
		}
	}
	return ret;
}

int main(int argc, char **argv)
{
	int ret = 0;
	char *path = NULL;
	struct xwii_iface *iface;

	if (argc > 2 || (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0))) {
	    printf("This is %s version %s.\n\n", PACKAGE, VERSION);
		printf("Usage: %s [-h | --help] [-o | --once]\n", argv[0]);
		printf("\t-h\t--help\tShow this information.\n");
		printf("\t-o\t--once\tFind one device and do not try to rediscover.\n");
		return 1;
	} 
	

	while (true) {
	    struct xwii_monitor *mon = xwii_monitor_new(false, false);
	    if (!mon) {
		    printf("Cannot create monitor\n");
		    return 2;
	    }
    	path = xwii_monitor_poll(mon);
    	xwii_monitor_unref(mon);
		usleep(100000);
       	if (path != NULL) {
		    ret = xwii_iface_new(&iface, path);
		    if (ret) {
			    printf("Error newing '%s' err:%d\n", path, ret);
		    } else {
                ret = xwii_iface_open(iface, XWII_IFACE_CORE |
					                     XWII_IFACE_WRITABLE);
	            if (ret) {
		            printf("Error opening '%s' err:%d\n", path, ret);
	            } else {
		            ret = run_once(iface);
		            if (ret) {
            		    printf("Event-Loop exit with status %d\n", ret);
            		}
            	}
		    }
		    free(path);
		    xwii_iface_unref(iface);
		}
		if (argc >= 2 && (strcmp(argv[1], "-o") == 0 || strcmp(argv[1], "--once") == 0))
		    break;
	}


	return ret;
}
