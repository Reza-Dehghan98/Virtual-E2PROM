#include "VirtualEeprom.h"


/**
 * @brief Private Variable
 */
const VirtualEeprom_Driver* virtualEepromDriver;
VirtualEeprom*    lastVirtualEeprom;
#define           __eeprom()              lastVirtualEeprom
#define           __next(VirtualEeprom)   VirtualEeprom = (VirtualEeprom)->Previous


/**
 * @brief this is Function pointer Array that content is NonBlockingProcessMethod
 */
static const VirtualEeprom_processFn  VIRTUAL_VirtualEeprom_PROCESS_ARRAY [2] = {VirtualEeprom_readProcess, VirtualEeprom_writeProcess}; 


/**
 * @brief with this Function user can init the VirtualEeprom and that`s Driver  
 * 
 * @param driver Address of user VirtualEeprom_Driver struct
 */
void VirtualEeprom_driverInit (const VirtualEeprom_Driver* driver) {
    virtualEepromDriver = driver;
}




/**
 * @brief with this Function user can add VirtualEeprom Struct to the LinkedList
 * 
 * @param eeprom Address of VirtualEeprom struct user wan to add that to the Linked List
 */
VirtualEeprom_Result VirtualEeprom_add (VirtualEeprom* eeprom) {
  eeprom->Previous   = lastVirtualEeprom;
  lastVirtualEeprom  = eeprom;
  eeprom->TempLen    = 0;
  eeprom->OVF        = 0;
  //eeprom->NextTick   = 0;
  eeprom->Configured = 0;  
  memset (&eeprom->CommandHeaderInProcess, 0, sizeof(VirtualEeprom_CommandHeader)); 
  memset (&eeprom->ReadCommand,            0, sizeof(VirtualEeprom_CommandHeader)); 
}



/**
 * @brief Remove VirtualEeprom from LinkedList
 * 
 * @param remove Address of VirtualEeprom struct user want to remove that From Linked List
 * @return VirtualEeprom_Result 
 */
VirtualEeprom_Result VirtualEeprom_remove (VirtualEeprom* remove) {
    VirtualEeprom* pEeprom = lastVirtualEeprom;
    // check last VirtualEeprom first
    if (remove == pEeprom) {
        // remove VirtualEeprom  dropped from link list
        pEeprom  = remove->Previous;
        remove->Previous   = VIRTUAL_EEPROM_NULL;
        remove->Configured = 0;
        remove->Enabled    = 0;
        return VirtualEeprom_Ok;
    }
    while (VIRTUAL_EEPROM_NULL != pEeprom) {
        if (remove == pEeprom->Previous) {
            // remove VirtualEeprom dropped from link list
            pEeprom->Previous  = remove->Previous;
            remove->Previous   = VIRTUAL_EEPROM_NULL;
            remove->Configured = 0;
            remove->Enabled    = 0;
            return VirtualEeprom_Ok;
        }
        pEeprom = pEeprom->Previous;
    }
    return VirtualEeprom_NotFound;
}



/**
 * @brief this Function use for Initialize the VirtualEeprom for NonBlocking 
 * 
 * @param eeprom Address of user VirtualEeprom struct
 * @param commandQBuffer Address of user commandQBuffer 
 * @param commandQLen length of commandQBuffer -> sizeof(commandQBuffer)
 * @param qReadBuffer Address of user qReadBuffer
 * @param qReadLen length of qReadBuffer -> sizeof(qReadBuffer)
 * @param streamWriteBuffer Address of user streamWriteBuffer
 * @param streamWriteLen length of streamWriteBuffer -> sizeof(streamWriteBuffer)
 * @param streamReadBuffer Address of user streamReadBuffer
 * @param streamReadLen length of streamReadBuffer -> sizeof(streamReadBuffer)
 */
void VirtualEeprom_init (VirtualEeprom* eeprom, uint8_t*   commandQBuffer, uint16_t commandQLen, uint8_t* qReadBuffer, uint16_t qReadLen, uint8_t* streamWriteBuffer, uint16_t streamWriteLen, uint8_t* streamReadBuffer, uint16_t streamReadLen) {
    Queue_init  (&eeprom->CommandQueue, commandQBuffer,    commandQLen,    sizeof(VirtualEeprom_CommandHeader));
    Queue_init  (&eeprom->ReadQueue,    qReadBuffer,       qReadLen,       sizeof(VirtualEeprom_CommandHeader));
    Stream_init (&eeprom->WriteStream,  streamWriteBuffer, streamWriteLen);
    Stream_init (&eeprom->ReadStream,   streamReadBuffer,  streamReadLen); 
}




