/*
  Copyright (C) 2006-2007 Werner Dittmann

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * Authors: Werner Dittmann <Werner.Dittmann@t-online.de>
 */

#include <libzrtpcpp/ZrtpPacketErrorAck.h>

ZrtpPacketErrorAck::ZrtpPacketErrorAck() {
    DEBUGOUT((fprintf(stdout, "Creating ErrorAck packet without data\n")));

    zrtpHeader = &data.hdr;	// the standard header

    setZrtpId();
    setLength((sizeof (ErrorAckPacket_t) / ZRTP_WORD_SIZE) - 1);
    setMessageType((uint8_t*)ErrorAckMsg);
}

ZrtpPacketErrorAck::ZrtpPacketErrorAck(uint8_t *data) {
    DEBUGOUT((fprintf(stdout, "Creating ErrorAck packet from data\n")));

    zrtpHeader = (zrtpPacketHeader_t *)&((ErrorAckPacket_t*)data)->hdr;	// the standard header
}

ZrtpPacketErrorAck::~ZrtpPacketErrorAck() {
    DEBUGOUT((fprintf(stdout, "Deleting ErrorAck packet: alloc: %x\n", allocated)));
}
