#ifndef __DATA_MODEL_HEAD__
#define __DATA_MODEL_HEAD__	
	typedef unsigned char  byte;
	typedef unsigned char  byte1;
	typedef unsigned short byte2; 
	typedef unsigned long  byte4;	
	//---------------------------------------------------
	#ifndef FALSE 
		#define FALSE 0
	#endif 	
	#ifndef TRUE 
		#define TRUE 1
	#endif	
	#ifndef false 
		#define false 0
	#endif 	
	#ifndef true 
		#define true 1
	#endif
	//---------------------------------------------------
	#ifndef NULL 
		#define NULL ((void *)0)
	#endif
	#ifndef null 
		#define null ((void *)0)
	#endif
	//---------------------------------------------------
	
	#define isTrue(value)  (value!=0)
	#define isFalse(value) (value==0)

	
	#define BIG_ENDIAN_GET_BYTE4(buffer,startByte) ( (buffer[startByte]<<24)+(buffer[startByte+1]<<16)+(buffer[startByte+2]<<8)+buffer[startByte+3])
	#define BIG_ENDIAN_GET_BYTE3(buffer,startByte) ( (buffer[startByte]<<16)+(buffer[startByte+1]<<8)+(buffer[startByte+2]))
	#define BIG_ENDIAN_GET_BYTE2(buffer,startByte) ( (buffer[startByte]<<8)+buffer[startByte+1])
	#define BIG_ENDIAN_GET_BYTE1(buffer,startByte) (  buffer[startByte] )

	#define LITTLE_ENDIAN_GET_BYTE4(buffer,startByte) (buffer[startByte]+(buffer[startByte+1]<<8)+(buffer[startByte+2]<<16)+(buffer[startByte+3]<<24))
	#define LITTLE_ENDIAN_GET_BYTE3(buffer,startByte) (buffer[startByte]+(buffer[startByte+1]<<8)+(buffer[startByte+2]<<16))
	#define LITTLE_ENDIAN_GET_BYTE2(buffer,startByte) (buffer[startByte]+(buffer[startByte+1]<<8))
	#define LITTLE_ENDIAN_GET_BYTE1(buffer,startByte) (buffer[startByte] )
		
	#define BIG_ENDIAN_GET_BCD2BIN_BYTE4(buffer,startByte) ( bcd2Bin(buffer[startByte])*1000000+bcd2Bin(buffer[startByte+1])*10000+bcd2Bin(buffer[startByte+2])*100+bcd2Bin(buffer[startByte+3]))
	#define BIG_ENDIAN_GET_BCD2BIN_BYTE3(buffer,startByte) ( bcd2Bin(buffer[startByte])*10000  +bcd2Bin(buffer[startByte+1])*100  +bcd2Bin(buffer[startByte+2]))
	#define BIG_ENDIAN_GET_BCD2BIN_BYTE2(buffer,startByte) ( bcd2Bin(buffer[startByte])*100    +bcd2Bin(buffer[startByte+1]))
	#define BIG_ENDIAN_GET_BCD2BIN_BYTE1(buffer,startByte) ( bcd2Bin(buffer[startByte]))
	extern unsigned long	SysTickCounter;
	#define tickGet() 	(SysTickCounter)
	#define TICK_MS_TO_TICK(ms) 				(ms) 
	#define TICK_ESCAPE_TICK1(nowTick,lastTick) 	(0xFFFFFFFF-(lastTick)+(nowTick))
	#define TICK_ESCAPE_TICK2(nowTick,lastTick) 	(nowTick-lastTick)
	#define TICK_ESCAPE_TICK(nowTick,lastTick)	((SysTickCounter>=lastTick)?TICK_ESCAPE_TICK2(SysTickCounter,lastTick):TICK_ESCAPE_TICK1(SysTickCounter,lastTick))
	#define TICK_CHECK_TIMEOUT(lastTick,ms) 		(  (TICK_ESCAPE_TICK(SysTickCounter,lastTick)>TICK_MS_TO_TICK(ms) ) ? 1 : 0   )

	#define PRINT_ARRAY4(ARRAY) ARRAY[0],ARRAY[1],ARRAY[2],ARRAY[3]
	#define PRINT_ARRAY6(ARRAY) ARRAY[0],ARRAY[1],ARRAY[2],ARRAY[3],ARRAY[4],ARRAY[5]
#endif 

