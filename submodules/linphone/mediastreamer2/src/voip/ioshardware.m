/*
 ioshardware.m
 Copyright (C) 2013 Belledonne Communications, Grenoble, France
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */


#import "ioshardware.h"

#include <sys/types.h>
#include <sys/sysctl.h>


@implementation IOSHardware

+ (NSString *) platform {
	size_t size;
	sysctlbyname("hw.machine", NULL, &size, NULL, 0);
	char *machine = malloc(size);
	sysctlbyname("hw.machine", machine, &size, NULL, 0);
	NSString *platform = [NSString stringWithUTF8String:machine];
	free(machine);
	return platform;
}

+ (BOOL) isHDVideoCapable {
	if ([[IOSHardware platform] hasPrefix:@"iPad3"]) return TRUE;
	return FALSE;
}

+ (MSVideoSize) HDVideoSize:(const char *) deviceId {
	if ([[IOSHardware platform] isEqualToString:@"iPad3,4"]) {
		return MS_VIDEO_SIZE_720P;
	}
	return MS_VIDEO_SIZE_VGA;
}

+ (BOOL) isFrontCamera:(const char *) deviceId {
	if ([[NSString stringWithCString:deviceId encoding:[NSString defaultCStringEncoding]] hasSuffix:@"1"])
		return TRUE;
	return FALSE;
}

+ (BOOL) isBackCamera:(const char *) deviceId {
	if ([[NSString stringWithCString:deviceId encoding:[NSString defaultCStringEncoding]] hasSuffix:@"0"])
		return TRUE;
	return FALSE;
}

@end
