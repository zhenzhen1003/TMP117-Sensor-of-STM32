/*
  @
  @   Date               :        1.3.2021
  @
  @																Writing by zhenzhen
  @
  @   Description        :        This Library is about TMP117 Temperature Sensor on Texas Instruments.
  @                               This is depend on Standard library for STM32 series and software simulation of SMBus.
  @
*/
/*
		���ڣ�2021��3��1��
		����ļ��ǹ��ڵ���������TMP117�����̡�
		����STM32��׼�������ģ��ķ�ʽʵ��SMBusͨѶ��TMP117�Ĵ�����д��
*/


#include "TMP117.h"

/*******************************************************************************
* ������: TMP117 ����ʼλ SMBus_StartBit
* ����  : TMP117 ����ʼλ ������ʼλ
* Input          : None
* Output         : None
* Return         : None

*******************************************************************************/
void SMBus_StartBit(void)
{
    SMBUS_SDA_H();		// Set SDA line
    SMBus_Delay(5);	    // Wait a few microseconds
    SMBUS_SCK_H();		// Set SCL line
    SMBus_Delay(5);	    // Generate bus free time between Stop
    SMBUS_SDA_L();		// Clear SDA line
    SMBus_Delay(5);	    // Hold time after (Repeated) Start
    // Condition. After this period, the first clock is generated.
    //(Thd:sta=4.0us min)��SCK=1ʱ����⵽SDA��1��0��ʾͨ�ſ�ʼ���½��أ�
    SMBUS_SCK_L();	    // Clear SCL line
    SMBus_Delay(5);	    // Wait a few microseconds
}

/*******************************************************************************
* ������: SMBus_StopBit
* ����: TMP117 ��ֹͣλ STOP condition on SMBus
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SMBus_StopBit(void)
{
    SMBUS_SCK_L();		// Clear SCL line
    SMBus_Delay(5);	    // Wait a few microseconds
    SMBUS_SDA_L();		// Clear SDA line
    SMBus_Delay(5);	    // Wait a few microseconds
    SMBUS_SCK_H();		// Set SCL line
    SMBus_Delay(5);	    // Stop condition setup time(Tsu:sto=4.0us min)
    SMBUS_SDA_H();		// Set SDA line
}

/*******************************************************************************
* ������: SMBus_SendByte
* ����: TMP117 ����һ���ֽ� Send a byte on SMBus
* Input          : Tx_buffer
* Output         : None
* Return         : None
*******************************************************************************/
u8 SMBus_SendByte(u8 Tx_buffer)
{
    u8	Bit_counter;
    u8	Ack_bit;
    u8	bit_out;

    for(Bit_counter=8; Bit_counter; Bit_counter--)
    {
        if (Tx_buffer&0x80)//������λΪ1
        {
            bit_out=1;   // �����λ��1
        }
        else  //������λΪ0
        {
            bit_out=0;  // �����λ��0
        }
        SMBus_SendBit(bit_out);	// �����λ���ͳ�ȥ
        Tx_buffer<<=1;// ����һλ�����λ�Ƴ�ȥ�ȴ���һ�����λ��ѭ��8�Σ�ÿ�ζ������λ���Ϳɰ�һ���ֽڷ���ȥ��
    }
    Ack_bit=SMBus_ReceiveBit();
    return	Ack_bit;
}

/*******************************************************************************
* ������: SMBus_SendBit
* ����: TMP117 ����һ��λ Send a bit on SMBus 82.5kHz
* Input          : bit_out
* Output         : None
* Return         : None
*******************************************************************************/
void SMBus_SendBit(u8 bit_out)
{
    if(bit_out==0)
    {
        SMBUS_SDA_L();
    }
    else
    {
        SMBUS_SDA_H();
    }
    SMBus_Delay(2);					// Tsu:dat = 250ns minimum
    SMBUS_SCK_H();					// Set SCL line
    SMBus_Delay(6);					// High Level of Clock Pulse
    SMBUS_SCK_L();					// Clear SCL line
    SMBus_Delay(3);					// Low Level of Clock Pulse
//	SMBUS_SDA_H();				    // Master release SDA line ,
    return;
}

/*******************************************************************************
* Function Name  : SMBus_ReceiveBit
* Description    : Receive a bit on SMBus
* Input          : None
* Output         : None
* Return         : Ack_bit
*******************************************************************************/
u8 SMBus_ReceiveBit(void)
{
    u8 Ack_bit;

    SMBUS_SDA_H();          //���ſ��ⲿ������������������
	SMBus_Delay(2);			// High Level of Clock Pulse
    SMBUS_SCK_H();			// Set SCL line
    SMBus_Delay(5);			// High Level of Clock Pulse
    if (SMBUS_SDA_PIN())
    {
        Ack_bit=1;
    }
    else
    {
        Ack_bit=0;
    }
    SMBUS_SCK_L();			// Clear SCL line
    SMBus_Delay(3);			// Low Level of Clock Pulse

    return	Ack_bit;
}

