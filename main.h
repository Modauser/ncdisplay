#ifndef MAIN_H_
#define MAIN_H_

/**
 * Searches for and mounts the SD card, and optionally the USB drive.
 * @return Zero if both mounted, <0 if no SD card, >0 if no USB drive
 */
int initDisks(void);

#endif // MAIN_H_
