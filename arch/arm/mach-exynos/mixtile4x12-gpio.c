#include <linux/gpio.h>
#include <linux/serial_core.h>
#include <plat/devs.h>
#include <plat/gpio-cfg.h>
#include <plat/regs-serial.h>
#include <mach/gpio.h>
#include <mach/gpio-mixtile4x12.h>
#include "mixtile4x12.h"

struct gpio_init_data {
	uint num;
	uint cfg;
	uint val;
	uint pud;
	uint drv;
	bool request;
	char name[24];
};

extern int s3c_gpio_slp_cfgpin(unsigned int pin, unsigned int config);
extern int s3c_gpio_slp_setpull_updown(unsigned int pin, unsigned int config);

static struct gpio_init_data mixtile4x12_init_gpios[] = {
  {GPIO_POWER_KEY,      S3C_GPIO_INPUT,  S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_UP,   S5P_GPIO_DRVSTR_LV1, 0, "GPIO_POWER_KEY"},     // EXYNOS4_GPX0(1)
  {GPIO_HEADPHONE,      S3C_GPIO_INPUT,  S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_UP,   S5P_GPIO_DRVSTR_LV1, 0, "GPIO_HEADPHONE"},     // EXYNOS4_GPX0(3)
  {GPIO_GPS_EN,         S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 1, "GPIO_GPS_EN"},        // EXYNOS4_GPX0(7)

  {GPIO_HOME_KEY,       S3C_GPIO_INPUT,  S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_UP,   S5P_GPIO_DRVSTR_LV1, 0, "GPIO_HOME_KEY"},      // EXYNOS4_GPX1(2)
  {GPIO_MENU_KEY,       S3C_GPIO_INPUT,  S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_UP,   S5P_GPIO_DRVSTR_LV1, 0, "GPIO_MENU_KEY"},      // EXYNOS4_GPX1(4)
  {GPIO_BACK_KEY,       S3C_GPIO_INPUT,  S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_UP,   S5P_GPIO_DRVSTR_LV1, 0, "GPIO_BACK_KEY"},      // EXYNOS4_GPX1(5)
#ifdef CONFIG_TOUCHSCREEN_GT81XX
  {GPIO_TP_INT,         S3C_GPIO_INPUT,  S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_UP,   S5P_GPIO_DRVSTR_LV1, 0, "GPIO_TP_INT"},        // EXYNOS4_GPX1(7)
  {GPIO_TP_RST,         S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 1, "GPIO_TP_RST"},        // EXYNOS4_GPX1(6)
#else
  {GPIO_TP_INT,         S3C_GPIO_INPUT,  S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 0, "GPIO_TP_INT"},        // EXYNOS4_GPX1(7)
  {GPIO_TP_RST,         S3C_GPIO_INPUT,  S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 1, "GPIO_TP_RST"},        // EXYNOS4_GPX1(6)
#endif

  {GPIO_VOLUMEUP_KEY,   S3C_GPIO_INPUT,  S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_UP,   S5P_GPIO_DRVSTR_LV1, 0, "GPIO_VOLUMEUP_KEY"},  // EXYNOS4_GPX2(1)
  {GPIO_VOLUMEDOWN_KEY, S3C_GPIO_INPUT,  S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_UP,   S5P_GPIO_DRVSTR_LV1, 0, "GPIO_VOLUMEDOWN_KEY"},// EXYNOS4_GPX2(2)
  {GPIO_MEDIA_KEY,      S3C_GPIO_INPUT,  S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 0, "GPIO_MEDIA_KEY"},     // EXYNOS4_GPX2(3)
  {GPIO_LCD_POWER,      S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 1, "GPIO_LCD_POWER"},     // EXYNOS4_GPX2(4)

  {GPIO_CHARGE_STATE1,  S3C_GPIO_INPUT,  S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_UP,   S5P_GPIO_DRVSTR_LV1, 1, "GPIO_CHARGE_STATE1"}, // EXYNOS4_GPX3(0)
  {GPIO_CHARGE_STATE2,  S3C_GPIO_INPUT,  S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_UP,   S5P_GPIO_DRVSTR_LV1, 1, "GPIO_CHARGE_STATE2"}, // EXYNOS4_GPX3(1)
  {GPIO_USB5V_POWER,    S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 1, "GPIO_USB5V_POWER"},   // EXYNOS4_GPX3(4)
   {EXYNOS4_GPX3(4),    S3C_GPIO_INPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_UP, S5P_GPIO_DRVSTR_LV1, 1, "HDMI_res_ctrl"},   // EXYNOS4_GPX3(4)

