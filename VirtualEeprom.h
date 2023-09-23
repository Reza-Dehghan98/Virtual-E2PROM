/**********InTheNameOfGod*********/
/**
 * @file VirtualEeprom.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _VIRTUAL_EEPROM_H_
#define _VIRTUAL_EEPROM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "StreamBuffer.h"
#include "Queue.h"
#include <string.h>

/*
#define     VIRTUAL_EEPROM_MCU_F0		0x00
#define     VIRTUAL_EEPROM_MCU_F1		0x01
#define     VIRTUAL_EEPROM_MCU_F2		0x02
#define     VIRTUAL_EEPROM_MCU_F3		0x03
#define     VIRTUAL_EEPROM_MCU_F4		0x04
#define     VIRTUAL_EEPROM_MCU_F7		0x07
#define     VIRTUAL_EEPROM_MCU_H7		0x17
*/




/**
 * @brief define your MCU series
 */
//#define VIRTUAL_EEPROM_MCU_F1   
//#define   VIRTUAL_EEPROM_MCU_F479
#define VIRTUAL_EEPROM_MCU_F407
//#define VIRTUAL_EEPROM_MCU_F417




#if defined(VIRTUAL_EEPROM_MCU_F407) || defined(VIRTUAL_EEPROM_MCU_F479) 

 #define VIRTUAL_EEPROM_FLASH_SECTOR_0     0U   /*!< Sector Number 0   */
 #define VIRTUAL_EEPROM_FLASH_SECTOR_1     1U   /*!< Sector Number 1   */
 #define VIRTUAL_EEPROM_FLASH_SECTOR_2     2U   /*!< Sector Number 2   */
 #define VIRTUAL_EEPROM_FLASH_SECTOR_3     3U   /*!< Sector Number 3   */
 #define VIRTUAL_EEPROM_FLASH_SECTOR_4     4U   /*!< Sector Number 4   */
 #define VIRTUAL_EEPROM_FLASH_SECTOR_5     5U   /*!< Sector Number 5   */
 #define VIRTUAL_EEPROM_FLASH_SECTOR_6     6U   /*!< Sector Number 6   */
 #define VIRTUAL_EEPROM_FLASH_SECTOR_7     7U   /*!< Sector Number 7   */
 #define VIRTUAL_EEPROM_FLASH_SECTOR_8     8U   /*!< Sector Number 8   */
 #define VIRTUAL_EEPROM_FLASH_SECTOR_9     9U   /*!< Sector Number 9   */
 #define VIRTUAL_EEPROM_FLASH_SECTOR_10    10U  /*!< Sector Number 10  */
 #define VIRTUAL_EEPROM_FLASH_SECTOR_11    11U  /*!< Sector Number 11  */
 
#ifdef  VIRTUAL_EEPROM_MCU_F479
 #define VIRTUAL_EEPROM_FLASH_SECTOR_12    12U  /*!< Sector Number 12  */
 #define VIRTUAL_EEPROM_FLASH_SECTOR_13    13U  /*!< Sector Number 13  */
 #define VIRTUAL_EEPROM_FLASH_SECTOR_14    14U  /*!< Sector Number 14  */
 #define VIRTUAL_EEPROM_FLASH_SECTOR_15    15U  /*!< Sector Number 15  */
 #define VIRTUAL_EEPROM_FLASH_SECTOR_16    16U  /*!< Sector Number 16  */
 #define VIRTUAL_EEPROM_FLASH_SECTOR_17    17U  /*!< Sector Number 17  */
 #define VIRTUAL_EEPROM_FLASH_SECTOR_18    18U  /*!< Sector Number 18  */
 #define VIRTUAL_EEPROM_FLASH_SECTOR_19    19U  /*!< Sector Number 19  */
 #define VIRTUAL_EEPROM_FLASH_SECTOR_20    20U  /*!< Sector Number 20  */
 #define VIRTUAL_EEPROM_FLASH_SECTOR_21    21U  /*!< Sector Number 21  */
 #define VIRTUAL_EEPROM_FLASH_SECTOR_22    22U  /*!< Sector Number 22  */
 #define VIRTUAL_EEPROM_FLASH_SECTOR_23    23U  /*!< Sector Number 23  */    
