#include <hidcomposite.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

char buffer [32];
int cnt = 0;


class HIDSelector : public HIDComposite
{
public:
  HIDSelector(USB *p) : HIDComposite(p) {};

protected:
  void ParseHIDData(USBHID *hid, uint8_t ep, bool is_rpt_id, uint8_t len, uint8_t *buf); // Called by the HIDComposite library
  bool SelectInterface(uint8_t iface, uint8_t proto);
};

bool HIDSelector::SelectInterface(uint8_t iface, uint8_t proto)
{
  if(proto != 0)
    return true;

  return false;
}

void HIDSelector::ParseHIDData(USBHID *hid, uint8_t ep, bool is_rpt_id, uint8_t len, uint8_t *buf) {
      if (len && buf)  {
              Serial.print("#C=");
              Serial.print(ep, HEX);
              Serial.print(',');
              Serial.print(len, HEX);
              Serial.print(',');
              for (int i = 0; i < len; i++)
              {
                Serial.print(buf[i], HEX);
                if (i < len - 1)
                  Serial.print(',');
              }
              Serial.println("");
      }
}


USB     Usb;
//USBHub     Hub(&Usb);
HIDSelector    hidSelector(&Usb);


void setup()
{
  Serial.begin( 115200 );
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  Serial.println("#A");

  if (Usb.Init() == -1)
    Serial.println("#X");
  else
    Serial.println("#B");

  delay( 200 );
}

void loop()
{
  Usb.Task();

  while (Serial.available())
  {
      char c = Serial.read();
      if ((c == '\n') || (cnt == sizeof(buffer) - 1))
      {
          buffer[cnt] = '\0';
          cnt = 0;

          if (strcmp(buffer, "INIT") == 0)
            Serial.println("#B");
      }
      else
        buffer[cnt++] = c;
  }
}

