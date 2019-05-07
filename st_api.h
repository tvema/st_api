//---------------------------------------------------------------------------
#ifndef ST_API_H
#define ST_API_H
#include <windows.h>
#include <math.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define DEST_STREAM_PORT 1457
#define DEST_PKT_PORT 1456
#define ST_PORT 1456

#define ST_IP_ADDR "192.168.1.31"

#define FRAMECAM_MCAST_Group    "239.255.0.1"  ///< MCAST ������ ������� �����
#define SYNCRO_MCAST_GROUP      "239.255.0.2"  ///< MCAST ������ ��������������
#define UZ_MCAST_GROUP          "239.255.0.3"  ///< MCAST ������ �������������� ���
#define LINCAM_MCAST_GROUP      "239.255.0.4"  ///< MCAST ������ �������� �����

#define BASE_PORT	4300

#define STREAM_DATA_PORT	(BASE_PORT + 11)   ///<
#define DATA_PORT	(BASE_PORT + 12)  		   ///< ������������� ������ �� ������
#define CONF_PORT   (BASE_PORT + 13)  		   ///< ������������� �������� �� ������
#define MAC_CONF_PORT (BASE_PORT + 14)
#define SYNC_DATA_PORT (BASE_PORT + 15)  	  ///< ������������� ������ �� ��������������

#define CONF_DISABLE	0			///< ������������� ���������
#define CONF_USER		3			///< ������������� ����������������
#define CONF_1MM		4			///< ������������� 1��
#define CONF_5MM		5			///< ������������� 5��
#define CONF_50MM		6			///< ������������� 50��
#define CONF_100MM		7			///< ������������� 100��


void InitTimeout(void);
void TestTimeout(void);
void delay(unsigned int ms);
//void WaitVSync(void);

#define CMD_ScanA(Proc) CMD_Set_B(Proc)
#define CMD_ScanB(Proc) CMD_Set_A(Proc)

/*!
	\union SyncGroup
	\brief ���������, ����������� ������ � ������� �������������

	 �������� �����:
	-	000	���������
	-	001	������ (���������)
	-	010	2��
	-	011	User*1��
	-	100	1�� 
	-	101	5�� 
	-	110	50��
	-	111	100��

*/
typedef union {
	struct {
		unsigned
            	grp_0:3, ///<	������ �������������� 1-4
		 	   	grp_1:3, ///<	������ �������������� 5-8
			   	grp_2:3, ///<	������ �������������� 9-12
				grp_3:3, ///<	������ �������������� 13-16
				grp_4:3, ///<	������ �������������� 17-20
				grp_5:3, ///<	������ �������������� 21-24
				grp_6:3, ///<	������ �������������� 25-28
				grp_7:3; ///<	������ �������������� 29-32
		} data;
	unsigned all;     ///<  ������ � ������ ����� ����� ������� ������
    unsigned char c[4]; ///< ���������� ������
} SyncGroup;

#pragma pack(push)
#pragma pack(1)

#define Uint32 unsigned


struct POWER_CTRL_BITS {          // bits   description
   Uint32 sw0:1;             // 0      
   Uint32 on0:1;             // 1      
   Uint32 sw1:1;             // 2      
   Uint32 on1:1;             // 3      
   Uint32 sw2:1;             // 4
   Uint32 on2:1;             // 5      
   Uint32 sw3:1;             // 6      
   Uint32 on3:1;             // 7      
   Uint32 sw4:1;             // 8      
   Uint32 on4:1;             // 9      
   Uint32 sw5:1;             // 10      
   Uint32 on5:1;             // 11      
   Uint32 sw6:1;             // 12      
   Uint32 on6:1;             // 13      
   Uint32 sw7:1;             // 14      
   Uint32 on7:1;             // 15      
   Uint32 pwr0:1;            // 16      
   Uint32 pwr1:1;            // 17      
   Uint32 uval0:1;			 // 18
   Uint32 uval1:1;			 // 19
   Uint32 reserved1:7;		 // 20:26
   Uint32 led:1;
   Uint32 reserved2:4;		 // 28:31
};

typedef union POWER_CTRL_REG_ {
   Uint32               all;
   struct POWER_CTRL_BITS   bit;
} POWER_CTRL_REG;

/*!
	\brief ����������� ��������� ��� ������������ ������ � �������������� ���� ��� ������� �� ���� �64��
*/
static struct {
    unsigned long cmd;		///< ��� �������
    unsigned short size;    ///< ������ ���������� ������
    unsigned long addr;     ///< ����� ������ ���������� ������
    unsigned long data[1024]; ///< ������ 
} pkt;

/*!
	\brief ����������� ��������� ��� ������������ ������ � �������������� ���� ��� ������� �� ���� �54��
*/
static struct {
    unsigned long cmd;		///< ��� �������
    unsigned short size;	///< ������ ���������� ������
    unsigned short addr;	///< ����� ������ ���������� ������
    unsigned short data[1024]; ///< ������ 
} pkt16;


/*!
	\struct ch_setup
	\brief ���������, ����������� ��������� ������ ������ ������
	 
*/
typedef struct {
	unsigned short v_end : 8,		///< �������� �������� ���
			 v_start : 8;			///< ��������� �������� ���
	unsigned short 	 porog   : 10,	///< �����
             v_len   : 6;			///< ����� ���
	unsigned short   freq	 : 8,	///< ������� �����������
			 pause	 : 8;			///< �������� ����� ������������
} ch_setup;

typedef struct {
    char a1, a2, a3, a4 , a5, a6;
} tMAC_ADDR;

#pragma pack(pop)

#define T_DATA_CONF 1
#define T_DATA_MAC  2

inline unsigned short revers(unsigned short d)
{
    return (d >> 8) | (d << 8);
}

inline unsigned long revers(unsigned long d)
{
    unsigned char b1,b2,b3,b4;
    b1 = d & 0xFF;
    b2 = (d >> 8) & 0xFF;
    b3 = (d >> 16) & 0xFF;
    b4 = (d >> 24) & 0xFF;

    return ((long)b1 << 24) |((long)b2 << 16) |((long)b3 << 8) |((long)b4);
}

/*!
	\class BaseUDP
	\brief ������� ����� 
	
	���������� �������� ������� ������ � ����
*/

class BaseUDP {
  protected:
    int AddRemoveMcastGroup(int cmd, char *group);

  protected:
    SOCKET UDPSocket;		///<	���������� ������
    char buffer[1024];		///<	���������� �����
    struct sockaddr_in send_addr;  ///< ��������� �����, ��������� � �������
    struct sockaddr_in remote_addr; ///< ����� ��������� ��������� ������
  public:
    BaseUDP(int remote_port = ST_PORT){
        UDPSocket = INVALID_SOCKET;
        send_addr.sin_family =  AF_INET;
    	send_addr.sin_addr.s_addr=inet_addr(ST_IP_ADDR);
	    send_addr.sin_port=htons((u_short )remote_port);
    };