#endif


 #define   SECTOR_0_ADDRESS     0x08000000  //16K
 #define   SECTOR_1_ADDRESS     0x08004000  //16K 
 #define   SECTOR_2_ADDRESS     0x08008000  //16K
 #define   SECTOR_3_ADDRESS     0x0800C000  //16K
 #define   SECTOR_4_ADDRESS     0x08010000  //64K
 #define   SECTOR_5_ADDRESS     0x08020000  //128K
 #define   SECTOR_6_ADDRESS     0x08040000  //128K
 #define   SECTOR_7_ADDRESS     0x08060000  //128K
 #define   SECTOR_8_ADDRESS     0x08080000  //128K
 #define   SECTOR_9_ADDRESS     0x080A0000  //128K 
 #define   SECTOR_10_ADDRESS    0x080C0000  //128K
 #define   SECTOR_11_ADDRESS    0x080E0000  //128K
                                            
#ifdef VIRTUAL_EEPROM_MCU_F479              
 #define   SECTOR_12_ADDRESS    0x08100000  //16K
 #define   SECTOR_13_ADDRESS    0x08104000  //16K 
 #define   SECTOR_14_ADDRESS    0x08108000  //16K
 #define   SECTOR_15_ADDRESS    0x0810C000  //16K
 #define   SECTOR_16_ADDRESS    0x08110000  //64K
 #define   SECTOR_17_ADDRESS    0x08120000  //128K
 #define   SECTOR_18_ADDRESS    0x08140000  //128K
 #define   SECTOR_19_ADDRESS    0x08160000  //128K
 #define   SECTOR_20_ADDRESS    0x08180000  //128K
 #define   SECTOR_21_ADDRESS    0x081A0000  //128K 
 #define   SECTOR_22_ADDRESS    0x081C0000  //128K
 #define   SECTOR_23_ADDRESS    0x081E0000  //128K
 #define   SECTOR_END_ADDRESS   0x081FFFFF  //
#endif  

#endif



/******************************************************************************/
/*
#if   VIRTUAL_EEPROM_MCU == VIRTUAL_EEPROM_MCU_F0
	#include "stm32f0xx.h"
#elif VIRTUAL_EEPROM_MCU == VIRTUAL_EEPROM_MCU_F1
	#include "stm32f1xx.h"
#elif VIRTUAL_EEPROM_MCU == VIRTUAL_EEPROM_MCU_F2
	#include "stm32f2xx.h"
#elif VIRTUAL_EEPROM_MCU == VIRTUAL_EEPROM_MCU_F3
	#include "stm32f3xx.h"
#elif VIRTUAL_EEPROM_MCU == VIRTUAL_EEPROM_MCU_F4
	#include "stm32f4xx.h"
#elif VIRTUAL_EEPROM_MCU == VIRTUAL_EEPROM_MCU_F7
	#include "stm32f7xx.h"
#elif VIRTUAL_EEPROM_MCU == VIRTUAL_EEPROM_MCU_H7
	#include "stm32h7xx.h"
#else
	#error "Wrong MCU series!"
#endif
*/

/**
 * @brief VirtualEEPROM Maximum Try for erase Sector
 */
#define  VIRTUAL_EEPROM_MAXIMUM_TRY       3


#define  VIRTUAL_EEPROM_ERASE_TIMEOUT     500


typedef  uint32_t VirtualEeprom_Timestamp;
typedef  uint8_t  VirtualEeprom_ResultType;

/**
 * @brief EEPROM DataType
 */
typedef enum {
    VirtualEeprom_Const     = 0x00,
    VirtualEeprom_Variable  = 0x01,
} VirtualEeprom_DataType;



/**
 * @brief EEPROM Command Header
 */
typedef struct {
    uint32_t MemAddress;
    uint16_t Len;
    uint8_t  Mode;
    uint8_t  Type;
} VirtualEeprom_CommandHeader;



/**
 * @brief EEPROM Mode
 */
typedef enum {
    VirtualEeprom_ReadMode        = 0x00,
    VirtualEeprom_WriteMode       = 0x01,
   // VirtualEeprom_EraseMode       = 0x02,
    //VirtualEeprom_NoiseEraseMode  = 0x03,
} VirtualEeprom_Mode;



/**
 * @brief config Struct
 */
typedef struct {
    uint32_t                  EepromStartAddress;
    uint16_t                  PageSize;
} VirtualEeprom_Config;


