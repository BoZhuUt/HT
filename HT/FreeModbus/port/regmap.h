/*
 * fileName: regmap.h
 * attention: 每个结构体中寄存器的总个数不可以超过50，否则会破坏参数在flash内的正常存取
 *
 */

/* Define to prevent recursive inclusion --------------------------------------*/

#ifndef __REGMAP_H

#define __REGMAP_H

/* Includes -------------------------------------------------------------------*/

#include "stm32f10x.h"

/* Exported_Macros ------------------------------------------------------------*/

typedef uint16_t uint16;
typedef uint32_t uint32;

#define SERIAL_NUMBER_STRING_REGADDR 41015 //system_status.serialNum register address
#define SERIAL_NUMBER_U32_REGADDR 41039  //system_status.serialNum_u32 register address
#define RESET_REGADDR 43006  //measure_settings.reset register address
#define CALIB_SOLUTION_FLOAT_REGADDR 44002 //calib_settings.calibSolution register address
#define SAMPLE_CONC_U16_REGADDR	44005 //calib_settings.sampleConc_u16 register address
/* Exported_Types -------------------------------------------------------------*/

//2 byte aligned
#pragma pack(2)

typedef	struct sysStatus
{											        /* Register		Type  		 R/W */
	uint16		runStatus;					/* 41001	 16bit integer	r/w */
	uint16   	commStatus;					/* 41002	 16bit integer	r/w */
	uint16 		calibStatus;				/* 41003 	 16bit integer 	r/w */
	uint16		configStatus;				/* 41004	 16bit integer	r/w */
	uint32    productNum;					/* 41005	 32bit integer 	r/w */
	char		  deviceName[16];     /* 41007	 128bit char 	  r/w */
	char		  serialNum[16];      /* 41015	 128bit char 	  r/w */
	char		  hardwareVer[16];    /* 41023   128bit char    r/w */
	char		  softwareVer[16];    /* 41031   128bit char    r/w */
	uint32		serialNum_u32;			/* 41039   32bit  */
	uint16		t365gain;		        /* 41041   16bit integer */
	uint16    s365gain;           /* 41042   16bit integer */
	uint16    t420gain;           /* 41043   16bit integer */
	uint16		s420gain;				    /* 41044   reserved */
	uint16    reserved[6];        /* 41045-41050   reserved */
} SYS_STATUS_T;

typedef struct commSettings
{											        /* Register		Type  		 R/W */
	uint16		modbusAddr;					/* 42001 	 16bit integer   r/w */
	uint16		modbusDatabits;			/* 42002 	 16bit integer   r/w */
	uint16		modbusParity;				/* 42003 	 16bit integer   r/w */
	uint32		modbusBaud;					/* 42004 	 32bit integer   r/w */
	uint16		reserved[45];				/* 42006-42050   reserved */
} COMM_SETTINGS_T;

typedef	struct measureSettings
{											        /* Register		Type  		 R/W */
	uint16		sampleCycle;				/* 43001	 16bit integer	 r/w  useless*/
	float		  measureRange;			  /* 43002	 32bit float	   r/w */
	float		  smoothingFactor;		/* 43004 	 32bit float     r/w */
	uint16		reset;				      /* 43006	 16bit integer	 used to compat with st500 1025 reset register*/
	char      sampleName[16];     /* 43007-43014 */
	float     mAtest;             /* 43015-43016 */
	uint16		reserved[34];			  /* 43017-43050   reserved */
} MEASURE_SETTINGS_T;

