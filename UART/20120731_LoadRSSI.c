
/*----------------------------------------------------------------------------
 *  Demo Application for SimpliciTI
 *
 *  L. Friedman
 *  Texas Instruments, Inc.
 *----------------------------------------------------------------------------
 */
/******************************************************************************************

  Copyright 2007-2009 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights granted under
  the terms of a software license agreement between the user who downloaded the software,
  his/her employer (which must be your employer) and Texas Instruments Incorporated (the
  "License"). You may not use this Software unless you agree to abide by the terms of the
  License. The License limits your use, and you acknowledge, that the Software may not be
  modified, copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio frequency
  transceiver, which is integrated into your product. Other than for the foregoing purpose,
  you may not use, reproduce, copy, prepare derivative works of, modify, distribute,
  perform, display or sell this Software and/or its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS?  WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY
  WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
  IN NO EVENT SHALL TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE
  THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY
  INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST
  DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY
  THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
**************************************************************************************************/
#define INDICATOR_TIME_OUT 250;

#include <stdio.h>
#include "bsp.h"
#include "mrfi.h"
#include "nwk_types.h"
#include "nwk_api.h"
#include "nwk_pll.h"
#include "bsp_leds.h"
#ifdef MRFI_CC430
  #include "uart_intfc_cc430.h"
#else
  #include "uart_intfc.h"
#endif


//static int8_t  sSample;
static void itoa(int n,unsigned char s[]);
static void reverse(unsigned char s[]);
size_t strlen(const unsigned char * str);
size_t strlen_b(const signed char * str);

void main1 (void);
void main (void)
{
  main1();
}

