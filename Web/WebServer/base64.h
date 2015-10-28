/* base64.h -- Encode binary data using printable characters.
   Copyright (C) 2004-2006, 2009-2012 Free Software Foundation, Inc.
   Written by Simon Josefsson.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, see <http://www.gnu.org/licenses/>.  */

#ifndef BASE64_H
# define BASE64_H

extern void base64_encode (const char *in,unsigned int inlen,
               char *out, unsigned int outlen);

#endif /* BASE64_H */
