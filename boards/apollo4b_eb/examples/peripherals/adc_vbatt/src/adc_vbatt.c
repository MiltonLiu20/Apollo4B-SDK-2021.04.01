//*****************************************************************************
//
//! @file adc_vbatt.c
//!
// @brief Example of ADC sampling VBATT voltage divider, BATT load, and temperature.
//
// Purpose: This example initializes the ADC. Two times per second it
// reads the VBATT voltage divider and temperature sensor and prints them.
// It monitors button 0 and if pressed, it turns on the BATT LOAD resistor.
// One should monitor MCU current to see when the load is on or off.
//
// Printing takes place over the ITM at 1M Baud.
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

#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"

#define SHOW_WITH_AND_WITHOUT_ADC_ADJUSTMENT
#define SLOT_BATT   5
#define SLOT_TEMP   7

//*****************************************************************************
//
// Global Variables
//
//*****************************************************************************
//
// ADC Device Handle.
//
static void *g_ADCHandle;

//
// Sample Count semaphore from ADC ISR to base level.
//
uint32_t g_ui32SampleCount;

//
// ADC code for voltage divider from ADC ISR to base level.
//
uint16_t g_ui16ADCVDD_code;

//
// ADC code for temperature sensor from ADC ISR to base level.
//
uint16_t g_ui16ADCTEMP_code;

//*****************************************************************************
//
// ADC Configuration
//
//*****************************************************************************
const static am_hal_adc_config_t g_sADC_Cfg =
{
    //
    // Select the ADC Clock source.
    //
    .eClock = AM_HAL_ADC_CLKSEL_HFRC_DIV2,

    //
    // Polarity
    //
    .ePolarity = AM_HAL_ADC_TRIGPOL_RISING,

    //
    // Select the ADC trigger source using a trigger source macro.
    //
    .eTrigger = AM_HAL_ADC_TRIGSEL_SOFTWARE,

    //
    // Apollo4 uses a 1.2v reference.
    // Select the clock mode.
    //
    .eClockMode = AM_HAL_ADC_CLKMODE_LOW_POWER,

    //
    // Choose the power mode for the ADC's idle state.
    //
    .ePowerMode = AM_HAL_ADC_LPMODE1,

    //
    // Enable repeating samples
    //
    .eRepeat = AM_HAL_ADC_REPEATING_SCAN,

    .eRepeatTrigger = AM_HAL_ADC_RPTTRIGSEL_INT


};

//*****************************************************************************
//
// ADC Interrupt Service Routine (ISR)
//
//*****************************************************************************
void
am_adc_isr(void)
{
    uint32_t ui32IntStatus;

    //
    // Clear the ADC interrupt.
    //
    am_hal_adc_interrupt_status(g_ADCHandle, &ui32IntStatus, true);
    am_hal_adc_interrupt_clear(g_ADCHandle, ui32IntStatus);

#ifdef AM_BSP_NUM_LEDS
    //
    // Toggle LED 3.
    //
    am_devices_led_toggle(am_bsp_psLEDs, 3);
#endif // AM_BSP_NUM_LEDS

    //
    // Keep grabbing samples from the ADC FIFO until it goes empty.
    //
    uint32_t ui32NumSamples = 1;
    am_hal_adc_sample_t sSample;

    //
    // Go get the sample.
    // Empty the FIFO, we'll just look at the last one read.
    //
    while ( AM_HAL_ADC_FIFO_COUNT(ADC->FIFO) )
    {
        ui32NumSamples = 1;
        am_hal_adc_samples_read(g_ADCHandle, true, NULL, &ui32NumSamples, &sSample);

        //
        // Determine which slot it came from?
        //
        if (sSample.ui32Slot == SLOT_BATT )
        {
            //
            // The returned ADC sample is for the battery voltage divider.
            //
            g_ui16ADCVDD_code = AM_HAL_ADC_FIFO_SAMPLE(sSample.ui32Sample);
        }
        else if (sSample.ui32Slot == SLOT_TEMP )
        {
            //
            // The returned ADC sample is for the temperature sensor.
            // We need the integer part in the low 16-bits.
            //
            g_ui16ADCTEMP_code = sSample.ui32Sample & 0xFFC0;
        }
    }

    //
    // Signal interrupt arrival to base level.
    //
    g_ui32SampleCount++;
}

