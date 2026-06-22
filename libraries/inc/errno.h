#ifndef ERRNO_H
#define ERRNO_H

/* Error codes returned by all functions across the project.
 * Sequentially numbered 1-79 to fit in int8_t when negated.
 * Names are POSIX-compatible; values are not. */

#define EPERM           1  /**< Not owner */
#define ENOENT          2  /**< No such file or directory */
#define ESRCH           3  /**< No such context */
#define EINTR           4  /**< Interrupted system call */
#define EIO             5  /**< I/O error */
#define ENXIO           6  /**< No such device or address */
#define E2BIG           7  /**< Arg list too long */
#define ENOEXEC         8  /**< Exec format error */
#define EBADF           9  /**< Bad file number */
#define ECHILD          10 /**< No children */
#define EAGAIN          11 /**< No more contexts */
#define ENOMEM          12 /**< Not enough core */
#define EACCES          13 /**< Permission denied */
#define EFAULT          14 /**< Bad address */
#define ENOTBLK         15 /**< Block device required */
#define EBUSY           16 /**< Mount device busy */
#define EEXIST          17 /**< File exists */
#define EXDEV           18 /**< Cross-device link */
#define ENODEV          19 /**< No such device */
#define ENOTDIR         20 /**< Not a directory */
#define EISDIR          21 /**< Is a directory */
#define EINVAL          22 /**< Invalid argument */
#define ENFILE          23 /**< File table overflow */
#define EMFILE          24 /**< Too many open files */
#define ENOTTY          25 /**< Not a typewriter */
#define ETXTBSY         26 /**< Text file busy */
#define EFBIG           27 /**< File too large */
#define ENOSPC          28 /**< No space left on device */
#define ESPIPE          29 /**< Illegal seek */
#define EROFS           30 /**< Read-only file system */
#define EMLINK          31 /**< Too many links */
#define EPIPE           32 /**< Broken pipe */
#define EDOM            33 /**< Argument too large */
#define ERANGE          34 /**< Result too large */
#define ENOMSG          35 /**< Unexpected message type */
#define EDEADLK         36 /**< Resource deadlock avoided */
#define ENOLCK          37 /**< No locks available */
#define ENOSTR          38 /**< STREAMS device required */
#define ENODATA         39 /**< Missing expected message data */
#define ETIME           40 /**< STREAMS timeout occurred */
#define ENOSR           41 /**< Insufficient memory */
#define EPROTO          42 /**< Generic STREAMS error */
#define EBADMSG         43 /**< Invalid STREAMS message */
#define ENOSYS          44 /**< Function not implemented */
#define ENOTEMPTY       45 /**< Directory not empty */
#define ENAMETOOLONG    46 /**< File name too long */
#define ELOOP           47 /**< Too many levels of symbolic links */
#define EOPNOTSUPP      48 /**< Operation not supported on socket */
#define EPFNOSUPPORT    49 /**< Protocol family not supported */
#define ECONNRESET      50 /**< Connection reset by peer */
#define ENOBUFS         51 /**< No buffer space available */
#define EAFNOSUPPORT    52 /**< Addr family not supported */
#define EPROTOTYPE      53 /**< Protocol wrong type for socket */
#define ENOTSOCK        54 /**< Socket operation on non-socket */
#define ENOPROTOOPT     55 /**< Protocol not available */
#define ESHUTDOWN       56 /**< Can't send after socket shutdown */
#define ECONNREFUSED    57 /**< Connection refused */
#define EADDRINUSE      58 /**< Address already in use */
#define ECONNABORTED    59 /**< Software caused connection abort */
#define ENETUNREACH     60 /**< Network is unreachable */
#define ENETDOWN        61 /**< Network is down */
#define ETIMEDOUT       62 /**< Connection timed out */
#define EHOSTDOWN       63 /**< Host is down */
#define EHOSTUNREACH    64 /**< No route to host */
#define EINPROGRESS     65 /**< Operation now in progress */
#define EALREADY        66 /**< Operation already in progress */
#define EDESTADDRREQ    67 /**< Destination address required */
#define EMSGSIZE        68 /**< Message size */
#define EPROTONOSUPPORT 69 /**< Protocol not supported */
#define ESOCKTNOSUPPORT 70 /**< Socket type not supported */
#define EADDRNOTAVAIL   71 /**< Can't assign requested address */
#define ENETRESET       72 /**< Network dropped connection on reset */
#define EISCONN         73 /**< Socket is already connected */
#define ENOTCONN        74 /**< Socket is not connected */
#define ETOOMANYREFS    75 /**< Too many references: can't splice */
#define ENOTSUP         76 /**< Unsupported value */
#define EILSEQ          77 /**< Illegal byte sequence */
#define EOVERFLOW       78 /**< Value overflow */
#define ECANCELED       79 /**< Operation canceled */

#endif /* ERRNO_H */
