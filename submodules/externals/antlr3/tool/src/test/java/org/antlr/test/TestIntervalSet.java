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
package org.antlr.test;

import org.antlr.analysis.Label;
import org.antlr.misc.IntervalSet;
import org.junit.Test;

import java.util.ArrayList;
import java.util.List;


public class TestIntervalSet extends BaseTest {

    /** Public default constructor used by TestRig */
    public TestIntervalSet() {
    }

    @Test public void testSingleElement() throws Exception {
        IntervalSet s = IntervalSet.of(99);
        String expecting = "99";
        assertEquals(s.toString(), expecting);
    }

    @Test public void testIsolatedElements() throws Exception {
        IntervalSet s = new IntervalSet();
        s.add(1);
        s.add('z');
        s.add('\uFFF0');
        String expecting = "{1, 122, 65520}";
        assertEquals(s.toString(), expecting);
    }

    @Test public void testMixedRangesAndElements() throws Exception {
        IntervalSet s = new IntervalSet();
        s.add(1);
        s.add('a','z');
        s.add('0','9');
        String expecting = "{1, 48..57, 97..122}";
        assertEquals(s.toString(), expecting);
    }

    @Test public void testSimpleAnd() throws Exception {
        IntervalSet s = IntervalSet.of(10,20);
        IntervalSet s2 = IntervalSet.of(13,15);
        String expecting = "13..15";
        String result = (s.and(s2)).toString();
        assertEquals(result, expecting);
    }

    @Test public void testRangeAndIsolatedElement() throws Exception {
        IntervalSet s = IntervalSet.of('a','z');
        IntervalSet s2 = IntervalSet.of('d');
        String expecting = "100";
        String result = (s.and(s2)).toString();
        assertEquals(result, expecting);
    }

	@Test public void testEmptyIntersection() throws Exception {
		IntervalSet s = IntervalSet.of('a','z');
		IntervalSet s2 = IntervalSet.of('0','9');
		String expecting = "{}";
		String result = (s.and(s2)).toString();
		assertEquals(result, expecting);
	}

	@Test public void testEmptyIntersectionSingleElements() throws Exception {
		IntervalSet s = IntervalSet.of('a');
		IntervalSet s2 = IntervalSet.of('d');
		String expecting = "{}";
		String result = (s.and(s2)).toString();
		assertEquals(result, expecting);
	}

    @Test public void testNotSingleElement() throws Exception {
        IntervalSet vocabulary = IntervalSet.of(1,1000);
        vocabulary.add(2000,3000);
        IntervalSet s = IntervalSet.of(50,50);
        String expecting = "{1..49, 51..1000, 2000..3000}";
        String result = (s.complement(vocabulary)).toString();
        assertEquals(result, expecting);
    }

	@Test public void testNotSet() throws Exception {
		IntervalSet vocabulary = IntervalSet.of(1,1000);
		IntervalSet s = IntervalSet.of(50,60);
		s.add(5);
		s.add(250,300);
		String expecting = "{1..4, 6..49, 61..249, 301..1000}";
		String result = (s.complement(vocabulary)).toString();
		assertEquals(result, expecting);
	}

	@Test public void testNotEqualSet() throws Exception {
		IntervalSet vocabulary = IntervalSet.of(1,1000);
		IntervalSet s = IntervalSet.of(1,1000);
		String expecting = "{}";
		String result = (s.complement(vocabulary)).toString();
		assertEquals(result, expecting);
	}

	@Test public void testNotSetEdgeElement() throws Exception {
		IntervalSet vocabulary = IntervalSet.of(1,2);
		IntervalSet s = IntervalSet.of(1);
		String expecting = "2";
		String result = (s.complement(vocabulary)).toString();
		assertEquals(result, expecting);
	}

    @Test public void testNotSetFragmentedVocabulary() throws Exception {
        IntervalSet vocabulary = IntervalSet.of(1,255);
        vocabulary.add(1000,2000);
        vocabulary.add(9999);
        IntervalSet s = IntervalSet.of(50,60);
        s.add(3);
        s.add(250,300);
        s.add(10000); // this is outside range of vocab and should be ignored
        String expecting = "{1..2, 4..49, 61..249, 1000..2000, 9999}";
        String result = (s.complement(vocabulary)).toString();
        assertEquals(result, expecting);
    }

