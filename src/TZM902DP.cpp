//********************************************************
//  ZETA R3(TZM902DP) SDK
//  ver1.0 2023.9.12 
//  For ZETA-P ACK downlink
//********************************************************
#include <Arduino.h>
#include <string.h>
#include "TZM902DP.h"
#include "LowPower.h"

/**
 * @brief Construct a new TZM902DP::TZM902DP object
 *
 */
TZM902DP::TZM902DP()
{
}

/**
 * @brief Initialize
 *
 * @return bool
 */
bool TZM902DP::begin()
{
  Serial2.begin(115200, SERIAL_8N1);
  Serial2.setTimeout(10000);

  pinMode(ZETA_wakeup, OUTPUT);
  pinMode(ZETA_intpin, INPUT_PULLDOWN);

  digitalWrite(ZETA_wakeup, HIGH);

  downlinkWp = 0;
  responseWp = 0;

  return true;
}

/**
 * @brief Finalize
 *
 * @return void
 */
void TZM902DP::end()
{
}

/**
 * @brief connect
 *
 * @return uin32_t wait_time
 */
int TZM902DP::connect()
{
  // wait
  Serial.println("waiting for TZM902DP connect to ZETA server");
  for (int i = 0; i < 10; i++)
  {
    delay(1000);
    Serial.println(i);
  }

  int sleep_time = 10000;

  // wait for registation
  char rx[8];
  while (true)
  {
    char sts = network_status(rx, sizeof(rx));

    if (sts == MODULE_STATUS_REGISTERED)
    {
      Serial.println("registered");
      sleep_time = 1;
      break;
    }
    else if (sts == MODULE_STATUS_SHORT_SLEEP)
    {
      sleep_time = rx[4] << 8 | rx[5];
      Serial.printf("short sleep %ld sec\n", sleep_time);
      break;
    }
    else if (sts == MODULE_STATUS_LONG_SLEEP)
    {
      sleep_time = (rx[4] << 8 | rx[5]) * 60;
      Serial.printf("long sleep %ld sec\n", sleep_time);
      break;
    }
    delay(sleep_time);
  }

  return sleep_time;
}

/**
 * @brief activate
 *
 * @return bool
 */
bool TZM902DP::activate()
{
  Serial.println("初回起動UL");
  /* 1:データ型1byte 0x0F 2:バージョン情報2byte 0x0009 */
  char wakeupsensUL[3] = {0x0F, 0x02, 0x00};
  int size = sizeof(wakeupsensUL);
  /* ZETAアップリンク Zeta.send("デバイス名(自動取得）"SEND_VARIABLE_LENGTH_DATA, "ペイロード"packet, "パケットの大きさ"size) */
  send(wakeupsensUL, size);

  return true;
}

/**
 * @brief モジュールステータス確認
 *
 * @param rx 受信バッファ
 * @param size 受信バッファサイズ
 * @return char ステータス
 */
char TZM902DP::network_status(char *rx, int size)
{
  if (size < 4)
  {
    return -1;
  }

  // SEND
  send_no_retry(INQUIRE_MODULE_STATUS, NULL, 0); 

  // RECV
  read(rx, size);

  return rx[3];
}

/**
 * @brief 受信データ判定
 *
 * @return char データタイプ
 */
char TZM902DP::tick()
{
  if (!Serial2.available())
  {
    return -1;
  }

  char rx[DOWNLINK_SIZE] = {0};
  int n = read(rx, sizeof(rx));
  if (n <= 0)
  {
    return -1;
  }

  if (rx[3] == WAKEUP_REASON_DOWNLINK_DATA)
  {
    memcpy(downlink, rx, n);
    downlinkWp = n;
  }
  else
  {
    memcpy(response, rx, n);
    responseWp = n;
  }

  return rx[3];
}

/**
 * @brief 受信
 *
 * @param rx 受信バッファ
 * @param size 受信バッファサイズ
 * @return int 受信データ数
 */
int TZM902DP::read(char *rx, int size)
{
  if (size < 3)
  {
    return -1;
  }

  int n = 0;
  int len = 0;
  n = Serial2.readBytes(rx, 3);
  if (n != 3)
  {
    return 0;
  }
  len += n;

  n = Serial2.readBytes(&rx[3], rx[2]);
  if (n != rx[2])
  {
    return 0;
  }
  len += n;

  uint16_t crc1 = Crc16_CCITT_Xmodem(&rx[2], len - 4);
  uint16_t crc2 = rx[len - 2] << 8 | rx[len - 1];
  if (crc1 != crc2)
  {
    return -1;
  }

  Serial.print("rx : ");
  for (uint16_t i = 0; i < len; i++)
  {
    Serial.printf("%02X ", rx[i]);
  }
  Serial.println();

  return len;
}

/**
 * @brief 送信
 *
 * @param cmd 送信コマンド
 * @param var 送信データ
 * @param size 送信データサイズ
 * @return bool
 */
bool TZM902DP::send(TZM902DP_TR_COMMANDS cmd, char *var, int size)
{
  bool ret = false;
  char rx[PACKET_SIZE];

  unsigned long tmout = Serial2.getTimeout();
  Serial2.setTimeout(3000);

  for (size_t i = 0; i < 3; i++)
  {
    send_no_retry(cmd, var, size);

    int n = read(rx, sizeof(rx));

    if (n > 0)
    {
      // FA F5 03 01 45 72 /* データ送信成功 */
      // FA F5 03 02 75 11 /* バッファフル; 送信失敗*/
      // FA F5 03 03 65 30 /* データ長エラー */
      if (rx[3] == RECEIVE_DATA_FRAMES_ACK)
      {
        ret = true;
        break;
      }
    }

    delay(60 * 1000);
  }

  Serial2.setTimeout(tmout);

  return ret;
}