typedef enum {
#if defined(VIRTUAL_EEPROM_MCU_F407)  || defined(VIRTUAL_EEPROM_MCU_F479) 
    FlashTypeProgram_Byte           = 0x00U,
#endif                              
    FlashTypeProgram_HalfWord       = 0x01U,  /*!<Program a half-word (16-bit) at a specified address.*/
    FlashTypeProgram_Word           = 0x02U,  /*!<Program a word (32-bit) at a specified address.*/
    FlashTypeProgram_DoubleWord     = 0x03U,  /*!<Program a double word (64-bit) at a specified address*/
} VirtualEeprom_FlashTypeProgram;


/*PreDefined Structure*/
struct         __VirtualEeprom;
typedef struct __VirtualEeprom VirtualEeprom;



/**
 * @brief Result of your Process 
 */
typedef enum {
    VirtualEeprom_Ok                = 0x00U,
    VirtualEeprom_Error             = 0x01U, 
    VirtualEeprom_Busy              = 0x02U,
    VirtualEeprom_Timeout           = 0x03U,  
    VirtualEeprom_InvalidParameter  = 0x04U,
    VirtualEeprom_Null              = 0x05U,
    VirtualEeprom_TimeOutError      = 0x06U,
    VirtualEeprom_NotFound          = 0x07U,
} VirtualEeprom_Result;



/**
 * @brief Callback Function Pointer 
 */
typedef void (*VirtualEeprom_CallbackFn) (Stream* stream, VirtualEeprom_CommandHeader* commandHeader);



typedef struct {
   VirtualEeprom_CallbackFn   onRead;
} VirtualEeprom_Callbacks;




/**
 * @brief VirtualEeprom main Struct
 */
struct __VirtualEeprom {
    struct __VirtualEeprom*      Previous;///linked list
    const VirtualEeprom_Config*  Config;
    VirtualEeprom_Callbacks      Callbacks;  
    Stream                       WriteStream;
    Stream                       ReadStream;
    Queue                        CommandQueue;
    Queue                        ReadQueue;
    VirtualEeprom_CommandHeader  CommandHeaderInProcess;
    VirtualEeprom_CommandHeader  ReadCommand;
    void*                        Args;                  /**< user arguments */
    VirtualEeprom_Timestamp      NextTick; 
    uint8_t*                     ConstVal;
    uint8_t                      TempLen;
    uint8_t                      Lock           : 1;
    uint8_t                      Enabled        : 1;
    uint8_t                      Configured     : 1;
    uint8_t                      EraseExecute   : 1;
    uint8_t                      InBlocking     : 1;
    uint8_t                      OVF            : 1;
    uint8_t                      Reserved       : 2;
};




/*Driver Function Pointer*/
typedef uint8_t                 (*VirtualEeprom_flashUnLockFn)          (void);
typedef uint8_t                 (*VirtualEeprom_flashLockFn)            (void);
typedef uint8_t                 (*VirtualEeprom_flashProgramFn)         (VirtualEeprom_FlashTypeProgram flashTypeProgram, uint32_t address, uint16_t data);
typedef uint8_t                 (*VirtualEeprom_readFn)                 (VirtualEeprom* eeprom, uint16_t address, uint16_t* buffer);
typedef uint8_t                 (*VirtualEeprom_erasePageFn)            (VirtualEeprom* eeprom, uint32_t pageAddress);
typedef VirtualEeprom_Timestamp (*VirtualEeprom_getTimestampFn)         (void);



/**
 * @brief EEPROM Driver Struct
 */
typedef struct {
    VirtualEeprom_flashUnLockFn            FlashUnLock;
    VirtualEeprom_flashLockFn              FlashLock;
    VirtualEeprom_flashProgramFn           FlashProgram;
    VirtualEeprom_erasePageFn              ErasePage;
    VirtualEeprom_getTimestampFn           GetTimestamp;
} VirtualEeprom_Driver;



