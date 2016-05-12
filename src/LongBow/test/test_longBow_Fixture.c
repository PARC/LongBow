/*
 * Copyright (c) 2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
#include <stdio.h>
#include <inttypes.h>
#include <LongBow/testing.h>
#include <LongBow/private/longBow_Memory.h>

LONGBOW_TEST_RUNNER(longBow_Fixture)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(CreateDestroy);
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Local);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(longBow_Fixture)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(longBow_Fixture)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(CreateDestroy)
{
    LONGBOW_RUN_TEST_CASE(CreateDestroy, LongBowTestFixture_Create_Destroy);
}

LONGBOW_TEST_FIXTURE_SETUP(CreateDestroy)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(CreateDestroy)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(CreateDestroy, LongBowTestFixture_Create_Destroy)
{
    uint64_t allocations = longBowMemory_OutstandingAllocations();
    LongBowTestRunner *runner = longBowTestRunner_Create("runner", NULL, NULL, NULL);

    LongBowTestFixture *fixture = longBowTestFixture_Create(runner, "fixture", NULL, NULL, NULL);
    assertNotNull(fixture, "Expected non-null result from LongBowTestFixture_Create");
    longBowTestFixture_Destroy(&fixture);
    longBowTestRunner_Destroy(&runner);
    assertTrue(longBowMemory_OutstandingAllocations() == allocations,
               "Memory leaks %" PRId64, longBowMemory_OutstandingAllocations());
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, longBowTestFixture_GetRunner);
    LONGBOW_RUN_TEST_CASE(Global, longBowTestFixture_GetClipBoard);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    LongBowTestRunner *runner = longBowTestRunner_Create("testRunner", NULL, NULL, NULL);
    LongBowTestFixture *fixture = longBowTestFixture_Create(runner, "testFixture", NULL, NULL, NULL);

    longBowClipBoard_Set(testClipBoard, "runner", runner);
    longBowClipBoard_Set(testClipBoard, "fixture", fixture);
    longBowTestCase_SetClipBoardData(testCase, fixture);
    
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    LongBowTestFixture *fixture = longBowTestCase_GetClipBoardData(testCase);

    LongBowTestRunner *runner = longBowTestFixture_GetRunner(fixture);
    
    longBowTestRunner_Destroy(&runner);
    longBowTestFixture_Destroy(&fixture);
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, longBowTestFixture_GetRunner)
{
    LongBowTestRunner *xrunner = longBowClipBoard_Get(testClipBoard, "runner");
    LongBowTestFixture *xfixture = longBowClipBoard_Get(testClipBoard, "fixture");
    LongBowTestFixture *fixture = longBowTestCase_GetClipBoardData(testCase);
    LongBowTestRunner *runner = longBowTestFixture_GetRunner(fixture);
    
    assertTrue(xrunner == runner, "Expected runner to be equal.");
    assertTrue(xfixture == fixture, "Expected runner to be equal.");

    assertNotNull(runner, "Expected the test runner to not be null");
}

LONGBOW_TEST_CASE(Global, longBowTestFixture_GetClipBoard)
{
    LongBowTestFixture *fixture = longBowTestCase_GetClipBoardData(testCase);
    LongBowClipBoard *clipboard = longBowTestFixture_GetClipBoard(fixture);
    assertNotNull(clipboard, "Expected non-null result from longBowTestFixture_GetClipBoard");
}

LONGBOW_TEST_FIXTURE(Local)
{
}

LONGBOW_TEST_FIXTURE_SETUP(Local)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Local)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

int
main(int argc, char *argv[])
{
    LongBowTestRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(longBow_Fixture);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