//*****************************************************************************
//
// ADC INIT Function
//
//*****************************************************************************
void
adc_init(void)
{
    am_hal_adc_slot_config_t sSlotCfg;

    //
    // Initialize the ADC and get the handle.
    //
    if ( AM_HAL_STATUS_SUCCESS != am_hal_adc_initialize(0, &g_ADCHandle) )
    {
        am_util_stdio_printf("adc_init() Error - reservation of the ADC instance failed.\n");
    }

    //
    // Power on the ADC.
    //
    if (AM_HAL_STATUS_SUCCESS != am_hal_adc_power_control(g_ADCHandle,
                                                          AM_HAL_SYSCTRL_WAKE,
                                                          false) )
    {
        am_util_stdio_printf("adc_init() Error - ADC power on failed.\n");
    }

    //
    // Configure the ADC.
    //
    if ( am_hal_adc_configure(g_ADCHandle, (am_hal_adc_config_t*)&g_sADC_Cfg) != AM_HAL_STATUS_SUCCESS )
    {
        am_util_stdio_printf("adc_init() Error - configuring ADC failed.\n");
    }

    //
    // Set up internal repeat trigger timer
    //
    am_hal_adc_irtt_config_t      ADCIrttConfig =
    {
        .bIrttEnable        = true,
        .eClkDiv            = AM_HAL_ADC_RPTT_CLK_DIV16, // 48MHz
        .ui32IrttCountMax   = 30,   // 48MHz / 20 = 2.4MHz
    };

    am_hal_adc_configure_irtt(g_ADCHandle, &ADCIrttConfig);

    //
    // For this example, the samples will be coming in slowly. This means we
    // can afford to wake up for every conversion.
    //
    am_hal_adc_interrupt_enable(g_ADCHandle, AM_HAL_ADC_INT_FIFOOVR1 | AM_HAL_ADC_INT_DERR | AM_HAL_ADC_INT_DCMP | AM_HAL_ADC_INT_CNVCMP | AM_HAL_ADC_INT_SCNCMP );

    //
    // Enable the ADC.
    //
    if (AM_HAL_STATUS_SUCCESS != am_hal_adc_enable(g_ADCHandle))
    {
        am_util_stdio_printf("Error - enabling ADC failed.\n");
    }

    //
    // Enable internal repeat trigger timer
    //
    am_hal_adc_irtt_enable(g_ADCHandle);

    sSlotCfg.bEnabled       = false;
    sSlotCfg.bWindowCompare = false;
    sSlotCfg.eChannel       = AM_HAL_ADC_SLOT_CHSEL_SE0;    // 0
    sSlotCfg.eMeasToAvg     = AM_HAL_ADC_SLOT_AVG_1;        // 0
    sSlotCfg.ePrecisionMode = AM_HAL_ADC_SLOT_12BIT;        // 0

    am_hal_adc_configure_slot(g_ADCHandle, 0, &sSlotCfg);   // Unused slot
    am_hal_adc_configure_slot(g_ADCHandle, 1, &sSlotCfg);   // Unused slot
    am_hal_adc_configure_slot(g_ADCHandle, 2, &sSlotCfg);   // Unused slot
    am_hal_adc_configure_slot(g_ADCHandle, 3, &sSlotCfg);   // Unused slot
    am_hal_adc_configure_slot(g_ADCHandle, 4, &sSlotCfg);   // Unused slot
    am_hal_adc_configure_slot(g_ADCHandle, 6, &sSlotCfg);   // Unused slot

    sSlotCfg.bEnabled       = true;
    sSlotCfg.bWindowCompare = true;
    sSlotCfg.eChannel       = AM_HAL_ADC_SLOT_CHSEL_BATT;
    sSlotCfg.eMeasToAvg     = AM_HAL_ADC_SLOT_AVG_1;
    sSlotCfg.ePrecisionMode = AM_HAL_ADC_SLOT_12BIT;
    am_hal_adc_configure_slot(g_ADCHandle, SLOT_BATT, &sSlotCfg);   // BATT

    sSlotCfg.bEnabled       = true;
    sSlotCfg.bWindowCompare = true;
    sSlotCfg.eChannel       = AM_HAL_ADC_SLOT_CHSEL_TEMP;
    sSlotCfg.eMeasToAvg     = AM_HAL_ADC_SLOT_AVG_1;
    sSlotCfg.ePrecisionMode = AM_HAL_ADC_SLOT_10BIT;
    am_hal_adc_configure_slot(g_ADCHandle, SLOT_TEMP, &sSlotCfg);   // TEMP

    //
    // Enable the ADC.
    //
    am_hal_adc_enable(g_ADCHandle);
}