  {GPIO_LCD_PWM,        S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 0, "GPIO_LCD_PWM"},       // EXYNOS4_GPD0(0)
  {GPIO_FLASH_EN,       S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 1, "GPIO_FLASH_EN"},      // EXYNOS4_GPD0(2)

  {GPIO_CAMERA_F_RST,   S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 1, "GPIO_CAMERA_F_RST"},  // EXYNOS4_GPM1(2)
  {GPIO_CAMERA_F_PWDN,  S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ONE,  S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 1, "GPIO_CAMERA_F_PWDN"}, // EXYNOS4_GPM1(3)
  {GPIO_CAMERA_POWER,   S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 1, "GPIO_CAMERA_POWER"},  // EXYNOS4_GPM1(4)

  {GPIO_CAMERA_B_RST,   S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 1, "GPIO_CAMERA_B_RST"},  // EXYNOS4_GPM2(3)
  {GPIO_CAMERA_B_PWDN,  S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 1, "GPIO_CAMERA_B_PWDN"}, // EXYNOS4_GPM2(4)

  {GPIO_3G_POWER,       S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 1, "GPIO_3G_POWER"},      // EXYNOS4_GPM3(0)
  {GPIO_LCD_BACKLED,    S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 1, "GPIO_LCD_BACKLED"},   // EXYNOS4_GPM3(2)
  {GPIO_LVDS_POWER,     S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 1, "GPIO_LVDS_POWER"},    // EXYNOS4_GPM3(3)
  {GPIO_USBOTG_POWER,   S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 0, "GPIO_USBOTG_POWER"},  // EXYNOS4_GPM3(4)
  {GPIO_MOTOR_POWER,    S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 0, "GPIO_MOTOR_POWER"},   // EXYNOS4_GPM3(5)
  {GPIO_USBHUB_POWER,   S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 1, "GPIO_USBHUB_POWER"},  // EXYNOS4_GPM3(7)

  {GPIO_GPS_POWER,      S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 1, "GPIO_GPS_POWER"},     // EXYNOS4_GPM4(0)
  {GPIO_FLASH_MODE,     S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 1, "GPIO_FLASH_MODE"},    // EXYNOS4_GPM4(1)
  {GPIO_TP_POWER,       S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 1, "GPIO_TP_POWER"},      // EXYNOS4_GPM4(2)
  {GPIO_TVP5151_RST,    S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 1, "GPIO_TVP5151_RST"},   // EXYNOS4_GPM4(3)
  {GPIO_WIFI_POWER,     S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 1, "GPIO_WIFI_POWER"},    // EXYNOS4_GPM4(5)
  {GPIO_TVP5151_PWDN,   S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 1, "GPIO_TVP5151_PWDN"},  // EXYNOS4_GPM4(7)

  {GPIO_DM9000_CS0,     S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1, 1, "GPIO_DM9000_CS0"},    // EXYNOS4_GPY0(0)
};

void mixtile4x12_config_gpio_table(void)
{
	u32 i, gpio;

	for (i = 0; i < ARRAY_SIZE(mixtile4x12_init_gpios); i++)
  {
		gpio = mixtile4x12_init_gpios[i].num;
		s3c_gpio_cfgpin(gpio, mixtile4x12_init_gpios[i].cfg);
		s3c_gpio_setpull(gpio, mixtile4x12_init_gpios[i].pud);

		if (mixtile4x12_init_gpios[i].val != S3C_GPIO_SETPIN_NONE)
			gpio_set_value(gpio, mixtile4x12_init_gpios[i].val);

		s5p_gpio_set_drvstr(gpio, mixtile4x12_init_gpios[i].drv);
		
		if(mixtile4x12_init_gpios[i].request)
		  gpio_request(mixtile4x12_init_gpios[i].num, mixtile4x12_init_gpios[i].name);
	}
}
