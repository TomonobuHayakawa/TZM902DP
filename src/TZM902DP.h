//********************************************************
//  ZETA R3(TZM902DP) SDK
//  ver1.0 2023.9.12 
//  For ZETA-P ACK downlink
//********************************************************
#ifndef TZM902DP_h
#define TZM902DP_h

#include <Arduino.h>

#define TZM902DP_PREAMBLE_SIZE 4
#define TZM902DP_SEND_VARIABLE_LENGTH_SIZE 50
#define CRC16_POLY 0x1021 // x16 + x12 + x5 + 1

///////////////////////////////////
// TZM902DP Commands             //
///////////////////////////////////
enum TZM902DP_TR_COMMANDS
{
  SEND_VARIABLE_LENGTH_DATA = 1
};

#define RECEIVE_DATA_FRAMES_ACK 0x01
#define RECEIVE_BUFFER_FULL 0x02
#define RECEIVE_LENGTH_ERROR 0x03
#define RECEIVE_CRC_ERROR 0x04

#define SEND_VARIABLE_LENGTH_DATA 0x02

#define INQUIRE_VERSION 0x00
#define INQUIRE_MAC 0x10
#define INQUIRE_NETWORK_TIME 0x11
#define INQUIRE_NETWORK_QUALITY 0x13
#define INQUIRE_MODULE_STATUS 0x14

#define SET_TEST_MODE 0x22

#define COMMANDS_ACK_SET_SUCCEED 0x20
#define COMMANDS_ACK_SET_FAILED 0x21

#define WAKEUP_REASON_DOWNLINK_DATA 0x30

#define MODULE_STATUS_UNREGISTERED 0x43
#define MODULE_STATUS_REGISTERED 0x44
#define MODULE_STATUS_LONG_SLEEP 0x45
#define MODULE_STATUS_SHORT_SLEEP 0x46

#define PACKET_SIZE 64
#define DOWNLINK_SIZE PACKET_SIZE

////////////////////////////////
// TZM902DP Class Declaration //
////////////////////////////////
class TZM902DP
{
public:
  TZM902DP();

  int ZETA_wakeup = 20; // ZETA_wakeupはD20（デジタルピン20、GPIO R#5）に接続
  int ZETA_intpin = 21; // ZETA_intpinはD21（デジタルピン21、GPIO R#4）に接続
  uint32_t interval_sec;

  bool begin();
  void end();
	
  int  connect();
  bool activate();
  char tick();

  int read(char *store, int size);
  bool send(TZM902DP_TR_COMMANDS cmd, char *var, int size);
  bool send(char *var, int size);
  bool send_no_retry(TZM902DP_TR_COMMANDS cmd, char *var, int size );
  bool send_no_retry(char *var, int size );
  bool send_raw(char *var, int size);

  int downlink_check();
  int get_downlink(char *data, int size);

private:
  char downlink[DOWNLINK_SIZE];
  char response[DOWNLINK_SIZE];

  uint16_t downlinkWp;
  uint16_t responseWp;

  char network_status(char *rx, int size);
  bool getPreamble(TZM902DP_TR_COMMANDS cmd, char *store, char *var, int size);
  uint16_t Crc16_CCITT_Xmodem(char *pmsg, uint16_t msg_size);
  void downlink_proccess(char *downlink, int size);
};

#endif /* TZM902DP_h */
