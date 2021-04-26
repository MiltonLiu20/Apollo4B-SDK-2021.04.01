// -----------------------------------------------------------------------------
// Copyright (c) 2019 Think Silicon S.A.
// Think Silicon S.A. Confidential Proprietary
// -----------------------------------------------------------------------------
//     All Rights reserved - Unpublished -rights reserved under
//         the Copyright laws of the European Union
//
//  This file includes the Confidential information of Think Silicon S.A.
//  The receiver of this Confidential Information shall not disclose
//  it to any third party and shall protect its confidentiality by
//  using the same degree of care, but not less than a reasonable
//  degree of care, as the receiver uses to protect receiver's own
//  Confidential Information. The entire notice must be reproduced on all
//  authorised copies and copies may only be made to the extent permitted
//  by a licensing agreement from Think Silicon S.A..
//
//  The software is provided 'as is', without warranty of any kind, express or
//  implied, including but not limited to the warranties of merchantability,
//  fitness for a particular purpose and noninfringement. In no event shall
//  Think Silicon S.A. be liable for any claim, damages or other liability, whether
//  in an action of contract, tort or otherwise, arising from, out of or in
//  connection with the software or the use or other dealings in the software.
//
//
//                    Think Silicon S.A.
//                    http://www.think-silicon.com
//                    Patras Science Park
//                    Rion Achaias 26504
//                    Greece
// -----------------------------------------------------------------------------


#ifdef BAREMETAL

#  ifndef WAIT_IRQ_POLL
#    define WAIT_IRQ_POLL               1
#  endif

#else  // BAREMETAL

#  ifndef WAIT_IRQ_POLL
#    define WAIT_IRQ_POLL               0
#  endif

#  ifndef WAIT_IRQ_BINARY_SEMAPHORE
#    define WAIT_IRQ_BINARY_SEMAPHORE   0
#  endif

#  include "FreeRTOS.h"
#  include "portable.h"
#  include "task.h"

#endif // BAREMETAL

//#include "interpose.h"
#include "nema_hal.h"
#include "nema_regs.h"
#include "nema_ringbuffer.h"
#include "am_mcu_apollo.h"
#include "am_hal_global.h"
#include "am_util_delay.h"

#include <stdlib.h>

#ifdef USE_TSI_MALLOC
#include "tsi_malloc.h"

#ifndef VMEM_BASEADDR
#define VMEM_BASEADDR       tsi_buffer
#endif   //VMEM_BASEADDR

#ifndef VMEM_SIZE
#define VMEM_SIZE           (0xF0000)
#endif //VMEM_SIZE

static AM_SHARED_RW uint64_t tsi_buffer[VMEM_SIZE/8];

#endif // USE_TSI_MALLOC

#ifndef NEMA_BASEADDR
#include "apollo4.h"
#define NEMA_BASEADDR       GPU_BASE
#endif

// IRQ number
#ifndef NEMA_IRQ
#define NEMA_IRQ            ((IRQn_Type)28U)
#endif

// IRQ handler
//#define prvNemaInterruptHandler     am_gpu_isr

#if (defined(NEMA_MULTI_PROCESS) || defined(NEMA_MULTI_THREAD))
static int enable_mutices = 0;
#endif

static const uintptr_t nema_regs = (uintptr_t) NEMA_BASEADDR;

#if (defined(NEMA_MULTI_PROCESS) || defined(NEMA_MULTI_THREAD))
    #include "semphr.h"
    static SemaphoreHandle_t xMutex[MUTEX_MAX+1] = {NULL};
#endif

#ifndef BAREMETAL
#if WAIT_IRQ_BINARY_SEMAPHORE == 1

    #include "semphr.h"
    static SemaphoreHandle_t xSemaphore = NULL;

#else
    #include "task.h"
    static TaskHandle_t xHandlingTask = 0;

#endif
#endif /* BAREMETAL */

static volatile int last_cl_id = -1;

static void prvNemaInterruptHandler( void *pvUnused )
{
    last_cl_id = (int)nema_reg_read(NEMA_CLID);

    /* Clear the interrupt */
    nema_reg_write(NEMA_INTERRUPT, 0);
#ifndef BAREMETAL
#if WAIT_IRQ_BINARY_SEMAPHORE == 1
    xSemaphoreGiveFromISR(xSemaphore, NULL);
#else   
    if ( xHandlingTask )
    {
        xTaskNotifyFromISR( xHandlingTask,
                            0,
                            eNoAction,
                            NULL );
    }

    /* If xHigherPriorityTaskWoken is now set to pdTRUE then a context switch
    should be performed to ensure the interrupt returns directly to the highest
    priority task.  The macro used for this purpose is dependent on the port in
    use and may be called portEND_SWITCHING_ISR(). */
    // portYIELD_FROM_ISR( xHigherPriorityTaskWoken ); 
#endif // WAIT_SEMA  
#endif // BAREMETAL   
}

