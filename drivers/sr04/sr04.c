/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-01-18     111       the first version
 */

#include <rtdevice.h>
#include <board.h>
#define sr04_pin_trig  GET_PIN(B, 9)
#define sr04_pin_echo  GET_PIN(B, 8)
#define sr04_timer "timer2"
#include <sr04.h>
rt_hwtimerval_t timeout_s; /* 定 时 器 超 时 值 */
rt_device_t hw_dev = RT_NULL; /* 定 时 器 设 备 句 柄 */

int us=0;
void echo_int(){

    int tmp=rt_pin_read(sr04_pin_echo);
    switch (tmp) {
        case 0://停止定时
            rt_device_control(hw_dev,HWTIMER_CTRL_STOP,RT_NULL);

            break;
        case 1://开始定时
            us=0;
            /* 设 置 定 时 器 超 时 值 为5s并 启 动 定 时 器 */
                            timeout_s.sec = 0; /* 秒 */
                            timeout_s.usec = 100; /* 微 秒 */
                            if (rt_device_write(hw_dev, 0, &timeout_s, sizeof(timeout_s)) != sizeof(
                            timeout_s))
                            {
                            rt_kprintf("set timeout value failed\n");
                            return RT_ERROR; }

            break;
        default:
            break;
    }


}

static rt_err_t timeout_cb(rt_device_t dev, rt_size_t size) {
   us++;
return 0;
}


void sr04_gpio_init(){

    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    rt_pin_mode(sr04_pin_trig, PIN_MODE_OUTPUT);
    rt_pin_mode(sr04_pin_echo, PIN_MODE_INPUT);
    /* 设 置 低 电 平 */
    rt_pin_write(sr04_pin_trig, PIN_LOW);
    rt_pin_write(sr04_pin_echo, PIN_LOW);

    rt_pin_attach_irq( sr04_pin_echo,PIN_IRQ_MODE_RISING_FALLING,echo_int, NULL);
    rt_pin_irq_enable(sr04_pin_echo,  PIN_IRQ_ENABLE);

    rt_err_t ret = RT_EOK;
    rt_hwtimer_mode_t mode; /* 定 时 器 模 式 */
    /* 查 找 定 时 器 设 备 */
    hw_dev = rt_device_find(sr04_timer);
    if (hw_dev == RT_NULL) {rt_kprintf("hwtimer sample run failed! can't find %s device!\n",
            sr04_timer);
            return RT_ERROR; }
            /* 以 读 写 方 式 打 开 设 备 */
            ret = rt_device_open(hw_dev, RT_DEVICE_OFLAG_RDWR);
            if (ret != RT_EOK) {
            rt_kprintf("open %s device failed!\n", sr04_timer);
            return ret; }
            /* 设 置 超 时 回 调 函 数 */
            rt_device_set_rx_indicate(hw_dev, timeout_cb);
            /* 设 置 模 式 为 周 期 性 定 时 器 */
            mode = HWTIMER_MODE_PERIOD;
            ret = rt_device_control(hw_dev, HWTIMER_CTRL_MODE_SET, &mode);
            if (ret != RT_EOK) {
            rt_kprintf("set mode failed! ret is :%d\n", ret);
            return ret; }



};
double sr04_get_distance(){
    rt_pin_write(sr04_pin_trig, PIN_LOW);
    rt_pin_write(sr04_pin_trig, PIN_HIGH);
    rt_thread_mdelay(1);
    rt_pin_write(sr04_pin_trig, PIN_LOW);
    rt_thread_mdelay(100);

    double tmp=us;
    return tmp*340/20;


};