bool TZM902DP::send(char *var, int size)
{
  return send(SEND_VARIABLE_LENGTH_DATA, var, size);
}

/**
 * @brief 送信
 *
 * @param cmd 送信コマンド
 * @param var 送信データ
 * @param size 送信データサイズ
 * @return bool
 */
bool TZM902DP::send_no_retry(TZM902DP_TR_COMMANDS cmd, char *var, int size)
{
  char pkt[4 + size + 2];

  // Send TZM902DP's Preanble
  if (getPreamble(cmd, pkt, var, size) == false)
  {
    return false;
  }
  memcpy(&pkt[4], var, size);
  uint16_t CRC_check_bytes = Crc16_CCITT_Xmodem(&pkt[2], size + 2);

  pkt[4 + size + 0] = (CRC_check_bytes & 0xFF00) >> 8;
  pkt[4 + size + 1] = CRC_check_bytes & 0x00FF;

  digitalWrite(ZETA_wakeup, LOW);
  delay(20);

  Serial2.write((uint8_t*)pkt, sizeof(pkt));

  Serial.print("tx : ");
  for (size_t i = 0; i < sizeof(pkt); i++)
  {
    Serial.printf("%02X ", pkt[i]);
  }
  Serial.println();

  Serial2.flush();
  delay(20);
  digitalWrite(ZETA_wakeup, HIGH);

  return true;
}

bool TZM902DP::send_no_retry(char *var, int size)
{
  return send(SEND_VARIABLE_LENGTH_DATA, var, size);
}

/**
 * @brief 送信(生データ)
 *
 * @param var 送信データ
 * @param size 送信データサイズ
 * @return bool
 */
bool TZM902DP::send_raw(char *var, int size)
{
  char pkt[size + 2];

  memcpy(pkt, var, size);
  uint16_t CRC_check_bytes = Crc16_CCITT_Xmodem(&pkt[2], size - 2);

  pkt[size + 0] = (CRC_check_bytes & 0xFF00) >> 8;
  pkt[size + 1] = CRC_check_bytes & 0x00FF;

  digitalWrite(ZETA_wakeup, LOW);
  delay(20);

  Serial2.write((uint8_t*)pkt, sizeof(pkt));

  Serial.print("tx : ");
  for (int i = 0; i < sizeof(pkt); i++)
  {
    Serial.printf("%02X ", pkt[i]);
  }
  Serial.println();

  Serial2.flush();
  delay(20);
  digitalWrite(ZETA_wakeup, HIGH);

  return true;
}

/**
 * @brief ヘッダー取得
 *
 * @param cmd 送信コマンド
 * @param store ヘッダー格納バッファ
 * @param var 送信データ
 * @param size 送信データサイズ
 * @return bool
 */
bool TZM902DP::getPreamble(TZM902DP_TR_COMMANDS cmd, char *store, char *var, int size)
{
  // error
  if ((cmd == SEND_VARIABLE_LENGTH_DATA) && (var != NULL && sizeof(var) == 0))
  {
    return false;
  }

  char temp[TZM902DP_PREAMBLE_SIZE] = {0xfa, 0xf5, 0x00, cmd};
  switch (cmd)
  {
  case SEND_VARIABLE_LENGTH_DATA:
    temp[2] = 0x03 + size;
    break;

  case INQUIRE_VERSION:
  case INQUIRE_MAC:
  case INQUIRE_NETWORK_TIME:
  case INQUIRE_NETWORK_QUALITY:
  case INQUIRE_MODULE_STATUS:
    temp[2] = 0x03;
    break;

  case SET_TEST_MODE:
    temp[2] = 0x04;
    break;

  default:
    return false;
  }
  memcpy(store, temp, sizeof(temp));
  return true;
}

/**
 * @brief CRC計算
 *
 * @param pmsg 計算対象バッファ
 * @param msg_size 計算対象データサイズ
 * @return uint16_t CRC計算結果
 */
uint16_t TZM902DP::Crc16_CCITT_Xmodem(char *pmsg, uint16_t msg_size)
{
  uint16_t i = 0, j = 0;
  uint16_t msg = 0;
  uint16_t crc = 0x0000;
  for (i = 0; i < msg_size; i++)
  {
    msg = *pmsg;
    msg <<= 8;
    pmsg++;
    for (j = 0; j < 8; j++)
    {
      if ((msg ^ crc) >> 15)
      {
        crc <<= 1;
        crc ^= CRC16_POLY;
      }
      else
        crc <<= 1;
      msg <<= 1;
    }
  }
  return (crc);
}

/**
 * @brief ZETAモジュールから応答またはダウンリンクを取得する
 *
 */
int TZM902DP::downlink_check()
{
  char packet[PACKET_SIZE];
  int n;
  int retry = 10;

  while (true) {
    switch (tick()) {
      case WAKEUP_REASON_DOWNLINK_DATA:
        n = get_downlink(packet, sizeof(packet));
 //       downlink_proccess(packet, n);
        return WAKEUP_REASON_DOWNLINK_DATA;

      case 0xFF:
        if (--retry == 0)
        {
          return 0xFF;
        }
        delay(1000);
        break;

      default:
        break;
    }
  }

  return -1;
}

/**
 * @brief ダウンリンクデータ取得
 *
 * @param data 格納バッファ
 * @param size 格納バッファサイズ
 * @return int 格納データ数
 */
int TZM902DP::get_downlink(char *data, int size)
{
  int n = size < downlinkWp ? size : downlinkWp;
  memcpy(data, downlink, n);

  Serial.print("downlink : ");
  for (int i = 0; i < n; i++)
  {
    Serial.printf("%02X ", data[i]);
  }
  Serial.println();

  return n;
}
