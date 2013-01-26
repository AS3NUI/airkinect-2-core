//
//  AKOpenNIHardware.cpp
//  KinectExtension
//
//  Created by Wouter Verweirder on 15/10/12.
//  Based on ofxHardwareDriver from ofxOpenNI:
/*
 * ofxHardwareDriver.cpp
 *
 * Copyright 2011 (c) Matthew Gingold http://gingold.com.au
 * Originally forked from a project by roxlu http://www.roxlu.com/
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "AKOpenNIHardware.h"

#ifdef AIRKINECT_TARGET_OPENNI
#ifdef AIRKINECT_OS_OSX

AKOpenNIHardware::AKOpenNIHardware(OpenNIDevice* device)
{
    _device = device;
    dev = 0;
    isKinect = false;
}

AKOpenNIHardware::~AKOpenNIHardware()
{
    shutDown();
}

void AKOpenNIHardware::setup(int index)
{
	//libusb_context *ctx;
	libusb_init(&ctx);
	libusb_device **devs; //pointer to pointer of device, used to retrieve a list of devices
	ssize_t cnt = libusb_get_device_list (ctx, &devs); //get the list of devices
	if (cnt < 0)
	{
        _device->dispatchErrorMessage((const uint8_t*) "No device on USB");
		return;
	}
	
	int nr_mot(0);
	for (int i = 0; i < cnt; ++i)
	{
		struct libusb_device_descriptor desc;
		const int r = libusb_get_device_descriptor (devs[i], &desc);
		if (r < 0)
			continue;
		
		// Search for the aux
		if (desc.idVendor == MS_MAGIC_VENDOR && desc.idProduct == MS_MAGIC_MOTOR_PRODUCT)
		{
			// If the index given by the user matches our camera index
			if (nr_mot == index)
			{
				if ((libusb_open (devs[i], &dev) != 0) || (dev == 0))
				{
                    char * buffer = new char[128];
                    sprintf(buffer, "Cannot open aux %d", index);
                    _device->dispatchErrorMessage((const uint8_t *)buffer);
                    delete buffer;
					return;
				}
				// Claim the aux
                char * buffer = new char[128];
                sprintf(buffer, "Openning device aux %d on 0", i);
                _device->dispatchInfoMessage((const uint8_t *)buffer);
                delete buffer;
				libusb_claim_interface (dev, 0);
                isKinect = true;
				break;
			}
			else
				nr_mot++;
		}
	}
	
	libusb_free_device_list (devs, 1);  // free the list, unref the devices in it
	
	// capture current tilt angle (all good if we don't move the kinect)
	update();
    
    tilt_angle = getTiltAngle();
    if(tilt_angle < -30) tilt_angle = -30;
    if(tilt_angle > 30) tilt_angle = 30;
}

void AKOpenNIHardware::update()
{
    if(isKinect)
    {
        //freenect_context *ctx = dev->parent;
        uint8_t buf[10];
        uint16_t ux, uy, uz;
        
        int ret = libusb_control_transfer(dev, 0xC0, 0x32, 0x0, 0x0, buf, 10, 10);
        if (ret != 10) {
            char * buffer = new char[128];
            sprintf(buffer, "Error in accelerometer reading, libusb_control_transfer returned %d", ret);
            _device->dispatchErrorMessage((const uint8_t *)buffer);
            delete buffer;
            return;// ret < 0 ? ret : -1;
        }
        
        ux = ((uint16_t)buf[2] << 8) | buf[3];
        uy = ((uint16_t)buf[4] << 8) | buf[5];
        uz = ((uint16_t)buf[6] << 8) | buf[7];
        
        //the documentation for the accelerometer (http://www.kionix.com/Product%20Sheets/KXSD9%20Product%20Brief.pdf)
        //states there are 819 counts/g
        tilt_state.accelerometer_x = (int16_t)ux/FREENECT_COUNTS_PER_G*GRAVITY;
        tilt_state.accelerometer_y = (int16_t)uy/FREENECT_COUNTS_PER_G*GRAVITY;
        tilt_state.accelerometer_z = (int16_t)uz/FREENECT_COUNTS_PER_G*GRAVITY;
        tilt_state.tilt_angle = (int)buf[8];
        tilt_state.tilt_status = (tilt_status_code)buf[9];
        
        //return ret;
    }
}

void AKOpenNIHardware::setTiltAngle(int angle)
{
    if(isKinect)
    {
        if(angle < -30) angle = -30;
        if(angle > 30) angle = 30;
        
        uint8_t empty[0x1];
        
        angle = (angle<MIN_TILT_ANGLE) ? MIN_TILT_ANGLE : ((angle>MAX_TILT_ANGLE) ? MAX_TILT_ANGLE : angle);
        angle = angle * 2;
        const int ret = libusb_control_transfer(dev, 0x40, 0x31, (uint16_t)angle, 0x0, empty, 0x0, 0);
        if (ret != 0)
        {
            char * buffer = new char[128];
            sprintf(buffer, "Error in setting tilt angle, libusb_control_transfer returned %i", ret);
            _device->dispatchErrorMessage((const uint8_t *)buffer);
            delete buffer;
            return;
        }
        
        //tilt_angle = angle;
    }
}

int AKOpenNIHardware::getTiltAngle() {
    if(isKinect)
    {
        return tilt_state.tilt_angle/2;
    }
    return 0;
}

void AKOpenNIHardware::setLedOption(uint16_t option)
{
    if(isKinect)
    {
        uint8_t empty[0x1];
        
        const int ret = libusb_control_transfer(dev, 0x40, 0x06, (uint16_t)option, 0x0, empty, 0x0, 0);
        if (ret != 0)
        {
            char * buffer = new char[128];
            sprintf(buffer, "Error in setting led, libusb_control_transfer returned %i", ret);
            _device->dispatchErrorMessage((const uint8_t *)buffer);
            delete buffer;
        }
    }
}

void AKOpenNIHardware::shutDown() {
    if(isKinect && dev != 0)
    {
        libusb_close(dev);
        libusb_release_interface(dev, 0);
    }
	libusb_exit(ctx);
}

#endif
#endif