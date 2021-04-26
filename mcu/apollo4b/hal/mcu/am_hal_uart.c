//*****************************************************************************
//
//! @file am_hal_uart.c
//!
//! @brief Hardware abstraction for the UART
//!
//! @addtogroup
//! @ingroup
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2021, Ambiq Micro, Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// Third party software included in this distribution is subject to the
// additional license terms as defined in the /docs/licenses directory.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// This is part of revision b0-release-20210111-833-gc25608de46 of the AmbiqSuite Development Package.
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "am_mcu_apollo.h"

//*****************************************************************************
//
// UART magic number for handle verification.
//
//*****************************************************************************
#define AM_HAL_MAGIC_UART               0xEA9E06

#define AM_HAL_UART_CHK_HANDLE(h)                                             \
    ((h) &&                                                                   \
     ((am_hal_handle_prefix_t *)(h))->s.bInit &&                              \
     (((am_hal_handle_prefix_t *)(h))->s.magic == AM_HAL_MAGIC_UART))

//*****************************************************************************
//
// UART FIFO Size
//
//*****************************************************************************
#define AM_HAL_UART_FIFO_MAX 32

//*****************************************************************************
//
// Delay constant
//
//*****************************************************************************
#define ONE_BYTE_DELAY(pState)      am_hal_delay_us(10);
#define RETURN_ON_ERROR(x)                                                    \
    if ((x) != AM_HAL_STATUS_SUCCESS)                                         \
    {                                                                         \
        return (x);                                                           \
    };
//*****************************************************************************
//
// Structure for handling UART register state information for power up/down
//
//*****************************************************************************
typedef struct
{
    bool bValid;
    uint32_t regILPR;
    uint32_t regIBRD;
    uint32_t regFBRD;
    uint32_t regLCRH;
    uint32_t regCR;
    uint32_t regIFLS;
    uint32_t regIER;
}
am_hal_uart_register_state_t;

//*****************************************************************************
//
// Structure for handling UART HAL state information.
//
//*****************************************************************************
typedef struct
{
    // For internal verification purposes
    am_hal_handle_prefix_t prefix;

    // Register state for power-up/power-down
    am_hal_uart_register_state_t sRegState;

    // UART module number.
    uint32_t ui32Module;

    // Most recently configured baud rate. (Do we really need this?)
    uint32_t ui32BaudRate;

    // State information about nonblocking transfers.
    bool bCurrentlyReading;
    am_hal_uart_transfer_t sActiveRead;

    bool bCurrentlyWriting;
    am_hal_uart_transfer_t sActiveWrite;

    // Queued write/read implementation
    bool bEnableTxQueue;
    am_hal_queue_t sTxQueue;

    bool bEnableRxQueue;
    am_hal_queue_t sRxQueue;
}
am_hal_uart_state_t;

// Create one state structure for each physical UART.
am_hal_uart_state_t g_am_hal_uart_states[AM_REG_UART_NUM_MODULES];

//*****************************************************************************
//
// Static function prototypes.
//
//*****************************************************************************
static uint32_t config_baudrate(uint32_t ui32Module,
                                uint32_t ui32DesiredBaudrate,
                                uint32_t *pui32ActualBaud);

static uint32_t blocking_write(void *pHandle,
                               const am_hal_uart_transfer_t *psTransfer);

static uint32_t blocking_read(void *pHandle,
                              const am_hal_uart_transfer_t *psTransfer);

static uint32_t nonblocking_write(void *pHandle,
                                  const am_hal_uart_transfer_t *psTransfer);

static uint32_t nonblocking_read(void *pHandle,
                                 const am_hal_uart_transfer_t *psTransfer);

static void nonblocking_write_sm(void *pHandle);
static void nonblocking_read_sm(void *pHandle);
static uint32_t tx_queue_update(void *pHandle);
static uint32_t rx_queue_update(void *pHandle);

