#include "stdafx.h"

//#include <windows.h>
#include <math.h>
#pragma hdrstop

#include "st_api.h"
//#include "ss_log.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------

void Log(int a, wchar_t *b) {}
void Log(int a, wchar_t *b, SOCKET c) {}
#define	LOGTYPE_NORMAL		0
#define	LOGTYPE_CRITICAL	0
//==============================================================================
int BaseUDP::Init_Lan(int port)
{
	WSADATA wsaData;
	sockaddr_in local;

	int wsaret=WSAStartup(0x0202,&wsaData);
	if(wsaret==0)
	{
		//Winsock DLL load success.
		//Right now there is nothing to put here.
		//Later on the need might arise
		Log(LOGTYPE_NORMAL, L"WSA Startup OK");
	}
	else
	{
		//Winsock load error!
		//Just terminate the thread.
		Log(LOGTYPE_CRITICAL, L"WSA Startup Error");
		return SOCKET_ERROR;
	}

	local.sin_family=AF_INET;
	local.sin_addr.s_addr=INADDR_ANY;
	local.sin_port=htons((u_short )port);

	UDPSocket = socket(AF_INET,SOCK_DGRAM,0);

	if(UDPSocket == INVALID_SOCKET)
	{
		//Error creating POP server socket!
		//No point in continuing now :-(
		Log(LOGTYPE_CRITICAL, L"UDP Socket INVALID");
		return SOCKET_ERROR;
	}

	if(connect(UDPSocket,(sockaddr *)&send_addr,sizeof(send_addr))!=0)
	{
		//damn! socket bind failed
		UDPSocket = INVALID_SOCKET;
		Log(LOGTYPE_CRITICAL, L"Connect ERROR");
		return SOCKET_ERROR;
	}

	Log(LOGTYPE_NORMAL, L"Socket=%d", UDPSocket);

	SetRxBufferSize(1024*1024);

	return 0;

}

int BaseUDP::Bind(int local_port)
{
	sockaddr_in local;

	local.sin_family=AF_INET;
	local.sin_addr.s_addr=INADDR_ANY;
	local.sin_port=htons((u_short )local_port);

    closesocket(UDPSocket);
    UDPSocket = socket(AF_INET,SOCK_DGRAM,0);

	int err = bind(UDPSocket,(sockaddr *)&local,sizeof(local));
	if(err != 0)
	{
		//damn! socket bind failed
		UDPSocket = INVALID_SOCKET;
		int error = WSAGetLastError();
		Log(LOGTYPE_CRITICAL, L"Bind ERROR = %d", error);
		return SOCKET_ERROR;
	}

    return 0;
}


int BaseUDP::AddRemoveMcastGroup(int cmd, char *group)
{
    struct ip_mreq multi;

    char hostname[300];
    hostent * hp;
    gethostname(hostname,300);
    struct in_addr * addr;
    struct in_addr m_interface;

    if ((hp=gethostbyname(hostname))==NULL) {
//      fprintf(stderr,"Can't get host name\n");
      exit(1);
    } else {
      for(int i=0; i<10; i++) {
        addr=(struct in_addr *)hp->h_addr_list[i];
        if(addr == NULL) break;
        int netw = addr->S_un.S_addr & 0xFFFFFF;
        if(netw == 0x01A8C0) { // add ONLY!!! 192.168.1.0 network addr
            m_interface.s_addr=addr->s_addr;
            break;
        }
      }  
    }


      multi.imr_multiaddr.s_addr=inet_addr(group);  // Lin Cam
      multi.imr_interface=m_interface;
      if (setsockopt(UDPSocket,IPPROTO_IP,cmd,(char *)&multi,sizeof(multi))==SOCKET_ERROR)
	  {
		int error = WSAGetLastError();
//        socketerror("Error joining multicast group for listener");
          return -1;
      }
    return 0;
}

