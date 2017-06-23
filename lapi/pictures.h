#ifndef PICTURES_H
#define PICTURES_H

/*for now*/
#define LINUX
/*#define WINDOWS*/

#ifdef LINUX

#define IMAGE_UMARU "/home/leo/pic/ts/umaru.png"
#define IMAGE_BACK "/home/leo/pic/ts/bak.jpg"

extern const char IMAGE_DEFAULT_BACKGROUND[];
extern const char IMAGE_DEFAULT_BALL[];
extern const char IMAGE_DEFAULT_PRACKET[];
extern const char IMAGE_DEFAULT_ORACKET[];

#else
/*macro for windows*/

#endif


#endif // PICTURES_H
