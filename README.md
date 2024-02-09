# NightsWatch

NightsWatch, inspired by the iNotify API, is a daemon that keeps an eye on system files for any changes. When a file is modified, it sends a desktop notification to alert the user.


## Dependencies

 `libnotify`: A library for sending desktop notifications.

We can either build with gcc -o main `pkg-config --cflags --libs libnotify` main.c

The configurations are specified in makefile. For more information please check https://wiki.archlinux.org/title/Desktop_notifications

If you get error related to <glib.h> then we need to install *-dev packages.

```bash
 sudo apt-get install libnotify-dev
```

Still if error persists we might need to manually specify the information about glib2.0 in our makefile. Run following command -

```bash
 pkg-config --cflags --libs glib-2.0
```

Include the output in makefile.


### Event handling

Bitmasks are often used to represent sets of Boolean flags efficiently.So,we are using it to check the status of file.To determine what events have occured, an application read() from the inotify file descriptor. If no events have so far occured, then assuming a blocking file descriptor, read() will block untill atleast one event occurs or unless interrupted by a signal in which case the call fails with the error.

Then we integrate our application with the libnotify API to deliver desktop notifications.Using the libnotify API,our application can construct and send notifications to the desktop environment.These notifications inform the user about specific file events such as file creation, modification or deletion.

