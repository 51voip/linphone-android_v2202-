// [The "BSD licence"]
// Copyright (c) 2006-2007 Kay Roepke 2010 Alan Condit
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
#import "ANTLRRuntimeException.h"
#import "ANTLRToken.h"
#import "ANTLRIntStream.h"
#import "ANTLRTree.h"

@interface ANTLRRecognitionException : ANTLRRuntimeException {
	id<ANTLRIntStream> input;
	NSInteger index;
	id<ANTLRToken> token;
	id<ANTLRTree> node;
	unichar c;
	NSInteger line;
	NSInteger charPositionInLine;
}

@property (retain, getter=getStream, setter=setStream:) id<ANTLRIntStream> input;
@property (retain, getter=getToken, setter=setToken:) id<ANTLRToken>token;
@property (retain, getter=getNode, setter=setNode:) id<ANTLRTree>node;
@property (getter=getLine, setter=setLine:) NSInteger line;
@property (getter=getCharPositionInLine, setter=setCharPositionInLine:) NSInteger charPositionInLine;

+ (ANTLRRecognitionException *) newANTLRRecognitionException;
+ (ANTLRRecognitionException *) exceptionWithStream:(id<ANTLRIntStream>) anInputStream; 
- (id) init;
- (id) initWithStream:(id<ANTLRIntStream>)anInputStream;
- (id) initWithStream:(id<ANTLRIntStream>)anInputStream reason:(NSString *)aReason;
- (NSInteger) unexpectedType;
- (id<ANTLRToken>)getUnexpectedToken;

- (id<ANTLRIntStream>) getStream;
- (void) setStream: (id<ANTLRIntStream>) aStream;

- (id<ANTLRToken>) getToken;
- (void) setToken: (id<ANTLRToken>) aToken;

- (id<ANTLRTree>) getNode;
- (void) setNode: (id<ANTLRTree>) aNode;

- (NSString *)getMessage;

- (NSInteger)getCharPositionInLine;
- (void)setCharPositionInLine:(NSInteger)aPos;

@end
