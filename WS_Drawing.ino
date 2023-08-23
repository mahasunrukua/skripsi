// =========================================
// Drawing Content Block====================
// =========================================

//h_month 12 x 11
const char static h_month_E[] PROGMEM = { "MUHARRAM\0\0\0"
                                          "SHAFAR\0\0\0\0\0"
                                          "RAB.AWAL\0\0\0"
                                          "RAB.AKHIR\0\0"
                                          "JUM.AWAL\0\0\0"
                                          "JUM.AKHIR\0\0"
                                          "RAJAB\0\0\0\0\0\0"
                                          "SYA'BAN\0\0\0\0"
                                          "RAMADHAN\0\0\0"
                                          "SYAWAL\0\0\0\0\0"
                                          "DZULQA'DAH\0"
                                          "DZULHIJAH\0\0"
                                        };
//m_month 12 x 10
const char static m_month_E[] PROGMEM = { "JAN\0"
                                          "FEB\0"
                                          "MAR\0"
                                          "APR\0"
                                          "MEI\0"
                                          "JUN\0"
                                          "JUL\0"
                                          "AGS\0"
                                          "SEP\0"
                                          "OKT\0"
                                          "NOV\0"
                                          "DES\0"
                                        };

const char static sholatN_E[] PROGMEM = { "IMSAK\0\0\0"
                                          "SUBUH\0\0\0"
                                          "TERBT\0\0\0"
                                          "DHUHA\0\0\0"
                                          "DUHUR\0\0\0"
                                          "ASHAR\0\0\0"
                                          "MAGRB\0\0\0"
                                          "ISY'A\0\0\0"
                                          "JUMAT\0\0\0"
                                        };

void drawAzzan(int DrawAdd) {
  // check RunSelector
  uint8_t           ct_limit = 50; //harus angka genap
  static uint8_t    ct;
  static uint16_t   lsRn;
  uint16_t          Tmr = millis();

  if ((Tmr - lsRn) > 500 and ct <= ct_limit) {
    lsRn = Tmr;

    if ((ct % 2) == 0) {
      lcd.setCursor(5, 0);
      lcd.print("ADZAN");
      if (jumat) {
        lcd.setCursor(5, 1);
        lcd.print(sholatN(8));
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(sholatN(SholatNow));
      }
      Buzzer(1);
      //Disp.drawRect(1,2,62,13);
      /*fType(0);
        dwCtr(0,0,"ADZAN");
        fType(1);
        if(jumat) {dwCtr(0,8,sholatN(8));}
        else      {dwCtr(0,8,sholatN(SholatNow));}
        Buzzer(1);*/

    }
    else {
      Buzzer(0);
    }
    //DoSwap = true;
    //lcdclear();
    ct++;
  }

  if ((Tmr - lsRn) > 2000 and (ct > ct_limit)) {
    //dwDone(DrawAdd);
    lcdclear();
    RunSel = 1;
    ct = 0;
    Buzzer(0);
  }
}

void drawIqomah(int DrawAdd)  // Countdown Iqomah (9 menit)
{
  // check RunSelector
  if (!dwDo(DrawAdd)) return;

  static uint16_t   lsRn;
  uint16_t          Tmr = millis();
  static int        ct;
  int               mnt, scd, cn_l ;
  char              locBuff[6];

  cn_l  = (Iqomah[SholatNow] * 60);
  //Disp.drawRect(1,2,62,13);
  if ((Tmr - lsRn) > 1000 and ct <= cn_l)
  {
    lsRn = Tmr;
    mnt = floor((cn_l - ct) / 60);
    scd = (cn_l - ct) % 60;
    if (mnt > 0) {
      sprintf(locBuff, "%02d:%02d", mnt, scd);
    }
    else    {
      sprintf(locBuff, "%02d", scd);
    }
    if ((ct % 2) == 0) {
      /*fType(0);
        dwCtr(0, -1, "IQOMAH");*/
    }
    /*fType(0);
      dwCtr(0, 8, locBuff);*/
    if (ct > (cn_l - 10)) Buzzer(1); // Buzzer on 2 seccon before Iqomah
    ct++;
    //DoSwap = true;

  }
  if (ct > cn_l)
  {
    dwDone(DrawAdd);
    ct = 0;
    Buzzer(0);
  }
}

void drawSholat_S(int sNum, int c) // Box Sholah Time
{
  char  BuffTime[10];
  char  BuffShol[7];
  float   stime   = sholatT[sNum];
  uint8_t shour   = floor(stime);
  uint8_t sminute = floor((stime - (float)shour) * 60);
  uint8_t ssecond = floor((stime - (float)shour - (float)sminute / 60) * 3600);

  lcd.setCursor(11, 0);
  lcd.print(sholatN(sNum));

  sprintf(BuffTime, "%02d:%02d", shour, sminute);
  lcd.setCursor(11, 1);
  lcd.print(BuffTime);
  // Disp.drawRect(c+1,2,62,13);
  /*fType(1); dwCtr(c, 0, sholatN(sNum));
    fType(0); dwCtr(c, 9, BuffTime);*/
  //DoSwap = true;
}

