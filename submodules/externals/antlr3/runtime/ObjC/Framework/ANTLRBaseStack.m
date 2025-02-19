//
//  ANTLRBaseRecognizer.m
//  ANTLR
//
//  Created by Alan Condit on 6/16/10.
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

#define SUCCESS (0)
#define FAILURE (-1)

#import "ANTLRBaseStack.h"
#import "ANTLRTree.h"

/*
 * Start of ANTLRBaseStack
 */
@implementation ANTLRBaseStack

@synthesize LastHash;

+(ANTLRBaseStack *)newANTLRBaseStack
{
    return [[ANTLRBaseStack alloc] init];
}

+(ANTLRBaseStack *)newANTLRBaseStackWithLen:(NSInteger)cnt
{
    return [[ANTLRBaseStack alloc] initWithLen:cnt];
}

-(id)init
{
	self = [super initWithLen:HASHSIZE];
	if ( self != nil ) {
	}
    return( self );
}

-(id)initWithLen:(NSInteger)cnt
{
	self = [super initWithLen:cnt];
    if ( self != nil ) {
	}
    return( self );
}

- (void)dealloc
{
#ifdef DEBUG_DEALLOC
    NSLog( @"called dealloc in ANTLRBaseStack" );
#endif
	[super dealloc];
}

- (id) copyWithZone:(NSZone *)aZone
{
    ANTLRBaseStack *copy;
    
    copy = [super copyWithZone:aZone];
    return copy;
}

- (NSUInteger)count
{
    NSUInteger aCnt = 0;
    
    for (int i = 0; i < BuffSize; i++) {
        if (ptrBuffer[i] != nil) {
            aCnt++;
        }
    }
    return aCnt;
}

- (NSUInteger) size
{
    return BuffSize;
}

-(void)deleteANTLRBaseStack:(ANTLRBaseStack *)np
{
    id tmp, rtmp;
    NSInteger idx;
    
    if ( self.fNext != nil ) {
        for( idx = 0; idx < BuffSize; idx++ ) {
            tmp = (ANTLRLinkBase *)ptrBuffer[idx];
            while ( tmp ) {
                rtmp = tmp;
                tmp = [tmp getfNext];
                [rtmp release];
            }
        }
    }
}

- (NSInteger)getLastHash
{
    return LastHash;
}

- (void)setLastHash:(NSInteger)aVal
{
    LastHash = aVal;
}

@end