	SOCKET GetSocketID() { return UDPSocket; }
/*!
	\brief �������������� ��������� ����� ��� �������� ������
	\param ip_addr IP ����� ���������� ����������
	\param remote_port IP port ���������� ����������

	�������������� ��������� ����� ��� �������� ������. ��� ����� ��������� � #Init_Lan
*/  	
    BaseUDP(char * ip_addr = ST_IP_ADDR, int remote_port = ST_PORT){
        UDPSocket = INVALID_SOCKET;
        send_addr.sin_family =  AF_INET;
    	send_addr.sin_addr.s_addr=inet_addr(ip_addr);
	    send_addr.sin_port=htons((u_short )remote_port);
    };
  
    ~BaseUDP(void) {
	closesocket(UDPSocket);
        //WSACleanup();
    };

  /*!
  	\brief ��������� ����� 
  */
    void Close() { closesocket(UDPSocket); }

/*!
	\brief �������������� ����� �� �������� �����
	\param port ��������� ����
	\retval 0  ������ �������������
	\retval 1  �������� ������������� 
*/
    int Init_Lan(int port=DEST_PKT_PORT);
    
    int Bind(int local_port);

    SOCKET getSock(void) { return UDPSocket;};
/*!
	\brief ������ ��������� ������� ��� ������ ���������� ������
	\param Handle ����� ����
	\param wMsg ID �������
*/
    void SetEvent(HWND Handle, unsigned int wMsg, long lEvent=FD_READ) {
        WSAAsyncSelect(UDPSocket,Handle,wMsg,lEvent);
    };

/*!
	\brief ������������� ������ �������� ������
	\param bufferSize ������ ������
	\retval true  �������� ���������
	\retval false  ������
*/
	bool SetRxBufferSize(unsigned int bufferSize);
/*!
	\brief �������� ������� ������ �������� ������
	\return ������ �������� ������
*/
    unsigned int GetRxBufferSize();
/*!
	\brief ��������� ��� ��������� �������� ����������������� ������� �� ������
	\param broadcast true, ���� �������� ���������
	\retval true  �������� ���������
	\retval false  ������
*/
    bool SetBroadcast(bool broadcast);

/*!
	\brief ��������� ����� � MCAST ������ addr
	\param addr ����� MCAST ������
*/
    int AddMcastGroup(char *addr) {return AddRemoveMcastGroup(IP_ADD_MEMBERSHIP,addr); };
/*!
	\brief ������� ����� �� MCAST ������ addr
	\param addr ����� MCAST ������

	������ ����������� �� ���� MCAST �������
	\code
		BaseUDP St;
		St->Init_Lan(1456);

		St->AddMcastGroup(FRAMECAM_MCAST_Group);
		St->AddMcastGroup(SYNCRO_MCAST_GROUP);
		St->AddMcastGroup(UZ_MCAST_GROUP);
		St->AddMcastGroup(LINCAM_MCAST_GROUP);


	\endcode
*/
    int DeleteMcastGroup(char *addr) {return AddRemoveMcastGroup(IP_DROP_MEMBERSHIP,addr); };


/*!
	\brief �������� ������� ���������� ����������
	\param proc ����� ������
	\param p1 	�������
	\param p2 	������� ���� ���������
	\param p3   ������� ���� ���������
*/
    int SendCMD(int proc, int p1, int p2=0, int p3=0);
    
/*!
	\brief �������� ��������� ������ � ��������� ���������� ����������
	\param buff ����� ������
	\param len ������ ������
*/
    int SendCMD(unsigned char *buff, int len);
/*!
	\brief �������� ������ ���������� ����������
	\param buff ����� ������
	\param len ������ ������
*/
    int SendBuff(unsigned char *buff, int len);
/*!
	\brief �������� ������ �� ��������� ����� � ����
	\param buff ����� ������
	\param len ������ ������
	\param inet_addr ��������� �����
	\param port ����� ��������� ����
*/
    int SendTo(unsigned char *buff, int len, unsigned long inet_addr, unsigned port);
/*!
	\brief ����� ������ �� ������
	\param data ����� ������
	\param len ������ ������
	\param port ����� ����� ���������
	\return ������ ��������� ������
*/
    int ReceiveData(char *data, unsigned len, unsigned &port) {
        int addr_len = sizeof(remote_addr);
        int res = recvfrom(UDPSocket,data,len,0,(struct sockaddr FAR*)&remote_addr, (int FAR*)&addr_len);
        port = htons((u_short)remote_addr.sin_port);
        int err = WSAGetLastError();
//        if(res < 0) {
//			err=err;
//        }
        return res;
    }
/*!
	\brief ����� ������ �� ������ � ������������ ���������
	\param data ����� ������
	\param len ������ ������
	\param addr ����� ���������
	\param port ����� ����� ���������
	\return ������ ��������� ������
*/
	int ReceiveData(char *data, unsigned len, unsigned &addr, unsigned &port, int &aErr) {
		int addr_len = sizeof(remote_addr);
		int res = recvfrom(UDPSocket,data,len,0,(struct sockaddr FAR*)&remote_addr, (int FAR*)&addr_len);
		addr = remote_addr.sin_addr.S_un.S_addr;
		port = htons((u_short)remote_addr.sin_port);
		int err = WSAGetLastError();
		aErr = err;
        return res;
    }

    int ReceiveDataTimeout(char *data, unsigned len, unsigned &addr, unsigned &port,struct timeval *timeout) {
		fd_set readfds;
		int c;

		FD_ZERO(&readfds);
		FD_SET(UDPSocket, &readfds);
		c = select(1, &readfds, 0, 0, timeout);
		if (c < 0) {
			return -1;
		} else if(c == 0) { 
			/* timeout reached */
			return 0;
		}
		if (FD_ISSET(UDPSocket, &readfds)) {
			int addr_len = sizeof(remote_addr);
			int res = recvfrom(UDPSocket,data,len,0,(struct sockaddr FAR*)&remote_addr, (int FAR*)&addr_len);
			addr = remote_addr.sin_addr.S_un.S_addr;
			port = htons((u_short)remote_addr.sin_port);
			int err = WSAGetLastError();
			return res;
		} else {
			return 0;
		}
    }

};