bool BaseUDP::SetRxBufferSize(unsigned int bufferSize)
{
   if (bufferSize <= 0) return false;
   unsigned int oldBufferSize = GetRxBufferSize();
   if (setsockopt(UDPSocket, SOL_SOCKET, SO_RCVBUF, (char*)&bufferSize, sizeof(bufferSize)) < 0)
   {
        setsockopt(UDPSocket, SOL_SOCKET, SO_RCVBUF, (char*)&oldBufferSize, sizeof(oldBufferSize));
        return false;
   }
   return true;
}  // end Stoika::SetRxBufferSize()

unsigned int BaseUDP::GetRxBufferSize()
{
    unsigned int rxBufferSize = 0;
    int len = sizeof(rxBufferSize);
    if (getsockopt(UDPSocket, SOL_SOCKET, SO_RCVBUF, (char*)&rxBufferSize, &len) < 0)
    {
	    return 0;
    }
    return ((unsigned int)rxBufferSize);
}  // end Stoika::GetRxBufferSize()

bool BaseUDP::SetBroadcast(bool broadcast)
{
    BOOL state = broadcast ? TRUE : FALSE;
    if (setsockopt(UDPSocket, SOL_SOCKET, SO_BROADCAST, (char*)&state, sizeof(state)) < 0)
    {
        return false;
    }
    return true;
}  // end Stioka::SetBroadcast()

int BaseUDP::SendCMD(int proc, int p1, int p2, int p3)
{
   	buffer[0] = 0xAA;
	buffer[1] = 0x55;
	buffer[2] = (char)proc;
	buffer[3] = (char)p1;
	buffer[4] = (char)p2;
	buffer[5] = (char)p3;
    return sendto(UDPSocket,buffer,6,0,(struct sockaddr *)&send_addr,sizeof(send_addr));
}
int BaseUDP::SendCMD(unsigned char *buff, int len)
{
   	buffer[0] = 0xAA;
	buffer[1] = 0x55;
    memcpy(&buffer[2],buff,len);
    return sendto(UDPSocket,buffer,len,0,(struct sockaddr *)&send_addr,sizeof(send_addr));
}

int BaseUDP::SendBuff(unsigned char *buff, int len)
{
    return sendto(UDPSocket, (char*)buff,len,0,(struct sockaddr *)&send_addr,sizeof(send_addr));
}

int BaseUDP::SendTo(unsigned char *buff, int len, unsigned long inet_addr, unsigned port)
{
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
  	addr.sin_addr.s_addr=inet_addr;
    addr.sin_port=htons((u_short )port);

    return sendto(UDPSocket,(const char *)buff,len,0,(struct sockaddr *)&addr,sizeof(addr));
}


//    void CMD_Set_Pause(int proc,int pause)  { SendCMD(proc,4,pause >> 8, pause); }
//    void CMD_Set_VRC(int proc,int start, int end, int len) { SendCMD(proc,11,0, start); Sleep(30); SendCMD(proc,7,end, len); }
//    void CMD_Set_Imp(int proc,int len)      { SendCMD(proc,8,0, len); }
//    void CMD_Set_Imp1(int proc,int len)     { SendCMD(proc,9,0, len); }

void Echo2::CMD_Set_Pause(int ch_num, int pause)
{
		unsigned char cmd[30];

		cmd[0] = 0xAA;
		cmd[1] = 0x55;
		cmd[2] = 0x00;
		cmd[3] = 0xFB;
		cmd[4] = ch_num;	// chnum
		cmd[5] = 0x00;
		cmd[6] = (char)4;	// cmd
		cmd[7] = (char)0;	// proc
		cmd[8] = pause;	// amp_len
		cmd[9] = pause >> 8; // amp_end
		SendBuff(cmd,20);
}

void Echo2::CMD_SetRS(int ch_num, int OnOff)
{
		unsigned char cmd[30];

		cmd[0] = 0xAA;
		cmd[1] = 0x55;
		cmd[2] = 0x00;
		cmd[3] = 0xFB;
		cmd[4] = ch_num;	// chnum
		cmd[5] = 0x00;
		cmd[6] = (char)62;	// cmd
		cmd[7] = (char)0;	// proc
		cmd[8] = (char)OnOff;	// amp_len
		cmd[9] = (char)0; // amp_end
		SendBuff(cmd,20);
}

