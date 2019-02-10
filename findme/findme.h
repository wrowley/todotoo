/* Copyright (c) 2019 William Rowley
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. */

/* This header-only library is intended to help an executable find its own path
 * in the filesystem. It is not going to be robust to strange corner cases or
 * deliberate trickery. So far it supports:
 *    Windows : NO
 *    Linux   : YES
 *    FreeBSD : NO
 *    Solaris : NO
 */

#include <stdlib.h>
#include <unistd.h>

static inline
void
findme_bin_path
    (char     *p_bin_path
    ,unsigned  max_length
    )
{
    memset(p_bin_path, 0, max_length);
    if (readlink("/proc/self/exe", p_bin_path, max_length) == -1) abort();
}

static inline
void
findme_bin_dir
    (char     *p_bin_path
    ,unsigned  max_length
    )
{
    unsigned i = max_length - 1;
    findme_bin_path(p_bin_path, max_length);
    while (i > 0)
    {
        if (p_bin_path[i] == '/')
        {
            p_bin_path[i] = 0;
            break;
        }
        i--;
    }

}