    @Test public void testSubtractOfCompletelyContainedRange() throws Exception {
        IntervalSet s = IntervalSet.of(10,20);
        IntervalSet s2 = IntervalSet.of(12,15);
        String expecting = "{10..11, 16..20}";
        String result = (s.subtract(s2)).toString();
        assertEquals(result, expecting);
    }

    @Test public void testSubtractOfOverlappingRangeFromLeft() throws Exception {
        IntervalSet s = IntervalSet.of(10,20);
        IntervalSet s2 = IntervalSet.of(5,11);
        String expecting = "12..20";
        String result = (s.subtract(s2)).toString();
        assertEquals(result, expecting);

        IntervalSet s3 = IntervalSet.of(5,10);
        expecting = "11..20";
        result = (s.subtract(s3)).toString();
        assertEquals(result, expecting);
    }

    @Test public void testSubtractOfOverlappingRangeFromRight() throws Exception {
        IntervalSet s = IntervalSet.of(10,20);
        IntervalSet s2 = IntervalSet.of(15,25);
        String expecting = "10..14";
        String result = (s.subtract(s2)).toString();
        assertEquals(result, expecting);

        IntervalSet s3 = IntervalSet.of(20,25);
        expecting = "10..19";
        result = (s.subtract(s3)).toString();
        assertEquals(result, expecting);
    }

    @Test public void testSubtractOfCompletelyCoveredRange() throws Exception {
        IntervalSet s = IntervalSet.of(10,20);
        IntervalSet s2 = IntervalSet.of(1,25);
        String expecting = "{}";
        String result = (s.subtract(s2)).toString();
        assertEquals(result, expecting);
    }

    @Test public void testSubtractOfRangeSpanningMultipleRanges() throws Exception {
        IntervalSet s = IntervalSet.of(10,20);
        s.add(30,40);
        s.add(50,60); // s has 3 ranges now: 10..20, 30..40, 50..60
        IntervalSet s2 = IntervalSet.of(5,55); // covers one and touches 2nd range
        String expecting = "56..60";
        String result = (s.subtract(s2)).toString();
        assertEquals(result, expecting);

        IntervalSet s3 = IntervalSet.of(15,55); // touches both
        expecting = "{10..14, 56..60}";
        result = (s.subtract(s3)).toString();
        assertEquals(result, expecting);
    }

	/** The following was broken:
	 	{0..113, 115..65534}-{0..115, 117..65534}=116..65534
	 */
	@Test public void testSubtractOfWackyRange() throws Exception {
		IntervalSet s = IntervalSet.of(0,113);
		s.add(115,200);
		IntervalSet s2 = IntervalSet.of(0,115);
		s2.add(117,200);
		String expecting = "116";
		String result = (s.subtract(s2)).toString();
		assertEquals(result, expecting);
	}

    @Test public void testSimpleEquals() throws Exception {
        IntervalSet s = IntervalSet.of(10,20);
        IntervalSet s2 = IntervalSet.of(10,20);
        Boolean expecting = new Boolean(true);
        Boolean result = new Boolean(s.equals(s2));
        assertEquals(result, expecting);

        IntervalSet s3 = IntervalSet.of(15,55);
        expecting = new Boolean(false);
        result = new Boolean(s.equals(s3));
        assertEquals(result, expecting);
    }

    @Test public void testEquals() throws Exception {
        IntervalSet s = IntervalSet.of(10,20);
        s.add(2);
        s.add(499,501);
        IntervalSet s2 = IntervalSet.of(10,20);
        s2.add(2);
        s2.add(499,501);
        Boolean expecting = new Boolean(true);
        Boolean result = new Boolean(s.equals(s2));
        assertEquals(result, expecting);

        IntervalSet s3 = IntervalSet.of(10,20);
        s3.add(2);
        expecting = new Boolean(false);
        result = new Boolean(s.equals(s3));
        assertEquals(result, expecting);
    }

    @Test public void testSingleElementMinusDisjointSet() throws Exception {
        IntervalSet s = IntervalSet.of(15,15);
        IntervalSet s2 = IntervalSet.of(1,5);
        s2.add(10,20);
        String expecting = "{}"; // 15 - {1..5, 10..20} = {}
        String result = s.subtract(s2).toString();
        assertEquals(result, expecting);
    }

    @Test public void testMembership() throws Exception {
        IntervalSet s = IntervalSet.of(15,15);
        s.add(50,60);
        assertTrue(!s.member(0));
        assertTrue(!s.member(20));
        assertTrue(!s.member(100));
        assertTrue(s.member(15));
        assertTrue(s.member(55));
        assertTrue(s.member(50));
        assertTrue(s.member(60));
    }