/**
 * @brief this Function use for set user Config 
 * 
 * @param eeprom Address of VirtualEeprom struct
 * @param config Address of user Config
 */
void VirtualEeprom_setConfig  (VirtualEeprom* eeprom, const VirtualEeprom_Config* config) {
    eeprom->Config     = config;
    eeprom->Configured = 1;
}




/**
 * @brief with this Function user can write Bytes in Blocking Method 
 * 
 * @param eeprom Address of VirtualEeprom struct
 * @param virtualAddress virtualAddress user want to write there 
 * @param data Address of user data
 * @param length length of data user Want to Write
 * @return VirtualEeprom_Result 
 */
VirtualEeprom_Result VirtualEeprom_writeBlocking (VirtualEeprom* eeprom, uint16_t virtualAddress, uint8_t* data, uint16_t len) {    
    uint32_t address = eeprom->Config->EepromStartAddress + virtualAddress; //Calculate the physical address
    uint16_t i = 0;
    VirtualEeprom_Result result = VirtualEeprom_Ok; 
  if (len > 0) {
   
#ifdef VIRTUAL_EEPROM_MCU_F1
    uint16_t halfWordData;
    
    virtualEepromDriver->FlashUnLock(); //Unlock the Flash memory for write operation
    
    for (i = 0; (i < len) ; i += 2) {
       if ((i == (len-1)) && ((len % 2) != 0)) { 
          halfWordData = (uint16_t)data[i] | ((uint16_t)0 << 8);  
       }
       else {
          halfWordData = (uint16_t)data[i] | ((uint16_t)data[i + 1] << 8);  //Combine two bytes into a half-word
       }
       result = virtualEepromDriver->FlashProgram(FlashTypeProgram_HalfWord, address + i, halfWordData); //Program one half-word at a time
       if (result != VirtualEeprom_Ok){
          return result; 
       }
    }    
    virtualEepromDriver->FlashLock();
    
    return VirtualEeprom_Ok;
#endif

#if defined(VIRTUAL_EEPROM_MCU_F407)  || defined(VIRTUAL_EEPROM_MCU_F479)
    virtualEepromDriver->FlashUnLock();
    for (i = 0; i < len ; i++) {
        if (result == VirtualEeprom_Ok) {
            result = virtualEepromDriver->FlashProgram(FlashTypeProgram_Byte, address + i, data[i]); //Program one Byte at a time
        }
        else {
          return result;
     }
  }
    virtualEepromDriver->FlashLock();
    return VirtualEeprom_Ok;
#endif
   }
   else {
      return VirtualEeprom_InvalidParameter;
   } 
}






/**
 * @brief with this Function user can read Bytes in Blocking Method 
 * 
 * @param eeprom Address of VirtualEeprom struct
 * @param virtualAddress virtualAddress user want to read From there 
 * @param buffer Address of Buffer user want to store read value in there
 * @param length length of data user want to read from VirtualEeprom
 */
VirtualEeprom_Result VirtualEeprom_readBlocking (VirtualEeprom* eeprom, uint16_t virtualAddress, uint8_t* buffer, uint16_t len) {
    
   uint32_t address = eeprom->Config->EepromStartAddress + virtualAddress;  // Calculate the physical address
/*
    for (uint16_t i = 0; i < length; i++) {
        buffer[i] = *(uint8_t*)address;  // Read one byte at a time
        address++;  
    }
*/
   if(len > 0) {   
       memcpy(buffer, address, len);
       return VirtualEeprom_Ok;
   }
    else {
       return VirtualEeprom_InvalidParameter;
    }
}




/**
 * @brief tis Function use for NonBlocking write into the VirtualEeprom
 * 
 * @param eeprom Address of VirtualEeprom struct
 * @param virtualAddress VirtualAddress u want to write in that
 * @param data Address of data
 * @param type type of Data/// Variable or Const
 * @param len  length of Data
 * @return VirtualEeprom_Result 
 */
