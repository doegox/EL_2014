#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#ifdef __cplusplus

extern "C" 
{
#endif /*__cplusplus*/

#ifndef byte
	typedef unsigned char byte;
#endif /* byte */


#ifndef uchar
	#define uchar unsigned char
#endif /* uchar */

#ifndef uint
	#define uint unsigned int
#endif /* uint */

#ifndef ulong
	#define ulong unsigned long
#endif /* ulong */


/**************************************************************************
MACRO
**************************************************************************/
/*
< MACRO>
	<NAME></NAME>
	<TYPE>GLOBAL</TYPE>
	<DESCRIPTION>
	</DESCRIPTION>
< / MACRO>
*/


#ifndef false
	#define  false     0
	#define  true      1
#endif /*false*/

typedef struct s_Parameters 
{
	int iIoAddr;
	int iIrq;
	int iPort;
	int iSetting;
	int iBaudrate;
	int iDtr;
	int iRts;
	uint uiHandshake;
}
s_Parameters;
/*Type of Pc*/
#define MAV_JITES_1						1
#define MAV_JITES_2						2
#define URNE_JITES_1					3
#define URNE_JITES_2					4
#define MAV_DIGIVOTE_1					5
#define MAV_DIGIVOTE_2					6
#define URNE_DIGIVOTE_1					7
#define URNE_DIGIVOTE_2					8

#define DIVIGOTE						1
#define JITES							2

/*returned Code for urne*/
#define  _LECTEUR_GOOD    				 0
#define  _LECTEUR_BUSY    				-1
#define  _TIME_OUT        				-2
#define  _CARTE_OVERFLOW  				-3
#define  _INVALID_CARD    				-4 
#define  _EJECT_CARD      				-5 
#define  _NON_EJECT_CARD  				-6 
#define  _DONNEES         				-7 
#define  _DO_CLEAR        				-8
#define  _DO_RECOMM       				-9
#define  _CARTE_BLOCKE    				-10
#define  _EJECT_CARD_REAR 				-11 
#define  _PASS_OUT_FRONT				1
#define  _PASS_OUT_REAR					2
#define  NO_ACTION						-12
#define  _TRANSMISSION_ERROR			-13
#define  _EJECT_ERREUR					-14

/*Command jites*/
#define JITES_BLOQ						 "m\x0D"
#define JITES_POSITION					 "s\x0D"
#define JITES_EJECT_FRONT				 "f\x0D"
#define JITES_EJECT_FRONT_FAST			 "x\x0D"
#define JITES_EJECT_REAR				 "o\x0D"
#define JITES_READ						 "r\x0D"
#define JITES_WRITE						 "w%s\x0D"
#define _WAIT_TIME_S					100
#define _WAIT_TIME						200
#define _RX_SIZE						4096
#define _TX_SIZE						1024
#define MAX_BUFFER_VALIDATOR			100


/*Define Card Reader Digivote*/
#define CARD_GATE						200
#define CARD_HOME						'a'
#define CARD_NO							1
#define CARD_ERROR						-151
#define SLEEP_WAIT_TILL					100

/*Returned code for MAV*/
#define READING_ERROR					2
#define WRITING_ERROR					3
#define TIMEOUT_ERROR					4
#define STR_TOO_LONG					5
#define CARD_REJECTED					6
#define NO_CARD							8
#define TRANSMISSION_ERROR				9
#define A_NORMAL_CARD_DETECTED			10
#define UNKNOWN_COMMAND					20
#define ERROR_EJECTION					21

/* << 9.06 */
/*#define MAX_TIMEOUT						5*/
#define MAX_TIMEOUT						2
/* 9.06 >> */
#define TIME_OUT_CARD					20
/* << 9.06 */
/*#define MAX_RETIES						5*/
#define MAX_RETIES						3
/* 9.06 >> */
#define SEND_LECTEUR_INIT				MAX_RETIES-2
#define SEND_DLE_EOT_AFTER				MAX_RETIES-2

/*NO error*/
#define ACK					0

/*Error */
#define ERROR_NONE_AVAILABLE			-100
#define ERROR_CHANGE_STATE				-110
#define ERROR_TIMEOUT					-120
#define ERROR_RETIES					-130
#define ERROR_READ_ONLY					-140
#define RECEIVE_NACK					-150
#define ERROR_OPEN_SERIAL				-160
#define ERROR_CLOSE_SERIAL				-161
#define ERROR_TIMEOUT_SERIAL			-162
#define ERROR_SEND_DATA_SERIAL			-163
#define ERROR_TIMEOUT_BREAK_SERIAL		-164
#define ERROR_READER_CARD				-165
#define ERROR_LENGTH_READ_COMMAND		-170
#define ERROR_INVALID_RESPONSE			-171
#define ERROR_WAIT_ACK					-172
#define ERROR_WAIT_RESPONCE				-173
#define ERROR_INVALID_FRAME				-174
#define ERROR_CR						-175
#define ERROR_BCC						-176
#define ERROR_TIMEOUT_CARD				-177
#define ERROR_SEND_DATA					-180
#define ERROR_READ_DATA					-181
#define ERROR_INSTANCE					-200
#define ERROR_PORT_COM_NO_CREATED		-210

/*Direction ejection*/
#define EJECT_REAR						1
#define EJECT_FRONT						0

/* << 9.06 */
/*#define TIME_OUT_SERIAL			364	*/
#define TIME_OUT_SERIAL			54
/* 9.06 >> */
#define TIME_OUT_ERROR_SERIAL	0X80000000
/* << 9.06 */
#define MY_TIME_OUT_ERROR_SERIAL	0X40000000
/* 9.06 >> */

#ifndef UTT_TICK_IN_A_DAY
	#define UTT_TICK_IN_A_DAY  (1573040L)
#endif 


//#define STESUD_TRACE 

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /* __SYSTEM_H__ */