/*******************************************************************************
* ������: SMBus_ReceiveByte
* ����: Receive a byte on SMBus ��SMBus�н���һ���ֽڵ�����
* Input          : ack_nack
* Output         : None
* Return         : RX_buffer
*******************************************************************************/
u8 SMBus_ReceiveByte(u8 ack_nack)
{
    u8 	RX_buffer;
    u8	Bit_Counter;

    for(Bit_Counter=8; Bit_Counter; Bit_Counter--)
    {
        if(SMBus_ReceiveBit())// Get a bit from the SDA line
        {
            RX_buffer <<= 1;// If the bit is HIGH save 1  in RX_buffer
            RX_buffer |=0x01;//���Ack_bit=1�����յ�Ӧ���ź�1��0000 0001 ���л����㣬ȷ��Ϊ1
        }
        else
        {
            RX_buffer <<= 1;// If the bit is LOW save 0 in RX_buffer
            RX_buffer &=0xfe;//���Ack_bit=1�����յ�Ӧ���ź�0��1111 1110 ���������㣬ȷ��Ϊ0
        }
    }
    SMBus_SendBit(ack_nack);// Sends acknowledgment bit ��Ӧ���źŷ���ȥ�����1���ͽ�����һ��ͨ�ţ����Ϊ0�����Ͱݰ���
    return RX_buffer;
}

/*******************************************************************************
* ������: SMBus_Delay
* ����: ��ʱ  һ��ѭ��Լ1us
* Input          : time
* Output         : None
* Return         : None
*******************************************************************************/
void SMBus_Delay(u16 time)
{
    u16 i, j;
    for (i=0; i<4; i++)
    {
        for (j=0; j<time; j++);
    }
}

/*******************************************************************************
* ������: SMBus_Init
* ����: SMBus��ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SMBus_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(SMBUS_RCC, ENABLE);	 //ʹ��PB�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = SMBUS_SCK|SMBUS_SDA;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
  SMBUS_SCK_H();
  SMBUS_SDA_H();
	
	TMP117_Initialization_DEFAULT();
}

/*******************************************************************************
 * ������: SMBus_ReadMemory
 * ����: READ DATA FROM RAM/EEPROM  ��RAM��EEPROM�ж�ȡ����
 * Input          : slaveAddress, command
 * Return         : Data
 * SMBus_ReadMemory(0x00, 0x07) 0x00 ��ʾIIC�豸�Ĵӵ�ַ ��0x07����Ĵ�����ʼ��ȡ
*******************************************************************************/
u16 SMBus_ReadMemory(u8 slaveAddress, u8 command)
{
    u16 data;			// Data storage (DataH:DataL)
    u8 Pec;				// PEC byte storage
    u8 DataL=0;			// Low data byte storage
    u8 DataH=0;			// High data byte storage
    u8 arr[6];			// Buffer for the sent bytes
    u8 PecReg;			// Calculated PEC byte storage
    u8 ErrorCounter;	// Defines the number of the attempts for communication with MLX90614

    ErrorCounter=0x00;	// Initialising of ErrorCounter
	slaveAddress <<= 1;	//2-7λ��ʾ�ӻ���ַ �ӻ���ַ����һλ���Ѷ�дλ�ճ���
	
    do
    {
repeat:
        SMBus_StopBit();			    //If slave send NACK stop comunication
        --ErrorCounter;				    //Pre-decrement ErrorCounter
        if(!ErrorCounter) 			    //ErrorCounter=0?
        {
            break;					    //Yes,go out from do-while{}
        }
        SMBus_StartBit();				//Start condition
        if(SMBus_SendByte(slaveAddress))//Send SlaveAddress ���λWr=0��ʾ������д����
        {
            goto	repeat;			    //Repeat comunication again
        }
        if(SMBus_SendByte(command))	    //Send command
        {
            goto	repeat;		    	//Repeat comunication again
        }

        SMBus_StartBit();					//Repeated Start condition
        if(SMBus_SendByte(slaveAddress+1))	//Send SlaveAddress ���λRd=1��ʾ������������
        {
            goto	repeat;             	//Repeat comunication again
        }

        DataH = SMBus_ReceiveByte(ACK);	//Read high data,master must send ACK
        DataL = SMBus_ReceiveByte(ACK); //Read low data,master must send ACK
        Pec = SMBus_ReceiveByte(NACK);	//Read PEC byte, master must send NACK
        SMBus_StopBit();				//Stop condition

        arr[5] = slaveAddress;		//
        arr[4] = command;			//
        arr[3] = slaveAddress+1;	//Load array arr
        arr[2] = DataH;				//
        arr[1] = DataL;				//
        arr[0] = 0;					//
        PecReg=PEC_Calculation(arr);//Calculate CRC ����У��
    }
    while(PecReg != Pec);//If received and calculated CRC are equal go out from do-while{}
		data = (DataH<<8) | DataL;	//data=DataH:DataL
    return data;
}

