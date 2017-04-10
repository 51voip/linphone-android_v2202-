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
#import "ANTLRBaseRecognizer.h"
#import "ANTLRCharStream.h"
#import "ANTLRNoViableAltException.h"

@interface ANTLRDFA : NSObject {
	// the tables are set by subclasses to their own static versions.
	const int *eot;
	const int *eof;
	const unichar *min;
	const unichar *max;
	const int *accept;
	const int *special;
	const int **transition;
	
	ANTLRBaseRecognizer *recognizer;
	NSInteger decisionNumber;
    NSInteger len;
}

@property (retain, getter=getRecognizer,setter=setRecognizer:) ANTLRBaseRecognizer *recognizer;
@property (assign, getter=getDecision,setter=setDecision:) NSInteger decisionNumber;
@property (assign, getter=getLen,setter=setLen:) NSInteger len;

- (id) initWithRecognizer:(id) theRecognizer;
// simulate the DFA using the static tables and predict an alternative
- (NSInteger) predict:(id<ANTLRCharStream>)anInput;
- (void) noViableAlt:(NSInteger)state Stream:(id<ANTLRIntStream>)anInput;

- (NSInteger) specialStateTransition:(NSInteger)state Stream:(id<ANTLRIntStream>)anInput;
// - (NSInteger) specialStateTransition:(NSInteger) state;
//- (unichar) specialTransition:(unichar) state symbol:(NSInteger) symbol;

// hook for debugger support
- (void) error:(ANTLRNoViableAltException *)nvae;

- (NSString *) description;
- (BOOL) evaluateSyntacticPredicate:(SEL)synpredFragment;

+ (void) setIsEmittingDebugInfo:(BOOL) shouldEmitDebugInfo;

- (NSInteger)getDecision;
- (void)setDecision:(NSInteger)aDecison;

- (ANTLRBaseRecognizer *)getRecognizer;
- (void)setRecognizer:(ANTLRBaseRecognizer *)aRecognizer;
- (NSInteger)length;

@end
