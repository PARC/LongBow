/*
 * Copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEROX OR PARC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * ################################################################################
 * #
 * # PATENT NOTICE
 * #
 * # This software is distributed under the BSD 2-clause License (see LICENSE
 * # file).  This BSD License does not make any patent claims and as such, does
 * # not act as a patent grant.  The purpose of this section is for each contributor
 * # to define their intentions with respect to intellectual property.
 * #
 * # Each contributor to this source code is encouraged to state their patent
 * # claims and licensing mechanisms for any contributions made. At the end of
 * # this section contributors may each make their own statements.  Contributor's
 * # claims and grants only apply to the pieces (source code, programs, text,
 * # media, etc) that they have contributed directly to this software.
 * #
 * # There is no guarantee that this section is complete, up to date or accurate. It
 * # is up to the contributors to maintain their portion of this section and up to
 * # the user of the software to verify any claims herein.
 * #
 * # Do not remove this header notification.  The contents of this section must be
 * # present in all distributions of the software.  You may only modify your own
 * # intellectual property statements.  Please provide contact information.
 * 
 * - Palo Alto Research Center, Inc
 * This software distribution does not grant any rights to patents owned by Palo
 * Alto Research Center, Inc (PARC). Rights to these patents are available via
 * various mechanisms. As of January 2016 PARC has committed to FRAND licensing any
 * intellectual property used by its contributions to this software. You may
 * contact PARC at cipo@parc.com for more information or visit http://www.ccnx.org
 */
/**
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <LongBow/private/longBow_Memory.h>

#if HAVE_REALLOC == 0
static void *
_LongBow_rplRealloc(void *oldAlloc, size_t newSize)
{
    if (newSize == 0) {
        newSize = 1;
    }
    
    char *newAlloc = malloc(newSize);
    
    if (oldAlloc != NULL) {
        memcpy(newAlloc, oldAlloc, newSize);
        free(oldAlloc);
    }
    return newAlloc;
}
#endif

static uint64_t _outstandingAllocations;

void *
longBowMemory_Allocate(const size_t size)
{
    _outstandingAllocations++;
    return calloc(1, size);
}

void *
longBowMemory_Reallocate(void *oldAllocation, const size_t newSize)
{
#if HAVE_REALLOC
    void *result = realloc(oldAllocation, newSize);
#else
    void *result = _LongBow_rplRealloc(oldAllocation, newSize);
#endif    
    
    if (oldAllocation == NULL) {
        _outstandingAllocations++;
    }
    
    return result;
}

void
longBowMemory_Deallocate(void **pointerPointer)
{
    free(*pointerPointer);
    _outstandingAllocations--;
    *pointerPointer = NULL;
}

uint64_t
longBowMemory_OutstandingAllocations(void)
{
    return _outstandingAllocations;
}

char *
longBowMemory_StringCopy(const char *string)
{
    char *result = NULL;
    
    if (string != NULL) {
        size_t length = strlen(string);
        result = longBowMemory_Allocate(length + 1);
        strcpy(result, string);
        result[length] = 0;
    }
    return result;
}
