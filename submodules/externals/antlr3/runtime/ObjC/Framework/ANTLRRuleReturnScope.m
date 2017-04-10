//
//  ANTLRRuleReturnScope.m
//  ANTLR
//
//  Created by Alan Condit on 6/17/10.
// [The "BSD licence"]
// Copyright (c) 2010 Alan Condit
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#import "ANTLRRuleReturnScope.h"


@implementation ANTLRRuleReturnScope

/** Return the start token or tree */
- (id) getStart
{
    return nil;
}

/** Return the stop token or tree */
- (id) getStop
{
    return nil;
}

/** Has a value potentially if output=AST; */
- (id) getTree
{
    return nil;
}

/** Has a value potentially if output=template; Don't use StringTemplate
 *  type as it then causes a dependency with ST lib.
 */
- (id) getTemplate
{
    return nil;
}

// create a copy, including the text if available
// the input stream is *not* copied!
- (id) copyWithZone:(NSZone *)theZone
{
    ANTLRRuleReturnScope *copy = [[[self class] allocWithZone:theZone] init];
    return copy;
}

@end
