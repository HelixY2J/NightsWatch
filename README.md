# NightsWatch

NightsWatch, inspired by the iNotify API, is a daemon that keeps an eye on system files for any changes. When a file is modified, it sends a desktop notification to alert the user.


## Dependencies

- `libnotify`: A library for sending desktop notifications.

We can either build with gcc -o main `pkg-config --cflags --libs libnotify` main.c

The configurations are specified in makefile. For more information please check https://wiki.archlinux.org/title/Desktop_notifications

If you get error related to <glib.h> then try the following steps

```bash
- sudo apt install libnotify-dev
```

As glib.h is a part of libglib2.0-dev package. We have to install the package

```bash
sudo apt-get install libglib2.0-dev
```

Still if error persists we might need to manually specify the information about glib2.0 in our makefile. Run following command -

```bash
- pkg-config --cflags --libs glib-2.0
```

Include the following the output in makefile.