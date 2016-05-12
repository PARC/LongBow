/*
 * Copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include "config.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <LongBow/private/longBow_String.h>
#include <LongBow/private/longBow_Memory.h>

struct longbow_string {
    char *buffer;
    size_t cursor; // always the index of the nul terminating byte of the stored string.
    size_t end; // always the index of the very last byte in buffer;
};

static size_t
_longBowString_RemainingSpace(const LongBowString *string)
{
    size_t result = string->end - string->cursor;
    
    return result;
}

LongBowString *
longBowString_Create(const size_t initialSize)
{
    LongBowString *result = longBowMemory_Allocate(sizeof(LongBowString));
    result->end = initialSize;
    result->buffer = longBowMemory_Allocate(initialSize);
    result->cursor = 0;
    
    return result;
}

LongBowString *
longBowString_CreateString(const char *string)
{
    LongBowString *result = longBowMemory_Allocate(sizeof(LongBowString));
    result->end = strlen(string) + 1;
    result->buffer = longBowMemory_StringCopy(string);
    result->cursor = result->end - 1;
    result->buffer[result->cursor] = 0;
    
    return result;
}

LongBowString *
longBowString_CreateFormat(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    char *cString;
    if (vasprintf(&cString, format, ap) == -1) {
        return NULL;
    }
    va_end(ap);
    
    LongBowString *string = longBowString_CreateString(cString);
    
    free(cString);
    
    return string;
}

void
longBowString_Destroy(LongBowString **stringPtr)
{
    LongBowString *string = *stringPtr;
    if (string != NULL) {
        longBowMemory_Deallocate((void **) &string->buffer);
        longBowMemory_Deallocate((void **) stringPtr);
    }
}

LongBowString *
longBowString_Append(LongBowString *string, const char *value)
{
    size_t length = strlen(value) + 1;
    
    if (_longBowString_RemainingSpace(string) < length) {
        size_t size = string->end + length;
        string->buffer = longBowMemory_Reallocate(string->buffer, size);
        string->end = size - 1;
    }
    strcpy(&string->buffer[string->cursor], value);
    string->cursor += (length - 1);
    string->buffer[string->cursor] = 0;

    return string;
}

LongBowString *
longBowString_Format(LongBowString *string, const char *format, ...)
{
    LongBowString *result = NULL;
    
    va_list ap;
    va_start(ap, format);
    
    char *cString;
    int status = vasprintf(&cString, format, ap);
    va_end(ap);
    if (status != -1) {
        result = longBowString_Append(string, cString);
        free(cString);
    } else {
        result = NULL;
    }
    
    return result;
}

char *
longBowString_ToString(const LongBowString *string)
{
    char *result = strndup(string->buffer, string->end);
    return result;
}

bool
longBowString_StartsWith(const char *string, const char *prefix)
{
    bool result = strncmp(string, prefix, strlen(prefix)) == 0;
    return result;
}

bool
longBowString_Equals(const char *string, const char *other)
{
    return strcmp(string, other) == 0;
}

bool
longBowString_Write(const LongBowString *string, FILE *fp)
{
    bool result = false;
    size_t nwrite = string->end;
    
    if (fwrite(string->buffer, sizeof(char), string->end, fp) == nwrite) {
        result = true;
    }
    
    return result;
}

LongBowArrayList *
longBowString_Tokenise(const char *string, const char *separators)
{
    LongBowArrayList *result = longBowArrayList_Create(longBowMemory_Deallocate);
    if (string != NULL) {
        char *workingCopy = longBowMemory_StringCopy(string);
        
        char *p = strtok(workingCopy, separators);
        while (p) {
            longBowArrayList_Add(result, longBowMemory_StringCopy(p));
            p = strtok(NULL, separators);
        }
        
        longBowMemory_Deallocate((void **) &workingCopy);
    }
    
    return result;
}