/*******************************************************************************
 * ������: SMBus_WriteMemory
 * ����: WRITE DATA TO EEPROM  ��EEPROM��д������
 * Input          : slaveAddress, command, DataH, DataL
 * Return         : void
*******************************************************************************/
void SMBus_WriteMemory(u8 slaveAddress, u8 command, u8 DataH, u8 DataL)
{
	slaveAddress <<= 1;	//2-7λ��ʾ�ӻ���ַ �ӻ���ַ����һλ���Ѷ�дλ�ճ���
  SMBus_StopBit();			    //If slave send NACK stop comunication
  SMBus_StartBit();				//Start condition
	SMBus_SendByte(slaveAddress);//Send SlaveAddress ���λWr=0��ʾ������д����
	SMBus_SendByte(command);	    //Send command
	SMBus_SendByte(DataH);	//Send dataH
	SMBus_SendByte(DataL);	//Send dataL
  SMBus_StopBit();				//Stop condition
}

/*******************************************************************************
* ������: PEC_calculation
* ���� : ����У��
* Input          : pec[]
* Output         : None
* Return         : pec[0]-this byte contains calculated crc value
*******************************************************************************/
u8 PEC_Calculation(u8 pec[])
{
    u8 	crc[6];//��Ŷ���ʽ
    u8	BitPosition=47;//��������������λ��6*8=48 ���λ����47λ
    u8	shift;
    u8	i;
    u8	j;
    u8	temp;

    do
    {
        /*Load pattern value 0x00 00 00 00 01 07*/
        crc[5]=0;
        crc[4]=0;
        crc[3]=0;
        crc[2]=0;
        crc[1]=0x01;
        crc[0]=0x07;

        /*Set maximum bit position at 47 ( six bytes byte5...byte0,MSbit=47)*/
        BitPosition=47;

        /*Set shift position at 0*/
        shift=0;

        /*Find first "1" in the transmited message beginning from the MSByte byte5*/
        i=5;
        j=0;
        while((pec[i]&(0x80>>j))==0 && i>0)
        {
            BitPosition--;
            if(j<7)
            {
                j++;
            }
            else
            {
                j=0x00;
                i--;
            }
        }/*End of while */

        /*Get shift value for pattern value*/
        shift=BitPosition-8;

        /*Shift pattern value */
        while(shift)
        {
            for(i=5; i<0xFF; i--)
            {
                if((crc[i-1]&0x80) && (i>0))
                {
                    temp=1;
                }
                else
                {
                    temp=0;
                }
                crc[i]<<=1;
                crc[i]+=temp;
            }/*End of for*/
            shift--;
        }/*End of while*/

        /*Exclusive OR between pec and crc*/
        for(i=0; i<=5; i++)
        {
            pec[i] ^=crc[i];
        }/*End of for*/
    }
    while(BitPosition>8); /*End of do-while*/

    return pec[0];
}

 /*******************************************************************************
 * ������: TMP117_get_Temperature
 * ����: ���㲢�����¶�ֵ
 * Return         : SMBus_ReadMemory(SA, TMP117_TemperatureRegister)*0.0078125
*******************************************************************************/
float TMP117_get_Temperature(void)
{   
	float temp;
	temp = SMBus_ReadMemory(SA, TMP117_TemperatureRegister)*0.0078125;
	return temp;
}

/*
   @Brief         Default Initialization
   @Description   ��������������
   @Parameter     void
   @Return value  void
 */
void TMP117_Initialization_DEFAULT(void)
{
	TMP117_set_Configuration(0x02, 0x20);        //����
	TMP117_set_Temperature_Offset(0x00, 0x00); //�����¶�ƫ����
  //TMP117_set_LowLimit(0x80,0x00);       //�������ޱ���
  //TMP117_set_HighLimit(0x60,0x00);      //�������ޱ���
	TMP117_set_EEPROM_Unlock(0x00, 0x00);   //���ý���EEPROM
}

/*
   @Brief         Set Temperature Offset Value
   @Description   �����¶�ƫ����
   @Parameter     u8 first     ->  [15:8]
									u8 second    ->  [7:0]
   @Return value  void
 */
void TMP117_set_Temperature_Offset(u8 first, u8 second)
{
	SMBus_WriteMemory(SA, TMP117_Temperature_Offset, first, second);
}

/*  
   @Brief         Get Temperature Offset Value
   @Description   ��ȡ�¶�ƫ������������
   @Parameter     void
   @Return value  u16
 */
