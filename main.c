#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>
#include <sys/inotify.h>
// #include <gio/gio.h>
#include <libnotify/notify.h>


#define EXT_YES 0
#define EXT_FEW_ARGS 1
#define EXT_ERR_INOTIFY 2
#define EXT_ERR_WATCH 3
#define EXT_ERR_READ_EVENT 4
#define EXT_INIT_STATUS 5

int iEventQueue = -1;
int iEventStatus = -1;

char* title = "NightsWatch";

int main(int argc, char** argv) {

    bool notifyinitStatus = false;
    

    char *basePath = NULL;
    char *token    = NULL;
    char *notifMsg = NULL;

    NotifyNotification * handle;
    char buffer[4096];
    int readLen;

    const struct inotify_event* watcher_event;

    const uint32_t mask = IN_CREATE | IN_DELETE | IN_CLOSE_WRITE | IN_MODIFY | IN_MOVE_SELF;
    // combination of filesystem event flags

    if(argc < 2){
        fprintf(stderr,"USAGE: And the watch has ended\n");
        exit(EXT_FEW_ARGS);
    }
    
    basePath = (char*)malloc(sizeof(char)*(strlen(argv[1]) + 1));
    // dynamically allocate memory for string in first cmd line arg
    if (basePath == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    exit(EXIT_FAILURE); // checking null pointer
    }

    strcpy(basePath, argv[1]); // Copy the input string to basePath
     
    token = strtok(basePath,"/");
    
    while(token != NULL){
        basePath = token;
        token = strtok(NULL,"/"); // Avoid the modification of argv[1]
    }

    notifyinitStatus = notify_init(title);

    if(!notifyinitStatus){
        fprintf(stderr,"error intializing libnotify\n");
        exit(EXT_INIT_STATUS);
    }
    
    iEventQueue = inotify_init();
    if (iEventQueue == -1){
        fprintf(stderr,"ERROR intilaizing inotify instance\n");
        exit(EXT_ERR_INOTIFY);
    }

    iEventStatus = inotify_add_watch(iEventQueue,argv[1],mask);
    if (iEventStatus == -1){
        fprintf(stderr,"Error adding file descriptor to watch event\n");
        exit(EXT_ERR_WATCH);
    }

    while(true){
        printf("Waiting for event...\n");
        readLen = read(iEventQueue,buffer,sizeof(buffer));
        if(readLen == -1){
            fprintf(stderr,"Error reading from inotify instance");
            exit(EXT_ERR_READ_EVENT);
        }


    /* Reading data from the inotify file descriptor into a buffer 
    Then its iterating over the buffer to process the filesystem events  */
        for(char *bufferptr = buffer; bufferptr < buffer + readLen; bufferptr += sizeof(struct inotify_event) + watcher_event -> len){

            notifMsg = NULL;
            watcher_event = (const struct inotify_event *) bufferptr ; 

            if (watcher_event->mask & IN_CREATE){
                notifMsg = "file has been created\n";
            }
            
            if (watcher_event->mask & IN_DELETE){
                notifMsg = "file has been reduced to atoms\n";
            }

            if (watcher_event->mask & IN_ACCESS){
                notifMsg = "file has been accessed\n";
            }

            if (watcher_event->mask & IN_CLOSE_WRITE){
                notifMsg = "file has been written and closed\n";
            }

            if (watcher_event->mask & IN_MODIFY){
                notifMsg = "file has been modified\n";
            }

            if (watcher_event->mask & IN_MOVE_SELF){
                notifMsg = "file has been moved\n";
            }

            if (notifMsg == NULL){
                continue;
            }
            /* Incase, Eventread has returned an event that we dont 
            want to send the notification to */
            
            handle = notify_notification_new(basePath, notifMsg,"dialog-information");
            if(handle == NULL){
                fprintf(stderr,"Notification handle is null"); 
                continue;
            }
            notify_notification_show(handle,NULL);
        }

    }


    
    exit(EXT_YES); 
       
}