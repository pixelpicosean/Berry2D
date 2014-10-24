//////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2008-2014, Shane Liesegang
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the copyright holder nor the names of any
//       contributors may be used to endorse or promote products derived from
//       this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//////////////////////////////////////////////////////////////////////////////

/**
 * @file StringUtil.h
 * A set of C-style utility functions for manipulating strings.
 */
#pragma once

#include <string>
#include <vector>
#include <set>
#include <map>

//Typedefs to let us reference things more sanely
typedef std::string			     String;
typedef std::vector<String>	     StringList;
typedef std::set<String>	     StringSet;
typedef std::map<String, String> StringMap;

/**
 * Convert a string to an integer.
 *
 * @param s The string to convert
 * @return The int representation thereof (0 if the string couldn't be parsed)
 */
int stringToInt(const String& s);

/**
 * Convert a string to a float.
 *
 * @param s The string to convert
 * @return The float representation thereof (0.0f if the string couldn't be
 *   parsed)
 */
float stringToFloat(const String& s);

/**
 * Convert a string to a bool. First looks to see if the string is "true" or
 *  "false" with any capitalization. If not, then it tries to convert it to
 *  an integer and uses that as the result (0 being false; any other value
 *  being true).
 *
 * @param s The string to convert
 * @return The bool representation thereof
 */
bool stringToBool(const String& s);

/**
 * Converts an integer to a string.
 *
 * @param val The integer to convert
 * @return The string representation of the integer
 */
String intToString(int val);

/**
 * Converts a float to a string.
 *
 * @param val The float to convert
 * @return The string representation of the float
 */
String floatToString(float val);


/**
 * Converts a bool to a string.
 *
 * @param val The bool to convert
 * @return The string representation of the bool (in numeric form)
 */
String boolToString(bool val);

/**
 * Converts a string to all uppercase
 *
 * @param s The string to convert
 * @return The transformed string
 */
String toUpper(const String& s);

/**
 * Converts a string to all lowercase
 *
 * @param s The string to convert
 * @return The transformed string
 */
String toLower(const String& s);

/**
 * Joins a list of strings into one, with a specified separator.
 *
 * @param The string list to join
 * @param The string to put between each entry in the final string.
 * @return The joined string.
 */
String joinString(const StringList &list, const String& joinString);

/**
 * Splits a long string into a StringList.
 *
 * @param splitMe The string to split up
 * @param splitChars The set of delimiter characters to use for the splitting
 * @param bRemoveEmptyEntries Whether or not empty strings (resulting from
 *   multiple delimiters) should be added to the return list
 * @return The list of strings derived from the split
 */
StringList splitString(const String& splitMe, const String& splitChars, bool bRemoveEmptyEntries = true);

/**
 * Splits a long string into a StringList wherever newlines or tabs appear.
 *
 * @param splitMe The string to split
 * @return The list of strings derived from the split
 */
StringList splitString(const String& splitMe);

/**
 * Remove a series of duplicated characters from a string.
 *
 * @param trimMe The string to trim
 * @param trimChars The characters to remove
 * @return The trimmed string
 */
String trimString(const String& trimMe, const String& trimChars);

/**
 * Remove whitespace from the beginning or end of a string
 *
 * @param trimMe The string to trim
 * @return The trimmed string
 */
String trimString(const String& trimMe);
