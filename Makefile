all: wiimote camera

camera:
	gcc glxgears.c camera_tracker.c -o camera_tracker -lGL -lcwiid

wiimote:
	gcc glxgears.c wiimote_tracker.c -o wiimote_tracker -lGL -lcwiid

