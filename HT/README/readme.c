/*主要modbus寄存器 设定读取表*/
/*

	uint16		configStatus;				 41004	16bit integer	read  配置状态(0系统未配置,1系统已配置)
	
	uint16		firstCalibSolution;			44002	32bit float		r/w  第一点标定值 
	uint16		calibCommand;				 44004	16bit integer    r/w 标定命令 1校准  2(4-20ma设置)  3参数复位
	float    K;                    44005  电导率系数
	
  uint16		filterCoefficient_1;		 45002	 32bit float     r/w 设置 4MA值 
  uint16		filterCoefficient_2;		 45003	 32bit float     r/w 设置20MA值 	
	
	float		sensorValue;				 46001	32bit float		read  测量值
	float		sensorValue_mA;				 46003	32bit float		read  测量值(4-20ma形式)
	float   temperatureValue;			 46005	32bit float		read  温度(摄氏度)

*/	


/*初始状态说明*/
/*  
	comm_settings.modbusAddr=0x0E;	从机地址14
	comm_settings.modbusDatabits=8;8位数据
	comm_settings.modbusParity=MODBUS_PARITY_	EVEN 偶校验
	comm_settings.modbusBaud=9600;9600波特率
	
	filter_settings.filterCoefficient_2=10000;    //默认测量范围为10000
	filter_settings.filterCoefficient_1=0;
	
	calib_settings.firstCalibSolution=0;//第一校准值
	calib_settings.calibCommand=8;    功能轮询寄存器值  8木有功能，直接退出
	calib_settings.K=0.30;            初始K值	
	calib_settings.Ra=510;						四档位电阻
	calib_settings.Rb=1500;
	calib_settings.Rc=6800;
	calib_settings.Rd=27000;	
*/