class PowerConf : public BaseUDP {
  public:
/*!
	\sa BaseUDP
*/
    PowerConf(int remote_port = 13490) : BaseUDP(remote_port) {};
/*!
	\sa BaseUDP
*/
    PowerConf(char * ip_addr, int remote_port = 13490) : BaseUDP(ip_addr,remote_port){};

/*!
	\brief ����������� ������� ������������ ����������� �������
	���������� ������� �������� �� ���� 1456 ��� DWORD, ���������� ��������� POWER_CTRL_REG;
	� ������ ��������� ��������� ������� ��������� �������, �� ������ ��������� � ����� onX ��������� ���������
	������ (��) ����� ��������� �����������. ���� ��������������� ��� onX ����������, �� �� ������ ������ ��� ���������
	����������� �������������� �� �� �������

*/
	void GetConfig() { SendCMD(0x7E,0x2); };
/*!
	\brief ��������� � EEPROM ������� ������������ ����������� �������
*/
    void SaveConfig() { SendCMD(0x7E,0x3); };
/*!
	\brief ������������� ������������ ����������� �������
*/
    void SetConfig(Uint32 Config) {
        union data {
            unsigned long all;
            unsigned char byte[4];
        };
        union data c;
        c.all = Config;
       	buffer[0] = 0xAA;
    	buffer[1] = 0x55;
    	buffer[2] = (char)0;
    	buffer[3] = (char)1;  // Write SyncData
    	buffer[4] = (char)c.byte[0];  // 1
    	buffer[5] = (char)c.byte[1];  // 0
    	buffer[6] = (char)c.byte[2];  // 1
    	buffer[7] = (char)c.byte[3];  // 0
        SendBuff((unsigned char*)buffer,8);
    }
};


/*!
	\class MasterSync
	\brief ����� ���������� ���������������
*/

class MasterSync : public BaseUDP {
  public:
/*!
	\sa BaseUDP
*/
    MasterSync(int remote_port = ST_PORT) : BaseUDP(remote_port) {};
/*!
	\sa BaseUDP
*/
    MasterSync(char * ip_addr, int remote_port = ST_PORT) : BaseUDP(ip_addr,remote_port){};
//    ~MasterSync(void){};

/*!
	\brief ��������� ����� � MCAST ������ ��������������
*/
	int AddMcast() {return AddRemoveMcastGroup(IP_ADD_MEMBERSHIP,SYNCRO_MCAST_GROUP/*UZ_MCAST_GROUP*/); };
/*!
	\brief ������� ����� �� MCAST ������ ��������������
*/
    int DeleteMcast() {return AddRemoveMcastGroup(IP_DROP_MEMBERSHIP,SYNCRO_MCAST_GROUP); };


/*!
	\brief ���������/���������� ������� �������������
	\param status 1 - ������� ������������� ��������
*/
    void CMD_ExtSync_OnOff(int status) { SendCMD(0x7e,6,0, status); }
/*!
	\brief ��������� ��������
	\param div ��������

	��������� �������� � ��������������. �������� ������� = ������� ������� *(div/100)

	�.�. ���� div <100, �� ������� �������, ���� >100, �� ����������

	\attention ��� ���������� ������������� �� ������������� ������� �������� ������� = ������� �������*(div/\b 10000)
*/
    void CMD_Div(int div)              { SendCMD(0x7E,0x10,div >> 8, div); }

/*!
	\brief ������������ ����� �������������
	\param onoff
		
	���� onoff=0, �� ������������� ��������� ������ �������
	���� onoff=1, �� ������������� ��������� �������� ����������. ��� ���� �������� �������� � ������
	�������� �������� ������������ ��������� (������ CMD_Div()) �� �� 100, � �� 10000

*/

    void CMD_Quadr_OnOff(int onoff)      { SendCMD(0x7E,81,0, onoff); }

/*!
	\brief ��������� �������� COM �����
	\param speed
		
	\code
	speed = 0   �������� 2400
	speed = 1   �������� 4800	
	speed = 2   �������� 9600	
	speed = 3   �������� 19200	
	speed = 4   �������� 38400
	\endcode

*/
    void CMD_Set_COM_Speed(int speed)      { SendCMD(0x7,82,0, speed); }


/*!
	\brief ������������� ������� ����������������� ���� �������������
	\param div 
		
*/

	void CMD_SetUserDivider(int div)      { SendCMD(13,82,0, div); }


/*!
	\brief ������ ������������ ������� �������������� � EEPROM
*/
    void CMD_SaveConfig() {
       	buffer[0] = 0xAA;
    	buffer[1] = 0x55;
    	buffer[2] = (char)0;
    	buffer[3] = (char)78;  // Write SyncData
    	buffer[4] = (char)0;  // 1
    	buffer[5] = (char)0;  // 0
        SendBuff((unsigned char*)buffer,6);
    }

/*!
	\brief ����������� ������������ ��������������

	����������� ������������ ��������������. � �������� ������, ���������� �� #DEST_PKT_PORT (1456) ����������
	������������ �������, ������ ������������� (�������/����������), ��������. ������ ������ ����� �����������
	�������� #ParsePacket

	������ word � �������� ������ ������ ��������������� ������. � �������� �� ������������� ������ �� �����
	#SYNC_DATA_PORT

	������ ������� �������� �������
\code
		MasterSync Syn;

		...

        unsigned port;
        unsigned ip_addr;
        unsigned short inData[10240];
	    int llen = Syn->ReceiveData((char *)inData,10240,ip_addr,port);
	    switch(inData[0]) {
    	    case CONF_PORT: {
    	    	...
    	    	break;
    	    }
    	    case SYNC_DATA_PORT: {
                  SyncGroup SyncData;
                  bool is_ext_sync;
                  unsigned divider;
	      		  Syn->ParsePacket(inData, SyncData,is_ext_sync,divider);
    	    	break;
    	    }
    	}
\endcode


*/
	void CMD_GetConfig() {
		buffer[0] = 0xAA;
		buffer[1] = 0x55;
		buffer[2] = (char)0;
		buffer[3] = (char)76;   // Get SyncData
		buffer[4] = (char)0;  // 1
		buffer[5] = (char)0;  // 0
		SendBuff((unsigned char*)buffer,6);
	}
	void CMD_GetNewConfig() {
		buffer[0] = 0xAA;
		buffer[1] = 0x55;
		buffer[2] = (char)0;
		buffer[3] = (char)74;   // Get SyncData
		buffer[4] = (char)0;  // 1
		buffer[5] = (char)0;  // 0
		SendBuff((unsigned char*)buffer,6);
	}

/*!
	\brief ������������� ����� ������������ ������� ��������������

	������ ������� �������� �������
\code
		MasterSync Syn;
		SyncGroup SyncData;
		. . .
		SyncData.data.grp_0 = CONF_1MM;
		SyncData.data.grp_1 = CONF_1MM;
		SyncData.data.grp_2 = CONF_5MM;
		SyncData.data.grp_3 = CONF_5MM;
		SyncData.data.grp_4 = CONF_50MM;
		SyncData.data.grp_5 = CONF_50MM;
		SyncData.data.grp_6 = CONF_100MM;
		SyncData.data.grp_7 = CONF_100MM;

		SYN->CMD_SetConfig(SyncData.all);

\endcode
*/
	void CMD_SetConfig(unsigned& config)
	{
		unsigned char *c = (unsigned char *)&config;
		buffer[0] = 0xAA;
		buffer[1] = 0x55;
		buffer[2] = (char)0;
		buffer[3] = (char)75;   // Set SyncData
		buffer[4] = (char)c[1];  // 1
		buffer[5] = (char)c[0];  // 0
		buffer[6] = c[3];        // 3
		buffer[7] = c[2];        // 2
		SendBuff((unsigned char*)buffer,8);
	}