void Echo2::CMD_Set_VRC(int ch_num, int start, int end, int len)
{
		unsigned char cmd[30];

		cmd[0] = 0xAA;
		cmd[1] = 0x55;
		cmd[2] = 0x00;
		cmd[3] = 0xFB;
		cmd[4] = ch_num;		// chnum
		cmd[5] = 0x00;
		cmd[6] = (char)11;	// cmd
		cmd[7] = (char)0;	// proc
		cmd[8] = (char)start;	// 0
		cmd[9] = (char)0; // amp_Start
		SendBuff(cmd,20);

		Sleep(20);

		cmd[0] = 0xAA;
		cmd[1] = 0x55;
		cmd[2] = 0x00;
		cmd[3] = 0xFB;
		cmd[4] = ch_num;	// chnum
		cmd[5] = 0x00;
		cmd[6] = (char)7;	// cmd
		cmd[7] = (char)0;	// proc
		cmd[8] = (char)len;	// amp_len
		cmd[9] = (char)end; // amp_end
		SendBuff(cmd,20);
		Sleep(20);
}
void Echo2::CMD_Set_Imp1(int ch_num,int len)
{
		unsigned char cmd[30];

		cmd[0] = 0xAA;
		cmd[1] = 0x55;
		cmd[2] = 0x00;
		cmd[3] = 0xFB;
		cmd[4] = ch_num;	// chnum
		cmd[5] = 0x00;
		cmd[6] = (char)9;	// cmd
		cmd[7] = (char)0;	// proc
		cmd[8] = (char)len;	// amp_len
		cmd[9] = (char)0; // amp_end
		SendBuff(cmd,20);
}

void Echo2::CMD_SetUserData(unsigned index, unsigned data)
{
		unsigned char cmd[30];

		cmd[0] = 0xAA;
		cmd[1] = 0x55;
		cmd[2] = 0x00;
		cmd[3] = 0xA0;
		cmd[4] = 0x00;	// chnum
		cmd[5] = index;
		cmd[6] = (char)data;	// cmd
		cmd[7] = (char)data >> 8;	// proc
		SendBuff(cmd,20);
}


void Echo2::ParseParam(unsigned char *c, Echo2_ChParam *param)
{
	param->amp_start  = *c++;
	param->amp_end    = *c++;
	param->amp_len    = *c & 0x7F;
	param->is_RS	  = *c++ >> 7;
	param->Pulse_RPT1 = *c++;
	param->delay      = *(short *)c;
	c+=2;
}

void Echo2_new::CMD_Set_Pause(int ch_num,int pause)
{
		unsigned char cmd[30];

		pause = pause*25;

		if(pause > 0xFFF) pause = 0xFFF;
		if(pause == 0) pause = 2;

		cmd[0] = 0xAA;
		cmd[1] = 0x55;
		cmd[2] = 0x00;
		cmd[3] = 0xFB;
		cmd[4] = ch_num;	// chnum
		cmd[5] = 0x00;
		cmd[6] = (char)pause;	// cmd
		cmd[7] = (char)(pause >> 8);	// proc
		cmd[8] = (char)0;	// amp_len
		cmd[9] = (char)2 << 4; // amp_end
		SendBuff(cmd,20);
}


void Echo2_new::CMD_SetRS(int ch_num, int OnOff)
{
		unsigned char cmd[30];

		int num;
		num = (OnOff) ? 16 : 32;

		cmd[0] = 0xAA;
		cmd[1] = 0x55;
		cmd[2] = 0x00;
		cmd[3] = 0xFB;
		cmd[4] = ch_num;	// chnum
		cmd[5] = 0x00;
		cmd[6] = (char)num;	// cmd
		cmd[7] = (char)1;	// proc
		cmd[8] = (char)0;	// amp_len
		cmd[9] = (char)5 << 4; // amp_end
		SendBuff(cmd,20);
}

