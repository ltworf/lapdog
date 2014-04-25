/*
 * lapdog - take actions on devices (dis)appearance on the LAN
 * Copyright (C) 2014  Salvo Tomaselli <tiposchi@tiscali.it>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <string>
#include <sstream>
#include <vector>

#include "split.h"

using namespace std;


/**
 * Similar to strtok_r, splits a string in tokens,
 * but limits the number of possible tokens and appends everything
 * to the last one.
 *
 * if l is 0, there will be no token limit
 *
 * Returns a vector of string with the various tokens.
 *
 * The vector will always contain at least l items, if the string
 * doesn't have enough to fill, they will be empty strings.
 **/
vector<string> split(istream & t, char s, size_t l) {
    string token;
    vector<string> tokens;

    while ((l>0?tokens.size() < l-1:true) && getline(t,token,s))
        tokens.push_back(token);
    if(getline(t,token,'\0'))
        tokens.push_back(token);
    while (tokens.size() < l)
        tokens.push_back(string(""));
    return tokens;
}

vector<string> split(const string & t, char s, size_t l) {
    istringstream streamed(t);
    return split(streamed,s,l);
}