u16 TMP117_get_Temperature_Offset(void)
{
	u16 temp;
	temp = SMBus_ReadMemory(SA, TMP117_Temperature_Offset);
	return temp;      
}

/*
   @Brief         Set EEPROM Unlock Register Value
   @Description   ����EEPROM��д״̬
   @Parameter     u8 first     ->  [15:8]
									u8 second    ->  [7:0]
   @Return value  void
 */
void TMP117_set_EEPROM_Unlock(u8 first, u8 second)
{
	SMBus_WriteMemory(SA, TMP117_EEPROM_Uclock, first, second);
}

/*
   @Brief         Set LowLimit
   @Description   ���ñ����¶ȵ�����
   @Parameter     u8 first     ->  [15:8]
									u8 second    ->  [7:0]
   @Return value  void
 */
void TMP117_set_LowLimit(u8 first, u8 second)
{
	SMBus_WriteMemory(SA, TMP117_TemperatureLowLimit, first, second);
}

/*  
   @Brief         Get LowLimit
   @Description   ��ȡ�¶ȱ�������ֵ
   @Parameter     void
   @Return value  u16
 */
u16 TMP117_get_LowLimit(void)
{
	u16 temp;
	temp = SMBus_ReadMemory(SA, TMP117_TemperatureLowLimit);
	return temp;  
}

/*
   @Brief         Set HighLimit
   @Description   ���ñ����¶ȵ�����
   @Parameter     u8 first     ->  [15:8]
									u8 second    ->  [7:0]
   @Return value  void
 */
void TMP117_set_HighLimit(u8 first, u8 second)
{
	SMBus_WriteMemory(SA, TMP117_TemperatureHighLimit, first, second);
}

/*  
   @Brief         Get Highlimit
   @Description   ��ȡ�¶ȱ�������ֵ
   @Parameter     void
   @Return value  u16
 */
u16 TMP117_get_HighLimit(void)
{
	u16 temp;
	temp = SMBus_ReadMemory(SA, TMP117_TemperatureHighLimit);
	return temp;        
}

/*
   @Brief         Set Configuration
   @Description   ����TMP117������
   @Parameter     u8 first     ->  [15:8]
									u8 second    ->  [7:0]
   @Return value  void
 */
void TMP117_set_Configuration(u8 first, u8 second)
{
	SMBus_WriteMemory(SA, TMP117_ConfigurationRegister, first, second);
}
/*  
   @Brief         Get Configuration
   @Description   ��ȡTMP117��������
   @Parameter     void
   @Return value  u16
 */
u16 TMP117_get_Configuration(void)
{
	u16 temp;
	temp = SMBus_ReadMemory(SA, TMP117_ConfigurationRegister);
	return temp;       
}

/*  
   @Brief         Get ID Register
   @Description   ��ȡTMP117��ID
   @Parameter     void
   @Return value  u16
 */
u16 TMP117_get_ID_Register(void)
{
	u16 temp;
	temp = SMBus_ReadMemory(SA, TMP117_ID_Register);
	return temp;
}

/*  
   @Brief         
   @Description   ����EEPROM1
   @Parameter     u8 first     ->  [15:8]
                  u8 second    ->  [7:0]
   @Return value  void
 */
void TMP117_set_EEPROM1(u8 first,u8 second)
{
	SMBus_WriteMemory(SA, TMP117_EEPROM1, first, second);
}

/*  
   @Brief         
   @Description   
   @Parameter     void
   @Return value  u16
 */
u16 TMP117_get_EEPROM1(void)
{
	u16 temp;
	temp = SMBus_ReadMemory(SA, TMP117_EEPROM1);
	return temp;      
}

/*  
   @Brief         
   @Description   
   @Parameter     u8 first     ->  [15:8]
                  u8 second    ->  [7:0]
   @Return value  void
 */
void TMP117_set_EEPROM2(u8 first,u8 second)
{
	SMBus_WriteMemory(SA, TMP117_EEPROM2, first, second);
}

/*  
   @Brief         
   @Description   
   @Parameter     void
   @Return value  u16
 */
u16 TMP117_get_EEPROM2(void)
{
	u16 temp;
	temp = SMBus_ReadMemory(SA, TMP117_EEPROM2);
	return temp;    
}

/*  
   @Brief         
   @Description   
   @Parameter     u8 first     ->  [15:8]
                  u8 second    ->  [7:0]
   @Return value  void
 */
void TMP117_set_EEPROM3(u8 first,u8 second)
{
	SMBus_WriteMemory(SA, TMP117_EEPROM3, first, second);
}

/*  
   @Brief         Get EEPROM3 Value
   @Description   
   @Parameter     void
   @Return value  u16
 */
u16 TMP117_get_EEPROM3(void)
{
	u16 temp;
	temp = SMBus_ReadMemory(SA, TMP117_EEPROM3);
	return temp;      
}
/*********************************END OF FILE*********************************/