	void CMD_SetNewConfig(short d1, short d2, short d3, short d4 )
	{
	//	unsigned char *c = (unsigned char *)&config;
		short *b = (short *)&buffer[4];
		buffer[0] = 0xAA;
		buffer[1] = 0x55;
		buffer[2] = (char)0;
		buffer[3] = (char)73;   // Set SyncData
		b[0] = d1;        // 2
		b[1] = d2;
		b[2] = d3;
		b[3] = d4;
		SendBuff((unsigned char*)buffer,12);
	}

/*!
	\brief ���������/���������� ��������� ������������� � �������������
	\param enable_sync 1 - ��������� ��������
	\param start_counter - ��������� �������� �������� �������
*/
	void CMD_StartSync(unsigned enable_sync, unsigned start_counter)
	{
		unsigned char *b = (unsigned char *)&start_counter;
		buffer[0] = 0xAA;
		buffer[1] = 0x55;
		buffer[2] = (char)0;
		buffer[3] = (char)79;   // Start Syncro
		buffer[4] = (char)0;  // 1
		buffer[5] = (char)enable_sync;  // 0
		buffer[6] = (char)b[1];
		buffer[7] = (char)b[0];
		buffer[8] = (char)b[3];
		buffer[9] = (char)b[2];
		SendBuff((unsigned char*)buffer,10);
	}

/*!
	\brief ��������� ���������� �������� �������� �������
	\param start_counter - ��������� �������� �������� �������
*/
	void CMD_SetCounter(unsigned start_counter)
	{
		unsigned char *b = (unsigned char *)&start_counter;
		buffer[0] = 0xAA;
		buffer[1] = 0x55;
		buffer[2] = (char)0;
		buffer[3] = (char)80; // Set Counter
		buffer[4] = (char)0;  // 1
		buffer[5] = (char)0;  // 0
		buffer[6] = (char)b[1];
		buffer[7] = (char)b[0];
		buffer[8] = (char)b[3];
		buffer[9] = (char)b[2];
		SendBuff((unsigned char*)buffer,10);
	}

/*!
	\brief ������ ��������� ����������������� ������ �� ��������������
	\param inData ����� ������
	\param SyncData ������������ �������
	\param is_ext_sync ������ ������� ������������� (�������� - true)
	\param is_sync_enable ������ ������ ������������� (�������� - true)
	\param divider �������� ��������
	\return �������� ������ ������� ��������������
		���� ������
		\li \b	0 ������� �������������
		\li \b	1 ������ ������������� ���������
		\li \b	2 ������������� ������������ ��������

*/
	unsigned ParsePacket(unsigned short *inData, SyncGroup &SyncData, bool &is_ext_sync, bool &is_sync_enable, unsigned &divider)
	{
			unsigned char *buf = (unsigned char *) inData;
			SyncData.c[1] = buf[7];  // 1
			SyncData.c[0] = buf[6];  // 0
			SyncData.c[3] = buf[5];  // 3
			SyncData.c[2] = buf[4];  // 2

			is_ext_sync    = (inData[4] & 1);
			is_sync_enable = (inData[4] & 2);

			divider = inData[5];

			return inData[4];
/*
	// Speed
			int sp = inData[6] << 16 | inData[7];
			double s = 36000.0/(((double)sp/25.0)*2.0);
			l1->Caption  = "Nei?inou: " + AnsiString(s);
*/
	}
	unsigned ParseNewPacket(unsigned short *inData, bool &is_ext_sync, bool &is_sync_enable, unsigned &divider, short *cnt)
	{
			unsigned char *buf = (unsigned char *) inData;
 //			SyncData.c[1] = buf[7];  // 1
 //			SyncData.c[0] = buf[6];  // 0
 //			SyncData.c[3] = buf[5];  // 3
 //			SyncData.c[2] = buf[4];  // 2

			is_ext_sync    = (inData[4] & 1);
			is_sync_enable = (inData[4] & 2);

			divider = inData[5];

			cnt[0] = inData[12];
			cnt[1] = inData[13];
			cnt[2] = inData[14];
			cnt[3] = inData[15];

			return inData[4];
/*
	// Speed
			int sp = inData[6] << 16 | inData[7];
			double s = 36000.0/(((double)sp/25.0)*2.0);
			l1->Caption  = "Nei?inou: " + AnsiString(s);
*/
	}

};

/*!
	\class CommonCam
	\brief ������� ����� ��� �����
*/

class CommonCam : public BaseUDP {
  protected:
	CommonCam(int remote_port = ST_PORT) : BaseUDP(remote_port) {};
	CommonCam(char * ip_addr, int remote_port = ST_PORT) : BaseUDP(ip_addr,remote_port){};
  public:
/*!
	\brief ������������� ������ ������� �����
	\param size ������ ������� �����
*/
	void SetCompressedSize(unsigned size)   { SendCMD(0,0xC4,size >> 8,size); }
};

//======================================================================================================
//======================================================================================================

/*!
	\class LinCam
	\brief ��������� � �������� �������
*/