    // {2,15,18} & 10..20
    @Test public void testIntersectionWithTwoContainedElements() throws Exception {
        IntervalSet s = IntervalSet.of(10,20);
        IntervalSet s2 = IntervalSet.of(2,2);
        s2.add(15);
        s2.add(18);
        String expecting = "{15, 18}";
        String result = (s.and(s2)).toString();
        assertEquals(result, expecting);
    }

    @Test public void testIntersectionWithTwoContainedElementsReversed() throws Exception {
        IntervalSet s = IntervalSet.of(10,20);
        IntervalSet s2 = IntervalSet.of(2,2);
        s2.add(15);
        s2.add(18);
        String expecting = "{15, 18}";
        String result = (s2.and(s)).toString();
        assertEquals(result, expecting);
    }

    @Test public void testComplement() throws Exception {
        IntervalSet s = IntervalSet.of(100,100);
        s.add(101,101);
        IntervalSet s2 = IntervalSet.of(100,102);
        String expecting = "102";
        String result = (s.complement(s2)).toString();
        assertEquals(result, expecting);
    }

	@Test public void testComplement2() throws Exception {
		IntervalSet s = IntervalSet.of(100,101);
		IntervalSet s2 = IntervalSet.of(100,102);
		String expecting = "102";
		String result = (s.complement(s2)).toString();
		assertEquals(result, expecting);
	}

	@Test public void testComplement3() throws Exception {
		IntervalSet s = IntervalSet.of(1,96);
		s.add(99,Label.MAX_CHAR_VALUE);
		String expecting = "97..98";
		String result = (s.complement(1,Label.MAX_CHAR_VALUE)).toString();
		assertEquals(result, expecting);
	}

    @Test public void testMergeOfRangesAndSingleValues() throws Exception {
        // {0..41, 42, 43..65534}
        IntervalSet s = IntervalSet.of(0,41);
        s.add(42);
        s.add(43,65534);
        String expecting = "0..65534";
        String result = s.toString();
        assertEquals(result, expecting);
    }

    @Test public void testMergeOfRangesAndSingleValuesReverse() throws Exception {
        IntervalSet s = IntervalSet.of(43,65534);
        s.add(42);
        s.add(0,41);
        String expecting = "0..65534";
        String result = s.toString();
        assertEquals(result, expecting);
    }

    @Test public void testMergeWhereAdditionMergesTwoExistingIntervals() throws Exception {
        // 42, 10, {0..9, 11..41, 43..65534}
        IntervalSet s = IntervalSet.of(42);
        s.add(10);
        s.add(0,9);
        s.add(43,65534);
        s.add(11,41);
        String expecting = "0..65534";
        String result = s.toString();
        assertEquals(result, expecting);
    }

	@Test public void testMergeWithDoubleOverlap() throws Exception {
		IntervalSet s = IntervalSet.of(1,10);
		s.add(20,30);
		s.add(5,25); // overlaps two!
		String expecting = "1..30";
		String result = s.toString();
		assertEquals(result, expecting);
	}

	@Test public void testSize() throws Exception {
		IntervalSet s = IntervalSet.of(20,30);
		s.add(50,55);
		s.add(5,19);
		String expecting = "32";
		String result = String.valueOf(s.size());
		assertEquals(result, expecting);
	}

	@Test public void testToList() throws Exception {
		IntervalSet s = IntervalSet.of(20,25);
		s.add(50,55);
		s.add(5,5);
		String expecting = "[5, 20, 21, 22, 23, 24, 25, 50, 51, 52, 53, 54, 55]";
		List foo = new ArrayList();
		String result = String.valueOf(s.toList());
		assertEquals(result, expecting);
	}

	/** The following was broken:
	    {'\u0000'..'s', 'u'..'\uFFFE'} & {'\u0000'..'q', 's'..'\uFFFE'}=
	    {'\u0000'..'q', 's'}!!!! broken...
	 	'q' is 113 ascii
	 	'u' is 117
	*/
	@Test public void testNotRIntersectionNotT() throws Exception {
		IntervalSet s = IntervalSet.of(0,'s');
		s.add('u',200);
		IntervalSet s2 = IntervalSet.of(0,'q');
		s2.add('s',200);
		String expecting = "{0..113, 115, 117..200}";
		String result = (s.and(s2)).toString();
		assertEquals(result, expecting);
	}

}
