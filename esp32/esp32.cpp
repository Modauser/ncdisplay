#include <WiFi.h>
#include "spi.hpp"

// WiFi server credentials
const char* ssid     = "ION";
const char* password = "naturalchoice";

// Wifi web server, bound to HTTP port 80
WiFiServer server (80);

/**
 * A packed structure that fits to the data received from the display.
 */
struct Info
{
  char modelNumber;
  char serialNumber[16];
  char softwareVersion[6];
  char filterType;
  char flowRate[9];

  char unused[3]; // Must round to a multiple of four bytes
} __attribute__ ((packed));

// An array to map Info::filterType to an identifying string
const std::array<const char *, 5> filterTypes = {
	"CarbonPlus",
	"CarbonPro",
	"FiberTek",
	"CarbonPhos",
	"CarbonSilv"
};
// String to contain model number based on Info::modelNumber
char productString[10] = "ION TS000";


/**
 * Updates the info structure with new data from the display.
 * Should only be called when we're aware of incoming data.
 */
static Info info;
void updateInfo(void);

/**
 * Handles an incoming client, most likely by displaying a webpage.
 * The client's connection is closed after handling is complete.
 * @param client The client to handle
 */
void handleClient(WiFiClient& client);

/**
 * Creates webpage content based on the data received from the display,
 * appending it to the given client's stream.
 * @param client The client to provide webpage content to
 */
void buildInfoHTML(WiFiClient& client);

/**
 * Initializes the WiFi access point and web server.
 */
void setup()
{
  pinMode(SPI_HS, INPUT_PULLUP);
  Serial.begin(115200);

  WiFi.softAP(ssid, password);//, 1, 1); // channel, ssid_hidden
  Serial.println("IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();
}

/**
 * Main loop: handles incoming clients, and checks for incoming data from the
 * display.
 */
void loop()
{
  // Handle a client of our web server if they're there
  WiFiClient client = server.available();
  if (client)
    handleClient(client);

  // Check for display signal of incoming update
  if (!digitalRead(SPI_HS))
    updateInfo();
  
  delay(1); // ms
}

void updateInfo(void)
{
  // Conduct the SPI transfer, storing it in the info structure.
  // TODO could probably receive directly into the info structure.
  SPI.begin();
  strncpy((char *)&info, SPI.receive(sizeof(Info)), sizeof(Info));
  SPI.end();

  // Scan the received data for high bytes (0xFF) and convert them to zeroes.
  // There's an SPI slave driver bug on the ESP32 that ends a transfer on a
  // received zero byte, so zero bytes are made into 0xFF during transit.
  char *bytes = (char *)&info;
  for (int i = 0; i < sizeof(Info); i++) {
    if (bytes[i] == 0xFF)
      bytes[i] = 0;
  }
  
  // Wait for release of handshake pin by display, to confirm the display is
  // finished transmitting data.
  // TODO may not be necessary.
  while (!digitalRead(SPI_HS))
    delay(1);
}

void buildInfoHTML(WiFiClient& client)
{
  // Update product info string
  productString[6] = '1' + info.modelNumber;

  // Output the webpage with the latest data from the display
  client.printf("<h3>Product Info</h3>"
                "<p><b>Model number:</b> %s</p>"
                "<p><b>Serial number:</b> %s</p>"
                "<p><b>Software version:</b> %s</p>"
                "<br>"
                "<h3>Filter Info</h3>"
                "<p><b>Type:</b> %s</p>"
                "<p><b>Flow Rate:</b> %s</p>",
                productString,
                info.serialNumber,
                info.softwareVersion,
                filterTypes[info.filterType],
                info.flowRate);
}

void handleClient(WiFiClient& client)
{
  Serial.println("New client.");
  String header = "";
  bool previousNewline = false;

  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      header += c;
      if (c == '\n') {
        // The HTTP request ends with two newline characters in a row.
        if (previousNewline) {
          // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
          // and a content-type so the client knows what's coming, then a blank line:
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();
          
          // Getting root/main page
          if (header.indexOf("GET / ") >= 0) {
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<body><h1>ION Web Server</h1>");
            buildInfoHTML(client);
            client.println("</body></html>");
          }

          // Potentially getting other pages?
          // e.g. could modify GPIOs
          //if (header.indexOf("GET /26/on") >= 0) {
          //  Serial.println("GPIO 26 on");
          //  output26State = "on";
          //  digitalWrite(output26, HIGH);
          //}
            
          // The HTTP response ends with another blank line
          client.println();
          break;
        } else {
            previousNewline = true;
        }
      } else if (c != '\r') {
        previousNewline = false;
      }
    }
  }

  // Done, close the connection.
  client.stop();
  Serial.println("Client disconnected.");
}