class LinCam : public CommonCam {
  public:
/*!
	\sa BaseUDP
*/
    LinCam(int remote_port = ST_PORT) : CommonCam(remote_port) {};
/*!
	\sa BaseUDP
*/
    LinCam(char * ip_addr, int remote_port = ST_PORT) : CommonCam(ip_addr,remote_port){};
//    ~MasterSync(void){};

/*!
	\brief ��������� ����� � MCAST ������ �������� �����
*/
    int AddMcast() {return AddRemoveMcastGroup(IP_ADD_MEMBERSHIP,LINCAM_MCAST_GROUP); };
/*!
	\brief ������� ����� �� MCAST ������ �������� �����
*/
    int DeleteMcast() {return AddRemoveMcastGroup(IP_DROP_MEMBERSHIP,LINCAM_MCAST_GROUP); };

/*!
	\brief ���������/���������� ������� �������������
	\param status 1 - ������� ������������� ��������
*/
    void CMD_ExtSync_OnOff(int status) { SendCMD(0x7e,6,0, status); }

/*!
	\brief ��������� ����������
	\param expos ������������ ���������� (��� - 0,02 �����, ����������� ������������ 50 �����)
*/
    void SetExposition(unsigned expos) { SendCMD(0x7e,0xFC,expos >> 8, expos); }
/*!
	\brief ��������� �������� ����� �� ������� (1 - 754)
	\param offset - ��������
*/
    void SetStartOffset(int offset) { SendCMD(0x7F,0xc5,offset >> 8, offset); }

/*!
	\brief ������������� ������ ������� �����
	\param size ������ ������� �����
*/
    void SetCompressedSize(unsigned size)   { SendCMD(0,0xC4,size >> 8,size); }

/*!
	\brief ��������� ������ ������� ������ ��������
	\param amplify ��������
*/
    void SetEvenBlackLevel(int amplify) { SendCMD(0x7F,0xED,amplify >> 8, amplify); }
/*!
	\brief ��������� �������� ������ ��������
	\param amplify ��������
*/
    void SetEvenAmp(int amplify) { SendCMD(0x7F,0xFD,amplify >> 8, amplify); }
/*!
	\brief ��������� �������� �������� ��������
	\param amplify ��������
*/
    void SetOddAmp(int amplify) { SendCMD(0x7F,0xFB,amplify >> 8, amplify); }

/*!
	\brief ��������� ������ ������� �������� ��������
	\param amplify ��������
*/
    void SetOddBlackLevel(int amplify) { SendCMD(0x7F,0xEB,amplify >> 8, amplify); }



/*!
	\brief Zoom IN (��� ���������� � �����)
*/
    void ZoomIn(void) { SendCMD(0x7F,0xEA,0, 3); }
/*!
	\brief Zoom OUT (��� ���������� � �����)
*/
    void ZoomOut(void) { SendCMD(0x7F,0xEA,0, 4); }
/*!
	\brief Focus IN (��� ���������� � �����)
*/
    void FocusIn(void) { SendCMD(0x7F,0xEA,0, 1); }
/*!
	\brief Focus Out (��� ���������� � �����)
*/
    void FocysOut(void) { SendCMD(0x7F,0xEA,0, 2); }
/*!
	\brief Iris IN (��� ���������� � �����)
*/
    void IrisIn(void) { SendCMD(0x7F,0xEA,0, 5); }
/*!
	\brief Iris Out (��� ���������� � �����)
*/
    void IrisOut(void) { SendCMD(0x7F,0xEA,0, 6); }


};

//======================================================================================================
//======================================================================================================
/*!
	\class FrameCam
	\brief ��������� � ��������� �������
*/

class FrameCam : public CommonCam {
  public:
/*!
	\sa BaseUDP
*/
    FrameCam(int remote_port = ST_PORT) : CommonCam(remote_port) {};
/*!
	\sa BaseUDP
*/
    FrameCam(char * ip_addr, int remote_port = ST_PORT) : CommonCam(ip_addr,remote_port){};
//    ~MasterSync(void){};

/*!
	\brief ��������� ����� � MCAST ������ ��������� �����
*/
    int AddMcast() {return AddRemoveMcastGroup(IP_ADD_MEMBERSHIP,FRAMECAM_MCAST_Group); };
/*!
	\brief ������� ����� �� MCAST ������ ��������� �����
*/
    int DeleteMcast() {return AddRemoveMcastGroup(IP_DROP_MEMBERSHIP,FRAMECAM_MCAST_Group); };

///*!
//	\brief ������������� ������ ������� �����
//	\param size ������ ������� �����
//*/
//    void SetCompressedSize(unsigned size)   { SendCMD(0,0xC4,size >> 8,size); }

};

//======================================================================================================
//======================================================================================================


/*!
	\class NewLinCam
	\brief ��������� � ����� �������� �������
*/

class NewLinCam : public CommonCam {
  public:
/*!
	\sa BaseUDP
*/
    NewLinCam(int remote_port = ST_PORT) : CommonCam(remote_port) {};
/*!
	\sa BaseUDP
*/
    NewLinCam(char * ip_addr, int remote_port = ST_PORT) : CommonCam(ip_addr,remote_port){};
//    ~MasterSync(void){};

/*!
	\brief ��������� ����� � MCAST ������ �������� �����
*/
    int AddMcast() {return AddRemoveMcastGroup(IP_ADD_MEMBERSHIP,LINCAM_MCAST_GROUP); };
/*!
	\brief ������� ����� �� MCAST ������ �������� �����
*/
    int DeleteMcast() {return AddRemoveMcastGroup(IP_DROP_MEMBERSHIP,LINCAM_MCAST_GROUP); };

/*!
	\brief ���������/���������� ������� �������������
	\param status 1 - ������� ������������� ��������
*/
    void CMD_ExtSync_OnOff(int status) { SendCMD(0x7e,6,0, status); }

/*!
	\brief ��������� ����������
	\param expos ������������ ���������� (������������ ���������� 37 ����� + expos*2 �����, ��� - 0,5 ����� )
*/
    void SetExposition(unsigned expos) { SendCMD(0x7e,0xFC,expos, expos >> 8); }

/*!
	\brief ��������� ������ ������� ������
	\param sft - ����� ������� ������ ����� �� �������� ����� ���
*/
    void SetShift(unsigned sft) { SendCMD(0x7e,3,sft*2, 0); } // expos >> 8); }

/*!
	\brief ��������� �������� ����� �� ������� (1 - 754)
	\param offset - ��������
*/
//    void SetStartOffset(int offset) { SendCMD(0x7F,0xc5,offset >> 8, offset); }

/*!
	\brief ������������� ������ ������� �����
	\param size ������ ������� �����
*/
//    void SetCompressedSize(unsigned size)   { SendCMD(0,0xC4,size >> 8,size); }

/*!
	\brief ��������� ������ ������� ������ ��������
	\param amplify ������� ������� (0..1023) 
*/
    void SetBlackLevel(int amplify) { 
  unsigned k = 0;
  unsigned Length = amplify;
    if(Length & 1) k |= 1; Length = Length >> 1; k = k << 1;
    if(Length & 1) k |= 1; Length = Length >> 1; k = k << 1;
    if(Length & 1) k |= 1; Length = Length >> 1; k = k << 1;
    if(Length & 1) k |= 1; Length = Length >> 1; k = k << 1;
    if(Length & 1) k |= 1; Length = Length >> 1; k = k << 1;
    if(Length & 1) k |= 1; Length = Length >> 1; k = k << 1;
    if(Length & 1) k |= 1; Length = Length >> 1; k = k << 1;
    if(Length & 1) k |= 1; Length = Length >> 1; k = k << 1;
    if(Length & 1) k |= 1; Length = Length >> 1; k = k << 1;
    if(Length & 1) k |= 1; Length = Length >> 1; k = k << 1;
    if(Length & 1) k |= 1; Length = Length >> 1; k = k << 1;
    if(Length & 1) k |= 1;

    Length = k;

	SendCMD(0x7F,0xFD,Length,Length >> 8); 
}

/*!
	\brief ��������� �������� ������ ��������
	\param amplify �������� (0..255)
*/
    void SetAmp(int amplify) { 

  unsigned k = 0;
  unsigned Length = amplify;
	if(Length & 1) k |= 1; Length = Length >> 1; k = k << 1;
    if(Length & 1) k |= 1; Length = Length >> 1; k = k << 1;
    if(Length & 1) k |= 1; Length = Length >> 1; k = k << 1;
    if(Length & 1) k |= 1; Length = Length >> 1; k = k << 1;
    if(Length & 1) k |= 1; Length = Length >> 1; k = k << 1;
    if(Length & 1) k |= 1; Length = Length >> 1; k = k << 1;
    if(Length & 1) k |= 1; Length = Length >> 1; k = k << 1;
    if(Length & 1) k |= 1; Length = Length >> 1; k = k << 1;
    if(Length & 1) k |= 1; Length = Length >> 1; k = k << 1;
    if(Length & 1) k |= 1; Length = Length >> 1; k = k << 1;
    if(Length & 1) k |= 1; Length = Length >> 1; k = k << 1;
    if(Length & 1) k |= 1; Length = Length >> 1; k = k << 1;
    if(Length & 1) k |= 1;

    Length = k;

	SendCMD(0x7F,0xFB,Length,Length >> 8); 
}

/*!
	\brief ��������� ����������� ����
	\param prec   0..2
		\li \b	0 8-bit
		\li \b	1 10-bit
		\li \b	2 12-bit
*/
    void SetPrec(int prec) { SendCMD(0x7F,0xF0,prec,0); }

/*!
	\brief ��������� �������� �������� ��������
	\param amplify ��������
*/


/*!
	\brief Zoom IN (��� ���������� � �����)
*/
    void ZoomIn(void) { SendCMD(0x7F,0xEA,0, 3); }
/*!
	\brief Zoom OUT (��� ���������� � �����)
*/
    void ZoomOut(void) { SendCMD(0x7F,0xEA,0, 4); }
/*!
	\brief Focus IN (��� ���������� � �����)
*/
    void FocusIn(void) { SendCMD(0x7F,0xEA,0, 1); }
/*!
	\brief Focus Out (��� ���������� � �����)
*/
    void FocysOut(void) { SendCMD(0x7F,0xEA,0, 2); }
/*!
	\brief Iris IN (��� ���������� � �����)
*/
    void IrisIn(void) { SendCMD(0x7F,0xEA,0, 5); }
/*!
	\brief Iris Out (��� ���������� � �����)
*/
    void IrisOut(void) { SendCMD(0x7F,0xEA,0, 6); }


};

