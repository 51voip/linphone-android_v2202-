/*
 * [The "BSD license"]
 *  Copyright (c) 2010 Terence Parr
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 *  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
package org.antlr.tool;

import org.stringtemplate.v4.ST;

/** The ANTLR code calls methods on ErrorManager to report errors etc...
 *  Rather than simply pass these arguments to the ANTLRErrorListener directly,
 *  create an object that encapsulates everything.  In this way, the error
 *  listener interface does not have to change when I add a new kind of
 *  error message.  I don't want to break a GUI for example every time
 *  I update the error system in ANTLR itself.
 *
 *  To get a printable error/warning message, call toString().
 */
public abstract class Message {
	// msgST is the actual text of the message
	public ST msgST;
	// these are for supporting different output formats
	public ST locationST;
	public ST reportST;
	public ST messageFormatST;

	public int msgID;
	public Object arg;
	public Object arg2;
	public Throwable e;
	// used for location template
	public String file;
	public int line = -1;
	public int column = -1;

	public Message() {
	}

	public Message(int msgID) {
		this(msgID, null, null);
	}

	public Message(int msgID, Object arg, Object arg2) {
		setMessageID(msgID);
		this.arg = arg;
		this.arg2 = arg2;
	}

	public void setLine(int line) {
		this.line = line;
	}

	public void setColumn(int column) {
		this.column = column;
	}

	public void setMessageID(int msgID) {
		this.msgID = msgID;
		msgST = ErrorManager.getMessage(msgID);
	}

	/** Return a new template instance every time someone tries to print
	 *  a Message.
	 */
	public ST getMessageTemplate() { return new ST(msgST); }

	/** Return a new template instance for the location part of a Message.
	 *  TODO: Is this really necessary? -Kay
	 */
	public ST getLocationTemplate() { return new ST(locationST); }

	public String toString(ST messageST) {
		// setup the location
		locationST = ErrorManager.getLocationFormat();
		reportST = ErrorManager.getReportFormat();
		messageFormatST = ErrorManager.getMessageFormat();
		boolean locationValid = false;
		if (line != -1) {
			locationST.add("line", line);
			locationValid = true;
		}
		if (column != -1) {
			locationST.add("column", column+1);
			locationValid = true;
		}
		if (file != null) {
			locationST.add("file", file);
			locationValid = true;
		}

		messageFormatST.add("id", msgID);
		messageFormatST.add("text", messageST);

		if (locationValid) {
			reportST.add("location", locationST);
		}
		reportST.add("message", messageFormatST);
		reportST.add("type", ErrorManager.getMessageType(msgID));

		return reportST.render();
	}
}
