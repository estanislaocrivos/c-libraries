#ifndef ERRNO_H
#define ERRNO_H

/* Definition of the error codes returned by all functions/methods across the project */

#define EACCES          1  /* Permission denied */
#define EIO             2  /* Input/output error */
#define ENOENT          3  /* Resource not found */
#define EAGAIN          4  /* Resource temporarily unavailable */
#define EBUSY           5  /* Device or resource busy */
#define EINVAL          6  /* Invalid argument */
#define EFAULT          7  /* Invalid pointer */
#define ETIMEDOUT       8  /* Operation timed out */
#define ENOTCONN        9  /* The device is not connected */
#define ENOMEM          10 /* Not enough space */
#define EEXIST          11 /* File exists */
#define ENOSPC          12 /* No space left on device */
#define ENOTDIR         13 /* Not a directory */
#define EISDIR          14 /* Is a directory */
#define ENFILE          15 /* Too many files open in system */
#define EMFILE          16 /* Too many open files */
#define EPIPE           17 /* Broken pipe */
#define ESRCH           18 /* No such process */
#define EPERM           19 /* Operation not permitted */
#define ENXIO           20 /* No such device or address */
#define E2BIG           21 /* Argument list too long */
#define ENOEXEC         22 /* Exec format error */
#define ECHILD          23 /* No child processes */
#define EDEADLK         24 /* Resource deadlock avoided */
#define ENOTEMPTY       25 /* Directory not empty */
#define ENAMETOOLONG    26 /* File name too long */
#define ENOTTY          27 /* Inappropriate I/O control operation */
#define EFBIG           28 /* File too large */
#define EROFS           29 /* Read-only file system */
#define EMLINK          30 /* Too many links */
#define EDOM            31 /* Mathematics argument out of domain of function */
#define ERANGE          32 /* Result too large */
#define ENOSYS          33 /* Function not implemented */
#define ELOOP           34 /* Too many levels of symbolic links */
#define ENOTBLK         35 /* Block device required */
#define EADDRINUSE      36 /* Address already in use */
#define EADDRNOTAVAIL   37 /* Cannot assign requested address */
#define ENETDOWN        38 /* Network is down */
#define ENETUNREACH     39 /* Network is unreachable */
#define ENETRESET       40 /* Network dropped connection on reset */
#define ECONNABORTED    41 /* Software caused connection abort */
#define ECONNRESET      42 /* Connection reset by peer */
#define ENOBUFS         43 /* No buffer space available */
#define EISCONN         44 /* Socket is connected */
#define ENOTSOCK        45 /* Socket operation on non-socket */
#define EDESTADDRREQ    46 /* Destination address required */
#define EMSGSIZE        47 /* Message too long */
#define EPROTOTYPE      48 /* Protocol wrong type for socket */
#define ENOPROTOOPT     49 /* Protocol not available */
#define EPROTONOSUPPORT 50 /* Protocol not supported */
#define ESOCKTNOSUPPORT 51 /* Socket type not supported */
#define EOPNOTSUPP      52 /* Operation not supported */
#define EPFNOSUPPORT    53 /* Protocol family not supported */
#define EAFNOSUPPORT    54 /* Address family not supported by protocol */
#define ESHUTDOWN       55 /* Cannot send after socket shutdown */
#define ECONNREFUSED    56 /* Connection refused */
#define EHOSTDOWN       57 /* Host is down */
#define EHOSTUNREACH    58 /* No route to host */
#define EALREADY        59 /* Operation already in progress */
#define EINPROGRESS     60 /* Operation now in progress */
#define ESTALE          61 /* Stale file handle */
#define EUCLEAN         62 /* Structure needs cleaning */
#define ENOTNAM         63 /* Not a XENIX named type file */
#define ENAVAIL         64 /* No XENIX semaphores available */
#define EREMOTE         65 /* Object is remote */
#define ENOLINK         66 /* Link has been severed */
#define EADV            67 /* Advertise error */
#define ESRMNT          68 /* Srmount error */
#define ECANCELED       69 /* Operation canceled */
#define ENOTSUP         70 /* Operation not supported */
#define EIDRM           71 /* Identifier removed */
#define ENOMSG          72 /* No message of desired type */
#define EOVERFLOW       73 /* Value too large for defined data type */
#define EILSEQ          74 /* Illegal byte sequence */
#define EBADMSG         75 /* Bad message */
#define EMULTIHOP       76 /* Multihop attempted */
#define ENODATA         77 /* No data available */
#define ENOSR           78 /* No stream resources */
#define ENOSTR          79 /* Not a stream */
#define ETIME           80 /* Stream ioctl timeout */
#define EPROTO          81 /* Protocol error */

#endif /* ERRNO_H */