//======================================================================================================
//======================================================================================================

class CommonStoika : public BaseUDP
{
  protected:
    CommonStoika(int remote_port = ST_PORT) : BaseUDP(remote_port) {};
    CommonStoika(char * ip_addr, int remote_port = ST_PORT) : BaseUDP(ip_addr,remote_port){};

  public:
/*!
	\brief ���������/���������� ����������� ������� �������������
	\param status < 5 - ���������� ������ ��������
		   status > 5 - ���������� ������� == 90000000/(900*param)/5
		                 status = 5  -- 4000 Hz
		                 status = 10 -- 2000 Hz

*/
    void CMD_Timer_OnOff(int status)   { SendCMD(0x7e,5,0, status);  }
/*!
	\brief ���������/���������� ������� �������������
	\param status 1 - ������� ������������� ��������
*/
    void CMD_ExtSync_OnOff(int status) { SendCMD(0,6,0, status);  }
};

typedef struct {
		unsigned char amp_end;
		unsigned char amp_start;
		unsigned char amp_len;
		unsigned char Pulse_RPT1;
		unsigned char is_RS;
		unsigned short delay;
} Echo2_ChParam;


class Echo2 : public CommonStoika
{
  public:

	Echo2(int remote_port = 13490) : CommonStoika(remote_port) {};
    Echo2(char * ip_addr, int remote_port = 13490) : CommonStoika(ip_addr,remote_port){};

    virtual void CMD_Set_Pause(int ch_num,int pause);
    virtual void CMD_Set_VRC(int ch_num,int start, int end, int len);
    virtual void CMD_Set_Imp1(int ch_num,int len);
	virtual void CMD_SetRS(int ch_num, int OnOff);
	virtual void CMD_TakeScanA(int ch_num)				{ SendCMD(0,20, ch_num,0);	   };
    void CMD_Timer_OnOff(int status)			{ SendCMD(0,5,status, 0);  }
	void ParseParam(unsigned char *data, Echo2_ChParam *param);
	void CMD_SetUserData(unsigned index, unsigned data);
};

class Echo2_new : public Echo2
{
	public:
	Echo2_new(int remote_port = 13490) : Echo2(remote_port) {};
    Echo2_new(char * ip_addr, int remote_port = 13490) : Echo2(ip_addr,remote_port) {};

	virtual void CMD_Set_Pause(int ch_num,int pause);
	virtual void CMD_Set_VRC(int ch_num,int start, int end, int len);
	virtual void CMD_Set_Imp1(int ch_num,int len);
	virtual void CMD_SetRS(int ch_num, int OnOff);
	void CMD_Set_ImpNum(int ch_num,int num);
	void CMD_Set_floor(int ch_num, int num);

	void CMD_SetZero(int ch_num,int level);
};

/*!
	\brief ��������� � ������ �������������� ������
*/
class Stoika : public CommonStoika {

  private:
    ch_setup ch_data[64];

  public:
/*!
	\sa BaseUDP
*/
    Stoika(int remote_port = ST_PORT) : CommonStoika(remote_port) {};
/*!
	\sa BaseUDP
*/
    Stoika(char * ip_addr, int remote_port = ST_PORT) : CommonStoika(ip_addr,remote_port){};
//    ~Stoika(void){};

    void CMD_Set_A(int proc)                { SendCMD(proc,1); }
    void CMD_Set_B(int proc)                { SendCMD(proc,2); }
    void CMD_Set_Porog(int proc,int porog)  { SendCMD(proc,3,porog >> 8, porog); }
    void CMD_Set_ContactPorog(int proc,int porog) { SendCMD(proc,61,porog >> 8, porog); }
    void CMD_Set_Pause(int proc,int pause)  { SendCMD(proc,4,pause >> 8, pause); }
    void CMD_Set_VRC(int proc,int start, int end, int len) { SendCMD(proc,11,0, start); Sleep(30); SendCMD(proc,7,end, len); }
    void CMD_Set_Imp(int proc,int len)      { SendCMD(proc,8,0, len); }
    void CMD_Set_Imp1(int proc,int len)     { SendCMD(proc,9,0, len); }
    void CMD_SendPulse(void)                { SendCMD(0x7E,10); }
    void CMD_Board_On(int proc)             { SendCMD(proc,0x20); }
    void CMD_Board_Off(int proc)            { SendCMD(proc,0x21); }
    void CMD_SetLength(int proc, int Length){ SendCMD(proc,52,Length >> 8, Length); }
    void CMD_MakeScanA(int proc)            { SendCMD(0x7F,54); }
    void CMD_TakeScanA(int proc)            { SendCMD(proc,53); }
    void CMD_Div(float Val)                 { int div = floor(Val * 16 + 0.5); SendCMD(0x7E,0x10,div >> 8, div); }
    void CMD_Filter(bool OnOff)             { SendCMD(0x7F,0x12,0, OnOff ? 1 : 0); }
    void CMD_SetRS(int proc, int OnOff)     { SendCMD(0x7F,0x12,0, OnOff); }
    void CMD_SaveToFlash(void)              { SendCMD(0x7E,0x11);   }
    void CMD_SetAmpValue(int proc, unsigned value, unsigned ind) {
        int val = (value > 0xFF) ? 0xFF : value; int i = (ind > 49) ? 49 : ind; SendCMD(0x7E,41,i, val);
    }