typedef	struct calibSettings
{											        /* Register		Type  		 R/W */
	uint16		updateMethod;				/* 44001	 16bit integer	 r/w  用于选择升级方式：0x55,BTLE; other numbers:update by RS485 */
	float		  calibSolution;		  /* 44002	 32bit float		 r/w  sample concentration */
	uint16		calibCommand;       /* 44004   16bit integer   r/w  DI calibration  ;*/  
	uint16		sampleConc_u16;			/* 44005   16bit  SAMPLE_CONC_U16_REGADDR , change sampleConc to float or uint32 */
	uint16		reserved[45];				/* 44006-44050   reserved */
} CALIB_SETTINGS_T;
//struct filterSettings is not used, reserved
typedef	struct userinfo
{											        /* Register		Type  		 R/W */
	char	emailAddr[32];					/* 45001-45016	    r/w */
	uint16	reserved[34];				  /* 45017-45050   reserved */
} USER_INFO_T;

typedef struct measureValues
{														  /* Register		Type  		 R/W */
	float		sensorValue;				  /* 46001	 32bit float	  r/w */
	float		sensorValue_mA;			  /* 46003	 32bit float	  r/w */
	float   temperatureValue;		  /* 46005	 32bit float	  r/w */
	float  	humidityValue; 			  /* 46007   32bit float    r/w (humidity) */
	float 	reserved[21];				  /* 46009-46050 reserved */
} MEASURE_VALUES_T;


// Below are sensor specific registers, pH meter may have different definition
// All sensor specific registers start from 48001

typedef	struct sensorParam
{	                           	/* Register		Type  		 R/W */	
	uint16    darks365;           /* 48001   16bit integer   it's useless for st510, only uesd to compat with st500 register */
	float		  slope;							/* 48002   32bit float     r/w */
	float     intercept;          /* 48004   32bit float     useless r/w */
	float		  t1;									/* 48006   32bit float     r/w */
	float		  t2;									/* 48008   32bit float     r/w */
	uint16    mARange;            /* 48010   16bit integer   r/w   4-20mA scaling factor, mA=ppb*16/mARange+4 */
	uint16		ct365;							/* 48011   16bit integer   r/w */
	uint16		cs365;							/* 48012   16bit integer   r/w */
	uint16		ct420;							/* 48013   16bit integer   r/w */
	uint16		cs420;							/* 48014   16bit integer   r/w */
	uint16		t365di;							/* 48015   16bit integer   r/w */
	uint16		s365di;							/* 48016   16bit integer   r/w */
	uint16		t420di;							/* 48017   16bit integer   r/w */
	uint16		s420di;							/* 48018   16bit integer   r/w */
	uint16		t365;								/* 48019   16bit integer   r/w */
	uint16		s365;								/* 48020   16bit integer   r/w */
	uint16		t420;								/* 48021   16bit integer   r/w */
	uint16		s420;								/* 48022   16bit integer   r/w */
	uint16		ct365Factory;				/* 48023   16bit integer   r/w */
	uint16		cs365Factory;				/* 48024   16bit integer   r/w */
	uint16		ct420Factory;				/* 48025   16bit integer   r/w */
	uint16		cs420Factory;				/* 48026   16bit integer   r/w */
	uint16		t365diFactory;			/* 48027   16bit integer   r/w */
	uint16		s365diFactory;			/* 48028   16bit integer   r/w */
	uint16		t420diFactory;			/* 48029   16bit integer   r/w */
	uint16		s420diFactory;			/* 48030   16bit integer   r/w */
	uint16  	errorCode;					/* 48031   16bit integer   useless r/w */
	float			slopeFactory;				/* 48032   float */
	uint16    saveFactoryParam;   /* 48034 */
	float     fluoresceinLowSlope;           /*48035   32bit integer   r/w */
	float			fluoresceinHighSlope;          /*48037   32bit integer   r/w */
	float			fluoresceinLowintercept;       /*48039   32bit integer   r/w */
	float			fluoresceinHighintercept;      /*48041  32bit integer   r/w */
	uint16		reserved[8];				/* 48042-48050   reserved */	
} SENSOR_PARAM_T;

typedef struct regmap
{
	uint16		st500reg;
	uint16		st510reg;
}REGMAP_T;

#pragma pack()

/* Exported_Functions ---------------------------------------------------------*/

#endif

