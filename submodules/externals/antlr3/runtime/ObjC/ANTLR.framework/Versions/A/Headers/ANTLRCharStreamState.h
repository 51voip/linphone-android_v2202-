//
//  ANTLRCharStreamState.h
//  ANTLR
//
// [The "BSD licence"]
// Copyright (c)  2010 Alan Condit
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

#import <Cocoa/Cocoa.h>


@interface ANTLRCharStreamState : NSObject
{
NSInteger p;
NSInteger line;
NSInteger charPositionInLine;
}

@property (getter=getP,setter=setP:) NSInteger p;
@property (getter=getLine,setter=setLine:) NSInteger line;
@property (getter=getCharPositionInLine,setter=setCharPositionInLine:) NSInteger charPositionInLine;

+ newANTLRCharStreamState;

- (id) init;

- (NSInteger) getP;
- (void) setP: (NSInteger) anIndex;

- (NSInteger) getLine;
- (void) setLine: (NSInteger) aLine;

- (NSInteger) getCharPositionInLine;
- (void) setCharPositionInLine:(NSInteger)aCharPositionInLine;

@end