    void CMD_SetMAC(unsigned short a1, unsigned short a2, unsigned short a3);
    void CMD_SetMAC(unsigned a1, unsigned a2);
    void CMD_SetMAC(tMAC_ADDR addr);
    void CMD_GetMAC(void);
    void ReadMAC(unsigned char *buff, tMAC_ADDR &addr);

    #define CMD_SaveData(x) CMD_SaveToFlash()

    void GetCHData(ch_setup *);
    void SetCHData(unsigned short *data, int len);
    void SetChParams(int proc, int step,int start, int stop);

};

/*!
	\class Stoika64
	\brief ��������� � ����� �������������� ����
*/
class Stoika64 : public CommonStoika
{
  public:
/*!
	\sa BaseUDP
*/
    Stoika64(int remote_port = ST_PORT) : CommonStoika(remote_port) {};
/*!
	\sa BaseUDP
*/
    Stoika64(char * ip_addr, int remote_port = ST_PORT) : CommonStoika(ip_addr,remote_port){};

/*!
	\brief ��������� ����� � MCAST ������ �����������
*/
    int AddMcast() {return AddRemoveMcastGroup(IP_ADD_MEMBERSHIP,UZ_MCAST_GROUP); };
/*!
	\brief ������� ����� �� MCAST ������ �����������
*/
    int DeleteMcast() {return AddRemoveMcastGroup(IP_DROP_MEMBERSHIP,UZ_MCAST_GROUP); };



//    void CMD64_SendCmd(unsigned proc, unsigned cmd);
/*!
	\brief ������� ������� � �����
	\param proc ����� ������ (��������� �� 1)
	\param cmd  ��� �������
	\param param1 �������� (������� �� �������)
*/
    void CMD64_SendCmd(unsigned proc, unsigned cmd, unsigned param1=0);
/*!
	\brief �������� ������� ������ �� ������
	\param proc ����� ������ (��������� �� 1)
	\param addr  ����� ������� ������
	\param length ����� ������� ������
*/
    void CMD64_GetMem(unsigned proc, unsigned addr, unsigned length);
/*!
	\brief ������� ��������� ������� ������� proc
	\param proc ����� ������ (��������� �� 1)
	
	������� �������� ��������� ������� � ������ proc. ��������� ������������ �������� � ������ 
	�� ������ 0x1000 ������ 250 ����� ����� �������

	������� ����������� ��������� � �������� #CMD64_GetMem

\code
		Stoika64 St;
		
		...
		
        St->CMD64_SendOnePulse(n);
        delay(1);
	    St->CMD64_GetMem(n,0x1000,2048);
\endcode
	    
*/
    void CMD64_SendOnePulse(unsigned proc)                  { CMD64_SendCmd(proc, 4); }
/*!
	\brief ���������� ������� ����������� �������
	\param proc ����� ������ (��������� �� 1)
	\param freq ������� (����������� �������� 10-15)
*/
    void CMD64_SetFreq(unsigned proc, unsigned freq)        { CMD64_SendCmd(proc, 5,freq); }
/*!
	\brief ��������/��������� �������� ������ � ������
	\param proc ����� ������ (��������� �� 1)
	\param filt �������� ������� (0 - ��������)
*/
    void CMD64_EnableFilter(unsigned proc, unsigned filt)   { CMD64_SendCmd(proc, 6,filt); }
/*!
	\brief ������������� �������� �������� ���������
	\param proc ����� ������ (��������� �� 1)
	\param mode ����� ������������
	- 0 Set 1.56 mSec interval
	- 1 Set 0.78 mSec interval
*/
    void CMD64_SetScanInterval(unsigned proc, unsigned mode){ CMD64_SendCmd(proc, 7,mode); } // mode == 0 Set 1.56 mSec interval
                                                                                             // mode == 1 Set 0.78 mSec interval
/*!
	\brief ������������� ����� RS ��� ������ 
	\param proc ����� ������ (��������� �� 1)
	\param mode ����� RS
	- 0 ������� �����
	- 1 RS ����� ������
*/
    void CMD64_SetRS(unsigned proc, unsigned mode)          { CMD64_SendCmd(proc, 8,mode); }
/*!
	\brief ������������� ��� ������
	\param proc ����� ������ (��������� �� 1)
	\param a_start ��� � ������
	\param a_end   ��� � �����
	\param a_len   ����� ���
*/
    void CMD64_SetVRC(unsigned proc, unsigned a_start, unsigned a_end, unsigned a_len, bool is_RS=false);
/*!
	\brief ������������� ��� ������
	\param proc ����� ������ (��������� �� 1)
	\param data 50 ���� �������� ���
*/
    void CMD64_SetVRC(unsigned proc, unsigned char * data);
};

//---------------------------------------------------------------------------

#define NEW_CH_PORT 27000

class MultiCH : public CommonStoika
{

  private:
//    ch_setup ch_data[64];

