/*

Copyright (c) 2013 Tilo Szepan, Immo Wache <https://github.com/tilos/AtMegaWebServer.git>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

// The DEBUG flag will enable serial console logging in this library and is 
// defined in global.h
//
// There is an overall size increase of about 5100 bytes in code size
// when the debugging is enabled
// on Arduino UNO is not enough memory for DEBUG informations, 
// set the flag in global.h to 0
#include "global.h"

#include <SPI.h>
#include <Ethernet.h>
#include <Flash.h>
#include <SdFat.h>
#include <AtMegaWebServer.h>
#include <UdpServices.h>



AtMegaWebServer::PathHandler handlers[] = {
  {"/" "*", AtMegaWebServer::GET, &WebServerHandler::get_handler},
  {"/" "*", AtMegaWebServer::PUT, &WebServerHandler::put_handler},
  {"/" "*", AtMegaWebServer::DELETE, &WebServerHandler::delete_handler},
  {NULL}
};

const char* headers[] = {
  "Content-Length",
  NULL
};

AtMegaWebServer web = AtMegaWebServer(handlers, headers);

static uint8_t mac[] = {
  0x90, 0xA2, 0xDA, 0x0D, 0x97, 0x1A };


void setup() {
  // initialize file system ( SdFat )
   WebServerHandler::init();

   // Initialize the Ethernet.
  Ethernet.begin(mac);

  // Start the web server.
  web.begin();

  //  get the current time/date for the file system via NTP, start the clock
  UdpServices::begin();
  UdpServices::maintainTime();
  SdBaseFile::dateTimeCallback(&UdpServices::dateTime);
}

void loop() {
  web.processRequest();
  UdpServices::maintainTime();
}


