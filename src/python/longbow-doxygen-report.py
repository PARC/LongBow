#! /usr/bin/env python
# Copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL XEROX OR PARC BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# ################################################################################
# #
# # PATENT NOTICE
# #
# # This software is distributed under the BSD 2-clause License (see LICENSE
# # file).  This BSD License does not make any patent claims and as such, does
# # not act as a patent grant.  The purpose of this section is for each contributor
# # to define their intentions with respect to intellectual property.
# #
# # Each contributor to this source code is encouraged to state their patent
# # claims and licensing mechanisms for any contributions made. At the end of
# # this section contributors may each make their own statements.  Contributor's
# # claims and grants only apply to the pieces (source code, programs, text,
# # media, etc) that they have contributed directly to this software.
# #
# # There is no guarantee that this section is complete, up to date or accurate. It
# # is up to the contributors to maintain their portion of this section and up to
# # the user of the software to verify any claims herein.
# #
# # Do not remove this header notification.  The contents of this section must be
# # present in all distributions of the software.  You may only modify your own
# # intellectual property statements.  Please provide contact information.
#
# - Palo Alto Research Center, Inc
# This software distribution does not grant any rights to patents owned by Palo
# Alto Research Center, Inc (PARC). Rights to these patents are available via
# various mechanisms. As of January 2016 PARC has committed to FRAND licensing any
# intellectual property used by its contributions to this software. You may
# contact PARC at cipo@parc.com for more information or visit http://www.ccnx.org
#
# @author Glenn Scott, Palo Alto Research Center (PARC)
# @copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.

import sys
import os
import pprint
import subprocess
import difflib
import csv
import argparse
sys.path.append("@INSTALL_PYTHON_DIR@")
sys.path.append("@DEPENDENCY_PYTHON_DIR@")
import LongBow

def concatenateContinuationLines(lines):
    '''
    Parse doxygen log lines.
    Lines that are indented by a space are continutations of the previous line.
    '''
    result = list()
    accumulator = ""
    for line in lines:
        line = line.rstrip()
        if line.startswith(" ") == False and line.startswith(" ") == False:
            if len(accumulator) > 0:
                result.append(accumulator)
            accumulator = line
        else:
            accumulator = accumulator + " " + line.lstrip()

    result.append(accumulator)

    return result

def parseLine(line):
    result = None
    if not line.startswith("<"):
        fields = line.split(":")
        if len(fields) >= 4:
            result = { "fileName" : fields[0].strip(),
                    "lineNumber" : int(fields[1].strip()),
                    "type" : "documentation",
                    "severity" : fields[2].strip(),
                    "message" : " ".join(fields[3:]).strip()}
        elif line.startswith("error"):
	          print line
        elif len(line) > 0:
          print "Consider using doxygen -s:", line

    return result

def canonicalize(lines):
    lines = concatenateContinuationLines(lines)
    parsedLines = map(lambda line: parseLine(line), lines)
    parsedLines = filter(lambda line: line != None, parsedLines)
    return parsedLines

def organize(entries):
    result = dict()

    for entry in entries:
        if not entry["fileName"] in result:
            result[entry["fileName"]] = dict()

        entryByFile = result[entry["fileName"]]

        if not str(entry["lineNumber"]) in entryByFile:
            entryByFile[str(entry["lineNumber"])] = list()
        if not entry in entryByFile[str(entry["lineNumber"])]:
            entryByFile[str(entry["lineNumber"])].append(entry)

    return result

def textualSummary(distribution, documentation):
    maxWidth = 0
    for entry in documentation:
        if len(entry) > maxWidth:
            maxWidth = len(entry)

    formatString ="%-" + str(maxWidth) + "s %8d %8d   %.2f%%"
    for entry in documentation:
        badLines = len(documentation[entry])
        totalLines =  LongBow.countLines(entry)
        score = float(totalLines - badLines) / float(totalLines) * 100.0
        LongBow.scorePrinter(distribution, score, formatString % (entry, totalLines, badLines, score))
    return

def textualAverage(distribution, documentation, format):
    sum = 0.0

    for entry in documentation:
        badLines = len(documentation[entry])
        totalLines =  LongBow.countLines(entry)
        score = float(totalLines - badLines) / float(totalLines) * 100.0
        sum = sum + score

    if len(documentation) == 0:
        averageScore = 100.0
    else:
        averageScore = sum / float(len(documentation))

    LongBow.scorePrinter(distribution, averageScore, format % averageScore)

def csvSummary(distribution, documentation):
        formatString ="documentation,%s,%d,%d,%.2f%%"
        for entry in documentation:
            badLines = len(documentation[entry])
            totalLines =  LongBow.countLines(entry)
            score = float(totalLines - badLines) / float(totalLines) * 100.0
            LongBow.scorePrinter(distribution, score, formatString % (entry, totalLines, badLines, score))
        return

def main(argv):
    parser = argparse.ArgumentParser(prog='longbow-doxygen-report', formatter_class=argparse.RawDescriptionHelpFormatter, description="")
    parser.add_argument('-l', '--doxygenlog', default=False, action="store", required=True, type=str, help="The doxygen output log to use.")
    parser.add_argument('-s', '--summary', default=False, action="store_true", required=False, help="Produce the score for each file")
    parser.add_argument('-a', '--average', default=False, action="store_true", required=False, help="Produce the simple average of all scores.")
    parser.add_argument('-d', '--distribution', default="[100, 95]", action="store", required=False, type=str, help="A list containing the score distributions for pretty-printing")
    parser.add_argument('-o', '--output', default="text", action="store", required=False, type=str, help="The required output format. text, csv")

    args = parser.parse_args()

    if not args.summary and not args.average:
        args.summary = True

    with open(args.doxygenlog, 'r') as f:
        lines = f.readlines()

    lines = canonicalize(lines)

    result = organize(lines)

    pp = pprint.PrettyPrinter(indent=4)
    #pp.pprint(result)

    distribution = eval(args.distribution)
    if args.summary:
        if args.output == "text":
            textualSummary(distribution, result)
        else:
            csvSummary(distribution, result)

    if args.average:
        textualAverage(distribution, result, "%.2f")


if __name__ == '__main__':
    '''
@(#) longbow-doxygen-report @VERSION@ @DATE@
@(#)   Copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).
@(#)   All Rights Reserved. Use is subject to license terms.
    '''
    main(sys.argv)
