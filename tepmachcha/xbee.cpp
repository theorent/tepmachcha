#include "tepmachcha.h"

// XBee is turned on:
//   For 1 hour after boot
//   For 1 hour after receiving XBee SMS
//   During the XBee window, daily
//
//  We maintain a power state flag, and whenever we set it, we set a shutoff hour/minute

boolean xBeeState = HIGH;          // XBee power state, default off (HIGH)
uint8_t xBeeShutoffHour = 0;       // Hour to turn off XBee
uint8_t xBeeShutoffMinute = 0;     // Minute to turn off XBee


// Turn the XBee on for 1 hour
void XBeeOn ()
{
    xBeeShutoffHour = (now.hour() + 1) % 24;
    xBeeShutoffMinute = now.minute();
    xBeeState = LOW;
    digitalWrite (BEEPIN, xBeeState);
}

void XBeeOff ()
{
    digitalWrite (BEEPIN, HIGH);
}


// Generate a message stating XBee shutoff time
void XBeeOnMessage(char *buffer)
{
    sprintf_P(buffer, (prog_char *)F("XBee on until %02d:%02d"), xBeeShutoffHour, xBeeShutoffMinute);
}


// - Check if we hit trigger times (window start, shutoff time)
// - Apply XBee state
void XBee (void)
{
  // Start of On window, set state on, and set shutoff time.
  // Note: The window should be > 1 hour, or this will overwrite
  // the shutofftime set by XBeeOn (1 hour).
  if ( now.hour() == XBEEWINDOWSTART )
  {
    xBeeShutoffHour = XBEEWINDOWEND;
    xBeeShutoffMinute = 0;
    xBeeState = LOW;
  }

  // Turn off when we reach shutoff time
  if ( now.hour() == xBeeShutoffHour && now.minute() >= xBeeShutoffMinute )
  {
    xBeeShutoffHour = 0;
    xBeeShutoffMinute = 0;
    xBeeState = HIGH;
  }

  // Actually turn the Xbee on/off according to flag
  digitalWrite (BEEPIN, xBeeState);
}