//*****************************************************************************
//
// Initialize the UART
//
//*****************************************************************************
uint32_t
am_hal_uart_initialize(uint32_t ui32Module, void **ppHandle)
{
    //
    // Check that the request module is in range.
    //
    if (ui32Module >= AM_REG_UART_NUM_MODULES )
    {
        return AM_HAL_STATUS_OUT_OF_RANGE;
    }

    //
    // Check for valid arguements.
    //
    if (!ppHandle)
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }

    //
    // Make sure we're not re-activating an active handle.
    //
    if (AM_HAL_UART_CHK_HANDLE(*ppHandle))
    {
        return AM_HAL_STATUS_INVALID_OPERATION;
    }

    //
    // Initialize the handle.
    //
    g_am_hal_uart_states[ui32Module].prefix.s.bInit = true;
    g_am_hal_uart_states[ui32Module].prefix.s.magic = AM_HAL_MAGIC_UART;
    g_am_hal_uart_states[ui32Module].ui32Module = ui32Module;
    g_am_hal_uart_states[ui32Module].sRegState.bValid = false;
    g_am_hal_uart_states[ui32Module].ui32BaudRate = 0;
    g_am_hal_uart_states[ui32Module].bCurrentlyReading = 0;
    g_am_hal_uart_states[ui32Module].bCurrentlyWriting = 0;
    g_am_hal_uart_states[ui32Module].bEnableTxQueue = 0;
    g_am_hal_uart_states[ui32Module].bEnableRxQueue = 0;

    //
    // Return the handle.
    //
    *ppHandle = (void *)&g_am_hal_uart_states[ui32Module];

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Deinitialize the UART interface.
//
//*****************************************************************************
uint32_t
am_hal_uart_deinitialize(void *pHandle)
{
    am_hal_uart_state_t *pState = (am_hal_uart_state_t *)pHandle;

    //
    // Check the handle.
    //
    if (!AM_HAL_UART_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    //
    // Reset the handle.
    //
    pState->prefix.s.bInit = false;
    pState->prefix.s.magic = 0;
    pState->ui32Module = 0;
    pState->sRegState.bValid = false;

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Change the power state of the UART module.
//
//*****************************************************************************
uint32_t
am_hal_uart_power_control(void *pHandle, uint32_t ePowerState,
                          bool bRetainState)
{
    am_hal_uart_state_t *pState = (am_hal_uart_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;

    am_hal_pwrctrl_periph_e eUARTPowerModule = ((am_hal_pwrctrl_periph_e)
                                                (AM_HAL_PWRCTRL_PERIPH_UART0 +
                                                 ui32Module));

    //
    // Check to make sure this is a valid handle.
    //
    if (!AM_HAL_UART_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    //
    // Decode the requested power state and update UART operation accordingly.
    //
    switch (ePowerState)
    {
        //
        // Turn on the UART.
        //
        case AM_HAL_SYSCTRL_WAKE:
            //
            // Make sure we don't try to restore an invalid state.
            //
            if (bRetainState && !pState->sRegState.bValid)
            {
                return AM_HAL_STATUS_INVALID_OPERATION;
            }

            //
            // Enable power control.
            //
            am_hal_pwrctrl_periph_enable(eUARTPowerModule);

            if (bRetainState)
            {
                //
                // Restore UART registers
                //
                // AM_CRITICAL_BEGIN

                UARTn(ui32Module)->ILPR = pState->sRegState.regILPR;
                UARTn(ui32Module)->IBRD = pState->sRegState.regIBRD;
                UARTn(ui32Module)->FBRD = pState->sRegState.regFBRD;
                UARTn(ui32Module)->LCRH = pState->sRegState.regLCRH;
                UARTn(ui32Module)->CR   = pState->sRegState.regCR;
                UARTn(ui32Module)->IFLS = pState->sRegState.regIFLS;
                UARTn(ui32Module)->IER  = pState->sRegState.regIER;

                pState->sRegState.bValid = false;

                // AM_CRITICAL_END
            }
            break;

        //
        // Turn off the UART.
        //
        case AM_HAL_SYSCTRL_NORMALSLEEP:
        case AM_HAL_SYSCTRL_DEEPSLEEP:
            if (bRetainState)
            {
                // AM_CRITICAL_BEGIN

                pState->sRegState.regILPR = UARTn(ui32Module)->ILPR;
                pState->sRegState.regIBRD = UARTn(ui32Module)->IBRD;
                pState->sRegState.regFBRD = UARTn(ui32Module)->FBRD;
                pState->sRegState.regLCRH = UARTn(ui32Module)->LCRH;
                pState->sRegState.regCR   = UARTn(ui32Module)->CR;
                pState->sRegState.regIFLS = UARTn(ui32Module)->IFLS;
                pState->sRegState.regIER  = UARTn(ui32Module)->IER;
                pState->sRegState.bValid = true;

                // AM_CRITICAL_END
            }

            //
            // Clear all interrupts before sleeping as having a pending UART
            // interrupt burns power.
            //
            am_hal_uart_interrupt_clear(pState, 0xFFFFFFFF);

            //
            // Disable power control.
            //
            am_hal_pwrctrl_periph_disable(eUARTPowerModule);
            break;

        default:
            return AM_HAL_STATUS_INVALID_ARG;
    }

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;
} // am_hal_uart_power_control()

//*****************************************************************************
//
// Allows the UART HAL to use extra space to store TX and RX data.
//
//*****************************************************************************
uint32_t
am_hal_uart_buffer_configure(void *pHandle, uint8_t *pui8TxBuffer,
                             uint32_t ui32TxBufferSize,
                             uint8_t *pui8RxBuffer, uint32_t ui32RxBufferSize)
{
    am_hal_uart_state_t *pState = (am_hal_uart_state_t *) pHandle;
    uint32_t ui32ErrorStatus;

    //
    // Check to make sure this is a valid handle.
    //
    if (!AM_HAL_UART_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    //
    // Check to see if we have a TX buffer.
    //
    if (pui8TxBuffer && ui32TxBufferSize)
    {
        //
        // If so, initialzie the transmit queue, and enable the TX FIFO
        // interrupt.
        //
        pState->bEnableTxQueue = true;
        am_hal_queue_init(&pState->sTxQueue, pui8TxBuffer, 1, ui32TxBufferSize);
        ui32ErrorStatus = am_hal_uart_interrupt_enable(pHandle, AM_HAL_UART_INT_TX);
        RETURN_ON_ERROR(ui32ErrorStatus);
    }
    else
    {
        //
        // If not, make sure the TX FIFO interrupt is disabled.
        //
        pState->bEnableTxQueue = false;
        ui32ErrorStatus = am_hal_uart_interrupt_disable(pHandle, AM_HAL_UART_INT_TX);
        RETURN_ON_ERROR(ui32ErrorStatus);
    }

    //
    // Check to see if we have an RX buffer.
    //
    if (pui8RxBuffer && ui32RxBufferSize)
    {
        //
        // If so, initialize the receive queue and the associated interupts.
        //
        pState->bEnableRxQueue = true;
        am_hal_queue_init(&pState->sRxQueue, pui8RxBuffer, 1, ui32RxBufferSize);
        ui32ErrorStatus = am_hal_uart_interrupt_enable(pHandle, (AM_HAL_UART_INT_RX |
                                                                 AM_HAL_UART_INT_RX_TMOUT));
        RETURN_ON_ERROR(ui32ErrorStatus);
    }
    else
    {
        pState->bEnableRxQueue = false;
        ui32ErrorStatus = am_hal_uart_interrupt_disable(pHandle, (AM_HAL_UART_INT_RX |
                                                                  AM_HAL_UART_INT_RX_TMOUT));
        RETURN_ON_ERROR(ui32ErrorStatus);
    }

    return AM_HAL_STATUS_SUCCESS;
} // buffer_configure()

//*****************************************************************************
//
// Set Baud Rate based on the UART clock frequency.
//
//*****************************************************************************
#define BAUDCLK     (16) // Number of UART clocks needed per bit.
static uint32_t
config_baudrate(uint32_t ui32Module, uint32_t ui32DesiredBaudrate, uint32_t *pui32ActualBaud)
{
    uint64_t ui64FractionDivisorLong;
    uint64_t ui64IntermediateLong;
    uint32_t ui32IntegerDivisor;
    uint32_t ui32FractionDivisor;
    uint32_t ui32BaudClk;
    uint32_t ui32UartClkFreq;

    //
    // Check that the baudrate is in range.
    //
    switch ( UARTn(ui32Module)->CR_b.CLKSEL )
    {
        case UART0_CR_CLKSEL_24MHZ:
            ui32UartClkFreq = 24000000;
            break;

        case UART0_CR_CLKSEL_12MHZ:
            ui32UartClkFreq = 12000000;
            break;

        case UART0_CR_CLKSEL_6MHZ:
            ui32UartClkFreq = 6000000;
            break;

        case UART0_CR_CLKSEL_3MHZ:
            ui32UartClkFreq = 3000000;
            break;

        default:
            *pui32ActualBaud = 0;
            return AM_HAL_UART_STATUS_CLOCK_NOT_CONFIGURED;
    }

    //
    // Calculate register values.
    //
    ui32BaudClk = BAUDCLK * ui32DesiredBaudrate;
    ui32IntegerDivisor = (uint32_t)(ui32UartClkFreq / ui32BaudClk);
    ui64IntermediateLong = (ui32UartClkFreq * 64) / ui32BaudClk;
    ui64FractionDivisorLong = ui64IntermediateLong - (ui32IntegerDivisor * 64);
    ui32FractionDivisor = (uint32_t)ui64FractionDivisorLong;

    //
    // Check the result.
    //
    if (ui32IntegerDivisor == 0)
    {
        *pui32ActualBaud = 0;
        return AM_HAL_UART_STATUS_BAUDRATE_NOT_POSSIBLE;
    }

    //
    // Write the UART regs.
    //
    UARTn(ui32Module)->IBRD = ui32IntegerDivisor;
    UARTn(ui32Module)->FBRD = ui32FractionDivisor;

    //
    // Return the actual baud rate.
    //
    *pui32ActualBaud = (ui32UartClkFreq / ((BAUDCLK * ui32IntegerDivisor) + ui32FractionDivisor / 4));
    return AM_HAL_STATUS_SUCCESS;
} // config_baudrate()

//*****************************************************************************
//
// Used to configure basic UART settings.
//
//*****************************************************************************
uint32_t
am_hal_uart_configure(void *pHandle, const am_hal_uart_config_t *psConfig)
{
    am_hal_uart_state_t *pState = (am_hal_uart_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;

    uint32_t ui32ErrorStatus;

    //
    // Check to make sure this is a valid handle.
    //
    if (!AM_HAL_UART_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    //
    // Reset the CR register to a known value.
    //
    UARTn(ui32Module)->CR = 0;

    //
    // Start by enabling the clocks, which needs to happen in a critical
    // section.
    //
    // AM_CRITICAL_BEGIN
    UARTn(ui32Module)->CR_b.CLKEN = 1;
    UARTn(ui32Module)->CR_b.CLKSEL = UART0_CR_CLKSEL_24MHZ;
    // AM_CRITICAL_END

    //
    // Disable the UART.
    //
    // AM_CRITICAL_BEGIN
    UARTn(ui32Module)->CR_b.UARTEN = 0;
    UARTn(ui32Module)->CR_b.RXE = 0;
    UARTn(ui32Module)->CR_b.TXE = 0;
    // AM_CRITICAL_END

    //
    // Set the baud rate.
    //
    ui32ErrorStatus = config_baudrate(ui32Module, psConfig->ui32BaudRate,
                                      &(pState->ui32BaudRate));

    if (ui32ErrorStatus != AM_HAL_STATUS_SUCCESS)
    {
        return ui32ErrorStatus;
    }

    //
    // Set the flow control options
    //
    // AM_CRITICAL_BEGIN
    UARTn(ui32Module)->CR_b.RTSEN = 0;
    UARTn(ui32Module)->CR_b.CTSEN = 0;
    UARTn(ui32Module)->CR |= psConfig->eFlowControl;
    // AM_CRITICAL_END

    //
    // Calculate the parity options.
    //
    uint32_t ui32ParityEnable = 0;
    uint32_t ui32EvenParity = 0;

    switch (psConfig->eParity)
    {
        case AM_HAL_UART_PARITY_ODD:
            ui32ParityEnable = 1;
            ui32EvenParity = 0;
            break;

        case AM_HAL_UART_PARITY_EVEN:
            ui32ParityEnable = 1;
            ui32EvenParity = 1;
            break;

        case AM_HAL_UART_PARITY_NONE:
            ui32ParityEnable = 0;
            ui32EvenParity = 0;
            break;
    }

    //
    // Set the data format.
    //
    // AM_CRITICAL_BEGIN
    UARTn(ui32Module)->LCRH_b.BRK  = 0;
    UARTn(ui32Module)->LCRH_b.PEN  = ui32ParityEnable;
    UARTn(ui32Module)->LCRH_b.EPS  = ui32EvenParity;
    UARTn(ui32Module)->LCRH_b.STP2 = psConfig->eStopBits;
    UARTn(ui32Module)->LCRH_b.FEN  = 1;
    UARTn(ui32Module)->LCRH_b.WLEN = psConfig->eDataBits;
    UARTn(ui32Module)->LCRH_b.SPS  = 0;
    // AM_CRITICAL_END

    //
    // Set the FIFO levels.
    //
    // AM_CRITICAL_BEGIN
    UARTn(ui32Module)->IFLS_b.TXIFLSEL = psConfig->eTXFifoLevel;
    UARTn(ui32Module)->IFLS_b.RXIFLSEL = psConfig->eRXFifoLevel;
    // AM_CRITICAL_END

    //
    // Enable the UART, RX, and TX.
    //
    // AM_CRITICAL_BEGIN
    UARTn(ui32Module)->CR_b.UARTEN = 1;
    UARTn(ui32Module)->CR_b.RXE = 1;
    UARTn(ui32Module)->CR_b.TXE = 1;
    // AM_CRITICAL_END

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Read from the UART RX FIFO.
//
//*****************************************************************************
uint32_t
am_hal_uart_fifo_read(void *pHandle, uint8_t *pui8Data, uint32_t ui32NumBytes,
                      uint32_t *pui32NumBytesRead)
{
    uint32_t i = 0;
    uint32_t ui32ReadData;
    uint32_t ui32ErrorStatus = AM_HAL_STATUS_SUCCESS;

    am_hal_uart_state_t *pState = (am_hal_uart_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;

    //
    // Start a loop where we attempt to read everything requested.
    //
    while (i < ui32NumBytes)
    {
        //
        // If the fifo is empty, return with the number of bytes we read.
        // Otherwise, read the data into the provided buffer.
        //
        if ( UARTn(ui32Module)->FR_b.RXFE )
        {
            break;
        }
        else
        {
            ui32ReadData = UARTn(ui32Module)->DR;

            //
            // If error bits are set, we need to alert the caller.
            //
            if (ui32ReadData & (_VAL2FLD(UART0_DR_OEDATA, UART0_DR_OEDATA_ERR) |
                                _VAL2FLD(UART0_DR_BEDATA, UART0_DR_BEDATA_ERR) |
                                _VAL2FLD(UART0_DR_PEDATA, UART0_DR_PEDATA_ERR) |
                                _VAL2FLD(UART0_DR_FEDATA, UART0_DR_FEDATA_ERR)) )
            {
                ui32ErrorStatus =  AM_HAL_UART_STATUS_BUS_ERROR;
                break;
            }
            else
            {
                pui8Data[i++] = ui32ReadData & 0xFF;
            }
        }
    }

    if (pui32NumBytesRead)
    {
        *pui32NumBytesRead = i;
    }

    return ui32ErrorStatus;
}

//*****************************************************************************
//
// Write to the UART TX FIFO
//
//*****************************************************************************
uint32_t
am_hal_uart_fifo_write(void *pHandle, uint8_t *pui8Data, uint32_t ui32NumBytes,
                       uint32_t *pui32NumBytesWritten)
{
    uint32_t i = 0;

    am_hal_uart_state_t *pState = (am_hal_uart_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;

    //
    // Start a loop where we attempt to write everything requested.
    //
    while (i < ui32NumBytes)
    {
        //
        // If the TX FIFO is full, break out of the loop. We've sent everything
        // we can.
        //
        if ( UARTn(ui32Module)->FR_b.TXFF )
        {
            break;
        }
        else
        {
            UARTn(ui32Module)->DR = pui8Data[i++];
        }
    }

    //
    // Let the caller know how much we sent.
    //
    if (pui32NumBytesWritten)
    {
        *pui32NumBytesWritten = i;
    }

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Empty the UART RX FIFO, and place the data into the RX queue.
//
//*****************************************************************************
static uint32_t
rx_queue_update(void *pHandle)
{
    am_hal_uart_state_t *pState = (am_hal_uart_state_t *) pHandle;

    uint8_t pui8Data[AM_HAL_UART_FIFO_MAX];
    uint32_t ui32BytesTransferred;
    uint32_t ui32ErrorStatus;

    AM_CRITICAL_BEGIN;

    //
    // Read as much of the FIFO as we can.
    //
    ui32ErrorStatus = am_hal_uart_fifo_read(pHandle, pui8Data, AM_HAL_UART_FIFO_MAX,
                                            &ui32BytesTransferred);
    //
    // If we were successful, go ahead and transfer the data along to the
    // buffer.
    //
    if (ui32ErrorStatus == AM_HAL_STATUS_SUCCESS)
    {
        if (!am_hal_queue_item_add(&pState->sRxQueue, pui8Data,
                                   ui32BytesTransferred))
        {
            ui32ErrorStatus = AM_HAL_UART_STATUS_RX_QUEUE_FULL;
        }
    }

    AM_CRITICAL_END;

    return ui32ErrorStatus;
} // rx_queue_update()

//*****************************************************************************
//
// Transfer as much data as possible from the TX queue to the TX FIFO.
//
//*****************************************************************************
static uint32_t
tx_queue_update(void *pHandle)
{
    am_hal_uart_state_t *pState = (am_hal_uart_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;

    uint8_t pui8Data;
    uint32_t ui32BytesTransferred;
    uint32_t ui32ErrorStatus = AM_HAL_STATUS_SUCCESS;

    AM_CRITICAL_BEGIN;

    //
    // Loop as long as the TX fifo isn't full yet.
    //
    while ( !UARTn(ui32Module)->FR_b.TXFF )
    {
        //
        // Attempt to grab an item from the queue, and add it to the fifo.
        //
        if (am_hal_queue_item_get(&pState->sTxQueue, &pui8Data, 1))
        {
            ui32ErrorStatus = am_hal_uart_fifo_write(pHandle, &pui8Data, 1,
                                                     &ui32BytesTransferred);

            if (ui32ErrorStatus != AM_HAL_STATUS_SUCCESS)
            {
                break;
            }
        }
        else
        {
            //
            // If we didn't get anything from the FIFO, we can just return.
            //
            break;
        }
    }

    AM_CRITICAL_END;

    return ui32ErrorStatus;
} // tx_queue_update()

//*****************************************************************************
//
// Run a UART transaction.
//
//*****************************************************************************
uint32_t
am_hal_uart_transfer(void *pHandle,
                     const am_hal_uart_transfer_t *psTransfer)
{
    switch (psTransfer->eType)
    {
        case AM_HAL_UART_BLOCKING_WRITE:
            return blocking_write(pHandle, psTransfer);

        case AM_HAL_UART_BLOCKING_READ:
            return blocking_read(pHandle, psTransfer);

        case AM_HAL_UART_NONBLOCKING_WRITE:
            return nonblocking_write(pHandle, psTransfer);

        case AM_HAL_UART_NONBLOCKING_READ:
            return nonblocking_read(pHandle, psTransfer);

        default:
            return AM_HAL_STATUS_FAIL;
    }
}

//*****************************************************************************
//
// Blocking UART write.
//
//
//         (start blocking_write)
//        ___________|_____________
//       / Check for timeout?      \     YES
//   ->-/(psTransfer->ui32TimeoutMs)\_________________________________________
//  |   \                           /                                         |
//  |    \_________________________/                                          |
//  ^                |  No                                                    |
//  |     ___________V____________                                            |
//  |    / Is the software Queue  \     YES                                   |
//  |   / (pHandle->bEnableTxQueue)\______________________                    |
//  |   \  active?                 /                      |                   V
//  |    \________________________/     __________________V________________   |
//  |                |  No             | Write as much as possible to      |  |
//  ^    ____________V___________      | software queue(pHandle->sTxQueue).|  |
//  |   | Write up to 32 samples |     |___________________________________|  |
//  |   | to hardware queue.     |                        |                   |
//  |   |________________________|      __________________V_________________  |
//  |                |                 | Read from software queue and write | V
//  |                |                 | up to 32 samples to hardware queue.| |
//  |                |                 | ('tx_queue_update()' function).    | |
//  |                |                 |____________________________________| |
//  ^                |                                    |                   |
//  |             ___V____________________________________V____               |
//  |            / Number of bytes transfered till now is equal\     YES      |
//  |           / to psTransfer->ui32NumBytes?                  \_____        V
//  |           \                                               /     |       |
//  |            \_____________________________________________/      V       |
//  |________________|  No                                            |       |
//                                ____________________________________|_______|_
//                               | Return number of bytes transfered to queues  |
//                               | (psTransfer->pui32BytesTransferred).         |
//                               |______________________________________________|
//                                                      |
//                                             (end blocking_write)
//
// Note: When returning from blocking_write, the Tx data is still in UART queues
//   (software or hardware queues).
//*****************************************************************************

uint32_t
blocking_write(void *pHandle, const am_hal_uart_transfer_t *psTransfer)
{
    uint32_t ui32Status, ui32RemainingBytes,
             ui32TimeSpent, i;
    uint32_t ui32TimeoutMs = psTransfer->ui32TimeoutMs;
    uint32_t ui32NumBytesWritten;

    i = 0;
    ui32NumBytesWritten = 0;
    ui32RemainingBytes = psTransfer->ui32NumBytes;
    ui32TimeSpent = 0;

    //
    // Loop until we're done write. This will either be because we hit a
    // timeout, or we sent the right number of bytes. If the caller specified
    // "wait forever", then don't check the timeout.
    //
    while (ui32RemainingBytes && (ui32TimeSpent < ui32TimeoutMs))
    {
        //
        // Write as much as we can.
        //
        ui32Status = am_hal_uart_fifo_write(pHandle, &psTransfer->pui8Data[i],
                                            ui32RemainingBytes, &ui32NumBytesWritten);

        //
        // Update the tracking variables.
        //
        i += ui32NumBytesWritten;
        ui32RemainingBytes -= ui32NumBytesWritten;

        if ((ui32Status != AM_HAL_STATUS_SUCCESS) && (0 == ui32RemainingBytes))
        {
            if (psTransfer->pui32BytesTransferred)
            {
                *psTransfer->pui32BytesTransferred = i;
            }

            return ui32Status;
        }

        //
        // Update the timeout.
        //
        if (ui32RemainingBytes)
        {
            am_hal_delay_us(1);

            if (ui32TimeoutMs != AM_HAL_UART_WAIT_FOREVER)
            {
                ui32TimeSpent++;
            }
        }
    }

    if (psTransfer->pui32BytesTransferred)
    {
        *psTransfer->pui32BytesTransferred = i;
    }

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Blocking UART write.
//
//*****************************************************************************
uint32_t
blocking_read(void *pHandle, const am_hal_uart_transfer_t *psTransfer)
{
    uint32_t ui32Status, ui32RemainingBytes,
             ui32TimeSpent, i;
    uint32_t ui32TimeoutMs = psTransfer->ui32TimeoutMs;
    uint32_t *pui32NumBytesRead = psTransfer->pui32BytesTransferred;

    i = 0;
    ui32RemainingBytes = psTransfer->ui32NumBytes;
    ui32TimeSpent = 0;

    //
    // Loop until we're done reading. This will either be because we hit a
    // timeout, or we got the right number of bytes. If the caller specified
    // "wait forever", then don't check the timeout.
    //
    while (ui32RemainingBytes && (ui32TimeSpent < ui32TimeoutMs))
    {
        //
        // Read as much as we can.
        //
        ui32Status = nonblocking_read(pHandle, psTransfer);

        //
        // Update the tracking variables.
        //
        i += *pui32NumBytesRead;
        ui32RemainingBytes -= *pui32NumBytesRead;

        if (ui32Status != AM_HAL_STATUS_SUCCESS)
        {
            if (pui32NumBytesRead)
            {
                *pui32NumBytesRead = i;
            }

            return ui32Status;
        }

        //
        // Update the timeout.
        //
        if (ui32RemainingBytes)
        {
            am_hal_delay_us(1);

            if (ui32TimeoutMs != AM_HAL_UART_WAIT_FOREVER)
            {
                ui32TimeSpent++;
            }
        }
    }

    if (pui32NumBytesRead)
    {
        *pui32NumBytesRead = i;
    }

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Non-blocking UART write.
//
//         (start nonblocking_write)
//                      |
//           ________________________
//          / Is the software Queue  \     YES
//         / (pHandle->bEnableTxQueue)\______________________
//         \  active?                 /                      |
//          \________________________/     __________________V________________
//                      |  No             | Write as much as possible to      |
//          ____________V___________      | software queue(pHandle->sTxQueue).|
//         | Write up to 32 samples |     |___________________________________|
//         | to hardware queue.     |                        |
//         |________________________|      __________________V_________________
//                      |                 | Read from software queue and write |
//                      |                 | up to 32 samples to hardware queue |
//                      |                 | ('tx_queue_update()' function).    |
//                      |                 |____________________________________|
//                      |                                    |
//                   ___V____________________________________V____
//                  | Return number of bytes transfered to queues |
//                  | (psTransfer->pui32BytesTransferred).        |
//                  |_____________________________________________|
//                                          |
//                               (end nonblocking_write)
//
// Note: The transmit data is always written to the software queue first
// because old data may be already in the software queue.
//*****************************************************************************
uint32_t
nonblocking_write(void *pHandle, const am_hal_uart_transfer_t *psTransfer)
{
    uint32_t ui32ErrorStatus;
    uint32_t ui32BufferSpace;
    uint32_t ui32BytesTransferred;
    uint32_t *pui32NumBytesWritten = psTransfer->pui32BytesTransferred;
    uint32_t ui32NumBytes = psTransfer->ui32NumBytes;
    uint8_t *pui8Data = psTransfer->pui8Data;

    am_hal_uart_state_t *pState = (am_hal_uart_state_t *) pHandle;

    //
    // Check to make sure this is a valid handle.
    //
    if (!AM_HAL_UART_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    //
    // Let the caller know how much we transferred if they provided us with a
    // pointer.
    //
    if (pui32NumBytesWritten)
    {
        *pui32NumBytesWritten = 0;
    }

    if (ui32NumBytes == 0)
    {
        return AM_HAL_STATUS_SUCCESS;
    }

    //
    // Check to see if the circular transmit buffer has been enabled.
    //
    if (pState->bEnableTxQueue)
    {
        //
        // If it has, been enabled, write as much data to it as we can, and let
        // the caller know how much that was.
        //
        ui32BufferSpace = am_hal_queue_space_left(&pState->sTxQueue);

        ui32BytesTransferred = (ui32NumBytes < ui32BufferSpace ?
                                ui32NumBytes : ui32BufferSpace);

        am_hal_queue_item_add(&pState->sTxQueue, pui8Data, ui32BytesTransferred);

        //
        // Transfer as much data as possible from the queue to the fifo.
        //
        ui32ErrorStatus = tx_queue_update(pHandle);
        if (ui32ErrorStatus)
        {
            return ui32ErrorStatus;
        }
    }
    else
    {
        //
        // If the buffer isn't enabled, just write straight to the FIFO.
        //
        am_hal_uart_fifo_write(pHandle, pui8Data, ui32NumBytes,
                               &ui32BytesTransferred);
    }

    //
    // Let the caller know how much we transferred if they provided us with a
    // pointer.
    //
    if (pui32NumBytesWritten)
    {
        *pui32NumBytesWritten = ui32BytesTransferred;
    }

    //
    // If we wrote everything, we should call the callback immediately.
    //
    //if (ui32BytesTransferred == ui32NumBytes)
    //{
    //    if (psTransfer->pfnCallback)
    //    {
    //        psTransfer->pfnCallback(ui32ErrorStatus, psTransfer->pvContext);
    //    }
    //}

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Non-blocking UART write.
//
//*****************************************************************************
uint32_t
nonblocking_read(void *pHandle, const am_hal_uart_transfer_t *psTransfer)
{
    uint32_t ui32BufferData;
    uint32_t ui32BytesTransferred;
    uint32_t ui32ErrorStatus = AM_HAL_STATUS_SUCCESS;
    uint32_t *pui32NumBytesRead = psTransfer->pui32BytesTransferred;
    uint32_t ui32NumBytes = psTransfer->ui32NumBytes;
    uint8_t *pui8Data = psTransfer->pui8Data;

    am_hal_uart_state_t *pState = (am_hal_uart_state_t *) pHandle;

    //
    // Check to make sure this is a valid handle.
    //
    if (!AM_HAL_UART_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
    //
    // Start by setting the number of bytes read to 0.
    //
    if (pui32NumBytesRead)
    {
        *pui32NumBytesRead = 0;
    }

    if (ui32NumBytes == 0)
    {
        return AM_HAL_STATUS_SUCCESS;
    }

    //
    // Check to see if the circular receive buffer has been enabled.
    //
    if (pState->bEnableRxQueue)
    {
        //
        // If it is, update it, and then try to read the requested number of
        // bytes, giving up if fewer were actually found.
        //
        ui32ErrorStatus = rx_queue_update(pHandle);
        if (ui32ErrorStatus)
        {
            return ui32ErrorStatus;
        }
        ui32BufferData = am_hal_queue_data_left(&pState->sRxQueue);

        ui32BytesTransferred = (ui32NumBytes < ui32BufferData ?
                                ui32NumBytes : ui32BufferData);

        am_hal_queue_item_get(&pState->sRxQueue, pui8Data, ui32BytesTransferred);
    }
    else
    {
        //
        // If the buffer isn't enabled, just read straight from the FIFO.
        //
        ui32ErrorStatus = am_hal_uart_fifo_read(pHandle, pui8Data, ui32NumBytes,
                                                &ui32BytesTransferred);
    }

    //
    // Let the caller know how much we transferred if they provided us with a
    // pointer.
    //
    if (pui32NumBytesRead)
    {
        *pui32NumBytesRead = ui32BytesTransferred;
    }

    //
    // If we read everything, we should call the callback immediately.
    //
    //if (ui32BytesTransferred == ui32NumBytes)
    //{
    //    if (psTransfer->pfnCallback)
    //    {
    //        psTransfer->pfnCallback(ui32ErrorStatus, psTransfer->pvContext);
    //    }
    //}

    return ui32ErrorStatus;
}

//*****************************************************************************
//
// Non-blocking UART write interrupt handling.
//
//*****************************************************************************
void
nonblocking_write_sm(void *pHandle)
{
    am_hal_uart_state_t *pState = (am_hal_uart_state_t *) pHandle;

    if (pState->bEnableTxQueue)
    {
        tx_queue_update(pHandle);
    }
}

//*****************************************************************************
//
// Non-blocking UART read interrupt handling.
//
//*****************************************************************************
void
nonblocking_read_sm(void *pHandle)
{
    am_hal_uart_state_t *pState = (am_hal_uart_state_t *) pHandle;

    if (pState->bEnableRxQueue)
    {
        rx_queue_update(pHandle);
    }

}

//*****************************************************************************
//
// Read the UART flags.
//
//*****************************************************************************
uint32_t
am_hal_uart_flags_get(void *pHandle, uint32_t *pui32Flags)
{
    am_hal_uart_state_t *pState = (am_hal_uart_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;

    //
    // Check to make sure this is a valid handle.
    //
    if ( !AM_HAL_UART_CHK_HANDLE(pHandle) )
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    if ( pui32Flags )
    {
        //
        // Set the flags value, then return success.
        //
        *pui32Flags = UARTn(ui32Module)->FR;

        return AM_HAL_STATUS_SUCCESS;
    }
    else
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
}

//*****************************************************************************
//
// Wait for all of the traffic in the TX pipeline to be sent.
//
//*****************************************************************************
uint32_t
am_hal_uart_tx_flush(void *pHandle)
{
    am_hal_uart_state_t *pState = (am_hal_uart_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;

    //
    // If we have a TX queue, we should wait for it to empty.
    //
    if (pState->bEnableTxQueue)
    {
        while (am_hal_queue_data_left(&(pState->sTxQueue)))
        {
            ONE_BYTE_DELAY(pState);
        }
    }

    //
    // Wait for the TX busy bit to go low.
    //
    while ( UARTn(ui32Module)->FR_b.BUSY )
    {
        ONE_BYTE_DELAY(pState);
    }

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Enable interrupts.
//
//*****************************************************************************
uint32_t
am_hal_uart_interrupt_enable(void *pHandle, uint32_t ui32IntMask)
{
    am_hal_uart_state_t *pState = (am_hal_uart_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;

    if (!AM_HAL_UART_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    UARTn(ui32Module)->IER |= ui32IntMask;

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Return the set of enabled interrupts.
//
//*****************************************************************************
uint32_t
am_hal_uart_interrupt_enable_get(void *pHandle, uint32_t *pui32IntMask)
{
    am_hal_uart_state_t *pState = (am_hal_uart_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;

    if (!AM_HAL_UART_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    *pui32IntMask = UARTn(ui32Module)->IER;

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_uart_interrupt_enable_get()

//*****************************************************************************
//
// Disable interrupts.
//
//*****************************************************************************
uint32_t
am_hal_uart_interrupt_disable(void *pHandle, uint32_t ui32IntMask)
{
    am_hal_uart_state_t *pState = (am_hal_uart_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;

    if (!AM_HAL_UART_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    UARTn(ui32Module)->IER &= ~ui32IntMask;

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Clear interrupt status.
//
//*****************************************************************************
uint32_t
am_hal_uart_interrupt_clear(void *pHandle, uint32_t ui32IntMask)
{
    am_hal_uart_state_t *pState = (am_hal_uart_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;

    if (!AM_HAL_UART_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    UARTn(ui32Module)->IEC = ui32IntMask;

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Read interrupt status.
//
//*****************************************************************************
uint32_t
am_hal_uart_interrupt_status_get(void *pHandle, uint32_t *pui32Status,
                                 bool bEnabledOnly)
{
    am_hal_uart_state_t *pState = (am_hal_uart_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;

    if (!AM_HAL_UART_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    //
    // If requested, only return the interrupts that are enabled.
    //
    *pui32Status = bEnabledOnly ? UARTn(ui32Module)->MIS : UARTn(ui32Module)->IES;

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Read interrupt status.
//
//*****************************************************************************
uint32_t
am_hal_uart_interrupt_service(void *pHandle, uint32_t ui32Status)
{
    //
    // Check to make sure this is a valid handle.
    //
    if (!AM_HAL_UART_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    if ( ui32Status & (AM_HAL_UART_INT_RX | AM_HAL_UART_INT_RX_TMOUT) )
    {
        nonblocking_read_sm(pHandle);
    }

    if ( ui32Status & (AM_HAL_UART_INT_TX) )
    {
        nonblocking_write_sm(pHandle);
    }

    return AM_HAL_STATUS_FAIL;
}

//*****************************************************************************
//
// End Doxygen group.
//
//*****************************************************************************
