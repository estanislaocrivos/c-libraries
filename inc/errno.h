#ifndef ERRNO_H
#define ERRNO_H

/* Definition of the error codes returned by all functions/methods across the project */

#define EACCES    1 /* Permission denied */
#define EIO       2 /* Input/output error */
#define ENOENT    3 /* Resource not found */
#define EAGAIN    4 /* Resource temporarily unavailable */
#define EBUSY     5 /* Device or resource busy */
#define EINVAL    6 /* Invalid argument */
#define EFAULT    7 /* Invalid pointer */
#define ETIMEDOUT 8 /* Operation timed out */
#define ENOTCONN  9 /* The device is not connected */

#endif /* ERRNO_H */
