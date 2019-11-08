// Load Wi-Fi library
#include <WiFi.h>
#include <driver/spi_slave.h>

#define SPI_MOSI 13
#define SPI_MISO 12
#define SPI_SCK  14
#define SPI_CS   15
#define SPI_HS   2

class SPIClass
{
private:
  static void postSetupCallback(spi_slave_transaction_t *trans);
  static void postTransferCallback(spi_slave_transaction_t *trans);
  
  static const spi_bus_config_t config;
  static const spi_slave_interface_config_t slvcfg;

  static char txBuffer[128];
  static char rxBuffer[128];
  
public:
  void begin(void)
  {
    auto error = spi_slave_initialize(HSPI_HOST, &config, &slvcfg, 2);
    if (error != ESP_OK)
      while (1);
    //gpio_set_pull_mode((gpio_num_t)SPI_CS, GPIO_FLOATING);
    //gpio_set_pull_mode((gpio_num_t)SPI_MOSI, GPIO_FLOATING);
    //gpio_set_pull_mode((gpio_num_t)SPI_MISO, GPIO_FLOATING);
    //gpio_set_pull_mode((gpio_num_t)SPI_SCK, GPIO_FLOATING);
  }

  void end(void)
  {
    spi_slave_free(HSPI_HOST);
  }

  const char *receive(size_t size)
  {
    if (size > 128)
      size = 128;
    spi_slave_transaction_t trans;
    memset(&trans, 0, sizeof(trans));
    memset(rxBuffer, 0, 128);
    memset(txBuffer, 0x2A, 128);
    trans.length = size * 8;
    trans.tx_buffer = txBuffer;
    trans.rx_buffer = rxBuffer;

    spi_slave_transmit(HSPI_HOST, &trans, portMAX_DELAY);
    return rxBuffer;
  }
};

const spi_bus_config_t SPIClass::config = {
  .mosi_io_num = SPI_MOSI,
  .miso_io_num = SPI_MISO,
  .sclk_io_num = SPI_SCK
};
const spi_slave_interface_config_t SPIClass::slvcfg = {
  .spics_io_num = SPI_CS,
  .flags = 0,
  .queue_size = 1,
  .mode = SPI_MODE0,
  .post_setup_cb = postSetupCallback,
  .post_trans_cb = postTransferCallback
};
char SPIClass::txBuffer[128];
char SPIClass::rxBuffer[128];
void SPIClass::postSetupCallback(spi_slave_transaction_t *trans)
{
  //digitalWrite(SPI_HS, LOW);
}
void SPIClass::postTransferCallback(spi_slave_transaction_t *trans)
{
  //digitalWrite(SPI_HS, HIGH);
}

static SPIClass SPI;

// Replace with your network credentials
const char* ssid     = "esp32";
const char* password = "esp32moda";

// Set web server port number to 80
WiFiServer server (80);

struct Info
{
  char modelNumber;
  char serialNumber[16];
  char softwareVersion[6];
  char filterType;
  char flowRate[9];

  char unused[3];
} __attribute__ ((packed));

static Info info;
void updateInfo(void);
void buildInfoHTML(WiFiClient& client);
void handleClient(WiFiClient& client);

void setup() {
  pinMode(SPI_HS, INPUT_PULLUP);
  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  Serial.println("IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients
  if (client)                               // If a new client connects,
    handleClient(client);

  if (!digitalRead(SPI_HS))
    updateInfo();
  
  delay(1);
}

void updateInfo(void)
{
  SPI.begin();
  strncpy((char *)&info, SPI.receive(sizeof(Info)), sizeof(Info));
  SPI.end();

  char *bytes = (char *)&info;
  for (int i = 0; i < sizeof(Info); i++) {
    if (bytes[i] == 0xFF)
      bytes[i] = 0;
  }
  
  while (!digitalRead(SPI_HS))
    delay(1);
}

void buildInfoHTML(WiFiClient& client)
{
  client.printf(R"(
    <h3>Product Info</h3>
    <p><b>Model number:</b> %d</p>
    <p><b>Serial number:</b> %s</p>
    <p><b>Software version:</b> %s</p>
    <br>
    <h3>Filter Info</h3>
    <p><b>Type:</b> %d</p>
    <p><b>Flow Rate:</b> %s</p>
  )", info.modelNumber,
      info.serialNumber,
      info.softwareVersion,
      info.filterType,
      info.flowRate);
}

void handleClient(WiFiClient& client)
{
  Serial.println("New Client.");          // print a message out in the serial port
  String currentLine = "";                // make a String to hold incoming data from the client
  String header = "";
  while (client.connected()) {            // loop while the client's connected
    if (client.available()) {             // if there's bytes to read from the client,
      char c = client.read();             // read a byte, then
      Serial.write(c);                    // print it out the serial monitor
      header += c;
      if (c == '\n') {                    // if the byte is a newline character
        // if the current line is blank, you got two newline characters in a row.
        // that's the end of the client HTTP request, so send a response:
        if (currentLine.length() == 0) {
          // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
          // and a content-type so the client knows what's coming, then a blank line:
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();
          
          if (header.indexOf("GET / ") >= 0) {
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<body><h1>ESP32 Web Server!!</h1>");
            buildInfoHTML(client);
            client.println("</body></html>");
          }
          /* turns the GPIOs on and off
          if (header.indexOf("GET /26/on") >= 0) {
            Serial.println("GPIO 26 on");
            output26State = "on";
            digitalWrite(output26, HIGH);
          }*/
            
          // The HTTP response ends with another blank line
          client.println();
          // Break out of the while loop
          break;
        } else { // if you got a newline, then clear currentLine
          currentLine = "";
        }
      } else if (c != '\r') {  // if you got anything else but a carriage return character,
        currentLine += c;      // add it to the end of the currentLine
      }
    }
  }
  // Clear the header variable
  header = "";
  // Close the connection
  client.stop();
  Serial.println("Client disconnected.");
  Serial.println("");
}