void Echo2_new::CMD_Set_Imp1(int ch_num, int num)
{
		unsigned char cmd[30];


		cmd[0] = 0xAA;
		cmd[1] = 0x55;
		cmd[2] = 0x00;
		cmd[3] = 0xFB;
		cmd[4] = ch_num;	// chnum
		cmd[5] = 0x00;
		cmd[6] = (char)num;	// cmd
		cmd[7] = (char)1;	// proc
		cmd[8] = (char)0;	// amp_len
		cmd[9] = (char)6 << 4; // amp_end
		SendBuff(cmd,20);
}


void Echo2_new::CMD_Set_ImpNum(int ch_num,int num)
{
		unsigned char cmd[30];

		cmd[0] = 0xAA;
		cmd[1] = 0x55;
		cmd[2] = 0x00;
		cmd[3] = 0xFB;
		cmd[4] = ch_num;	// chnum
		cmd[5] = 0x00;
/*
	if(num == 0) {
		cmd[6] = (char)0x00;	// cmd
		cmd[7] = (char)0x00;	// proc
	}
	if(num == 2) {
		cmd[6] = (char)0x05;	// cmd
		cmd[7] = (char)0x0A;	// proc
	} else if(num == 4) {
		cmd[6] = (char)0x55;	// cmd
		cmd[7] = (char)0xAA;	// proc
	}
*/
		cmd[6] = (char)(num & 0xF) | (char)((8 & 0xF) << 4);	// cmd
		cmd[7] = (char)0x00;	// proc
		cmd[8] = (char)0;	// amp_len
		cmd[9] = (char)1 << 4; // amp_end
		SendBuff(cmd,20);
}

void Echo2_new::CMD_Set_floor(int ch_num,int num)
{
		unsigned char cmd[30];

		num = 3-(num & 0x3);

		cmd[0] = 0xAA;
		cmd[1] = 0x55;
		cmd[2] = 0x00;
		cmd[3] = 0xFB;
		cmd[4] = ch_num;	// chnum
		cmd[5] = 0x00;
		cmd[6] = (char)num;			// cmd
		cmd[7] = (char)0;			// proc
		cmd[8] = (char)0;			// amp_len
		cmd[9] = (char)7 << 4;		// amp_end
		SendBuff(cmd,20);
}


void Echo2_new::CMD_SetZero(int ch_num,int level) {
		unsigned char cmd[30];

		cmd[0] = 0xAA;
		cmd[1] = 0x55;
		cmd[2] = 0x00;
		cmd[3] = 0xFB;
		cmd[4] = ch_num;	// chnum
		cmd[5] = 0x00;
		cmd[6] = (char)level;	// cmd
		cmd[7] = (char)0;	// proc
		cmd[8] = (char)0;	// amp_len
		cmd[9] = (char)4 << 4; // amp_end
		SendBuff(cmd,20);
}


void Echo2_new::CMD_Set_VRC(int ch_num, int start, int end, int len)
{
		unsigned char cmd[30];

		//out_param[cmd & 0x1F].data.amp_st = (param >> 8) & 0xFF;
		//out_param[cmd & 0x1F].data.amp_end = ((pdata[3] >> 8) & 0xFF);
		//out_param[cmd & 0x1F].data.amp_len = (pdata[3] & 0xFF);

		cmd[0] = 0xAA;
		cmd[1] = 0x55;
		cmd[2] = ch_num;
		cmd[3] = 0xF0;
		cmd[4] = 7;	
		cmd[5] = start;
		cmd[6] = len;	// cmd
		cmd[7] = end;	// proc
		cmd[8] = 0;	// amp_len
		cmd[9] = 0; // amp_end
		SendBuff(cmd,20);
}


//==============================================================================


void Stoika::SetCHData(unsigned short *data, int len)
{
    unsigned short *d = (unsigned short *)&ch_data;
    for(int i=0; i<((len > 64*3) ? 64*3 : len); i+=2) {
        d[i] = data[i+1];
        d[i+1] = data[i];
    }
}

void Stoika::GetCHData(ch_setup *data)
{
    memcpy(data,ch_data,16*6);
}