VirtualEeprom_Result VirtualEeprom_write (VirtualEeprom* eeprom, uint16_t virtualAddress, uint8_t* data, VirtualEeprom_DataType type, uint16_t len) {
  VirtualEeprom_CommandHeader cacheHeader;
  cacheHeader.MemAddress = virtualAddress;
  cacheHeader.Len  = len;
  cacheHeader.Mode = VirtualEeprom_WriteMode;
  cacheHeader.Type = type;
  
  if (cacheHeader.Len > 0) {
     switch(cacheHeader.Type) {
       case (VirtualEeprom_Const) :
            Stream_writeBytes(&eeprom->WriteStream, (uint8_t*)&data, sizeof(uint32_t));
         break;
       case (VirtualEeprom_Variable) :
            Stream_writeBytes(&eeprom->WriteStream, data, cacheHeader.Len);
         break;
     }
     Queue_writeItem (&eeprom->CommandQueue, &cacheHeader);
  }
  else {
     return VirtualEeprom_InvalidParameter;
  }
  return VirtualEeprom_Ok;
}





/**
 * @brief 
 * 
 * @param eeprom Address of VirtualEeprom struct
 * @param virtualAddress virtualAddress user want to write into the VirtualEeprom
 * @param len length of data user want to read from VirtualEeprom 
 * @return VirtualEeprom_Result 
 */
VirtualEeprom_Result VirtualEeprom_read (VirtualEeprom* eeprom, uint16_t virtualAddress, uint16_t len) {
  VirtualEeprom_CommandHeader cacheHeader;
  cacheHeader.MemAddress = virtualAddress;
  cacheHeader.Len        = len;
  cacheHeader.Mode       = VirtualEeprom_ReadMode;
  cacheHeader.Type       = VirtualEeprom_Variable;
    
  if (cacheHeader.Len > 0) {
     Queue_writeItem (&eeprom->CommandQueue, &cacheHeader);
  }
  else {
     return VirtualEeprom_InvalidParameter;
  }
  return VirtualEeprom_Ok;
}





/**
 * @brief this is handle Function that must be use into the interrupt Or while(1) to process the read and write NonBlocking
 * 
 * @return VirtualEeprom_Result 
 */
VirtualEeprom_Result VirtualEeprom_handle (void) {
  VirtualEeprom* pEeprom = __eeprom();
  VirtualEeprom_Result  result;
  while (pEeprom != NULL) {  
    if (Queue_available (&pEeprom->CommandQueue) > 0 && pEeprom->CommandHeaderInProcess.Len < 1) {
       Queue_readItem (&pEeprom->CommandQueue, &pEeprom->CommandHeaderInProcess);  
      switch (pEeprom->CommandHeaderInProcess.Mode) {
        case (VirtualEeprom_WriteMode):
          if (pEeprom->CommandHeaderInProcess.Type == VirtualEeprom_Const) {
                Stream_readBytes(&pEeprom->WriteStream, (uint8_t*)&pEeprom->ConstVal, sizeof(uint32_t));
          }
          break;
        case (VirtualEeprom_ReadMode):
             memcpy (&pEeprom->ReadCommand, &pEeprom->CommandHeaderInProcess, sizeof(VirtualEeprom_CommandHeader));
          break;
      }
    }

    if (pEeprom->CommandHeaderInProcess.Len > 0) {
        result = VIRTUAL_VirtualEeprom_PROCESS_ARRAY[pEeprom->CommandHeaderInProcess.Mode](pEeprom);
    }
    __next(pEeprom);
  }
}





/**
 * @brief this is private Function used in the handle to process NonBlocking write
 *  
 * @param eeprom Address of VirtualEeprom struct
 *
 */
