// [The "BSD licence"]
// Copyright (c) 2006-2007 Kay Roepke
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
#import "ANTLRDebugEventListener.h"
#import "ANTLRTreeAdaptor.h"
#import "ANTLRTreeNodeStream.h"

@interface ANTLRDebugTreeNodeStream : NSObject <ANTLRTreeNodeStream> {
	id<ANTLRDebugEventListener> debugListener;
	id<ANTLRTreeAdaptor> treeAdaptor;
	id<ANTLRTreeNodeStream> input;
	BOOL initialStreamState;
}

- (id) initWithTreeNodeStream:(id<ANTLRTreeNodeStream>)theStream debugListener:(id<ANTLRDebugEventListener>)debugger;

- (id<ANTLRDebugEventListener>) debugListener;
- (void) setDebugListener: (id<ANTLRDebugEventListener>) aDebugListener;

- (id<ANTLRTreeNodeStream>) input;
- (void) setInput: (id<ANTLRTreeNodeStream>) aTreeNodeStream;

- (id<ANTLRTreeAdaptor>) getTreeAdaptor;
- (void) setTreeAdaptor: (id<ANTLRTreeAdaptor>) aTreeAdaptor;

#pragma mark ANTLRTreeNodeStream conformance

- (id) LT:(NSInteger)k;
- (id<ANTLRTreeAdaptor>) getTreeAdaptor;
- (void) setUniqueNavigationNodes:(BOOL)flag;

#pragma mark ANTLRIntStream conformance
- (void) consume;
- (NSInteger) LA:(NSUInteger) i;
- (NSUInteger) mark;
- (NSUInteger) getIndex;
- (void) rewind:(NSUInteger) marker;
- (void) rewind;
- (void) release:(NSUInteger) marker;
- (void) seek:(NSUInteger) index;
- (NSUInteger) size;

@end