void main1 (void)
{
  /* holds length of current message */
  uint8_t len; 
  
  /* the link token */
  linkID_t LinkID = 0;
  
  /* the transmit and receive buffers */
  uint8_t rx[MAX_APP_PAYLOAD], tx[MAX_APP_PAYLOAD];

  /* holds led indicator time out counts */
  uint16_t led_tmr;
  
  int8_t rssi_value;
  

  BSP_Init( );

  SMPL_Init( NULL );
  
  uart_intfc_init( );

  /* turn on the radio so we are always able to receive data asynchronously */
  SMPL_Ioctl( IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_RXON, NULL );
  
  /* turn on LED. */
  BSP_TURN_ON_LED1( );

#ifdef  LINK_TO
  {
    uint8_t cnt = 0;
    tx_send_wait( "Linking to...\r\n", 15 );
    while (SMPL_SUCCESS != SMPL_Link(&LinkID))
      if( cnt++ == 0 )
      {
        /* blink LED until we link successfully */
        BSP_TOGGLE_LED1( );
      }
  }
#else // ifdef LINK_LISTEN
  tx_send_wait( "Listening for Link...\r\n", 23 );
  while (SMPL_SUCCESS != SMPL_LinkListen(&LinkID))
  {
    /* blink LED until we link successfully */
    BSP_TOGGLE_LED1( );
  }
#endif

  tx_send_wait( "Link Established!\r\nReady...\r\n", 29 );
  
  /* turn off the led */
  BSP_TURN_OFF_LED1( );
  
  main_loop:
    /* Check to see if we received any characters from the other radio
     * and if we have send them out the uart and reset indicator timeout.
     * Prioritize on radio received links as that buffer is the most time
     * critical with respect to the UART buffers.
     */
    if( SMPL_Receive( LinkID, tx, &len ) == SMPL_SUCCESS )
    {
      /* blocking call but should be ok if both ends have same uart buad rate */
      //tx_send_wait( tx, len );
      led_tmr = INDICATOR_TIME_OUT;   /* update activity time out */
      
      
      /*
      int8_t dbm;
      SMPL_Ioctl(IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_RSSI, (void *)&dbm);
      sSample= dbm;
      tx_send_wait(&char2send, 1 );//input only (unsigned char)
      tx_send_wait("\n", 1 );*/
      
      
      unsigned char Test2;
      //unsigned char Test;
      rssi_value = 0 ;
      
       ioctlRadioSiginfo_t sigInfo1;
       sigInfo1.lid=LinkID; 
       smplStatus_t SMPL_SUCCESS = SMPL_Ioctl(IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_SIGINFO, (void *)&sigInfo1);
    
      //if( SMPL_SUCCESS == SMPL_Ioctl( IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_RSSI,(void*)&rssi_value) )
      {
        
   
        //transmitData( i, (signed char)sigInfo.sigInfo.rssi, (char*)msg );
        
        //MRFI_Rssi()
        //Test2=RSSI;
        //Test=rssi_value;
        //int8_t i =-127;
        //int i =-127;
        unsigned char rssi_tmp[10];
        int8_t rssi_int = sigInfo1.sigInfo.rssi;
        itoa(rssi_int,rssi_tmp);
        //tx_send_wait("This is Test :", sizeof("This is Test :") );
        //tx_send_wait(&Test2, strlen(&Test2));
        tx_send_wait(&rssi_tmp, strlen(rssi_tmp));
        //tx_send_wait("\r\n", sizeof("\r\n") );
        
        int8_t mk=0x01,bitshift=0;
              
        tx_send_wait("=",1);  
        
        for (bitshift=0;bitshift<8;bitshift++)
        {
          if( (mk<<bitshift) & rssi_int ) 
            tx_send_wait("1",1);
          else
            tx_send_wait("0",1);
          //tx_send_wait("x",1);
          MRFI_DelayMs( 5 );
        }
        tx_send_wait("\r\n", sizeof("\r\n") );
        
        
         
        
        /*        
        tx_send_wait("This is Test2 :", sizeof("This is Test2 :") );
        tx_send_wait( &Test2, sizeof(Test2) );
        tx_send_wait("\n", sizeof("\n") );
        */
      }
      
    }

    FHSS_ACTIVE( if( nwk_pllBackgrounder( false ) != false ) );
    {
      /* check to see if the host has sent any characters and if it has
      * then send them over the radio link and reset indicator timeout.
      */
      len = rx_receive( rx, MAX_APP_PAYLOAD );
      if( len != 0 )
      {
        while( SMPL_Send( LinkID, rx, len ) != SMPL_SUCCESS )
          ;
        /*
        if( SMPL_SUCCESS == SMPL_Ioctl( IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_RSSI,&rssi_value ) ){
          SMPL_Send( LinkID, &rssi_value, sizeof(rssi_value) );
        }*/
        led_tmr = INDICATOR_TIME_OUT;   /* update activity time out */
        
        /* By forcing a minimum delay between transmissions we guarantee
        * a window for receiving packets.  This is necessary as the radio
        * link is half duplex while the UART is full duplex.  Without this
        * delay mechanism, packets can get lost as both ends may attempt to
        * transmit at the same time which the CCA algorithm fails to handle.
        */
        MRFI_DelayMs( 5 );
      }
    }
    
    /* manage led indicator */
    if( led_tmr != 0 )
    {
      led_tmr--;
      BSP_TURN_ON_LED1( );
    }
    else
      BSP_TURN_OFF_LED1( );

    
    goto main_loop; /* do it again and again and again and ... */
    

}

    /* itoa:  convert n to characters in s */
    static void itoa(int n,unsigned char s[])
    {
      int i, sign;
 
      if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
      i = 0;
      do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
      } while ((n /= 10) > 0);     /* delete it */
       if (sign < 0)
         s[i++] = '-';
      s[i] = '\0';
      reverse(s);
    }
     /* reverse:  reverse string s in place */
    static void reverse(unsigned char s[])
    {
      int i, j;
      unsigned char c;
 
      for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
      }
    }

    /*Too time-consuming*/
    size_t strlen(const unsigned char * str) {
      const unsigned char *cp =  str;
      while (*cp++ );
      return (cp - str - 1 );
    }
    /*Too time-consuming*/
    size_t strlen_b(const signed char * str) {
      const signed char *cp =  str;
      while (*cp++ );
      return (cp - str - 1 );
    } 