package org.antlr.runtime {
	
	/** A DFA implemented as a set of transition tables.
	 *
	 *  Any state that has a semantic predicate edge is special; those states
	 *  are generated with if-then-else structures in a specialStateTransition()
	 *  which is generated by cyclicDFA template.
	 *
	 *  There are at most 32767 states (16-bit signed short).
	 *  Could get away with byte sometimes but would have to generate different
	 *  types and the simulation code too.  For a point of reference, the Java
	 *  lexer's Tokens rule DFA has 326 states roughly.
	 */
	public class DFA {
		protected var eot:Array; // short[]
		protected var eof:Array; // short[]
		protected var min:Array; // char[]
	    protected var max:Array; // char[]
	    protected var accept:Array; //short[]
	    protected var special:Array; // short[]
	    protected var transition:Array; // short[][]
	
		protected var decisionNumber:int;
	
		/** Which recognizer encloses this DFA?  Needed to check backtracking */
		protected var recognizer:BaseRecognizer;
	
		private var _description:String;
		
		public static const debug:Boolean = false;

		public function DFA(recognizer:BaseRecognizer, decisionNumber:int, description:String,
							eot:Array, eof:Array, min:Array, max:Array, accept:Array, special:Array, transition:Array,
							specialStateTransitionFunction:Function = null, errorFunction:Function = null) {			
			this.recognizer = recognizer;
			this.decisionNumber = decisionNumber;
			this._description = description;
			
			this.eot = eot;
			this.eof = eof;
			this.min = min;
			this.max = max;
			this.accept = accept;
			this.special = special;
            this.transition = transition;
						
			if (specialStateTransitionFunction != null) {
				specialStateTransition = specialStateTransitionFunction;
			}
			
			if (errorFunction != null) {
				error = errorFunction;
			}
						
		}
		/** From the input stream, predict what alternative will succeed
		 *  using this DFA (representing the covering regular approximation
		 *  to the underlying CFL).  Return an alternative number 1..n.  Throw
		 *  an exception upon error.
		 */
		public function predict(input:IntStream):int	{
    		if ( debug ) {
    			trace("Enter DFA.predict for decision "+decisionNumber);
    		}

			var mark:int = input.mark(); // remember where decision started in input
			var s:int = 0; // we always start at s0
			try {
				while ( true ) {
					if ( debug ) trace("DFA "+decisionNumber+" state "+s+" LA(1)="+String.fromCharCode(input.LA(1))+"("+input.LA(1)+
													"), index="+input.index);
					var specialState:int = special[s];
					if ( specialState>=0 ) {
						if ( debug ) {
						    trace("DFA "+decisionNumber+
							" state "+s+" is special state "+specialState);
						}
						s = specialStateTransition(this, specialState,input);
						if ( debug ) {
						    trace("DFA "+decisionNumber+
							" returns from special state "+specialState+" to "+s);
					    }
						if ( s==-1 ) {
						    noViableAlt(s,input);
						    return 0;
					    }
						input.consume();
						continue;
					}
					if ( accept[s] >= 1 ) {
						if ( debug ) trace("accept; predict "+accept[s]+" from state "+s);
						return accept[s];
					}
					// look for a normal char transition
					var c:int = input.LA(1); // -1 == \uFFFF, all tokens fit in 65000 space
					if (c>=min[s] && c<=max[s]) {
						var snext:int = transition[s][c-min[s]]; // move to next state
						if ( snext < 0 ) {
							// was in range but not a normal transition
							// must check EOT, which is like the else clause.
							// eot[s]>=0 indicates that an EOT edge goes to another
							// state.
							if ( eot[s]>=0 ) {  // EOT Transition to accept state?
								if ( debug ) trace("EOT transition");
								s = eot[s];
								input.consume();
								// TODO: I had this as return accept[eot[s]]
								// which assumed here that the EOT edge always
								// went to an accept...faster to do this, but
								// what about predicated edges coming from EOT
								// target?
								continue;
							}
							noViableAlt(s,input);
							return 0;
						}
						s = snext;
						input.consume();
						continue;
					}
					if ( eot[s]>=0 ) {  // EOT Transition?
						if ( debug ) trace("EOT transition");
						s = eot[s];
						input.consume();
						continue;
					}
					if ( c==TokenConstants.EOF && eof[s]>=0 ) {  // EOF Transition to accept state?
						if ( debug ) trace("accept via EOF; predict "+accept[eof[s]]+" from "+eof[s]);
						return accept[eof[s]];
					}
					// not in range and not EOF/EOT, must be invalid symbol
					if ( debug ) {
						trace("min["+s+"]="+min[s]);
						trace("max["+s+"]="+max[s]);
						trace("eot["+s+"]="+eot[s]);
						trace("eof["+s+"]="+eof[s]);
						for (var p:int=0; p<transition[s].length; p++) {
							trace(transition[s][p]+" ");
						}
						trace();
					}
					noViableAlt(s,input);
					return 0;
				}
			}
			finally {
				input.rewindTo(mark);
			}
			// not reached -- added due to bug in Flex compiler reachability analysis of while loop with no breaks
			return -1;
		}
	
		protected function noViableAlt(s:int, input:IntStream):void {
			if (recognizer.state.backtracking>0) {
				recognizer.state.failed=true;
				return;
			}
			var nvae:NoViableAltException =
				new NoViableAltException(description,
										 decisionNumber,
										 s,
										 input);
			error(nvae);
			throw nvae;
		}
	
		/** A hook for debugging interface */
		public var error:Function = function(nvae:NoViableAltException):NoViableAltException { return nvae; }
	
		public var specialStateTransition:Function = function(dfa:DFA, s:int, input:IntStream):int {
			return -1;
		}
	
		public function get description():String {
			return _description;	
		}
	
		/** Given a String that has a run-length-encoding of some unsigned shorts
		 *  like "\1\2\3\9", convert to short[] {2,9,9,9}.  We do this to avoid
		 *  static short[] which generates so much init code that the class won't
		 *  compile. :(
		 */
		public static function unpackEncodedString(encodedString:String, unsigned:Boolean = false):Array {
			// walk first to find how big it is.
			/* Don't pre-allocate
			var size:int = 0;
			for (var i:int=0; i<encodedString.length; i+=2) {
				size += encodedString.charCodeAt(i);
			}
			*/
			var data:Array = new Array();
			var di:int = 0;
			for (var i:int=0; i<encodedString.length; i+=2) {
				var n:int = encodedString.charCodeAt(i);
				if (n > 0x8000) {
				    // need to read another byte
				    i++;
				    var lowBits:int = encodedString.charCodeAt(i);
				    n &= 0xff;
				    n <<= 8;
				    n |= lowBits;
				}
				var v:int = encodedString.charCodeAt(i+1);
				if (v > 0x8000) {
				    // need to read another byte
				    i++;
				    lowBits = encodedString.charCodeAt(i);
				    v &= 0xff;
				    v <<= 8;
				    v |= lowBits;
				}
				if (!unsigned && v > 0x7fff) {
				    v = -(0xffff - v + 1);
				}
				// add v n times to data
				for (var j:int=1; j<=n; j++) {
					data[di++] = v;
				}
			}
			return data;
		}
	
	}

}