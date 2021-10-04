#ifndef LIST
#define LIST

#include <linux/vmalloc.h>

// Functions
void addfile(const char *f);
void remfile(const char *f);
void emptylist(void);
int lookupfilename(const char *f);
int extractfilename(const char *f);

#endif