VirtualEeprom_Result VirtualEeprom_writeProcess (VirtualEeprom* eeprom) {
  uint16_t len     = 0;
  VirtualEeprom_Result result;
  switch (eeprom->CommandHeaderInProcess.Type) {  
    case (VirtualEeprom_Variable) :
      len             = eeprom->CommandHeaderInProcess.Len > Stream_directAvailable (&eeprom->WriteStream) ? Stream_directAvailable (&eeprom->WriteStream) : eeprom->CommandHeaderInProcess.Len; 
      eeprom->OVF     = len > eeprom->Config->PageSize ?     1 : 0;    
      eeprom->TempLen = eeprom->OVF ? eeprom->Config->PageSize : len;
      result = VirtualEeprom_writeBlocking (eeprom, eeprom->CommandHeaderInProcess.MemAddress, Stream_getReadPtr(&eeprom->WriteStream), eeprom->TempLen);
      if (result == VirtualEeprom_Ok) { 
        eeprom->CommandHeaderInProcess.Len        -= eeprom->TempLen;
        eeprom->CommandHeaderInProcess.MemAddress += eeprom->TempLen;
        Stream_moveReadPos(&eeprom->WriteStream, eeprom->TempLen);
      }
      break;
    
    case (VirtualEeprom_Const):
      eeprom->OVF     = eeprom->CommandHeaderInProcess.Len > eeprom->Config->PageSize ? 1 : 0; //- (eeprom->CommandHeaderInProcess.MemAddress % eeprom->Config->PageSize))     ? 1 : 0;
    eeprom->TempLen = eeprom->OVF ? eeprom->Config->PageSize : eeprom->CommandHeaderInProcess.Len; //- (eeprom->CommandHeaderInProcess.MemAddress % eeprom->Config->PageSize) : eeprom->CommandHeaderInProcess.Len;
      result = VirtualEeprom_writeBlocking (eeprom, eeprom->CommandHeaderInProcess.MemAddress, eeprom->ConstVal, eeprom->TempLen);
      if (result == VirtualEeprom_Ok) { 
        eeprom->ConstVal                          += eeprom->TempLen;           
        eeprom->CommandHeaderInProcess.MemAddress += eeprom->TempLen;
        eeprom->CommandHeaderInProcess.Len        -= eeprom->TempLen;
      }
      break;
  }
  return result;
}  





/**
 * @brief this Function use for compare 2 Array user can test the write and Read,  
 * 
 * @param arr1 Address of Array1
 * @param arr2 Address of Array2
 * @param len  Length user want to compare between the Array1 & Array2 
 * @return int8_t / return 0 if the two Array is Equal
 */
int8_t VirtualEeprom_AssertMemory (uint8_t* arr1, uint8_t* arr2, uint8_t len) {
  return memcmp (arr1, arr2, len);
}


/**
 * @brief this is private Function used in the handle to process NonBlocking Read
 * 
 * @param eeprom Address of VirtualEeprom struct
 *
 */
VirtualEeprom_Result VirtualEeprom_readProcess (VirtualEeprom* eeprom) {
   Stream  readTempStream; 
   VirtualEeprom_Result result;
   eeprom->OVF     = eeprom->CommandHeaderInProcess.Len > eeprom->Config->PageSize ? 1 : 0;
   eeprom->TempLen = eeprom->OVF ? eeprom->Config->PageSize : eeprom->CommandHeaderInProcess.Len;
   result          = VirtualEeprom_readBlocking (eeprom, eeprom->CommandHeaderInProcess.MemAddress, Stream_getWritePtr(&eeprom->ReadStream), eeprom->TempLen);     
   
   if (result == VirtualEeprom_Ok) {
     Stream_moveWritePos (&eeprom->ReadStream, eeprom->TempLen);      
     eeprom->CommandHeaderInProcess.Len -= eeprom->TempLen;
     if (eeprom->Callbacks.onRead != NULL && eeprom->CommandHeaderInProcess.Len < 1) {
       //Stream_lockRead     (&eeprom->ReadStream, &readTempStream, eeprom->TempLen);
       Stream_lockRead     (&eeprom->ReadStream, &readTempStream, eeprom->ReadCommand.Len);
       eeprom->Callbacks.onRead(&readTempStream, &eeprom->ReadCommand);
       Stream_unlockRead   (&eeprom->ReadStream, &readTempStream);
     }
     eeprom->CommandHeaderInProcess.MemAddress += eeprom->TempLen;
   }
   return result;
}



void VirtualEeprom_erase (VirtualEeprom* eeprom, uint32_t startAddress) {
   virtualEepromDriver->ErasePage(eeprom, startAddress);
}




#if defined(VIRTUAL_EEPROM_MCU_F407) || defined(VIRTUAL_EEPROM_MCU_F479)

