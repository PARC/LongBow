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
 * @file longBow_ArrayList.h
 * @ingroup internals
 * @brief A simple, list implementation using a dynamic array.
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef LongBow_ARRAYLIST_H
#define LongBow_ARRAYLIST_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @struct longbow_array_list
 * @brief  A LongBow_ArrayList is a (dynamic) array of <code>void *</code> pointers;
 */
struct longbow_array_list;

/**
 * @typedef LongBowArrayList
 * @brief The struct longbow_array_list
 */
typedef struct longbow_array_list LongBowArrayList;

/**
 * Assert that a LongBowArrayList instance is valid.
 *
 * @param [in] array A pointer to a valid LongBowArrayList instance.
 *
 */
void longBowArrayList_AssertValid(const LongBowArrayList *array);
/**
 * Add a pointer to an element to the given LongBowArrayList.
 *
 * If the list was constructed with a destroyer,
 * the pointer will be destroyed when element is removed or the list is destroyed.
 *
 * @param [in] array A pointer to a LongBowArrayList instance.
 * @param [in] pointer An arbitrary value to store.
 *
 * @return The input array pointer.
 */
LongBowArrayList *longBowArrayList_Add(LongBowArrayList *array, const void *pointer);

/**
 * Remove an element at a specific index from an Array List.
 *
 * The element is destroyed via the function provided when calling <code>longBowArrayList_Create</code>.
 *
 * @param [in] array A pointer to a LongBowArrayList instance.
 * @param [in] index The index of the element to remove.
 *
 * @return A pointer to the modified LongBowArrayList.
 */
LongBowArrayList *longBowArrayList_RemoveAtIndex(LongBowArrayList *array, size_t index);

/**
 * Add an element at the index location. Elements will be moved up if required.
 * If the index is higher than the current Length the Array will be grown to that size
 *
 * @param [in] array A pointer to a LongBowArrayList instance.
 * @param [in] pointer An arbitrary value to store.
 * @param [in] index The position that the value will be stored after.
 * @return A pointer to the modified LongBowArrayList.
 */
LongBowArrayList *longBowArrayList_Add_AtIndex(LongBowArrayList *array, const void *pointer, size_t index);

/**
 * Create an instance of an empty LongBowArrayList.
 *
 * @param [in] destroyElement
 *      A pointer to a function that will destroy (or equivalent) the element pointed to by <code>element</code>
 * @return A pointer to a LongBowArrayList instance, or NULL if no memory could be allocated.
 */
LongBowArrayList *longBowArrayList_Create(void (*destroyElement)(void **elementAddress));

/**
 * Create an instance of a LongBowArrayList pre-provisioned to contain the specified number of elements.
 *
 * @param [in] size
 *      The number of initial elements to provision for.
 * @param [in] destroyElement
 *      A pointer to a function that will destroy (or equivalent) the element pointed to by <code>element</code>
 * @return A pointer to a LongBowArrayList instance, or NULL if no memory could be allocated.
 */
LongBowArrayList *longBowArrayList_Create_Capacity(void (*destroyElement)(void **elementAddress), size_t size);

/**
 * Get an array of void * pointers.
 * Return a pointer to an array of void * pointers contained in this Array List.
 * The returned value may be the actual backing array for the Array List.
 *
 * @param [in] array The LongBow_ArrayList
 * @return A pointer to an array of void * pointers contained in this Array List.
 *
 */
void **longBowArrayList_GetArray(const LongBowArrayList *array);

/**
 * Copy a LongBowArrayList instance.
 * Create a new LongBowArrayList instance with the same structure and content as the original.
 *
 * @param [in] array A pointer to a LongBowArrayList instance to copy.
 * @return A pointer to a LongBowArrayList instance with a copy of the original, or NULL if no memory could be allocated.
 */
LongBowArrayList *longBowArrayList_Copy(const LongBowArrayList *array);