//*****************************************************************************
//
// Main function.
//
//*****************************************************************************
int
main(void)
{
    bool  bMeasured;
    float fTempF;
    int32_t i32BaseLevelCount;
    const float fReferenceVoltage = 1.2;
    float fVBATT;
    float fADCTempVolts;
    float fADCTempDegreesC;
    float fTrims[4];
    uint32_t ui32Retval;
    uint32_t ui32Count;

    //
    // Set the default cache configuration
    //
    am_hal_cachectrl_config(&am_hal_cachectrl_defaults);
    am_hal_cachectrl_enable();

    //
    // Configure the board for low power operation.
    //
    am_bsp_low_power_init();

#ifdef AM_BSP_NUM_LEDS
    //
    // Initialize device drivers for the LEDs on the board.
    //
    am_devices_led_array_init(am_bsp_psLEDs, AM_BSP_NUM_LEDS);
#endif // AM_BSP_NUM_LEDS

#ifdef AM_BSP_GPIO_BUTTON0
    //
    // Configure the button pin.
    //
    am_hal_gpio_pinconfig(AM_BSP_GPIO_BUTTON0, g_AM_BSP_GPIO_BUTTON0);
#endif // AM_BSP_GPIO_BUTTON0

    //
    // Initialize the printf interface for ITM output
    //
    am_bsp_itm_printf_enable();

    //
    // Clear the terminal screen, and print a quick message to show that we're
    // alive.
    //
    am_util_stdio_terminal_clear();
    am_util_stdio_printf("ADC VBATT and Temperature Sensing Example.\n");

    //
    // Enable floating point.
    //
    am_hal_sysctrl_fpu_enable();
    am_hal_sysctrl_fpu_stacking_enable(true);

    //
    // Initialize the ADC.
    //
    adc_init();

    //
    // Enable interrupts.
    //
    NVIC_SetPriority(ADC_IRQn, AM_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(ADC_IRQn);
    am_hal_interrupt_master_enable();

    //
    // Print out ADC initial register state.
    //
    am_util_stdio_printf("\n");
    am_util_stdio_printf("ADC REGISTERS @ 0x%08X\n", (uint32_t)REG_ADC_BASEADDR);
    am_util_stdio_printf("ADC CFG   = 0x%08X\n", ADC->CFG);
    am_util_stdio_printf("ADC SLOT0 = 0x%08X\n", ADC->SL0CFG);
    am_util_stdio_printf("ADC SLOT1 = 0x%08X\n", ADC->SL1CFG);
    am_util_stdio_printf("ADC SLOT2 = 0x%08X\n", ADC->SL2CFG);
    am_util_stdio_printf("ADC SLOT3 = 0x%08X\n", ADC->SL3CFG);
    am_util_stdio_printf("ADC SLOT4 = 0x%08X\n", ADC->SL4CFG);
    am_util_stdio_printf("ADC SLOT5 = 0x%08X\n", ADC->SL5CFG);
    am_util_stdio_printf("ADC SLOT6 = 0x%08X\n", ADC->SL6CFG);
    am_util_stdio_printf("ADC SLOT7 = 0x%08X\n", ADC->SL7CFG);

    //
    // Print out the temperature trim values as recorded in OTP.
    //
    fTrims[0] = fTrims[1] = fTrims[2] = 0.0F;
    fTrims[3] = -123.456f;
    am_hal_adc_control(g_ADCHandle, AM_HAL_ADC_REQ_TEMP_TRIMS_GET, fTrims);
    bMeasured = fTrims[3] ? true : false;
    am_util_stdio_printf("\n");
    am_util_stdio_printf("TRIMMED TEMP    = %.3f\n", fTrims[0]);
    am_util_stdio_printf("TRIMMED VOLTAGE = %.3f\n", fTrims[1]);
    am_util_stdio_printf("TRIMMED Offset  = %.3f\n", fTrims[2]);
    am_util_stdio_printf("Note - these trim values are '%s' values.\n",
                         bMeasured ? "calibrated" : "uncalibrated default");
    am_util_stdio_printf("\n");


    //
    // Enable the ADC interrupt in the NVIC.
    //
    NVIC_EnableIRQ(ADC_IRQn);
    am_hal_interrupt_master_enable();

    //
    // Enable the ADC interrupts in the ADC.
    //
    am_hal_adc_interrupt_enable(g_ADCHandle, AM_HAL_ADC_INT_WCINC       |
                                             AM_HAL_ADC_INT_WCEXC       |
                                             AM_HAL_ADC_INT_FIFOOVR2    |
                                             AM_HAL_ADC_INT_FIFOOVR1    |
                                             AM_HAL_ADC_INT_SCNCMP      |
                                             AM_HAL_ADC_INT_CNVCMP);

    //
    // Reset the sample count which will be incremented by the ISR.
    //
    g_ui32SampleCount = 0;

    //
    // Kick Start Repeat with an ADC software trigger in REPEAT used.
    //
    am_hal_adc_sw_trigger(g_ADCHandle);

    //
    // Track buffer depth for progress messages.
    //
    i32BaseLevelCount = g_ui32SampleCount;
    //
    // Wait here for the ISR to grab a buffer of samples.
    //
    ui32Count = 0;
    while (1)
    {
        //
        // Print the battery voltage and temperature for each interrupt
        //
        #define ISR_SAMPLE_COUNT   1000
        if (g_ui32SampleCount > (i32BaseLevelCount + ISR_SAMPLE_COUNT) )
        {
            i32BaseLevelCount = g_ui32SampleCount;

            //
            // Compute the voltage divider output.
            //
            fVBATT = ((float)g_ui16ADCVDD_code) * 3.0f * fReferenceVoltage / (1024.0f / 64.0f);

            //
            // Print the voltage divider output.
            //
            am_util_stdio_printf("VBATT = <%.3f> (0x%04X) ",
                                 fVBATT, g_ui16ADCVDD_code);

            //
            // Convert and scale the temperature.
            // Temperatures are in Fahrenheit range -40 to 225 degrees.
            // Voltage range is 0.825V to 1.283V
            // First get the ADC voltage corresponding to temperature.
            //
            fADCTempVolts = ((float)g_ui16ADCTEMP_code) * fReferenceVoltage / (1024.0f * 64.0f);

            //
            // Now call the HAL routine to convert volts to degrees Celsius.
            //
            float fVT[3];
            fVT[0] = fADCTempVolts;
            fVT[1] = 0.0f;
            fVT[2] = -123.456;
            ui32Retval = am_hal_adc_control(g_ADCHandle, AM_HAL_ADC_REQ_TEMP_CELSIUS_GET, fVT);
            if ( ui32Retval == AM_HAL_STATUS_SUCCESS )
            {
                fADCTempDegreesC = fVT[1];  // Get the temperature

                //
                // print the temperature value in Celsius.
                //
                am_util_stdio_printf("TEMP = %.2f C (0x%04X) ",
                                     fADCTempDegreesC, g_ui16ADCTEMP_code);

                //
                // Print the temperature value in Fahrenheit.
                //
                fTempF = (fADCTempDegreesC * (180.0f / 100.0f)) + 32.0f;
                am_util_stdio_printf(" %.2f F", fTempF);
            }
            else
            {
                am_util_stdio_printf("Error: am_hal_adc_control returned %d\n", ui32Retval);

            }

            //
            // Use button 0 to turn on or off the battery load resistor.
            //
            am_util_stdio_printf("\n");

#ifdef SHOW_WITH_AND_WITHOUT_ADC_ADJUSTMENT
extern bool g_bDoADCadjust;
            if ( ++ui32Count == 10 )
            {
                ui32Count = 0;
                g_bDoADCadjust = g_bDoADCadjust ? false : true;
                am_util_stdio_printf("Switching to %sADC correction:\n", g_bDoADCadjust ? "USING the " : "NO ");
            }
#endif
        }

        //
        // Sleep here until the next ADC interrupt comes along.
        //
#ifdef AM_BSP_NUM_LEDS
        am_devices_led_off(am_bsp_psLEDs, 0);
        am_hal_sysctrl_sleep(AM_HAL_SYSCTRL_SLEEP_DEEP);
        am_devices_led_on(am_bsp_psLEDs, 0);
#else
        am_hal_sysctrl_sleep(AM_HAL_SYSCTRL_SLEEP_DEEP);
#endif // AM_BSP_NUM_LEDS
    }
}
