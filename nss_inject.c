
/*
 * Copyright (c) 2012 Karl N. Redgate
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <netdb.h>
#include <nss.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>

/**
 * Need to add my notes on the semantics of these values.
 *
 * NSS_STATUS_TRYAGAIN - 
 * NSS_STATUS_UNAVAIL  - 
 * NSS_STATUS_NOTFOUND - 
 * NSS_STATUS_SUCCESS  - 
 * NSS_STATUS_RETURN   - 
 */

/**
 * Default the delay to 30 seconds, but allow the sentinal file to define
 * an alternative delay.
 */
static enum nss_status
inject( char *name ) {
    int enabled = 0;
    struct timespec delay = { 30, 0 };
    char buffer[80];
    FILE *sentinal;

    sprintf( buffer, "/tmp/inject.%s", name );
    sentinal = fopen( buffer, "r" );
    if ( sentinal != NULL ) {
        int assigned = fscanf( sentinal, "%ld.%09ld", &delay.tv_sec, &delay.tv_nsec );
        fclose( sentinal );
        if ( assigned != 2 ) return NSS_STATUS_NOTFOUND;
        enabled = 1;
    }

    if ( enabled ) {
        syslog( LOG_NOTICE, "{%s} inject delay %ld.%09ld", name, delay.tv_sec, delay.tv_nsec );
        nanosleep( &delay, 0 );
        syslog( LOG_NOTICE, "{%s} inject delay complete", name );
    }

    return NSS_STATUS_NOTFOUND;
}

/**
 */
enum nss_status
_nss_inject_gethostbyname_r( const char *name, struct hostent *result,
                          char *buffer, size_t buflen,
                          int *errnop, int *h_errnop )
{
    return inject( "gethostbyname" );
}

/**
 */
enum nss_status
_nss_inject_gethostbyname2_r( const char *name, int family, 
                           struct hostent *result,
                           char *buffer, size_t buflen,
                           int *errnop, int *h_errnop )
{
    return inject( "gethostbyname2" );
}

/** Lookups for getaddrinfo and related functions
 */
enum nss_status
_nss_inject_gethostbyname3_r( const char *name, int family, 
                           struct hostent *result,
                           char *buffer, size_t buflen,
                           int *errnop, int *h_errnop,
                           int32_t *ttlp, char **canonp )
{
    return inject( "gethostbyname3" );
}

/**
 */
enum nss_status
_nss_inject_gethostbyaddr_r( const char *address, socklen_t len, int family,
                          struct hostent *result,
                          char *buffer, size_t buflen,
                          int *errnop, int *h_errnop )
{
    if ( family != AF_INET6 ) return NSS_STATUS_NOTFOUND;
    return inject( "gethostbyaddr" );
}

/** Prepare for gethostent
 */
enum nss_status
_nss_inject_sethostent( int persist ) {
    return NSS_STATUS_SUCCESS;
}

/** Cleanup after gethostent
 */
enum nss_status
_nss_inject_endhostent() {
    return NSS_STATUS_SUCCESS;
}

/** Return next hostent in the current iterator
 */
enum nss_status
_nss_inject_gethostent_r( struct hostent *result, char *buffer, size_t buflen,
                       int *errnop, int *h_errnop )
{
    return inject( "gethostent" );
}

/* vim: set autoindent expandtab sw=4 : */
