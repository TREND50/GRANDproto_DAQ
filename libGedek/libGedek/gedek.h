/*
 * gedek.h
 *
 *  Created on: Jun 15, 2017
 *      Author: vvoisin
 */

#ifndef GEDEK_H_
#define GEDEK_H_

typedef enum {
	TRENDACK_HEADERA	= 0xAAAAAAAA,
	TRENDACK_TRAILERA	= TRENDACK_HEADERA,
	TRENDACK_MSGTYPE	= 0x00005D00,
	TRENDACK_LEN		= 5,
} TRENDACK_INFO;

typedef struct {
	union {
		uint32_t data[TRENDACK_LEN];
		struct {
			uint32_t Header;						/*!< HEADERA */
			uint32_t msgType;
			uint32_t IpNum;
			uint32_t MsgAck :16;
			uint32_t Trailer;
		};
	};
} TrendAck_t;


typedef enum {
	RW_READ =0 ,
	RW_WRITE = 1,
} RW;

/**
 * \enum TRENDINTREG_Y
 * \brief values to use to set (TRENDINTREG_t).y
 */
typedef enum {
	TRENDINTREG_HEADERA		= 0xAAAAAAAA,
	TRENDINTREG_TRAILERA	= TRENDINTREG_HEADERA,
	TRENDINTREG_MSGTYPE		= 0x00005E00,
//CTA TRENDINTREG_MSGTYPE		= 0x00007E50,

	TRENDINTREG_Y0_SIZE		= 4,

	TRENDINTREG_LEN			= 14,

	TRENDINTREG_Y_READBACK	= 0,	/*!< ask a readback */
	TRENDINTREG_Y_WRITE		= 1,	/*!< write TRENDINTREG_t to IP/MAC/PORT registers */
} TRENDINTREG_INFO;

typedef struct {
	union {
		uint32_t data[TRENDINTREG_LEN];
		struct {
			uint32_t Header;						/*!< HEADERA */
			uint32_t msgType;						/*!< TRENDINTREG */
			uint32_t Y:1;							/*!< Y=1 write the following registers Y=1 write the following registers, Y=0 read back */
			union {
				uint32_t askTrailer;
				struct {
					uint32_t gedekMac;				/*!< FPGA Board MAC Address (32 LSB) */
					uint32_t gedekIp;				/*!< FPGA Board IP Address (32 LSB) */
					uint32_t destinationMac1_low;	/*!< Destination MAC1 Address (32 LSB) */
					uint32_t destinationMac1_high;	/*!< Destination MAC1 Address (16 MSB) */
					uint32_t destinationIp1;		/*!< Destination IP2 Address (32 LSB) */
					uint32_t destinationMac2_low;	/*!< Destination MAC1 Address (32 LSB) */
					uint32_t destinationMac2_high;	/*!< Destination MAC1 Address (16 MSB) */
					uint32_t destinationIp2;		/*!< Destination IP2 Address (32 LSB) */
					uint32_t port1;					/*!< Destination PORT1 (16 LSB) */
					uint32_t port2;					/*!< Destination PORT2 (16 LSB) */
					uint32_t Trailer;				/*!< HEADERA */
				};
			};
		};
	};
} TrendIntReg_t;

typedef enum {
	TRENDRDINTREG_HEADERA		= 0xAAAAAAAA,
	TRENDRDINTREG_TRAILERA		= TRENDRDINTREG_HEADERA,

//CTA	TRENDRDINTREG_MSGTYPE		= 0x0000EE10,
	TRENDRDINTREG_MSGTYPE		= 0x00005C00,

	TRENDRDINTREG_LEN			= 16,
} TRENDRDINTREG_INFO;

typedef struct {
	union {
		uint32_t data[TRENDRDINTREG_LEN];
		struct {
			uint32_t	Header;					/*!< HEADERB */
			uint32_t	msgType;				/*!< TRENDINTREGREAD */
			uint32_t	IpNum;					/*!< Board IP Ad. (32-Bit) */
			uint32_t	gedekMac;				/*!< Board MAC Ad. (32 LSB) */
			uint32_t	gedekIp;					/*!< Board IP Ad. (32-Bit) */
			uint32_t	destinationMac1_low;		/*!< Dest. MAC Ad. (32 LSB) + Dest. MAC Ad. (16 MSB) */
			uint32_t	destinationMac1_high;
			uint32_t	destinationIp1;			/*!< Dest IP Ad. */

			uint32_t	destinationMac2_low;	/*!< Dest. MAC Ad. (32 LSB) + Dest. MAC Ad. (16 MSB) */
			uint32_t	destinationMac2_high;
			uint32_t	destinationIp2;			/*!< Dest IP Ad. */

			uint32_t	destinationPort1;	/*!< Dest. MAC Ad. (32 LSB) + Dest. MAC Ad. (16 MSB) */
			uint32_t	destinationPort2;

			uint32_t	serialNumberLow;			/*!< Destination serialNumber low (32 LSB) */
			uint32_t	serialNumberHigh;			/*!< Destination serialNumber high (32 LSB) */

			uint32_t	Trailer;			/*!< HEADERB */
		};
	};
} TrendRdIntReg_t;

#endif /* GEDEK_H_ */