int8_t VirtualEeprom_getSector (uint32_t address) {
  
  int8_t sectorNumber = 0;
  
  if (address >= SECTOR_0_ADDRESS && address < SECTOR_4_ADDRESS) {
    sectorNumber = address / (16 * 1024);
  }
  else if (address >= SECTOR_4_ADDRESS && address < SECTOR_5_ADDRESS) {
    sectorNumber = VIRTUAL_EEPROM_FLASH_SECTOR_4;
  }
  else if (address >= SECTOR_5_ADDRESS && address < SECTOR_11_ADDRESS + (128 * 1024)) {
    sectorNumber = VIRTUAL_EEPROM_FLASH_SECTOR_4 + (address - SECTOR_0_ADDRESS)/ (128 * 1024);
  }
  
#ifdef VIRTUAL_EEPROM_MCU_F479
  else if (address >= SECTOR_12_ADDRESS && address < SECTOR_16_ADDRESS) {
    sectorNumber = VIRTUAL_EEPROM_FLASH_SECTOR_11 + (address - SECTOR_0_ADDRESS) / (16 * 1024);
  }
  else if (address >= SECTOR_16_ADDRESS && address < SECTOR_17_ADDRESS) {
    sectorNumber = VIRTUAL_EEPROM_FLASH_SECTOR_16;
  }
  else if (address >= SECTOR_17_ADDRESS && address < SECTOR_END_ADDRESS) {
    sectorNumber = VIRTUAL_EEPROM_FLASH_SECTOR_16 + (address - SECTOR_0_ADDRESS) / (128 * 1024);
  }
#endif
  else {
    sectorNumber = -1; /// For UnKnown Address 
  }
  return sectorNumber;
}
  
#endif




/************************************************** Write/Read NonBlocking *********************************************************/
/**
 * @brief with this Function user can WriteNonBlocking UInt8 Value in VirtualEeprom
 * 
 * @param eeprom Address of VirtualEeprom struct
 * @param val Value
 * @param addr VirtualAddress user want to writeUInt8 Value there
 * @return VirtualEeprom_Result 
 */
VirtualEeprom_Result VirtualEeprom_writeUInt8 (VirtualEeprom* eeprom, uint8_t val, uint16_t addr) {
    return VirtualEeprom_write (eeprom, addr, (uint8_t*)&val, VirtualEeprom_Variable, sizeof(val));
}
/**
 * @brief with this Function user can readNonBlocking UInt8 Value From VirtualEeprom 
 * 
 * @param eeprom Address of VirtualEeprom struct
 * @param addr  VirtualAddress user want to readUInt8 from there
 */
void VirtualEeprom_readUInt8 (VirtualEeprom* eeprom, uint16_t addr) {
    VirtualEeprom_read (eeprom, addr, sizeof(uint8_t));
}




/**
 * @brief with this Function user can WriteNonBlocking UInt16 Value in VirtualEeprom
 * 
 * @param eeprom Address of VirtualEeprom struct
 * @param val Value
 * @param addr VirtualAddress user want to writeUInt16 value there
 * @return VirtualEeprom_Result 
 */
VirtualEeprom_Result VirtualEeprom_writeUInt16 (VirtualEeprom* eeprom, uint16_t val, uint16_t addr) {
    return VirtualEeprom_write (eeprom, addr, (uint8_t*)&val, VirtualEeprom_Variable, sizeof(val));
}
/**
 * @brief with this Function user can readNonBlocking UInt16 Value From VirtualEeprom
 * 
 * @param eeprom Address of VirtualEeprom struct
 * @param addr VirtualAddress user want to readUInt16 from there
 */
void VirtualEeprom_readUInt16 (VirtualEeprom* eeprom, uint16_t addr) {
    VirtualEeprom_read (eeprom, addr, sizeof(uint16_t));
}





/**
 * @brief with this Function user can WriteNonBlocking UInt32 Value in VirtualEeprom
 * 
 * @param eeprom Address of VirtualEeprom struct
 * @param val Value
 * @param addr VirtualAddress user want to writeUInt32 value there
 * @return VirtualEeprom_Result 
 */
VirtualEeprom_Result VirtualEeprom_writeUInt32 (VirtualEeprom* eeprom, uint32_t val, uint16_t addr) {
    return VirtualEeprom_write (eeprom, addr, (uint8_t*)&val, VirtualEeprom_Variable, sizeof(val));
}
/**
 * @brief with this Function user can readNonBlocking UInt32 Value From VirtualEeprom
 * 
 * @param eeprom Address of VirtualEeprom struct
 * @param addr VirtualAddress user want to readUInt32 from there
 */
