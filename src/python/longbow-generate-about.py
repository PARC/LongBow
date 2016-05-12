#! /usr/bin/env python
# Copyright (c) 2014-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
# @copyright (c) 2014-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
import os
import sys
import string
import datetime
import argparse
sys.path.append("@INSTALL_PYTHON_DIR@")
sys.path.append("@DEPENDENCY_PYTHON_DIR@")
import FileUtil

whatLineToken = "@(#)"

def translateCCharacter(character):
    result = character

    if character == '\t':
        result = "\\t"
    elif character == "\n":
        result = "\\n"
    elif character == "\"":
        result = "\\\""
    elif character == "\'":
        result = "\\'"

    return result


def makeWhatLine(line):
    return "@(#)" + line

def createCString(string):
    if string is None:
        result = "None"
    else:
        result = "".join(map(lambda character: translateCCharacter(character), string))
    return result

def createQuotedCString(string):
  	return "\"%s\"" % createCString(string)

def cIdentifier(name):
    translation = string.maketrans("-!@#$%^&*()_-+=[]{}|;:<>,./?", "____________________________")
    return name.translate(translation)


def validateArgument(arg):
    '''
    If the given parameter is equal to '-' return None, otherwise return the parameter.
    '''
    if arg == "-":
        return None
    return arg


class LongBowGenerateAboutHFile:
    def __init__(self, prefix):
        self.prefix = prefix
        return

    def headerdocFunction(self, functionName, OneLineDescription, returns):
    	result = "/**\n"
        result += " * %s\n" % OneLineDescription
        result += " *\n"
        result += " * @return %s\n" % returns
        result += " */\n"
        return result

    def FileName(self):
        return self.prefix + "_About.h"

    def Name(self):
        functionName = "%sAbout_Name" % self.prefix
        result = self.headerdocFunction(functionName, "Return the name as a C string.", "The name as a C string.")
        result += "const char *%s(void);\n" % functionName
        return result

    def Version(self):
        functionName = "%sAbout_Version" % self.prefix
        result = self.headerdocFunction(functionName, "Return the version as a C string.", "The version as a C string.")
        result += "const char *%s(void);\n" % functionName
        return result

    def About(self):
        functionName = "%sAbout_About" % self.prefix
        result = self.headerdocFunction(functionName, "Return the About text as a C string.", "The About text as a C string.")
        result += "const char *%s(void);\n" % functionName
        return result

    def MiniNotice(self):
        functionName = "%sAbout_MiniNotice" % self.prefix
        result = self.headerdocFunction(functionName,
                                "Return the minimum copyright notice as a C string.",
                                "The minimum copyright notice as a C string.")
        result += "const char *%s(void);\n" % functionName
        return result

    def ShortNotice(self):
        functionName = "%sAbout_ShortNotice" % self.prefix
        result = self.headerdocFunction(functionName,
                                "Return the short copyright notice as a C string.",
                                "The short copyright notice as a C string.")
        result += "const char *%s(void);\n" % functionName
        return result

    def LongNotice(self):
        functionName = "%sAbout_LongNotice" % self.prefix
        result = self.headerdocFunction(functionName,
                                "Return the long copyright notice as a C string.",
                                "The long copyright notice as a C string.")
        result += "const char *%s(void);\n" % functionName
        return result

    def WhatString(self):
    	result = "/**\n"
        result += " * Embedded string containing information for the what(1) command.\n"
        result += " *\n"
        result += " */\n"
        result += "extern const char *%s_What;\n" % (self.prefix)
        return result

    def __str__(self):
        result =  "// DO NOT EDIT THIS FILE.  IT IS AUTOMATICALLY GENERATED.\n"
        result += "// longbow-generate-about @VERSION@ @DATE@\n\n"
        result += "#ifndef %s_About_h\n" % (self.prefix)
        result += "#define %s_About_h\n" % (cIdentifier(self.prefix))
        result += self.WhatString() + "\n"
        result += self.Name() + "\n"
        result += self.Version() + "\n"
        result += self.About() + "\n"
        result += self.MiniNotice() + "\n"
        result += self.ShortNotice() + "\n"
        result += self.LongNotice() + "\n"
        result += "#endif // %s_About_h\n" % (cIdentifier(self.prefix))
        return result

    def writeFile(self):
        with open(self.FileName(), "w") as myfile:
            myfile.write(str(self))
        return