void drawSholat(int DrawAdd) {
  // check RunSelector
  //    int DrawAdd = 0b0000000000000100;
  static uint16_t   count;
  static uint8_t    s; // 0=in, 1=out
  static uint8_t    sNum;
  static uint16_t   lsRn;
  uint16_t          Tmr = millis();
  uint8_t           c = 0;
  uint8_t    first_sNum = 0;
  int               DrawWd =  0;//DWidth - c;

  if ((Tmr - lsRn) > 3000)
  {
    if (sNum < 7) {
      sNum++;
    }
    else {
      //dwDone(DrawAdd);
      sNum = 0;
    }
    lsRn = Tmr;
    //s = 0;
  }

  if (Prm.SI == 0) {
    first_sNum = 1;
  }
  else {
    first_sNum = 0;
  }
  if (Prm.SI == 0 and sNum == 0) {
    sNum = 1;
  }
  if (Prm.ST == 0 and sNum == 2) {
    sNum = 3;
  }
  if (Prm.SU == 0 and sNum == 3) {
    sNum = 4;
  }


  /*  if ((((sNum == first_sNum) and s == 0) or
           ((sNum == 7) and s == 1)) and x <= 20) {
      //drawSmallTS(int(x/2));
    }
    else {//drawSmallTS(10);
    }*/
  drawSholat_S(sNum, c);

  /*Disp.drawFilledRect(c, 0, c + DrawWd / 2 - x, 15, 0);
    Disp.drawFilledRect(DrawWd / 2 + x + c, 0, 63, 15, 0);*/
}

void drawtime(int DrawAdd, uint16_t x, uint16_t y, uint16_t x1, uint16_t y1) { // Draw Time
  char  Buff[20];
  char  locBuff[20];
  static char  out[45];
  int   locLen = (nowH.hM - 1) * 11;

  static uint8_t    count1;
  static uint16_t   lsRn;
  //static uint16_t   lsRn2;
  uint16_t          Tmr = millis();

  if (Tmr - lsRn >= 15000) {
    lcd.setCursor(0, 0);
    lcd.print("          ");
    lcd.setCursor(0, 1);
    lcd.print("          ");

    if (count1 < 1) {
      count1++;
    }
    else {
      count1 = 0;
    }

    //lsRn2 = Tmr;
    lsRn = Tmr;
  }

  if (count1 == 0) {
    sprintf(Buff, "%02d/%02d/%02d", now.day(), now.month(), now.year() - 2000);
    lcd.setCursor(x, y);
    lcd.print(Buff);

    sprintf(Buff, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
    lcd.setCursor(x1, y1);
    lcd.print(Buff);
  }

  if (count1 == 1) {
      memccpy_P(locBuff, h_month_E + locLen, 0, 11);
      sprintf(out, "%02d   %dH\0", nowH.hD, nowH.hY);
      lcd.setCursor(x, y);
      lcd.print(out);
      sprintf(out, "%s\0", locBuff);
      lcd.setCursor(x1, y1);
      lcd.print(out);
  }
  /*dwCtr(0, y, Buff);*/
  //DoSwap = true;
}

void blinkBlock(int DrawAdd) {
  // check RunSelector
  if (!dwDo(DrawAdd)) return;

  static uint16_t   lsRn;
  static uint16_t   ct, ct_l;
  uint16_t          Tmr = millis();
  int               mnt, scd;//
  char              locBuff[6];//

  if (jumat)       {
    ct_l = Prm.JM * 60;
  }
  else            {
    ct_l = Prm.SO * 60;
  }
  jumat = false;

  if ((Tmr - lsRn) > 1000)
  { lsRn = Tmr;

    //Disp.drawChar(1, 1 , ct);
    mnt = floor((ct_l - ct) / 60);
    scd = (ct_l - ct) % 60;
    sprintf(locBuff, "%d:%02d", mnt, scd);
    /*fType(2);
      Disp.drawText(1, 7, locBuff); // tampil tunggu sholat*/
    if ((ct % 2) == 0)
    {

      /*Disp.drawFilledRect(DWidth - 3, DHeight - 3, DWidth - 2, DHeight - 2);*/
    }
    //DoSwap = true;
    ct++;
  }
  if (ct > ct_l)
  { dwDone(DrawAdd);
    azzan = false;
    ct = 0;
  }
  // Disp.drawText(1,7,String(ct_l-ct));
}

// =========================================
// Drawing Tools============================
// =========================================
boolean dwDo(int DrawAdd)
{ if (RunSel == DrawAdd) {
    return true;
  }
  else return false;
}

void dwDone(int DrawAdd)
{ RunFinish = DrawAdd;
  RunSel = 0;
}

void Buzzer(uint8_t state)
{
  if (state == 1 and Prm.BZ == 1)
  {
    tone(BUZZ, 500, 400);
  }
  else
  {
    noTone(BUZZ);
  }
}

void lcdclear() {
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
}

// digunakan untuk menghitung hari pasaran
int jumlahhari() {
  DateTime  now = RTC.now();
  int d = now.day();
  int m = now.month();
  int y = now.year();
  int hb[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
  int ht = (y - 1970) * 365 - 1;
  int hs = hb[m - 1] + d;
  int kab = 0;
  int i;
  if (y % 4 == 0) {
    if (m > 2) {
      hs++;
    }
  }
  for (i = 1970; i < y; i++) {
    if (i % 4 == 0) {
      kab++;
    }
  }
  return (ht + hs + kab);
}