void Stoika::CMD_SetMAC(unsigned short a1, unsigned short a2, unsigned short a3)
{
    unsigned char buffer[8];
    buffer[0] = 0x7E;
    buffer[1] = 0x70;
    buffer[2] = a1 >> 8;
    buffer[3] = a1;
    buffer[4] = a2 >> 8;
    buffer[5] = a2;
    buffer[6] = a3 >> 8;
    buffer[7] = a3;
    SendCMD(buffer,8);
}

void Stoika::CMD_SetMAC(unsigned a1, unsigned a2)
{
    unsigned char buffer[8];
    buffer[0] = 0x7E;
    buffer[1] = 0x70;
    buffer[2] = a1 >> 8;
    buffer[3] = a1;
    buffer[4] = a2 >> 24;
    buffer[5] = a2 >> 16;
    buffer[6] = a2 >> 8;
    buffer[7] = a2;
    SendCMD(buffer,8);
}

void Stoika::CMD_SetMAC(tMAC_ADDR addr)
{
}

void Stoika::CMD_GetMAC(void)
{
    SendCMD(0x7E,69);
}

void Stoika::ReadMAC(unsigned char *buff, tMAC_ADDR &addr)
{
    addr.a1 = buff[4];
    addr.a2 = buff[5];
    addr.a3 = buff[6];
    addr.a4 = buff[7];
    addr.a5 = buff[0];
    addr.a6 = buff[1];

}


//========================= APPENDED ===========================================

void __fastcall WaitVSync(void) {}

static DWORD StartTickCount;

void InitTimeout(void)
{
  StartTickCount = GetTickCount();
}

void TestTimeout(void)
{
  DWORD t;

  t = GetTickCount();
//  if((t - StartTickCount) > 2000) throw Exception("");

  return;
}

/*!
	\brief Задержка
	\param ms длительность паузы в мСек
*/
void delay(unsigned int ms)
{
	Sleep(ms);
}

/*void Stoika64::CMD64_SendCmd(unsigned proc, unsigned cmd)
{
        pkt.cmd = revers(0xAA5500FD | (proc << 8)); //0xFD0055AA;
        pkt.size = revers((unsigned short)2);
        pkt.addr = revers(0x300UL);
        pkt.data[0] = revers((unsigned long)cmd);
        SendBuff((unsigned char *)&pkt,6+2*4);
        delay(1);
        SendCMD(proc,0xFA); // Start C64xx
}
*/
void Stoika64::CMD64_SendCmd(unsigned proc, unsigned cmd, unsigned param1)
{
    if((proc == 0) || (proc > 15)) return;

        pkt.cmd = revers((unsigned long)0xAA5500FD | (proc << 8)); //0xFD0055AA;
        pkt.size = revers((unsigned short)2);
        pkt.addr = revers(0x300UL);
        pkt.data[0] = revers((unsigned long)cmd);
        pkt.data[1] = revers((unsigned long)param1);
        SendBuff((unsigned char *)&pkt,6+3*4);
        delay(1);
        SendCMD(proc,0xFA); // Start C64xx
}


void Stoika64::CMD64_GetMem(unsigned proc, unsigned addr, unsigned length)
{
    if((proc == 0) || (proc > 15)) return;

        pkt.cmd = revers((unsigned long)0xAA5500FB | (proc << 8)); //0xFD0055AA;
        pkt.size = revers((unsigned short)length);
        pkt.addr = revers((unsigned long)addr);
        SendBuff((unsigned char *)&pkt,6+2*4);
}

/*
void Stoika64::CMD64_SetVRC(unsigned proc, unsigned a_start, unsigned a_end, unsigned a_len)
{
// 	VRC_data[i] = ((i) << 4) | 0x4000; // 0x4000

    unsigned short vrc[50];
    double angle = (double)a_start/((double)a_len+0.000000000001);
//    vrc[0] = 0x0600;
    for(int i=0; i<a_len; i++) {
  		vrc[i] = (((unsigned)(angle*(i+1)+0.5)) << 4) | 0x0000; // 0x4000
    }
    angle = (double)(a_end-a_start)/((double)50-a_len+0.000000000001);
    for(int i=a_len; i<50; i++) {
  		vrc[i] = (((unsigned)(a_start+angle*(i-a_len)+0.5)) << 4) | 0x0000; // 0x4000
    }

//        vrc[49] = 0;
//        vrc[50] = 0;

        pkt.cmd = revers((unsigned long)0xAA5500FD | (proc << 8)); //0xFD0055AA;
        pkt.size = revers((unsigned short)26);
        pkt.addr = revers(0x300UL);
        pkt.data[0] = revers((unsigned long)3);
        unsigned long *p = (unsigned long *)vrc;
        for(int i=1; i<26; i++) {
                pkt.data[i] = revers(*p++);
        }

        SendBuff((unsigned char *)&pkt,6+27*4);
        delay(1);
        SendCMD(proc,0xFA); // Start C64xx

}
*/