void VirtualEeprom_readUInt32 (VirtualEeprom* eeprom, uint16_t addr) {
    VirtualEeprom_read (eeprom, addr, sizeof(uint32_t));
}




/**
 * @brief with this Function user can WriteNonBlocking UInt64 Value in VirtualEeprom
 * 
 * @param eeprom Address of VirtualEeprom struct
 * @param val Value
 * @param addr VirtualAddress user want to writeUInt64 value there
 * @return VirtualEeprom_Result 
 */
VirtualEeprom_Result VirtualEeprom_writeUInt64 (VirtualEeprom* eeprom, uint64_t val, uint16_t addr) {
    return VirtualEeprom_write (eeprom, addr, (uint8_t*)&val, VirtualEeprom_Variable, sizeof(val));
}
/**
 * @brief with this Function user can readNonBlocking UInt64 Value From VirtualEeprom
 * 
 * @param eeprom Address of VirtualEeprom struct
 * @param addr VirtualAddress user want to readUInt64 from there
 */
void VirtualEeprom_readUInt64 (VirtualEeprom* eeprom, uint16_t addr) {
    VirtualEeprom_read (eeprom, addr, sizeof(uint64_t));
}



/************************************************** Write/Read Blocking *********************************************************/

/**
 * @brief with this Function user can WriteBlocking UInt8 Value in VirtualEeprom
 * 
 * @param eeprom Address of VirtualEeprom struct
 * @param val Value
 * @param addr VirtualAddress user want to writeUInt8 value there
 * @return VirtualEeprom_Result 
 */
VirtualEeprom_Result VirtualEeprom_writeUInt8Blocking (VirtualEeprom* eeprom, uint8_t val, uint16_t addr) {
    return VirtualEeprom_writeBlocking (eeprom, addr, (uint8_t*)&val, sizeof(val));
}
/**
 * @brief with this Function user can ReadBlocking UInt8 Value From VirtualEeprom
 * 
 * @param eeprom Address of VirtualEeprom struct
 * @param addr VirtualAddress user want to readUInt8 from there
 * @return uint8_t 
 */
uint8_t VirtualEeprom_readUInt8Blocking (VirtualEeprom* eeprom, uint16_t addr) {
    uint8_t val = 0;
    VirtualEeprom_readBlocking (eeprom, addr, (uint8_t*)&val, sizeof(val));
    return val;
}




/**
 * @brief with this Function user can WriteBlocking UInt16 Value in VirtualEeprom
 * 
 * @param eeprom Address of VirtualEeprom struct
 * @param val Value
 * @param addr VirtualAddress user want to writeUInt16 value there
 * @return VirtualEeprom_Result 
 */
VirtualEeprom_Result VirtualEeprom_writeUInt16Blocking (VirtualEeprom* eeprom, uint16_t val, uint16_t addr) {
    return VirtualEeprom_writeBlocking (eeprom, addr, (uint8_t*)&val, sizeof(val));
}
/**
 * @brief with this Function user can ReadBlocking UInt16 Value From VirtualEeprom
 * 
 * @param eeprom Address of VirtualEeprom struct
 * @param addr VirtualAddress user want to readUInt16 from there
 * @return uint16_t 
 */
uint16_t VirtualEeprom_readUInt16Blocking (VirtualEeprom* eeprom, uint16_t addr) {
    uint16_t val = 0;
    VirtualEeprom_readBlocking (eeprom, addr, (uint8_t*)&val, sizeof(val));
    return val;
}




/**
 * @brief with this Function user can WriteBlocking UInt32 Value in VirtualEeprom
 * 
 * @param eeprom Address of VirtualEeprom struct
 * @param val Value
 * @param addr VirtualAddress user want to writeUInt32 value there
 * @return VirtualEeprom_Result 
 */
VirtualEeprom_Result VirtualEeprom_writeUInt32Blocking (VirtualEeprom* eeprom, uint32_t val, uint16_t addr) {
    return VirtualEeprom_writeBlocking (eeprom, addr, (uint8_t*)&val, sizeof(val));
}
/**
 * @brief with this Function user can ReadBlocking UInt32 Value From VirtualEeprom
 * 
 * @param eeprom Address of VirtualEeprom struct
 * @param addr VirtualAddress user want to readUInt32 from there
 * @return uint32_t 
 */