/**
 * Destroy a LongBowArrayList instance.
 *
 * Destroy the given LongBowArrayList by freeing all memory used by it.
 *
 * @param [in,out] arrayPtr A pointer to a LongBowArrayList pointer.
 */
void longBowArrayList_Destroy(LongBowArrayList **arrayPtr);

/**
 * Get an element from the given list at a specified index.
 * The index must be 0 <= index < length.
 *
 * @return A pointer (void *) to the element in the list.
 *
 * @param [in] array A pointer to a LongBowArrayList instance.
 * @param [in] index The index of the required element.
 */
void *longBowArrayList_Get(const LongBowArrayList *array, size_t index);

/**
 * Return the number of elements in the given LongBowArrayList.
 *
 * @param [in] array A pointer to a LongBowArrayList instance.
 * @return A size_t of the number of elements in the given LongBowArrayList.
 */
size_t longBowArrayList_Length(const LongBowArrayList *array);


/**
 * Determine if two LongBowArrayList instances are equal.
 *
 * Two LongBowArrayList instances are equal if, and only if, they both contain the same pointers in the same order.
 *
 * The following equivalence relations on non-null `LongBowArrayList` instances are maintained:
 *
 *  * It is reflexive: for any non-null reference value x, `LongBowArrayList_Equals(x, x)`
 *      must return true.
 *
 *  * It is symmetric: for any non-null reference values x and y,
 *    `longBowArrayList_Equals(x, y)` must return true if and only if
 *        `longBowArrayList_Equals(y, x)` returns true.
 *
 *  * It is transitive: for any non-null reference values x, y, and z, if
 *        `longBowArrayList_Equals(x, y)` returns true and
 *        `longBowArrayList_Equals(y, z)` returns true,
 *        then  `longBowArrayList_Equals(x, z)` must return true.
 *
 *  * It is consistent: for any non-null reference values x and y, multiple
 *      invocations of `longBowArrayList_Equals(x, y)` consistently return true or
 *      consistently return false.
 *
 *  * For any non-null reference value x, `longBowArrayList_Equals(x, NULL)` must
 *      return false.
 *
 * @param a A pointer to a `LongBowArrayList` instance.
 * @param b A pointer to a `LongBowArrayList` instance.
 * @return true if the two `LongBowArrayList` instances are equal.
 *
 * Example:
 * @code
 * {
 *    LongBowArrayList *a = longBowArrayList_Create();
 *    LongBowArrayList *b = longBowArrayList_Create();
 *
 *    if (longBowArrayList_Equals(a, b)) {
 *        // true
 *    } else {
 *        // false
 *    }
 * }
 * @endcode
 */

bool longBowArrayList_Equals(const LongBowArrayList *a, const LongBowArrayList *b);

/**
 * Standard library free(3) wrapper for use as a destructor function for elements of a LongBowArrayList.
 *
 * The create functions for LongBowArrayList have an input parameter that is a pointer to a function that
 * will be called for each element when the Array List is destroyed, and when an element is removed via longBowArrayList_RemoveAtIndex.
 * This destroy function has a different calling signature than the standard library's free(3) function.
 * This function is a wrapper providing the correct facade for the standard library free(3) function.
 *
 * @param [in,out] element A pointer to the pointer to an element to be destroyed.
 */
void longBowArrayList_StdlibFreeFunction(void **element);

/**
 * Replace the first occurance of an existing element in the given LongBowArrayList.
 *
 * Paragraphs Of Explanation
 *
 * @param [in] array A pointer to a LongBowArrayList instance.
 * @param [in] old A pointer to an element in the list to replace.
 * @param [in] new A pointer to an element that will replace old.
 *
 * @return true If the element was found and replaced.
 * @return false If the element was not found.
 */
bool longBowArrayList_Replace(LongBowArrayList * array, const void *old, void *new);
#endif // LongBow_ARRAYLIST_H