void                  VirtualEeprom_driverInit     (const VirtualEeprom_Driver* driver);
VirtualEeprom_Result  VirtualEeprom_add            (VirtualEeprom* eeprom);
VirtualEeprom_Result  VirtualEeprom_handle         (void);
void                  VirtualEeprom_init           (VirtualEeprom* eeprom, uint8_t* commandQBuffer, uint16_t commandQLen, uint8_t* qReadBuffer, uint16_t qReadLen, uint8_t* streamWriteBuffer, uint16_t streamWriteLen, uint8_t* streamReadBuffer, uint16_t streamReadLen);
void                  VirtualEeprom_setConfig      (VirtualEeprom* eeprom, const VirtualEeprom_Config* config);
VirtualEeprom_Result  VirtualEeprom_writeBlocking  (VirtualEeprom* eeprom, uint16_t virtualAddress, uint8_t* data,   uint16_t len);
VirtualEeprom_Result  VirtualEeprom_readBlocking   (VirtualEeprom* eeprom, uint16_t virtualAddress, uint8_t* buffer, uint16_t len);
VirtualEeprom_Result  VirtualEeprom_write          (VirtualEeprom* eeprom, uint16_t virtualAddress, uint8_t* data, VirtualEeprom_DataType type, uint16_t len); 
VirtualEeprom_Result  VirtualEeprom_read           (VirtualEeprom* eeprom, uint16_t virtualAddress, uint16_t len);

#if defined(VIRTUAL_EEPROM_MCU_F407) || defined(VIRTUAL_EEPROM_MCU_F417) || defined(VIRTUAL_EEPROM_MCU_F479)
int8_t                VirtualEeprom_getSector     (uint32_t address);
#endif

typedef VirtualEeprom_Result (*VirtualEeprom_processFn) (VirtualEeprom* eeprom);
VirtualEeprom_Result         VirtualEeprom_writeProcess (VirtualEeprom* eeprom);
VirtualEeprom_Result         VirtualEeprom_readProcess  (VirtualEeprom* eeprom);

void                         VirtualEeprom_onRead       (VirtualEeprom* eeprom, VirtualEeprom_CallbackFn cb);

void                 VirtualEeprom_erase                (VirtualEeprom* eeprom, uint32_t startAddress);

int8_t               VirtualEeprom_AssertMemory         (uint8_t* arr1, uint8_t* arr2, uint8_t len);


VirtualEeprom_Result VirtualEeprom_writeUInt8Blocking   (VirtualEeprom* eeprom, uint8_t val, uint16_t addr); 
uint8_t              VirtualEeprom_readUInt8Blocking    (VirtualEeprom* eeprom, uint16_t addr);
                                                        
VirtualEeprom_Result VirtualEeprom_writeUInt16Blocking  (VirtualEeprom* eeprom, uint16_t val, uint16_t addr);
uint16_t             VirtualEeprom_readUInt16Blocking   (VirtualEeprom* eeprom, uint16_t addr);
                                                        
VirtualEeprom_Result VirtualEeprom_writeUInt32Blocking  (VirtualEeprom* eeprom, uint32_t val, uint16_t addr);
uint32_t             VirtualEeprom_readUInt32Blocking   (VirtualEeprom* eeprom, uint16_t addr);
                                                        
VirtualEeprom_Result VirtualEeprom_writeUInt64Blocking  (VirtualEeprom* eeprom, uint64_t val, uint16_t addr);
uint64_t             VirtualEeprom_readUInt64Blocking   (VirtualEeprom* eeprom, uint16_t addr);




VirtualEeprom_Result VirtualEeprom_writeUInt8  (VirtualEeprom* eeprom, uint8_t val, uint16_t addr); 
void                 VirtualEeprom_readUInt8   (VirtualEeprom* eeprom, uint16_t addr); 

VirtualEeprom_Result VirtualEeprom_writeUInt16 (VirtualEeprom* eeprom, uint16_t val, uint16_t addr);
void                 VirtualEeprom_readUInt16  (VirtualEeprom* eeprom, uint16_t addr);

VirtualEeprom_Result VirtualEeprom_writeUInt32 (VirtualEeprom* eeprom, uint32_t val, uint16_t addr); 
void                 VirtualEeprom_readUInt32  (VirtualEeprom* eeprom, uint16_t addr);

VirtualEeprom_Result VirtualEeprom_writeUInt64 (VirtualEeprom* eeprom, uint64_t val, uint16_t addr); 
void                 VirtualEeprom_readUInt64  (VirtualEeprom* eeprom, uint16_t addr);






/* Null Define */
#define                VIRTUAL_EEPROM_NULL_DRIVER   (VirtualEeprom_Driver*)0
#define                VIRTUAL_EEPROM_NULL          (VirtualEeprom*)0

/******************************************************************************************************************/






#ifdef __cplusplus
};
#endif

#endif /* _VIRTUAL_EEPROM_H_ */