  public:
/*!
	\sa BaseUDP
*/
    MultiCH(int remote_port = NEW_CH_PORT) : CommonStoika(remote_port) {};
/*!
	\sa BaseUDP
*/
    MultiCH(char * ip_addr, int remote_port = NEW_CH_PORT) : CommonStoika(ip_addr,remote_port){};

/*!
	\brief ��������� ����� � MCAST ������ �����������
*/
    int AddMcast() {return AddRemoveMcastGroup(IP_ADD_MEMBERSHIP,UZ_MCAST_GROUP); };
/*!
	\brief ������� ����� �� MCAST ������ �����������
*/
    int DeleteMcast() {return AddRemoveMcastGroup(IP_DROP_MEMBERSHIP,UZ_MCAST_GROUP); };

/*!
	\brief ������� ������� � �����
	\param cmd  ��� �������
	\param param1 �������� (������� �� �������)
*/
//    void SendCmd(unsigned addr, unsigned cmd, unsigned param1=0, unsigned param2 = 0, unsigned param3 = 0);

//    void SetVRC(unsigned addr, unsigned a_start, unsigned a_end, unsigned a_len);
/*!
	\brief ������������� ��� ������
	\param data 50 ���� �������� ���
*/
	void SetVRC(unsigned addr,unsigned char bChnl, unsigned char *vrch) {
			unsigned char buf[60] = {7,0,0,0,0,0};
		    buf[1] = bChnl; memcpy(&buf[2],vrch,50);
			SendTo(buf,60,addr, NEW_CH_PORT);
	};


/*!
	\brief ���������� ������� ����������� �������
	\param freq ������� (����������� �������� 10-15)
*/
    void SetFreq(unsigned addr, unsigned char bChnl, unsigned freq)
    {
        unsigned char buf[4] = {4,0,0,0};
        if(freq < 2) freq = 2;
        buf[2] = freq & 0xFF;
        buf[3] = (freq >> 8) & 0xFF;
        SendTo(buf, 4,addr,NEW_CH_PORT);
    };

/*!
	\brief ���������� ������� ���������
	\param freq - �������� ��������
      ������� ������������ ��� (150/(freq+1))
*/
	void SetADCFreq(unsigned addr, unsigned char bChnl, unsigned freq)
    {
        unsigned char buf[4] = {6,0,0,0};
        int t = freq;
        buf[2] = t & 0xFF;
        buf[3] = (t >> 8) & 0xFF;
        SendTo(buf,4,addr,NEW_CH_PORT);
    };

/*!
	\brief �������� ��������� ���������
*/
/*!
	\brief �������� ����� ������������
	\param delay_ms �������� � ����
*/
	void SetDelay(unsigned addr, unsigned char bChnl, unsigned delay_ms)
    {
        unsigned char buf[20] = {9,0,0,0,0};
        buf[2] = delay_ms;
        SendTo(buf,16,addr,NEW_CH_PORT);
    }


    // *****************
    // ��� ������ 2
    void GetDataLeng(unsigned addr, unsigned char bChnl, unsigned uBegin, unsigned uLeng);

    void SetGPIO(unsigned addr, unsigned char bChnl, unsigned char bData) {
		unsigned char buf[6] = {2,0,0,0,0,0};
	    buf[1] = bChnl; buf[2] = bData & 0xFF;
	    SendTo(buf,6,addr,NEW_CH_PORT);
	}


    // ���������� ���-�� ��������� �����������
	void SetImpNum(unsigned addr, unsigned char bChnl, unsigned uNum) {
		unsigned char buf[6] = {12,0,0,0,0,0};
	    buf[1] = bChnl; buf[2] = uNum & 0xFF;
	    SendTo(buf,6,addr,NEW_CH_PORT);
	}

    // ���������� ����� �����-��������
    void SetRcvTrmMode(unsigned addr, unsigned char bChnl, bool bOnOff);
};

class HandDef : public CommonStoika
{

  private:
//    ch_setup ch_data[64];

  public:
/*!
	\sa BaseUDP
*/
    HandDef(int remote_port = 27000) : CommonStoika(remote_port) {};
/*!
	\sa BaseUDP
*/
    HandDef(char * ip_addr, int remote_port = 27000) : CommonStoika(ip_addr,remote_port){};

/*!
	\brief ��������� ����� � MCAST ������ �����������
*/
    int AddMcast() {return 0;}; //return AddRemoveMcastGroup(IP_ADD_MEMBERSHIP,UZ_MCAST_GROUP); };
/*!
	\brief ������� ����� �� MCAST ������ �����������
*/
    int DeleteMcast() {return 0;}; // {return AddRemoveMcastGroup(IP_DROP_MEMBERSHIP,UZ_MCAST_GROUP); };

//    void CMD64_SendCmd(unsigned proc, unsigned cmd);
/*!
	\brief ������� ������� � �����
	\param cmd  ��� �������
	\param param1 �������� (������� �� �������)
*/
    void SendCmd(unsigned cmd, unsigned param1=0, unsigned param2 = 0, unsigned param3 = 0);

    void SetVRC(unsigned a_start, unsigned a_end, unsigned a_len);
/*!
	\brief ������������� ��� ������
	\param data 50 ���� �������� ���
*/
    //void SetVRC(unsigned char * data);

/*!
	\brief ���������� ������� ����������� �������
	\param freq ������� (����������� �������� 10-15)
*/
    void SetFreq(unsigned char bChnl, unsigned freq);
//    {
//        unsigned char buf[4] = {4,0,0,0};
//        if(freq < 2) freq = 2;
//        buf[2] = freq & 0xFF;
//        buf[3] = (freq >> 8) & 0xFF;
//        SendCMD(buf, 4);
//    };

/*!
	\brief ���������� ������� ���������
	\param freq - �������� ��������
      ������� ������������ ��� (150/(freq+1))
*/
    void SetADCFreq(unsigned char bChnl, unsigned freq);
//    {
//        unsigned char buf[4] = {6,0,0,0};
//        int t = freq;
//        buf[2] = t & 0xFF;
//        buf[3] = (t >> 8) & 0xFF;
//        SendCMD(buf,4);
//    };

/*!
	\brief �������� ��������� ���������
*/
    void GetData();
//    {
//        unsigned char buf[4] = {1,0,0,0};
//        SendCMD(buf,4);
//    };

    // 0x01-����� 0
    // 0x02-����� 1
    // 0x03-��� ������
    void GetData(unsigned char bChnl, unsigned uLeng);

    void SetVRC(unsigned char bChnl, unsigned char *vrch);
//    {
//        unsigned char buf[60] = {7,0,0,0};
//        memcpy(&buf[2],vrch,50);
//        SendCMD(buf,60);
//    };

/*!
	\brief �������� ����� ������������
	\param delay_ms �������� � ����
*/
    void SetDelay(unsigned char bChnl, unsigned delay_ms);
//    {
//        unsigned char buf[20] = {9,0,0,0,0};
//        buf[2] = delay_ms;
//        SendCMD(buf,16);
//    }

#define SCAN_LENGTH_4096    0
#define SCAN_LENGTH_8192    1
#define SCAN_LENGTH_16384   2
#define SCAN_LENGTH_24576   3

    void SetScanLength(unsigned char bChnl, unsigned len);
//    {
//        unsigned char buf[20] = {9,0,0,0,0};
//        buf[2] = len;
//        SendCMD(buf,16);
//    }

    void SetIOdata(unsigned char bData);

    // *****************
    // ��� ������ 2
    void GetDataLeng(unsigned char bChnl, unsigned uBegin, unsigned uLeng);

    void SetGPIO(unsigned char bChnl, unsigned char bData);

    // ���������� ���-�� ��������� �����������
    void SetImpNum(unsigned char bChnl, unsigned uNum);

    // ���������� ����� �����-��������
    void SetRcvTrmMode(unsigned char bChnl, bool bOnOff);
};



#endif

