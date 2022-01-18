/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-01-18     RT-Thread    first version
 */

#include <rtthread.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include <sr04.h>
int main(void)
{


     sr04_gpio_init();
     while(1)
     {
         double tmp;
         tmp=sr04_get_distance();
         rt_kprintf("%f\n",tmp);
         //rt_thread_mdelay(1000);
      }

        LOG_D("Hello RT-Thread!");
        rt_thread_mdelay(1000);


    return RT_EOK;
}