void Stoika64::CMD64_SetVRC(unsigned proc, unsigned char * dat)
{
    if((proc == 0) || (proc > 15)) return;

        pkt.cmd = revers(0xAA5500C9 | ((long)proc << 8)); //0xFD0055AA;
        pkt.size = revers((unsigned short)26);
        pkt.addr = revers(0x300UL);
        pkt.data[0] = revers((unsigned long)3);
        unsigned long *p = (unsigned long *)dat;
        for(int i=1; i<14; i++) {
                pkt.data[i] = revers(*p++);
//                 pkt.data[i] = (*p++);
         }

        unsigned char *c = (unsigned char *)&pkt.data[26];

//        for(int i=0; i<50; i++) {
//            *c++ = vrc[i]>>4;
//        }
//        pkt.data[26] = (a_start << 16) + (a_end << 8) + a_len;


        SendBuff((unsigned char *)&pkt,6+27*4);
}

unsigned char VRC[50];

void Stoika64::CMD64_SetVRC(unsigned proc, unsigned a_start, unsigned a_end, unsigned a_len, bool is_RS)
{
// 	VRC_data[i] = ((i) << 4) | 0x4000; // 0x4000

    int cn_num = 50;

    a_start &= 0xFF;
    a_end   &= 0xFF;
    a_len   &= 0x3F;

    if(is_RS) {
	cn_num = 25;
	a_len = a_len/2;
    }

    unsigned char vrc[50];
    double angle = (double)a_start/((double)a_len+0.000000000001);
//    vrc[0] = 0x0600;
    for(int i=0; i<a_len; i++) {
  		vrc[i] = (((unsigned)(angle*(i+1)+0.5))); // << 4) | 0x0000; // 0x4000
    }
    angle = (double)(a_end-a_start)/((double)cn_num-a_len+0.000000000001);
    for(int i=a_len; i<cn_num; i++) {
  		vrc[i] = (((unsigned)(a_start+angle*(i-a_len)+0.5))); // << 4) | 0x0000; // 0x4000
    }

    if(is_RS) {
	for(int i=cn_num; i<50; i++) {
  	    vrc[i] = vrc[cn_num-1];
        }
    }

    memcpy(VRC,vrc,sizeof(vrc));
    CMD64_SetVRC(proc,vrc);


/*
//        vrc[49] = 0;
//        vrc[50] = 0;

        pkt.cmd = revers(0xAA5500C9 | (proc << 8)); //0xFD0055AA;
        pkt.size = revers((unsigned short)26);
        pkt.addr = revers(0x300UL);
        pkt.data[0] = revers((unsigned long)3);
        unsigned long *p = (unsigned long *)vrc;
        for(int i=1; i<14; i++) {
                pkt.data[i] = revers(*p++);
        }

        unsigned char *c = (unsigned char *)&pkt.data[26];

        for(int i=0; i<50; i++) {
            *c++ = vrc[i]>>4;
        }
//        pkt.data[26] = (a_start << 16) + (a_end << 8) + a_len;


        SendBuff((unsigned char *)&pkt,6+27*4+50);
//        delay(1);
//        SendCMD(proc,0xFA); // Start C64xx
*/
}

