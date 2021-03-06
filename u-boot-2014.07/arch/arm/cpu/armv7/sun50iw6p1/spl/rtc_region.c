/*
 * (C) Copyright 2013-2016
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
/*
**********************************************************************************************************************
*
*						           the Embedded Secure Bootloader System
*
*
*						       Copyright(C), 2006-2014, Allwinnertech Co., Ltd.
*                                           All Rights Reserved
*
* File    :
*
* By      :
*
* Version : V2.00
*
* Date	  :
*
* Descript:
**********************************************************************************************************************
*/
#include <common.h>
#include <asm/io.h>
#include <asm/arch/timer.h>

#define  RTC_DATA_HOLD_REG_BASE        (SUNXI_RTC_BASE + 0x100)
#define  RTC_DATA_HOLD_REG_FEL         (RTC_DATA_HOLD_REG_BASE + 0x8)
#define  RTC_CRY_CFG  				   (SUNXI_RTC_BASE + 0x210)
#define  RTC_CRY_REG  				   (SUNXI_RTC_BASE + 0x214)
#define  RTC_CRY_EN  				   (SUNXI_RTC_BASE + 0x218)


/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
uint rtc_region_probe_fel_flag(void)
{
	uint fel_flag, reg_value;
	int  i;

	fel_flag = readl(RTC_DATA_HOLD_REG_FEL);

	for(i=0;i<=5;i++)
	{
		reg_value = readl(RTC_DATA_HOLD_REG_BASE + i*4);
		printf("rtc[%d] value = 0x%x\n", i, reg_value);
	}

	return fel_flag;
}
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
void rtc_region_clear_fel_flag(void)
{
	volatile uint flag = 0;
	do
	{
		writel(0, RTC_DATA_HOLD_REG_FEL);
		asm volatile("DSB");
		asm volatile("ISB");
		flag  = readl(RTC_DATA_HOLD_REG_FEL);
	}
	while(flag != 0);
}

#define RTC_BARRIER() do{asm volatile("DSB"); asm volatile("ISB");} while(0);
void rtc_region_ddr_scramble_en(int en, unsigned int key)
{
	if (en) {
		writel(0x1689UL, RTC_CRY_CFG);
		RTC_BARRIER();
		writel(key, RTC_CRY_REG);
		RTC_BARRIER();
		writel(0x1689UL, RTC_CRY_CFG);
		RTC_BARRIER();
		writel(1, RTC_CRY_EN);
	} else {
		writel(0x1689UL, RTC_CRY_CFG);
		RTC_BARRIER();
		writel(0, RTC_CRY_EN);
	}
}