void am_gpu_isr()
{
  prvNemaInterruptHandler(NULL);
}

static nema_ringbuffer_t ring_buffer_str = {{0}};

int32_t nema_sys_init (void)
{
    // disable clockgating of GPU    
    nema_reg_write(NEMA_CGCTRL, 0xffffffff);

#if (defined(NEMA_MULTI_PROCESS) || defined(NEMA_MULTI_THREAD))
    enable_mutices = 0;
#endif    
    nema_reg_write(NEMA_INTERRUPT, 0);
        
    /* Install Interrupt Handler */
    NVIC_SetPriority(NEMA_IRQ, AM_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(NEMA_IRQ);
    
#if WAIT_IRQ_BINARY_SEMAPHORE
    xSemaphore = xSemaphoreCreateBinary();
#endif


#if (defined(NEMA_MULTI_PROCESS) || defined(NEMA_MULTI_THREAD))
    for (int i = 0; i <= MUTEX_MAX; ++i) {
        xMutex[i] = xSemaphoreCreateMutex();
    }
#endif

#ifdef USE_TSI_MALLOC
    // Map and initialize Graphics Memory
    tsi_malloc_init((void *)VMEM_BASEADDR, (uintptr_t)VMEM_BASEADDR, VMEM_SIZE, 1);
#else
    //ring_buffer_str.bo may be already allocated
    if ( ring_buffer_str.bo.base_phys == 0U )
#endif
    {
        //allocate ring_buffer memory
        const int RING_SIZE=1024; 
        ring_buffer_str.bo = nema_buffer_create(RING_SIZE);
        (void)nema_buffer_map(&ring_buffer_str.bo);
    }

    //Initialize Ring BUffer
    int ret = nema_rb_init(&ring_buffer_str, 1);

#if (defined(NEMA_MULTI_PROCESS) || defined(NEMA_MULTI_THREAD))
    enable_mutices = 1;
#endif

    if (ret) {
        // am_util_stdio_printf("nema_rb_init FAILED\n");
        return ret;
    }

    last_cl_id = -1;

    return 0;
}

int nema_wait_irq (void)
{
#ifdef BAREMETAL
    /* Wait for the interrupt */
#if WAIT_IRQ_POLL == 1
    //irq_handler sets NEMADC_REG_INTERRUPT to 0. Poll until this happens
    while( nema_reg_read( NEMA_INTERRUPT ) != 0U ) {}
    //am_util_delay_ms(10);
#else // WAIT_IRQ_POLL
//    am_hal_sysctrl_sleep(0);    
#endif // WAIT_IRQ_POLL
#else /* BAREMETAL */
    /* Wait for the interrupt */
#if WAIT_IRQ_BINARY_SEMAPHORE == 1
    TickType_t block_ms = pdMS_TO_TICKS(1000);
    BaseType_t xResult;
    xResult = xSemaphoreTake( xSemaphore, block_ms );
    return (int)xResult;
#else
    BaseType_t xResult;

    xHandlingTask = xTaskGetCurrentTaskHandle();

    /* If a task is in the Blocked state to wait for a notification when the
       notification arrives then the task immediately exits the Blocked state
       and the notification does not remain pending. If a task was not waiting
       for a notification when a notification arrives then the notification
       will remain pending until the receiving task reads its notification
       value. */

    TickType_t block_ms = pdMS_TO_TICKS(1000);

    /* Wait to be notified of an interrupt. */
    xResult = xTaskNotifyWait( pdFALSE,    /* Don't clear bits on entry. */
                       0,                  /* Don't clear bits on exit. */
                       NULL,               /* No nitification value */
                       block_ms );

    return (int)xResult;
#endif
#endif /* BAREMETAL */

#ifdef BAREMETAL
    return 0;
#endif
}

int nema_wait_irq_cl (int cl_id)
{
  int loops = 0;

  while ( last_cl_id < cl_id) {
      
        //am_util_delay_ms(16);    
        loops++;
        int ret = nema_wait_irq();
        //last_cl_id = (int)nema_reg_read(NEMA_CLID);

        // if ( ret == pdFALSE ) {
        // sleep(5);
        // }
        (void)ret;
        
        //if (loops > 20000) {
        //  nema_reg_write(0xe8, 0);
        //  nema_reg_write(0xfc, 0);
        //  nema_reg_write(NEMA_CLID, cl_id);
        //}
    }

    return 0;
}

uint32_t nema_reg_read (uint32_t reg)
{
    volatile uint32_t *ptr = (volatile uint32_t *)(nema_regs + reg);
    return *ptr;
}

void nema_reg_write (uint32_t reg,uint32_t value)
{
    volatile uint32_t *ptr = (volatile uint32_t *)(nema_regs + reg);
    *ptr = value;
}

#ifndef USE_TSI_MALLOC
//
// According to Think Silicon, the frame buffer should be 8 bytes aligned
//
//#pragma data_alignment = 8, or define it in uint64_t is a more generic way 
#define FBUF_MAX      350*1024
AM_SHARED_RW uint64_t fbuf[FBUF_MAX/8];
uint8_t *current_buf = (uint8_t*)fbuf;
#endif

nema_buffer_t nema_buffer_create (int size)
{
    nema_mutex_lock(MUTEX_MALLOC);

    nema_buffer_t bo;
#ifdef USE_TSI_MALLOC
    bo.base_virt = tsi_malloc((size_t)size);
#else
    // TODO: align to 8 bytes
    //   size += 8;
    //   base_addr = (base_addr+7)&(~(0x7));

/*     bo.base_virt = pvPortMalloc((size_t)size); */
    bo.base_virt = current_buf;
    current_buf += size;
#endif

    bo.base_phys = (uintptr_t) (bo.base_virt);
    bo.size      = size;
    bo.fd        = 0;

    nema_mutex_unlock(MUTEX_MALLOC);
    return bo;
}

nema_buffer_t nema_buffer_create_pool (int pool, int size)
{
    return nema_buffer_create(size);
}

void *nema_buffer_map (nema_buffer_t * bo)
{
    return bo->base_virt;
}

void nema_buffer_unmap (nema_buffer_t * bo)
{

}

void nema_buffer_destroy (nema_buffer_t * bo)
{
    nema_mutex_lock(MUTEX_MALLOC);

#ifdef USE_TSI_MALLOC
    tsi_free(bo->base_virt);
#else
#ifdef BAREMETAL
#else    
    vPortFree(bo->base_virt);
#endif    
#endif

    bo->base_virt = (void *)NULL;
    bo->base_phys = 0;
    bo->size      = 0;
    bo->fd        = -1;

    nema_mutex_unlock(MUTEX_MALLOC);
}

uintptr_t nema_buffer_phys (nema_buffer_t * bo)
{
    return bo->base_phys;
}

void nema_buffer_flush(nema_buffer_t * bo)
{
    nema_mutex_lock(MUTEX_FLUSH);

#ifdef XPAR_CPU_ID
    // Only for Zynq platforms!!!
    // Use XPAR_CPU_ID to decide if Xil_DCacheFlushRange is available
    // For Zynq platforms, XPAR_CPU_ID is defined in Xilinx BSP
    // ps7_cortexa9_0/include/xparameters.h
    #include "xil_cache.h"
    Xil_DCacheFlushRange(bo->base_virt, bo->size);
#endif

    nema_mutex_unlock(MUTEX_FLUSH);
}

void * nema_host_malloc (size_t size)
{
    nema_mutex_lock(MUTEX_MALLOC);

#ifdef USE_TSI_MALLOC
    void *ptr = tsi_malloc(size);
#else
#ifdef BAREMETAL
    // TODO: align to 8 bytes
    //   size += 8;
    //   base_addr = (base_addr+7)&(~(0x7));
    //    
    void* ptr = current_buf;
    current_buf += size;
#else    
    void *ptr = pvPortMalloc(size);
#endif    
#endif

    nema_mutex_unlock(MUTEX_MALLOC);
    return ptr;
}

void nema_host_free (void * ptr)
{
    nema_mutex_lock(MUTEX_MALLOC);

#ifdef USE_TSI_MALLOC
    tsi_free(ptr);
#else
#ifdef BAREMETAL
#else    
    vPortFree(ptr);
#endif    
#endif

    nema_mutex_unlock(MUTEX_MALLOC);
}

int nema_mutex_lock (int mutex_id)
{
#if (defined(NEMA_MULTI_PROCESS) || defined(NEMA_MULTI_THREAD))
    if ((enable_mutices == 1) && (mutex_id >= 0) && (mutex_id <= MUTEX_MAX)) {
        xSemaphoreTake( xMutex[mutex_id], portMAX_DELAY );
    }
#endif
	return 0;
}

int nema_mutex_unlock (int mutex_id)
{
#if (defined(NEMA_MULTI_PROCESS) || defined(NEMA_MULTI_THREAD))
    if ((enable_mutices == 1) && (mutex_id >= 0) && (mutex_id <= MUTEX_MAX)) {
        xSemaphoreGive( xMutex[mutex_id] );
    }
#endif
	return 0;
}