uint32_t VirtualEeprom_readUInt32Blocking (VirtualEeprom* eeprom, uint16_t addr) {
    uint32_t val = 0;
    VirtualEeprom_readBlocking(eeprom, addr, (uint8_t*)&val, sizeof(val));
    return val;
}




/**
 * @brief with this Function user can WriteBlocking UInt64 Value in VirtualEeprom
 * 
 * @param eeprom Address of VirtualEeprom struct
 * @param val Value
 * @param addr VirtualAddress user want to writeUInt64 value there
 * @return VirtualEeprom_Result 
 */
VirtualEeprom_Result VirtualEeprom_writeUInt64Blocking (VirtualEeprom* eeprom, uint64_t val, uint16_t addr) {
    return VirtualEeprom_writeBlocking (eeprom, addr, (uint8_t*)&val, sizeof(val));
}
/**
 * @brief with this Function user can ReadBlocking UInt64 Value From VirtualEeprom
 * 
 * @param eeprom Address of VirtualEeprom struct
 * @param addr VirtualAddress user want to readUInt64 from there
 * @return uint64_t 
 */
uint64_t VirtualEeprom_readUInt64Blocking (VirtualEeprom* eeprom, uint16_t addr) {
    uint64_t val = 0;
    VirtualEeprom_readBlocking (eeprom, addr, (uint8_t*)&val, sizeof(val));
    return val;
}






/***********************************************Callbacks*****************************************************/
/**
 * @brief with this Function user can set OnRead Callback with desired Function to use readNonBlocking ability  
 * 
 * @param eeprom Address of VirtualEeprom struct
 * @param cb  userFunction
 */
void VirtualEeprom_onRead (VirtualEeprom* eeprom, VirtualEeprom_CallbackFn cb) {
   eeprom->Callbacks.onRead = cb;
}

/******************************** User Driver and CallBack Functions Example *********************************/
/*************************************************************************************************************/

/*
uint8_t VirtualEeprom_userFlashUnLock(void) {
  return HAL_FLASH_Unlock();
}



uint8_t VirtualEeprom_userFlashLock(void) {
  return HAL_FLASH_Lock();
}




uint8_t VirtualEeprom_userProgram (VirtualEeprom_FlashTypeProgram flashTypeProgram, uint32_t addr, uint16_t data) {
  return HAL_FLASH_Program (flashTypeProgram, addr, data); 
}




uint8_t VirtualEeprom_userErasePage (VirtualEeprom* eeprom, uint32_t pageAddress) {
  HAL_StatusTypeDef  status;  
  FLASH_EraseInitTypeDef eraseInitStruct;
  uint32_t pageError;
  uint8_t  cnt = 0;
  #ifdef VIRTUAL_EEPROM_MCU_F1
    eraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    eraseInitStruct.PageAddress = pageAddress;
    eraseInitStruct.NbPages     = 1;
  #endif
  
  #if defined(VIRTUAL_EEPROM_MCU_F407) || defined(VIRTUAL_EEPROM_MCU_F479)
  eraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
  eraseInitStruct.Sector        = VirtualEeprom_getSector(pageAddress);
  eraseInitStruct.NbSectors     = 1;
  eraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
  #endif
  
  status = HAL_FLASH_Unlock();  // Unlock the Flash memory 
  if (status == VirtualEeprom_Ok) {
      status = HAL_FLASHEx_Erase (&eraseInitStruct, &pageError); //Erase Flash
      if (status != VirtualEeprom_Ok) {
          while (cnt < VIRTUAL_EEPROM_MAXIMUM_TRY) {
            if (eeprom->NextTick < virtualEepromDriver->GetTimestamp()) {
                cnt++;
                status           = HAL_FLASHEx_Erase (&eraseInitStruct, &pageError);
                eeprom->NextTick = virtualEepromDriver->GetTimestamp ();
                if (status == VirtualEeprom_Ok) {
                   break;
                }
            }
         }
      }
  }
  status = HAL_FLASH_Lock();    // Lock the Flash memory
  return status;
}




VirtualEeprom_Timestamp VirtualEeprom_userGetTimeStamp (void) {
  return HAL_GetTick();
}




void VirtualEeprom_userOnRead (Stream* stream, VirtualEeprom_CommandHeader* commandHeader) {
    uint8_t temp[128] = {0};
    //TestStruct temp;
    Stream_readBytes (stream, (uint8_t*)temp, commandHeader->Len);
}
*/