class LongBowGenerateAboutCFile:
    def __init__(self, args):
        self.prefix = args.prefix
        self.name = args.name
        self.version = validateArgument(args.version)
        self.miniNotice = ""
        self.shortNotice = ""
        self.longNotice = ""
        self.about = None
        self.what = None

        self.args = args

        self.miniNotice = FileUtil.readFileString(args.miniNotice)
        self.shortNotice = FileUtil.readFileString(args.shortNotice)
        self.longNotice = FileUtil.readFileString(args.longNotice)

        self.buildDate = datetime.datetime.utcnow().isoformat()

        if self.version == None:
            self.version = " RELEASE_VERSION "

        if self.about == None:
            self.about = createQuotedCString("%s " % (self.name)) + \
            	self.version + \
            	createQuotedCString(" %s" % (self.buildDate)) + " " + \
            	createQuotedCString("\n%s" % (self.miniNotice))

        if self.what == None:
            if self.miniNotice != None:
                notice = "\n".join(map(lambda line: "\t" + line, self.miniNotice.split("\n")[:-1]))
            else:
                notice = ""
            self.what = createQuotedCString(whatLineToken) + " " + \
						createQuotedCString(self.name + " ") + " " + \
					 	self.version + " " + \
						createQuotedCString(" " + self.buildDate) + "\n" + \
						createQuotedCString(whatLineToken) + " " + \
					    createQuotedCString(notice)
        return

    def FileName(self):
        return self.prefix + "_About.c"

    def Name(self):
        functionName = "%sAbout_Name" % self.prefix
        return self.boilerPlateFunction(functionName, createQuotedCString(self.name))

    def Version(self):
        functionName = "%sAbout_Version" % self.prefix
        return self.boilerPlateFunction(functionName, self.version)

    def About(self):
        functionName = "%sAbout_About" % self.prefix
        return self.boilerPlateFunction(functionName, self.about)

    def MiniNotice(self):
        functionName = "%sAbout_MiniNotice" % self.prefix
        return self.boilerPlateFunction(functionName, createQuotedCString(self.miniNotice))

    def ShortNotice(self):
        functionName = "%sAbout_ShortNotice" % self.prefix
        return self.boilerPlateFunction(functionName, createQuotedCString(self.shortNotice))

    def LongNotice(self):
        functionName = "%sAbout_LongNotice" % self.prefix
        return self.boilerPlateFunction(functionName, createQuotedCString(self.longNotice))

    def WhatString(self):
        return "const char *%s_What = %s;\n" % (self.prefix, self.what)

    def boilerPlateFunction(self, functionName, string):
        result = "const char *\n%s(void)\n" % functionName
        result += "{\n"
        result += "    return %s;\n" % string
        result += "}\n"
        return result

    def __str__(self):
        result =  "// DO NOT EDIT THIS FILE.  IT IS AUTOMATICALLY GENERATED.\n"
        result += "// longbow-generate-about @VERSION@ @DATE@\n\n"
        result += "#include \"%s_About.h\"\n\n" % self.prefix
        result += self.WhatString() + "\n"
        result += self.Name() + "\n"
        result += self.Version() + "\n"
        result += self.About() + "\n"
        result += self.MiniNotice() + "\n"
        result += self.ShortNotice() + "\n"
        result += self.LongNotice() + "\n"
        return result

    def writeFile(self):
        with open(self.FileName(), "w") as myfile:
            myfile.write(str(self))
        return

if __name__ == '__main__':
    desc = '''
@(#) longbow-generate-about @VERSION@ @DATE@
@(#)   Copyright (c) 2014-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).
@(#)   All Rights Reserved. Use is subject to license terms.

Generate C code conforming to the About contract.

Create a .c and .h file pair with the specified prefix.
For the prefix 'xyzzy', the file names are 'xyzzy_About.c' and 'xyzzy_About.h' respectively.

The functions defined are:

const char *xyzzyAbout_Name(void)
const char *xyzzyAbout_Version(void)
const char *xyzzyAbout_About(void)
const char *xyzzyAbout_MiniNotice(void)
const char *xyzzyAbout_ShortNotice(void)
const char *xyzzyAbout_LongNotice(void)

And the constant string const char *xyzzy_What;
    '''

    parser = argparse.ArgumentParser(prog='longbow-generate-about', formatter_class=argparse.RawDescriptionHelpFormatter, description=desc)

    parser.add_argument("prefix", help="The file name and function name prefix.")
    parser.add_argument("name", help="The name of the entity this is about.")
    parser.add_argument("version", help="The version of the entity this is about.")
    parser.add_argument("miniNotice", help="The name of the file containing the smallest copyright or attribution notice.")
    parser.add_argument("shortNotice", help="The name of the file containing a short copyright or attribution notice.")
    parser.add_argument("longNotice", help="The name of the file containing a full copyright or attribution notice.")

    args = parser.parse_args()

    hfile = LongBowGenerateAboutHFile(args.prefix)
    hfile.writeFile()

    cfile = LongBowGenerateAboutCFile(args)
    cfile.writeFile()