//Установить частоту зондирующей посылки
void HandDef::SetFreq(unsigned char bChnl, unsigned freq)
{
    unsigned char buf[6] = {4,0,0,0,0,0};

    buf[1] = bChnl;

    if(freq < 2) freq = 2;
    buf[2] = freq & 0xFF;
    buf[3] = (freq >> 8) & 0xFF;

    SendCMD(buf, 6);
}

// Установить частоту оцифровки
void HandDef::SetADCFreq(unsigned char bChnl, unsigned freq)
{
    unsigned char buf[6] = {6,0,0,0,0,0};

    buf[1] = bChnl;

    if(freq < 2) freq = 2;
    if(freq > 149) freq = 149;
    buf[2] = freq & 0xFF;
    buf[3] = (freq >> 8) & 0xFF;

    SendCMD(buf, 6);
};

// Получить результат оцифровки
void HandDef::GetData()
{
    unsigned char buf[6] = {1,0,0,0,0,0};
    SendCMD(buf,6);
};

// Установить ВРЧ
void HandDef::SetVRC(unsigned char bChnl, unsigned char *vrch)
{
    unsigned char buf[60] = {7,0,0,0,0,0};

    buf[1] = bChnl;

    memcpy(&buf[2],vrch,50);

    SendCMD(buf,60);
};

// Задержка после зондирующего
void HandDef::SetDelay(unsigned char bChnl, unsigned delay_ms)
{
    unsigned char buf[6] = {9,0,0,0,0,0};

    buf[1] = bChnl;

    buf[2] = delay_ms;
    buf[3] = (delay_ms >> 8) & 0xFF;

    SendCMD(buf,6);
}

// Установить длину сканирования
void HandDef::SetScanLength(unsigned char bChnl, unsigned len)
{
    unsigned char buf[6] = {10,0,0,0,0,0};

    buf[1] = bChnl;

    buf[2] = len & 0xFF;

    SendCMD(buf,6);
}

// установить данные на портах ввода-вывода
void HandDef::SetIOdata(unsigned char bData)
{
    unsigned char buf[6] = {2,0,0,0,0,0};

    buf[1] = bData;

    SendCMD(buf,6);
}

// Получить результат оцифровки
void HandDef::GetDataLeng(unsigned char bChnl, unsigned uBegin, unsigned uLeng)
{
    unsigned char buf[6] = {1,0,0,0,0,0};

    buf[1] = bChnl;

    buf[2] = uBegin & 0xFF;
    buf[3] = (uBegin >> 8) & 0xFF;
    buf[4] = uLeng & 0xFF;
    buf[5] = (uLeng >> 8) & 0xFF;

    SendCMD(buf,6);
}

void HandDef::GetData(unsigned char bChnl, unsigned uLeng)
{
    unsigned char buf[6] = {1,0,0,0,0,0};

    buf[1] = bChnl;
    buf[2] = uLeng & 0xFF;
    buf[3] = (uLeng >> 8) & 0xFF;

    SendCMD(buf,6);
}

// установить данные на портах ввода-вывода
void HandDef::SetGPIO(unsigned char bChnl, unsigned char bData)
{
    unsigned char buf[6] = {2,0,0,0,0,0};

    buf[1] = bChnl;

    buf[2] = bData & 0xFF;

    SendCMD(buf,6);
}

// установить кол-во импульсов воздуждения
void HandDef::SetImpNum(unsigned char bChnl, unsigned uNum)
{
    unsigned char buf[6] = {12,0,0,0,0,0};

    buf[1] = bChnl;

    buf[2] = uNum & 0xFF;

    SendCMD(buf,6);
}

// установить режим приёма-передачи
void HandDef::SetRcvTrmMode(unsigned char bChnl, bool bOnOff)
{
    unsigned char buf[6] = {13,0,0,0,0,0};
    bool bRez = false;

    unsigned char bChMask = 0x03;   // включены по умолчанию

    if(bChnl == 0)
    {
        if(bOnOff)
            bChMask &= 0xFE;

        bRez = true;
    }
    else if(bChnl == 1)
    {
        if(bOnOff)
            bChMask &= 0xFD;

        bRez = true;
    }

    if(bRez)
    {
        buf[2] = bChMask;
        SendCMD(buf,6);
    }
}



